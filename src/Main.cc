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


#include <signal.h>
#include <mcs.hh>
using namespace mcs;

static bool sigreceived;

void handle_signal(int signum)
{
  cout << "Signal " << signum << " received" << endl;
  sigreceived = true;
}


void set_signal()
{
  signal(SIGINT,  handle_signal);	/* Interrupt (ANSI).    */
  signal(SIGQUIT, handle_signal);	/* Quit (POSIX).        */
  signal(SIGTERM, handle_signal);	/* Termination (ANSI).  */
  signal(SIGPIPE, handle_signal);       /* Broken pipe          */
}


void reset_signal()
{
  signal(SIGINT,  SIG_DFL);	/* Interrupt (ANSI).    */
  signal(SIGQUIT, SIG_DFL);	/* Quit (POSIX).        */
  signal(SIGTERM, SIG_DFL);	/* Termination (ANSI).  */
  signal(SIGPIPE, SIG_DFL);     /* Broken pipe          */
}


//Env* mcs::mcsStart(string appname, string inipath,
//		   Server* (*cb_newServer)(Env*))
//{
//  Env* env = NULL;
//
//  //We handle signals so the server can close all files and
//  //connections still open.
//  set_signal();
//  sigreceived = false;
//
//  try {
//    if (inipath.empty())
//      env = new Env(appname);
//    else
//      env = new Env(appname, inipath);
//
//    if (cb_newServer) {
//      env->server = (*cb_newServer)(env);
//    }
//    else
//      env->server = new Server(env);
//
//    //Start the server.
//    env->server->start();
//  }
//
//  catch (Event e) {
//    cerr << e.msg() << endl;
//
//    if (env) delete env;
//    return NULL;
//  }
//
//  return env;
//}
//
//
//void mcs::mcsWait(Env* env)
//{
//  if (! env)
//    return;
//
//  //env->server->allowTermination();
//
//  while ( (! sigreceived)   &&   (env->server->state() <= MCS_STATE_RUNNING))
//    sleep_ms(500);
//	
//  env->server->stop();
//  delete env->server;
//
//  reset_signal();
//  delete env;
//}





Env* mcs::mcsStart(string appname, string inipath,
		   Server* (*cb_newServer)(Env*))
{
  Env* env = NULL;

  try {
    if (inipath.empty())
      env = new Env(appname);
    else
      env = new Env(appname, inipath);
    env-> server = NULL;

    if (chdir(env->path.c_str()) != 0)
      throw MCS_ERROR(MSG_DIRECTORY_NOT_FOUND, env->path);

    int ret = daemon(1, (env->cl_logstdout   ?   1   :   0));
    if (ret != 0)
      throw MCS_ERROR(MSG_UNEXPECTED);

    //We handle signals so the server can close all files and
    //connections still open.
    set_signal();
    sigreceived = false;

    if (cb_newServer)
      env->server = (*cb_newServer)(env);
    else
      env->server = new Server(env);

    //Start the server.
    env->server->start();

    //Wait for a signal or the server termination
    while ( (! sigreceived)   &&   (env->server->state() <= MCS_STATE_RUNNING))
      sleep_ms(500);
	
    env->server->stop();
    reset_signal();
  }
  catch (Event e) {
    cerr << e.msg() << endl;
  }

  if (env->server)  delete env->server;
  if (env)          delete env;

  return EXIT_SUCCESS;
}


