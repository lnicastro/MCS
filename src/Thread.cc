// ----------------------------------------------------------------------^
// Copyright (C) 2004, 2005, 2006, 2007, 2008 Giorgio Calderone
// (mailto: <gcalderone@ifc.inaf.it>)
// 
// This file is part of MCS.
// 
// MCS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// MCS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with MCS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 
// ----------------------------------------------------------------------$

#include <assert.h>

#include "mcs.hh"
using namespace mcs;

#ifndef HAVE_PTHREAD_MUTEXATTR_SETTYPE
extern int pthread_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind)
    __THROW;
#endif



/*Begin Pierre debug stuff*/

#define TBOLD "\x1B[1m"
#define REV  "\x1B[7m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"
#define CYAN  "\x1B[36m"
#define YELLOW  "\x1B[33m"
#define MAGENTA "\x1B[35m"
#define BEEP  "\x7"
#define BGBLUE  "\x1B[44m"
#define NORMAL "\x1B[0m"
#define CLEANS  "\x1B[2J"


#define MERROR std::cerr << RED <<   "\r(E)\t"<< GREEN << __FILE__ << "," << __LINE__ << NORMAL << "\t" <<TBOLD<<__FUNCTION__<<NORMAL << "\t" 
#define MINFO std::cout << BLUE <<   "\r(I)\t"<< GREEN << __FILE__ << "," << __LINE__ << NORMAL << "\t" <<TBOLD<<__FUNCTION__<<NORMAL << "\t" 
#define MWARN std::cout << YELLOW << "\r(W)\t"<< GREEN << __FILE__ << "," << __LINE__ << NORMAL << "\t" <<TBOLD<<__FUNCTION__<<NORMAL << "\t" 

/*End Pierre debug stuff */

//--------------------------------------------------------
mcs::Synchro::Synchro()
{

    MCS_DEBUG_SETUP(0, "Synchro");
    MCS_DEBUG_ENTER(NOARGS);

  Count = 0;
  isActive = false;

  //MINFO << "synchro ptr ("<< this <<") created!"<<endl;

  // MCS_DEBUG_LEAVE(NOARGS);
}


mcs::Synchro::~Synchro()
{

  MCS_DEBUG_ENTER(NOARGS);

  synchronize(false);

  MCS_DEBUG_LEAVE(NOARGS);

 
}

int mcs::Synchro::count()
{ return Count; }


void mcs::Synchro::synchronize(bool setactive)
{
  MCS_DEBUG_ENTER( << isActive);

  if (Count != 0)
    throw MCS_FATAL( MSG_CANT_SET_SYNCHRO_STATE );

  if ((! isActive)   &&   (setactive)) {
//LN-darwin
/* 
   One of PTHREAD_MUTEX_RECURSIVE_NP and PTHREAD_MUTEX_RECURSIVE seem to be 
   present in different versions.  For example, Mac OS X 10.4 had 
   PTHREAD_MUTEX_RECURSIVE_NP but Mac OS X 10.5 does not; instead it has
   PTHREAD_MUTEX_RECURSIVE 
 */


#ifdef HAVE_PTHREAD_MUTEX_RECURSIVE_NP
#define PTHREAD_MUTEX_RECURSIVE_VALUE PTHREAD_MUTEX_RECURSIVE_NP
#elif defined (HAVE_PTHREAD_MUTEX_RECURSIVE)
#define PTHREAD_MUTEX_RECURSIVE_VALUE PTHREAD_MUTEX_RECURSIVE
#else
#define PTHREAD_MUTEX_RECURSIVE_VALUE PTHREAD_MUTEX_RECURSIVE
#error 'Unable to determine pthread mutex recursive value'
#endif /* pthread mutex recursive value */

    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_VALUE);
#ifdef HAVE_PTHREAD_MUTEXATTR_SETPROTOCOL
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_NONE);
#endif
//LN-darwin: apparently any attribute setting causes failure! 
#ifdef __APPLE__
    pthread_mutex_init(&mutex, NULL);
#else
    pthread_mutex_init(&mutex, &attr);
#endif



  }




  if ((isActive)   &&   (! setactive)) {
        int ret = pthread_mutex_destroy(&mutex);
    assert(ret == 0);
  }

  isActive = setactive;
  MCS_DEBUG_LEAVE( << isActive );
}


bool mcs::Synchro::enter(int op, unsigned int to)
{
  int ret = 0;

  if (! isActive) return true;
  MCS_DEBUG_ENTER(NOARGS);

  switch (op) {
  case MCS_SYNCHRO_LOCK:
    ret = pthread_mutex_lock(&mutex);
    break;
  case MCS_SYNCHRO_TRY_LOCK:
    ret = pthread_mutex_trylock(&mutex);
    break;
  case MCS_SYNCHRO_TRY_TIMED:
    struct timeval now;
    struct timespec timeout;
    unsigned int millisec, seconds;
    millisec = to % 1000;
    seconds  = (unsigned int) floor((double) (to / 1000.0));
    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + seconds;
    timeout.tv_nsec = now.tv_usec * 1000 + millisec * 1000000;

//LN-darwin
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS - 200112L) >= 0L
    ret = pthread_mutex_timedlock(&mutex, &timeout);
#else
    ret = pthread_mutex_lock(&mutex);
#endif
    break;
  }

//MINFO<<"pthread_mutex_lock ret="<<ret<<
//      "  EDEADLK:"<<EDEADLK<<" EAGAIN:"<<EAGAIN<<" EINVAL:"<<EINVAL<<endl;
  assert((ret == 0)          ||
	 (ret == EBUSY)      ||
	 (ret == ETIMEDOUT)    );


  if (ret == 0) {
    Count++;
  }


  MCS_DEBUG_LEAVE(NOARGS);
  return (ret == 0);
}



int mcs::Synchro::leave()
{
  if (! isActive) return 0;
  MCS_DEBUG_ENTER(<< Count);

  int ret;
  ret = pthread_mutex_unlock(&mutex);

  assert(ret == 0);

  if (ret == 0)
    Count--;

  return Count;
  MCS_DEBUG_LEAVE(NOARGS);
}


bool mcs::Synchro::tryenter(unsigned int timeout)
{
  if (timeout == 0)
    return enter(MCS_SYNCHRO_TRY_LOCK);
  else
    return enter(MCS_SYNCHRO_TRY_TIMED, timeout);
}



//bool mcs::Synchro::wait(unsigned int to)
//{
//  int ret;
//  bool bret;
//  if (! isActive) return true;
//
//  //waiting = true;
//
//  MCS_DEBUG_ENTER(NOARGS);
//
//  //Always use TIMED wait.
//  struct timeval now;
//  struct timespec timeout;
//  unsigned int millisec, seconds;
//  millisec = to % 1000;
//  seconds  = (unsigned int) floor((double) (to / 1000.0));
//  gettimeofday(&now, NULL);
//  timeout.tv_sec = now.tv_sec + seconds;
//  timeout.tv_nsec = now.tv_usec * 1000 + millisec * 1000000;
//
//  ret = pthread_cond_timedwait(&cond, &mutex, &timeout);
//  bret = (! ((ret == ETIMEDOUT)   ||   (ret == EINTR)));
//
//  waiting = false;
//  MCS_DEBUG_LEAVE(NOARGS);
//  return bret;
//}








//--------------------------------------------------------
mcs::Thread::Thread(int id, Thread* parent) //: syn_lstate()
{

  //  MCS_DEBUG_SETUP(0, "Thread");

  lparent = parent;
  lid = id;
  lerror = NULL;

  //MINFO << "Thread init lerror is null ? "<< lerror << endl;

  lstate = MCS_STATE_CREATED;
  //  syn_lstate.synchronize(true);

  detached = false;
  selfDelete = false;

  //  MCS_DEBUG_LEAVE(<< lid);
}


mcs::Thread::~Thread()
{

  //MINFO << "Thread "<< id() <<" delete lerror = " << lerror <<endl;

  MCS_DEBUG_ENTER(<< lid);
  if (pthread_self() != lthrID){
    stop();
  }

  if (lerror){
    //MINFO << id() << " : delete lerror "<< lerror <<  endl;
    delete lerror;
    lerror=0;
    //MINFO << id() << " : delete lerror  done "<< endl;
  }
  MCS_DEBUG_LEAVE(<< lid);

  //MINFO  << "thread "<< id() <<" delete... done"<<endl;
}

void mcs::Thread::initial()
{}

void mcs::Thread::final()
{}


int mcs::Thread::state()
{ return lstate; }

int mcs::Thread::id()
{ return lid; }

Event* mcs::Thread::error()
{ return lerror; }

Thread* mcs::Thread::parent()
{ return lparent; }

void mcs::Thread::cleanup_Handler(void* p)
{ ((Thread*) p)->final(); }


void mcs::Thread::run()
{ throw MCS_ERROR(MSG_METHOD_MUST_BE_OVERLOADED, "Thread::run"); }

void mcs::Thread::notify(int id, Thread* ref)
{
#if ENABLE_DEBUG
    char buf[10];
    sprintf(buf, "%d", id);
    string s = "Notify from " + string(buf);
    MCS_DEBUG(lid << " " << s.c_str());
#endif
}



void mcs::Thread::set_cancel_state(bool cancel)
{
  int ret;
  if (cancel) {
    ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    assert(ret == 0);
    ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    assert(ret == 0);
  }
  else {
    ret = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    assert(ret == 0);
  }
}


void mcs::Thread::test_cancel()
{
  int old, ret;
  ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
  assert(ret == 0);
  ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
  assert(ret == 0);

  pthread_testcancel();

  ret = pthread_setcancelstate(old, NULL);
  assert(ret == 0);
}



void mcs::Thread::startDetached(bool selfdelete)
{
  detached = true;
  selfDelete = selfdelete;
  start();
}


void mcs::Thread::start()
{


  //MINFO << "ID " << id() <<" Starting from main thread:   lerror is " << lerror << endl;

  int ret;

  MCS_DEBUG_ENTER(<< lid);


  
  ret = pthread_create(&lthrID, NULL, mcs::Thread::RunThread, this);

  if (ret)
      throw MCS_FATAL(MSG_CALLING_PTHREAD_CREATE);

  if (detached) {

    ret = pthread_detach(lthrID);

    if (ret != 0)
      throw MCS_FATAL(MSG_CALLING_PTHREAD_CREATE);
  }

  //Continue only after initial() have been executed

  while (lstate < MCS_STATE_RUNNING)
    sleep_ms(1);

  MCS_DEBUG_LEAVE(<< lid);
}



void* mcs::Thread::RunThread(void* args)
{
  Thread* thr = (Thread*) args;

  //MINFO << "ID " << thr->id() <<" Beginning :   lerror is " << thr->lerror << endl;

  //Install the cleanup handler

  pthread_cleanup_push(mcs::Thread::cleanup_Handler, thr);
  thr->set_cancel_state(true);

  try {
    thr->initial();
    thr->lstate = MCS_STATE_RUNNING;


    //From the run() method it is possible to
    // - return
    // - throw an exception
    // - another thread call the stop() method
    //MINFO << "ID " << thr->id() <<" Before run :   lerror is " << thr->lerror << endl;

    thr->run();

    //MINFO <<  "ID " << thr->id() << " After run :   lerror is " << thr->lerror << endl;
  }
  catch (Event e) {
    //MINFO << "creating lerror " << endl;

    thr->lerror = new Event(e);
    //cerr << e.msg() << endl;
  }

  //MINFO << thr->id() << " : Finished execution :  lerror is " << thr->lerror << endl;


  thr->set_cancel_state(false);

  if (thr->checkTerminating()) {
    pthread_exit(NULL);
    return NULL;
  }

  pthread_cleanup_pop(0);

  thr->final();

  thr->lstate = MCS_STATE_END;

  //The thread is terminating by itself and will eventually notify its
  //parent who won't call the stop() method. In this case the
  //pthread_cancel and pthread_join functions won't be called and the
  //resources won't be freed. Calling pthread_detach() here will
  //detach the thread and resources will be freed at the
  //pthread_exit() call.

  pthread_detach(thr->lthrID);

  if (thr->lparent)
    thr->lparent->notify(thr->lid, thr);

  if (thr->selfDelete){
    MWARN << " AUTO delete !!!!  " << endl;

    delete thr;

    MWARN << " AUTO delete !!!!  I survived !!" << endl;
  }


  //MINFO << thr->id() << " : Finished cleanup :  lerror is " << thr->lerror << endl;

  if((long int) thr->lerror == 1){
    MERROR << "Thread "<< thr->id() <<" Ohh lerror is 1? !"<< thr->lerror << endl;
  }else
  
    if( thr->lerror!=NULL){
      //MINFO << thr->id()  << " Exiting with non-null lerror, deleteing : lerror->where()= " << thr->lerror->where() << endl;

      delete thr->lerror;
      thr->lerror=0;
      //MINFO << thr->id()  << " deleting lerror OK " << endl; 
    }
  
  //  pthread_exit(NULL);

  //  MINFO << " Exiting 2 " << endl;


  return NULL;
}



void mcs::Thread::stop()
{

  //This method MUST not be called from the thread itself
  assert(pthread_self() != lthrID);

  if (detached)
    return; //ERROR: Can't stop a detached thread

  bool wasterminating = checkTerminating();
  //If the thread was already terminating the rest of the job has been
  //performed in the RunThread() method, so we can return.
  if (wasterminating)
    return;

  //If the thread is not detached then the parent thread MUST call
  //pthread_cancel() and pthread_join() to free resources.

  pthread_cancel(lthrID); //final() will be called here if the cleanup
			  //handler is still installed
  pthread_join(lthrID, NULL);

  //Il controllo wasterminating ==> return era qui.

  lstate = MCS_STATE_END;

  if (lparent)
    lparent->notify(lid, this);
}


bool mcs::Thread::checkTerminating()
{
  bool ret;

  //  syn_lstate.enter();

  ret = (lstate >= MCS_STATE_TERMINATING);
  if (!ret)
    lstate = MCS_STATE_TERMINATING;

  //  syn_lstate.leave();

  return ret;
}





mcs::ThreadFunc::ThreadFunc(int (*start_routine)(void*),
			    void* arg) :
  Thread(0, NULL)
{
  this->start_routine1 = start_routine;
  this->start_routine2 = NULL;
  this->arg = arg;
  ret = 0;
}

mcs::ThreadFunc::ThreadFunc(Event* (*start_routine)(void*),
			    void* arg) :
  Thread(0, NULL)
{
  this->start_routine1 = NULL;
  this->start_routine2 = start_routine;
  this->arg = arg;
  ret = 0;
}


void mcs::ThreadFunc::run()
{

  //MINFO << "lerror is " << lerror << endl;

  if (start_routine1)
    ret = (start_routine1)(arg);
  else{
    //MINFO << "Creating lerror in RUN "<<endl;
    lerror = (start_routine2)(arg);
    //MINFO << "Creating lerror in RUN lerror="<< lerror <<endl;
  }

  //MINFO << "lerror is " << lerror << endl;
}

int mcs::ThreadFunc::retcode()
{ return ret; }







void mcs::sleep_ms(unsigned int millisec)
{
  struct timespec tt, tmp;
  tt.tv_sec = 0;
  tt.tv_nsec = 0;


  if (millisec >= 1000) {
    tt.tv_sec = ((long int) (millisec / 1000.0));
  }

  tt.tv_nsec = (millisec % 1000) * 1000000;
  nanosleep(&tt, &tmp);
}



