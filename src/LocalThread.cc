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


//--------------------------------------------------------
mcs::LocalThread::LocalThread(Thread* parent) :
  BaseThread(parent, "LLL"),
  recv(true)
{
  MCS_DEBUG_SETUP(0, "LocalThread");
  Log(MCS_OK( MSG_THREAD_CREATE, "LocalThread" ));

  client = serverClients();
}


mcs::LocalThread::~LocalThread()
{
  Log(MCS_OK( MSG_THREAD_DESTROY, "LocalThread" ));
}


bool mcs::LocalThread::userID2clientID(int userid, int* cid)
{
  int i;

  MCS_DEBUG_ENTER(<< this->id());

  for (i=0; i<env->max_users; i++) {
    if (client[i]) {
      if (client[i]->userID() == userid) {
	*cid = i;
	return true;
      }
    }
  }

  MCS_DEBUG_LEAVE(<< this->id());
  return false;
}


void mcs::LocalThread::dataDispatcher()
{
    int userid, cid;

    MCS_DEBUG_ENTER(<< this->id());

    while (env->server->dispatch.count() > 0) {
      Data d = env->server->dispatch.pop();
      userid = d.getDestID();

      if (userid == MCS_ID_LOCAL)
	recv.addField(d);
      else
	if (userID2clientID(userid, &cid)) {
	  client[cid]->recv.addField(d);
	  client[cid]->wakeUpClient();
	}
	else
	  Log(MCS_WARN( MSG_UNKNOWN_USERID, userid));
    }

    MCS_DEBUG_LEAVE(<< this->id());
}


void mcs::LocalThread::run()
{
  try{

    while (state() == MCS_STATE_RUNNING) {
      while (env->server->dispatch.count() == 0)
	sleep_ms(500);

      dataDispatcher();
    }
  }
  catch(Event e) {
    Log(e);
    if (e.type() == FATAL)
      return;
  }
}



void mcs::LocalThread::hk_log(UserThread* p, Event e)
{
    if (env->server->cb_log)
        (*(env->server->cb_log))(p, e);

    if (env->server->cbwa_log)
        (*(env->server->cbwa_log))();
}


UserThread** mcs::LocalThread::serverClients()
{
    //Friendness of LocalThread to Server
    return ((Server* ) parent())->pClient;
}
