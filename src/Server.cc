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


#include "mcs.hh"
using namespace mcs;


#define  PIDFILE "mcs.pid"


//--------------------------------------------------------
mcs::Server::Server(Env* lenv) : BaseThread(NULL, "MMM"),
				 dispatch(true)
{
  //Initialize to NULL all callback function pointers.
  cb_log          = NULL;
  cb_connect	  = NULL;
  cb_auth	  = NULL;
  cb_exec	  = NULL;
  cb_postexec	  = NULL;
  cb_disconnect   = NULL;
  cb_newClient    = NULL;
  cbwa_log        = NULL;
  cbwa_connect    = NULL;
  cbwa_auth	  = NULL;
  cbwa_exec	  = NULL;
  cbwa_postexec   = NULL;

  pClient = NULL;
  env=lenv;  //env is a static member of Thread

  env->local = NULL;
  if (env->cl_logstdout) {
    //Print a banner
    cout << MCS_BANNER;
    cout << "\nYYYYMMDD HHMMSS thr MESSAGE" << endl;
    cout <<
      "-------- ------ --- " <<
      "------------------------------------------------------------" <<
      endl;
  }
}




mcs::Server::~Server()
{
  //  env->server_running=false;

  if (env->local) {
    Log(MCS_OK( MSG_KILLING_LOCAL ));
    env->cl_restartlocal = 0;
    env->local->stop();  //TESTING  //env->local->exit(false);
  }

  if (pClient) {
      killAllClients();
      free(pClient);
  }

  Log(MCS_OK( MSG_THREAD_DESTROY, "Server" ));
}


void mcs::Server::run()
{
  int i;
  int newsock;

  set_cancel_state(false);

  try {
    Log(MCS_OK( MSG_THREAD_CREATE, "Server" ));

    Log(MCS_OK( MSG_SERVER_DIR, env->path.csz ));

    if (env->max_users <= 0)
	throw MCS_FATAL(MSG_ERROR, "max_users <= 0");

    pClient = (UserThread**) malloc(env->max_users * sizeof(UserThread*));
    if (! pClient)
	throw MCS_FATAL(MSG_OUT_OF_MEMORY);

    env->local=NULL;

    for (i=0; i<env->max_users; i++)
      pClient[i]=NULL;

    if (env->cl_createlocal) {
	Log(MCS_OK( MSG_CREATING_LOCAL ));
	env->local=newLocalThread();
	env->local->start();
    }

    ServerSocket server(env->interface, env->port,
			env->cl_use_ssl, env->sslcert, env->sslpriv);
    env->ssl_ctx = server.getSSLContext(); //To be passed to UserThread objects

    Log(MCS_OK( MSG_SERVER_RUNNING, server.ipaddress().c_str(), env->port ));


    //Write the PIDFILE
    {
      ofstream of(PIDFILE, ios::out);
      pid_t pid = getpid();
      of << pid << endl;
      of.close();
    }


    for(;;) {
      set_cancel_state(true);
      if (server.acceptConnection(newsock, 1000)) {
	set_cancel_state(false);
	newClient(newsock);
      }
      set_cancel_state(false);

      if (! env->local) {
	if (env->cl_local_kills_mcs) {
	  Log(MCS_OK( MSG_LOCAL_THREAD_DIED ));
	  return;
	}

	else if(env->cl_restartlocal) {
	  Log(MCS_OK( MSG_CREATING_LOCAL ));
	  env->local=newLocalThread();
	  env->local->start();
	}
      }
    }
  }
  catch(Event e) {
    Log(e);
    if (e.type() == FATAL)
      return;
  }
}




void mcs::Server::notify(int id, Thread* ref)
{
#if ENABLE_DEBUG
  Thread::notify(id, ref);
#endif

  //ref->allowTermination();

  if (ref == env->local) {
    delete env->local; //TESTING
    env->local = NULL;
  }
  else {
    delete pClient[id]; //TESTING
    pClient[id]=NULL;
  }
}




int mcs::Server::find_free_id()
{
  int i, res=-1;
  for (i=0; i<env->max_users; i++)
    if (pClient[i] == NULL) {
      res=i;
      break;
    }

  return res;
}




void mcs::Server::newClient(int newsock)
{
  int i=find_free_id();
  if (i >= 0) {
    Log(MCS_OK( MSG_CREATING_CLIENT, i ));
    pClient[i] = newUserThread(i, newsock);
    pClient[i]->start();
    hk_newClient(i);
  }
  else {
    close(newsock);
    Log(MCS_ERROR( MSG_NO_MORE_CLIENT_ALLOWED ));
  }
}



void mcs::Server::killClient(int i)
{
  if (i>=0 && i<env->max_users)
    if (pClient[i]) {
      Log(MCS_OK( MSG_KILLING_CLIENT, i ));
      pClient[i]->stop(); //TESTING  //pClient[i]->exit(false);
    }
}


void mcs::Server::killAllClients()
{
  if (pClient) {
    Log(MCS_OK( MSG_KILLING_ALL_CLIENTS ));
    for (int i=0; i<env->max_users; i++)
      killClient(i);
  }
}


RecordSet* mcs::Server::getAll_ClientInfo()
{
  RecordSet* ret = new RecordSet();

  for (int i=0; i<env->max_users; i++)
    if (pClient[i])
      ret->insert(pClient[i]->info());

  return ret;
}

//vector<string> mcs::Server::clientsInfo()
//{
//    vector<string> ret;
//    string s;
//    static string tab = "\t";
//    struct ClientInfo* info;
//
//
//    for (int i=0; i<env->max_users; i++) {
//	info = NULL;
//	if (pClient[i])
//		info = pClient[i]->clientInfo();
//
//	if (info) {
//	    s  = itos(info->id)              + tab;
//	    s += info->ipaddress             + tab;
//	    s += info->hostname              + tab;
//	    s += info->timeConnetcted        + tab;
//	    s += info->username              + tab;
//	    s += info->lastCommand           + tab;
//	    s += info->timeLastCommand       + tab;
//	    s += itos(info->commandExecuted) + tab;
//	    s += itos(info->logged);
//	    ret.push_back(s);
//	}
//    }
//    return ret;
//}



UserThread* mcs::Server::newUserThread(int lID, int newsock)
{
  return new UserThread(this, lID, newsock);
}

LocalThread* mcs::Server::newLocalThread()
{
  return new LocalThread(this);
}


void mcs::Server::hk_newClient(int i)
{
    if (cb_newClient)
	(*(cb_newClient))(i);
}








mcs::ClientInfo::ClientInfo() :  Record()
{
  addField(new Data (NULL, INT,    "id"                 ));
  addField(new Data (NULL, STRING, "ipaddress"      , 15));
  addField(new Data (NULL, STRING, "hostname"       , 30));
  addField(new Data (NULL, TIME,   "timeConnected"      ));
  addField(new Data (NULL, STRING, "username"       , 15));
  addField(new Data (NULL, STRING, "lastCommand"    , 30));
  addField(new Data (NULL, TIME,   "timeLastCommand"    ));
  addField(new Data (NULL, INT,    "commandExecuted"    ));
  addField(new Data (NULL, TINY,   "logged"             ));
}


Data& mcs::ClientInfo::id()               { return operator[](0); }
Data& mcs::ClientInfo::ipaddress()	  { return operator[](1); }
Data& mcs::ClientInfo::hostname()	  { return operator[](2); }
Data& mcs::ClientInfo::timeConnetcted()   { return operator[](3); }
Data& mcs::ClientInfo::username()	  { return operator[](4); }
Data& mcs::ClientInfo::lastCommand()      { return operator[](5); }
Data& mcs::ClientInfo::timeLastCommand()  { return operator[](6); }
Data& mcs::ClientInfo::commandExecuted()  { return operator[](7); }
Data& mcs::ClientInfo::logged()           { return operator[](8); }



//mcs::ClientsInfo_set::ClientsInfo_set(UserThread** pClient, int max_users) :
//  RecordSet()
//{
//  this->pClient = pClient;
//  this->max_users = max_users;
//  lastpos = 0;
//
//  ClientsInfo tmp;
//  metarec() = tmp;
//  init(0, true, true);
//}
//
//
//bool mcs::ClientsInfo_set::fetch(unsigned int newpos, bool random)
//{
//  ClientsInfo tmp;
//
//  for (int i=lastpos; i<max_users; i++)
//    if (pClient[i]) {
//      rec() = pClient[i]->clientsInfo();
//      rec().emptyName();
//      lastpos = i;
//      return true;
//    }
//
//  return false;
//}

