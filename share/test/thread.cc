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
//MCS include file
#include <mcs.hh>
using namespace mcs;


//See test.h for auxiliary functions and macros
#include "test.h"


//Use a shared variable to test locking facilities
int shared_var = 0;

//The object used to implement critical sections.
Synchro syn;


//Derive the Thread class. The following methods will print some notes
//as well as the thread identifiers (both the MCS identifier and the
//pthread library identifier). Note that the constructor and
//destructor are executed in the main thread, while initial, final and run
//methods are executed in the child threads.
class testThread : public Thread
{
public:

  //Constructor
  testThread(int id) : Thread(id)
  {
    cout << "Thread " << this->id() << "(" << pthread_self() << ") constructor()" << endl;
    syn.synchronize(true); //Activate the Synchro object.
  }

  //Destructor
  ~testThread()
  { cout << "Thread " << id() << "(" << pthread_self() << ") destructor()" << endl; }

  void initial()
  { cout << "Thread " << id() << "(" << pthread_self() << ") initial()" << endl; }

  void final()
  { cout << "Thread " << id() << "(" << pthread_self() << ") final()" << endl; }

  //This is the child thread body. It will update the shared variable
  //"shared_var" inside a critical section.
  void run()
  {
    int local_var = 0;

    while (local_var < 10) {

      //Entering critical section, the shared variable is used only
      //inside this section.
      syn.enter();

      //If the shared variable has the same parity as the thread
      //identifier it will be incremented
      if (shared_var % 2 == id()) {
	cout << "Thread " << id() << "(" << pthread_self()
	     << ") modifying shared variable: " << shared_var << endl;

	shared_var++;
	local_var = shared_var;
      }

      //Leave the critical section and release the time slice for the
      //other thread.
      syn.leave();
      sleep_ms(50);
    }
  }
};




int main()
{
  try {

  cout << "Main thread identifier is: " << pthread_self() << endl;

  //Create the thread objects
  testThread thr0(0);
  testThread thr1(1);

  cout << "Starting child threads..." << endl;
  thr0.start();
  thr1.start();

  cout << "Waiting for child thread termination..." << endl;

  //Sleep for one second, it is enough to let child threads terminate
  sleep_ms(1000);

  //Check that both threads have terminated their execution.
  if (thr0.state() != MCS_STATE_END)
    ThrowErr("Child thread 0 has not terminated");

  if (thr1.state() != MCS_STATE_END)
    ThrowErr("Child thread 1 has not terminated");

  }
  catch(Event e) {
      cerr << e.where() << ": " << e.msg() << endl;
      exit(1);
  }
  catch(...) {
      cout << "Unknown error" << endl;
      exit(2);
  }

  cout << "Test successfull!" << endl;
  exit(0);
}
