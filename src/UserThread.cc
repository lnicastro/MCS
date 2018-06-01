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

// LN
#include <iomanip>
//#include "malloc.h"
#if HAVE_MALLOC_H
#include <malloc.h>
#endif

//LN-darwin
#ifdef __APPLE__
#include <sys/stat.h>
//# define THD_MKDIR_MODE (S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)
//# define SCANDIR_WANTS_CONST
#endif


void printMallinfo(int pos = 0) {
#if HAVE_MALLOC_H
  struct mallinfo mi;
  mi = mallinfo();

  cout << "\n\nDynamic memory (" << pos << "): " << mi.arena << endl << endl;

  cout << setw(10)<< mi.arena    <<" int arena;    /* non-mmapped space allocated from system */\n"
       << setw(10)<< mi.ordblks  <<" int ordblks;  /* number of free chunks */\n"
       << setw(10)<< mi.smblks   <<" int smblks;   /* number of fastbin blocks */\n"
       << setw(10)<< mi.hblks    <<" int hblks;    /* number of mmapped regions */\n"
       << setw(10)<< mi.hblkhd   <<" int hblkhd;   /* space in mmapped regions */\n"
       << setw(10)<< mi.usmblks  <<" int usmblks;  /* maximum total allocated space */\n"
       << setw(10)<< mi.fsmblks  <<" int fsmblks;  /* space available in freed fastbin blocks */\n"
       << setw(10)<< mi.uordblks <<" int uordblks; /* total allocated space */\n"
       << setw(10)<< mi.fordblks <<" int fordblks; /* total free space */\n"
       << setw(10)<< mi.keepcost <<" int keepcost; /* top-most, releasable (via malloc_trim) space */\n\n";
#else
  cout << "\n\nNo dynamic memory info for this system.\n";
#endif
}



string mcs::UserThread::wpath(string fn)
{
  string s;

  fn = subst(fn, " "      , ""); // blanks
  fn = subst(fn, "\\.\\." , ""); // ..
  fn = subst(fn, "\\\\"   , ""); // backslash
  fn = subst(fn, "\\\""   , ""); // double quotes
  fn = subst(fn, "\\\'"   , ""); // single quotes

  //s = env->path;
  s = ".";
  if (env->cl_work) {
      if (env->cl_work_cid)
	  s += string("/U") + string(tid());
      else
	  s += string("/") + user;

      s += "/";
  }

  return s + fn;
}



RetValue mcs::UserThread::Send(Event e, bool log)
{
    sock->print(e.codemsg());
    e.lmsg = " >" + e.lmsg;  //Friendness of UserThread to Event class
    if (log) Log(e);
    return e.type();
}



RetValue mcs::UserThread::Send(Data& data)
{
//    void* buf = NULL;
//    unsigned int size = data.prepareBuffer((void**) &buf);
//    Serializable sss(buf, size);
//	
//    Send(MCS_OK( MSG_SENDDATA_START, 1 )); //1 is for a Data object
//    sock->sendData(&sss);
//    Send(MCS_OK( MSG_DATA_STOP ));
//    free(buf);
//    return OK;

    Send(MCS_OK( MSG_SENDDATA_START, 1 )); //1 is for a Data object
    sock->sendData(&data);
    Send(MCS_OK( MSG_DATA_STOP ));
    return OK;
}


RetValue mcs::UserThread::Send(Record& vec)
{
//    void* buf = NULL;
//    unsigned int size = vec.prepareBuffer((void**) &buf);
//    Serializable sss(buf, size);
//
//    Send(MCS_OK( MSG_SENDDATA_START, 2 )); //2 is for a Record object
//    sock->sendData(&sss);
//    Send(MCS_OK( MSG_DATA_STOP ));
//    free(buf);
//    return OK;

    Send(MCS_OK( MSG_SENDDATA_START, 2 )); //2 is for a Record object
    sock->sendData(&vec);
    Send(MCS_OK( MSG_DATA_STOP ));
    return OK;
}


RetValue mcs::UserThread::Send(string filename, string path)
{
    Send(MCS_OK( MSG_SENDFILE_START, filename.csz ));
    Serializable sss(path, env->cl_chunksize);
    sock->sendData(&sss);
    Send(MCS_OK( MSG_FILE_STOP ));
    return OK;
}


RetValue mcs::UserThread::Send(vector<string> vec)
{
    for (unsigned int ui=0; ui<vec.size(); ui++)
	Send(MCS_OK( MSG_OUT, vec[ui].c_str() ), false);

  return OK;
}



#if ENABLE_MYSQL
RetValue mcs::UserThread::sendQueryRes()
{
  RetValue ret = OK;

  if (query->gotRecordSet()) {
    try {
      //printMallinfo(1);
      Send(MCS_OK( MSG_SENDFILE_START, "query.out" ));
      //printMallinfo(2);
      query->Result2Ascii(wpath("query.out"));
      //printMallinfo(3);
      Serializable file(wpath("query.out"), env->cl_chunksize);
      //printMallinfo(4);
      sock->sendData(&file);
      //printMallinfo(5);
    }
    catch (Event& e) {
	if (e.type() == FATAL) throw;
	ret = Send(e);
    }
    //printMallinfo();
    return Send(MCS_( ret, MSG_DATA_STOP )) ;
  }
  else return Send(MCS_WARN( MSG_NO_RESULT ));
}
#endif




void mcs::UserThread::prompt(RetValue ret)
{
    switch (ret) {
	case OK:
	    sock->print(MCS_PROMPT_OK);      break;
	case WARN:
	    sock->print(MCS_PROMPT_WARN);    break;
	case ERROR:
	    sock->print(MCS_PROMPT_ERROR);   break;
	default:
	    throw MCS_FATAL(MSG_UNEXPECTED);
    }
}


void mcs::UserThread::run()
{
  string cmd;
  bool first = true;
  RetValue ret = OK;

  set_cancel_state(false);

  for (;;) {
    try {
      if (first) {
	//A message to the user must be sent inside hk_connect
	ret=hk_connect();
	if (ret != OK) return;
	
	sock = new Socket(csocket, env->timeout, 1000, env->ssl_ctx);

	linfo.ipaddress() = sock->ipaddress();
	linfo.hostname() = sock->hostname();
	
	Send(MCS_OK( MSG_WELCOME, env->localhost.csz,
		     env->appname.csz, env->appvers.csz));
	first = false;
      }
      else {
	set_cancel_state(true);
	cmd = sock->getline();
	set_cancel_state(false);
	
	//Synchronize with wakeUpClient()
	syn.enter();
	ret = exec(cmd);
	syn.leave();

	//printMallinfo();
      }
      prompt(ret);
    }
    catch(Event e) {
      //Unlock the mutex
      while (syn.count())
	syn.leave();

      ret = e.type();

      if (e.type() == FATAL) {
	//If the client gives a "bye" command a FATAL exception with
	//code BYE will be thrown.
	if (e.code() == MSG_BYE) {
	
	  hk_disconnect();
	
	  //Wait until the client closes the connection, this is
	  //necessary to prevent "Address already in use" errors. The
	  //try..catch block is necessary because getline() will throw
	  //an error when the socket close.
	  try {cmd = sock->getline(); } catch(...) {}
	}
	else {
	  Log(e);
	}
	
	Log( MCS_OK(MSG_USERTHREAD_TERMINATING) );
	return;
      }
    }
  }
}



RetValue mcs::UserThread::sendStrings(string str)
{
  return Send(split(str, "\n"));
}


void mcs::UserThread::wakeUpClient(Event* e)
{
  //syn.enter();
  if (state() == MCS_STATE_RUNNING) {
    if (e)
      Send(*e);
    else
      Send(MCS_OK( MSG_WAKE_UP) );
  }
  //syn.leave();
}



#define BEGIN_CMD(A)                             \
if ((!cmd_executed)   &&   (cmd.cmpCmd(A))){     \
      cmd_executed=true;                         \


#define BEGIN_CMD_LOGIN(A)                       \
      BEGIN_CMD(A)                               \
      if (! loginok) throw MCS_ERROR(MSG_NOT_LOGIN);


#define CMD_HELP(A)                              \
      if (cmd.givenOpt(MCS_OPT_HELP)) {          \
          sendStrings(A);                        \
          goto lbl_cmdexecuted;                  \
      }


#define END_CMD goto lbl_cmdexecuted; }


RetValue mcs::UserThread::exec(string scmd, string _pars)
{
  int i;
  unsigned int ui;
  string fn, s, tmp;
  bool cmd_executed=false;
  unsigned int size=0;

  bool opt_sqascii;
  bool opt_sqfits ;
  bool opt_force  ;
  bool opt_werr   ;
  static string bl = " ";

  CommandParser cmd;
  RetValue ret=OK;
  RetValue sub=OK;

  batchlevel++;

  try {

  scmd += " " + _pars;
  cmd.parseCmd(scmd);
  if (cmd.cmd().empty()) return OK;

  opt_sqascii = cmd.givenOpt(MCS_OPT_SAVEQUERYASCII);
  opt_sqfits  = cmd.givenOpt(MCS_OPT_SAVEQUERYFITS);
  opt_force   = cmd.givenOpt(MCS_OPT_FORCE);
  opt_werr    = cmd.givenOpt(MCS_OPT_ALL_ERRORS);

  if (batchlevel > 0)
    Send(MCS_OK( MSG_BATCH_START, batchlevel, cmd.cline().csz ));

  if (cmd.cmpCmd(MCS_CMD_PASSWORD)) {
      Log(MCS_OK( MSG_LOG_COMMAND, "*** PASSWORD ***" ));
  }
  else if (cmd.cmpCmd(MCS_CMD_NOP))  //Don't log NOP commands.
    ;
  else if (cmd.cmpCmd(MCS_CMD_CLIENT_INFO))
    Log(MCS_OK( MSG_LOG_COMMAND, cmd.cline().csz ));
  else {
    Log(MCS_OK( MSG_LOG_COMMAND, cmd.cline().csz ));
    linfo.lastCommand() = cmd.cline().substr(0, linfo.lastCommand().maxLength());
    linfo.timeLastCommand().settimenow();
    linfo.commandExecuted() = linfo.commandExecuted().ival() + 1;
  }



#if ! ENABLE_CFITSIO
  if (opt_sqfits) {
      ret=Send(MCS_WARN( MSG_FITS_NOT_COMPILED ));
      opt_sqfits = false;
  }
#endif

#if ! ENABLE_MYSQL
  if ((opt_sqfits)   ||   (opt_sqascii)) {
      cmd_executed = true;
      throw MCS_ERROR(MSG_MYSQL_DISABLED);
  }
#endif

  //Custom user routine
  if (! cmd_executed)
      sub = hk_exec(&cmd, cmd_executed);
  if (sub > ret) ret = sub;


  //................................................................
  BEGIN_CMD(MCS_CMD_NOP);
  END_CMD;


  //................................................................
  BEGIN_CMD(MCS_CMD_CID);
  CMD_HELP(MCS_CMD_CID_HELP);
  Send(MCS_OK( MSG_CLIENT_ID, id(), env->cl_chunksize));
  END_CMD;

  //................................................................
  BEGIN_CMD(MCS_CMD_CLIENT_INFO);
  CMD_HELP(MCS_CMD_CLIENT_INFO_HELP);
  setActiveRS(env->server->getAll_ClientInfo(), true);
  END_CMD;


  //................................................................
  BEGIN_CMD(MCS_CMD_CLOSECLIENT);
  CMD_HELP(MCS_CMD_CLOSECLIENT_HELP);
  Send(MCS_OK( MSG_BYE ));
  throw MCS_FATAL(MSG_BYE);
  END_CMD;


  //................................................................
  BEGIN_CMD(MCS_CMD_USERNAME);
  CMD_HELP(MCS_CMD_USERNAME_HELP);
  if (cmd.argc() >= 1) {
      user=cmd.arg(0).sval();
      linfo.username() = user;
  }
  Send(MCS_OK( MSG_WELCOMEUSER, user.csz ));
  END_CMD;


  //................................................................
  BEGIN_CMD(MCS_CMD_PASSWORD);
  CMD_HELP(MCS_CMD_PASSWORD_HELP);
  if (cmd.argc() >= 1) pass=cmd.arg(0).sval();
  Send(MCS_OK( MSG_PASSWORD ));
  END_CMD;


  //................................................................
  BEGIN_CMD(MCS_CMD_DBNAME);
  CMD_HELP(MCS_CMD_DBNAME_HELP);
  if (cmd.argc() >= 1) dbname=cmd.arg(0).sval();
  Send(MCS_OK( MSG_DBNAME, dbname.csz ));
  END_CMD;


  //................................................................
#ifdef MCS_HAVE_DBH_CMD
  BEGIN_CMD(MCS_CMD_DBHOST);
  CMD_HELP(MCS_CMD_DBHOST_HELP);
  if (cmd.argc() >= 1) dbhost=cmd.arg(0).sval();
  Send(MCS_OK( MSG_DBHOST, dbhost.csz ));
  END_CMD;
#endif


  //................................................................
  BEGIN_CMD(MCS_CMD_DBCONNECT);
  CMD_HELP(MCS_CMD_DBCONNECT_HELP);
  if (loginok) throw MCS_WARN(MSG_ALREADY_CONNECTED);

  if (env->cl_custom_auth) {
      ret = hk_auth(grants, loginok);
      if (ret == OK)
	  Send(MCS_OK( MSG_CONNECTED, " " ));
  }
  else {
#if ENABLE_MYSQL
      //Check for the presence of values
      if      (user.empty()  ) throw MCS_ERROR(MSG_MISSING_FIELD, "username");
      else if (pass.empty()  ) throw MCS_ERROR(MSG_MISSING_FIELD, "password");
      else if (dbname.empty()) throw MCS_ERROR(MSG_MISSING_FIELD, "db name" );
      else if (dbhost.empty()) throw MCS_ERROR(MSG_MISSING_FIELD, "db host" );
      else {
	  db.connect(user, pass, dbname, dbhost);
	  query = new Query(&db, false);
	  if (env->cl_read_grants) {
	      query->query("select grants from users where username='"
			   + user + "'", true);
	      if (query->gotRecordSet() && query->nRows() == 1 ) {
		  grants = query->rec()[0].ival();
		  Log(MCS_OK( MSG_USER_HAS_GRANTS, user.csz, grants ));
	      }
	      else
		  throw MCS_FATAL(MSG_GRANTS_CANTGET_GRANTS);
	  }
	  else
	      grants=MCS_GRANT_ALL;
      }
#else
      grants = MCS_GRANT_ALL;
#endif

      if (! (grants & MCS_GRANT_LOGIN))
	  throw MCS_ERROR(MSG_GRANTS_LOGIN);
      else {
	  loginok=true;
	  Send(MCS_OK( MSG_CONNECTED, dbname.csz ));
      }
  }

  linfo.logged() = loginok;

  if (env->cl_work) {
    unsigned int size;
    if (! File_Dir_Exist(wpath(), size))
      mkDir(wpath(), MCS_MKDIR_PROTECT);
  }

  if (env->cl_autoexec)
      sub=exec(MCS_CMD_EXEC, "auto");
  END_CMD;


  //................................................................
//  BEGIN_CMD_LOGIN(MCS_CMD_CLOSE_DB_CONN);
//#if ENABLE_MYSQL
//  db.close();
//#endif
//  Send(MCS_OK( MSG_CLOSE_DB_CONN ));
//  loginok=false;
//  END_CMD;


#if ENABLE_MYSQL
  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_QUERY);
  CMD_HELP(MCS_CMD_QUERY_HELP);
  if (! env->cl_have_db) throw MCS_ERROR(MSG_DB_ERR_NOTACTIVATED);
  if (! (grants & MCS_GRANT_QUERY)) throw MCS_ERROR(MSG_GRANTS_QUERY);

  s=cmd.allargs();
  query->query(s, true);

  if (query->gotRecordSet()) {
      Send(MCS_OK( MSG_DB_QRY_OK ));
      setActiveRS(query, false);

      if (opt_sqfits) {
#if ENABLE_CFITSIO
	//query->Result2Fits(wpath(fnout));
#endif
	opt_sqfits=false;
      }
      else if (opt_sqascii) {
	  query->Result2Ascii(wpath(fnout));
	  opt_sqascii=false;
      }
  }
  else
      Send(MCS_OK( MSG_DB_ROWSAFF, query->nAffectedRows() ));
  END_CMD;


  ////................................................................
  //BEGIN_CMD_LOGIN(MCS_CMD_ROWS);
  //CMD_HELP(MCS_CMD_ROWS_HELP);
  //if (! env->cl_have_db) throw MCS_ERROR(MSG_DB_ERR_NOTACTIVATED);
  //if (! query->gotRecordSet()) throw MCS_WARN(MSG_NO_RESULT);
  //
  //if (query->gotRecordSet())
  //    Send(MCS_OK( MSG_DB_ROWSAFF, query->nRows() ));
  //END_CMD;


  ////................................................................
  //BEGIN_CMD_LOGIN(MCS_CMD_FIELDS);
  //CMD_HELP(MCS_CMD_FIELDS_HELP);
  //if (! env->cl_have_db) throw MCS_ERROR(MSG_DB_ERR_NOTACTIVATED);
  //if (! query->gotRecordSet()) throw MCS_WARN(MSG_NO_RESULT);
  //
  //Send( query->ExecutionDetails() );
  //END_CMD;


  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_SENDQUERYRES);
  CMD_HELP(MCS_CMD_SENDQUERYRES_HELP);
  if (! env->cl_have_db) throw MCS_ERROR(MSG_DB_ERR_NOTACTIVATED);
  if (! (grants & MCS_GRANT_GET)) throw MCS_ERROR(MSG_GRANTS_GET);
  if (! query->gotRecordSet()) throw MCS_WARN(MSG_NO_RESULT);

  sub=sendQueryRes();
  END_CMD;


  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_RECORD);
  CMD_HELP(MCS_CMD_RECORD_HELP);
  if (! env->cl_have_db) throw MCS_ERROR(MSG_DB_ERR_NOTACTIVATED);
  if (! (grants & MCS_GRANT_GET)) throw MCS_ERROR(MSG_GRANTS_GET);
  if (! rs) throw MCS_WARN(MSG_NO_RESULT);

  unsigned int pos = cmd.arg(0).ival();
  if (pos == rs->pos())
    Send(rs->rec());
  else if (pos == rs->pos()+1) {
    if (rs->setNext())
      Send(rs->rec());
    else
      Send( MCS_OK(MSG_FETCH_EOF) );
  }
  else {
    rs->setPos(pos);
    Send(rs->rec());
  }
  END_CMD;


  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_TABLELIST);
  CMD_HELP(MCS_CMD_TABLELIST_HELP);
  if (! env->cl_have_db) throw MCS_ERROR(MSG_DB_ERR_NOTACTIVATED);

  s = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = '"
    + dbname + "' AND TABLE_TYPE = 'BASE TABLE'";
  exec(MCS_CMD_QUERY + bl + s);
  //query->readTableList();
  //Send( query->tableList );
  END_CMD;


  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_TABLEINFO);
  CMD_HELP(MCS_CMD_TABLEINFO_HELP);
  if (! env->cl_have_db) throw MCS_ERROR(MSG_DB_ERR_NOTACTIVATED);
  if ((cmd.argc() < 1)   ||   (cmd.arg(0).sval().empty()))
      throw MCS_WARN(MSG_MISSING_FIELD, "table name");

  s = "SELECT * FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '"
    + dbname + "' AND TABLE_NAME = '" + cmd.arg(0).sval() + "'";

  exec(MCS_CMD_QUERY + bl + s);
  //exec(MCS_CMD_QUERY + bl + "DESCRIBE " + cmd.arg(0).sval());
  //Send( query->tableInfo(cmd.arg(0).sval()) );
  END_CMD;

#endif  //ENABLE_MYSQL



  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_EXEC);
  CMD_HELP(MCS_CMD_EXEC_HELP);
  if (! (grants & MCS_GRANT_SCRIPTS)) throw MCS_ERROR(MSG_GRANTS_SCRIPT);

  s = fn = cmd.arg(0).sval();
  i = chkExt(fn);

  if ((i == MCS_FT_BIN)   ||   (i == MCS_FT_SCRIPT)) {
#if ENABLE_DEBUG
    tmp = fn + " " + cmd.line_afterToken(1);
    Log(MCS_OK( MSG_SCRIPT_START, tmp.csz ));
#endif
    Send(MCS_OK( MSG_SCRIPT_START, s.csz ));

    i = spawn(fn, cmd.line_afterToken(1),
	      wpath(), itos(id()),
	      user, pass, dbname,
	      wpath(fnout), wpath(fnerr));
    if (i != 0)
      throw MCS_WARN(MSG_SCRIPT_ERROR, i);

    Send(MCS_OK( MSG_SCRIPT_STOP, s.csz ));
  }
  else {
    string c="";
    bool had_warn = false;
    bool had_err  = false;
    bool stop = false;
    vector<string> vec = cmd.replPars_onFile(fn);
    if (i == MCS_FT_BATCH)
      Send(MCS_OK( MSG_BATCH_START, batchlevel,  s.csz ));
    if (i == MCS_FT_SQL)  {
#if ENABLE_MYSQL
      Send(MCS_OK( MSG_SQL_START, s.csz )); c = MCS_CMD_QUERY;
#else
      throw MCS_ERROR(MSG_DB_NOT_COMPILED);
#endif
    }

    for (ui=0; ui<vec.size(); ui++) {

      sub=exec(c.csz, vec[ui]);

      switch(sub) {
      case OK:
	break;
      case WARN:
	had_warn = true;
	if (opt_werr) stop = true;
	break;
      case ERROR:
	had_err  = true;
	if (! opt_force) stop = true;
	break;
      case FATAL:
	throw MCS_FATAL(MSG_UNEXPECTED);
	break;  //Is unexpected because an exception should be thrown
      }

      if (stop) break;
    }

    if (had_err)       ret = ERROR;
    else if (had_warn) ret = WARN;
    else               ret = OK;

    if (i == MCS_FT_BATCH)  Send(MCS_OK( MSG_BATCH_STOP, batchlevel ));
    if (i == MCS_FT_SQL) {
      Send(MCS_OK( MSG_SQL_STOP ));

      if ((ret == OK)   &&   query->gotRecordSet()) {
	if (opt_sqfits) {
#if ENABLE_CFITSIO
	  //query->Result2Fits(wpath(fnout));
#endif
	  opt_sqfits=false;
	}
	else if (opt_sqascii) {
	  query->Result2Ascii(wpath(fnout));
	  opt_sqascii=false;
	}
      }
    }
  }
  END_CMD;


  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_GET);
  CMD_HELP(MCS_CMD_GET_HELP);
  if (! (grants & MCS_GRANT_GET)) throw MCS_ERROR(MSG_GRANTS_GET);

  fn = wpath(fnout);
  s  = fnout;
  if (cmd.argc() == 1) {
      fn = wpath(cmd.arg(0).sval());
      s  = cmd.arg(0).sval();
  }

  ret = Send(s, fn);
  END_CMD;



//This has been commented because data in the "send" vector are automatically
//sent after each command.
//  //................................................................
//  BEGIN_CMD_LOGIN(MCS_CMD_GETDATA);
//  CMD_HELP(MCS_CMD_GETDATA_HELP);
//  if (! (grants & MCS_GRANT_GET)) throw MCS_ERROR(MSG_GRANTS_GET);
//  if (cmd.argc() < 1) throw MCS_ERROR(MSG_MISSING_FIELD, "object index");
//
//  ui = cmd.arg(0).ival();
//  if (ui < send.count()) {
//      tbuf = NULL;
//      size = send.pop(ui).prepareBuffer((void**) &tbuf);
//      Serializable sss(tbuf, size);
//
//      Send(MCS_OK( MSG_SENDDATA_START, 1 )); //1 is for a Data object
//      sock->sendData(&sss);
//      Send(MCS_OK( MSG_DATA_STOP ));
//      free(tbuf);
//  }
//  else
//      throw MCS_ERROR(MSG_INDEX_OUT_RANGE, ui, send.count());
//  END_CMD;


  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_PUT);
  CMD_HELP(MCS_CMD_PUT_HELP);
  if (! (grants & MCS_GRANT_PUT)) throw MCS_ERROR(MSG_GRANTS_PUT);
  if (cmd.argc() < 1) throw MCS_ERROR(MSG_MISSING_FIELD, "file name, size");

  s  = cmd.arg(0).sval();
  if (strchr(s.c_str(), '/')) {  //A path is specified
    if (s[0] == '/')
      throw MCS_ERROR( MSG_NO_ABSOLUTE_PATH, s );

    if (strstr(s.c_str(), ".."))
      throw MCS_ERROR( MSG_NO_DOUBLEDOT_PATH, s );

    string subdir = wpath(s);
    unsigned int ui = subdir.rfind('/');
    subdir.resize(ui);
    if (! File_Dir_Exist(subdir, ui))
      mkDir(subdir, MCS_MKDIR_PROTECT);
  }
  fn = wpath(s);
  size = cmd.arg(1).ulval();

  Send(MCS_OK( MSG_RECVFILE_START, s.csz ));
  sock->recvData(fn);
  ret = Send(MCS_OK( MSG_FILE_STOP, s.csz ));
  END_CMD;


  //................................................................
  BEGIN_CMD_LOGIN(MCS_CMD_PUTDATA);
  CMD_HELP(MCS_CMD_PUTDATA_HELP);
  if (! (grants & MCS_GRANT_PUT)) throw MCS_ERROR(MSG_GRANTS_PUT);
  if (cmd.argc() < 1) throw MCS_ERROR(MSG_MISSING_FIELD, "size");

  size = cmd.arg(0).ulval();
  Send(MCS_OK( MSG_RECVDATA_START ));
  char* tbuf = NULL;
  size = sock->recvData(&tbuf, 0);

  Data d(tbuf);
  d.setSourceID(MCS_ID_CLIENT);
  d.setDestID(userID());
  recv.addField(d);

  Send(MCS_OK( MSG_DATA_STOP ));

  if (cmd.givenOpt(MCS_OPT_LOOP)) {
      Send(MCS_OK( MSG_COPYING_DATA_OBJECTS, "send Record" ));
      send.addField(d);
  }

  free(tbuf);
  END_CMD;


  //................................................................
  if (! cmd_executed)
      throw MCS_ERROR(MSG_SYNTAX, cmd.cline().csz);


  lbl_cmdexecuted:
  ; //Continue from here if the command has been excuted


//Eventually send objects in the send Record
  if (ret == OK)
      while (send.count()) {
	  Data d = send.pop();
	  Send(d);
      }

  hk_postexec(&cmd, ret);

  if (batchlevel > 0)
      Send(MCS_OK( MSG_BATCH_STOP, batchlevel ));


  }   // try
  catch(Event e) {
      if (e.type() != FATAL)
	  ret = Send(e);
      else
	  throw e;
  }

  batchlevel--;
  return ret;
}



mcs::UserThread::UserThread(Thread* parent, int lID, int newsock) :
  BaseThread(parent, lID),
  syn(),
  recv(true)
{
  Log(MCS_OK( MSG_THREAD_CREATE, "UserThread" ));
  //printMallinfo();

  sock = NULL;

  syn.synchronize(true);

  csocket = 0;
  if (newsock) {
    csocket = newsock;
  }

  loginok=false;
  grants=0;
  batchlevel=-1;

  fnout="out";
  fnerr="err";

  dbhost="localhost";
  dbname=env->appname;

  linfo.id() = lID;
  linfo.timeConnetcted().settimenow();

  luserid = MCS_ID_UNKNOWN;

#if ENABLE_MYSQL
  if (env->cl_have_db)
      query = NULL;
#endif

  rs = NULL;
}


void mcs::UserThread::setActiveRS(RecordSet* rs, bool delWhenFinished)
{
  if ( (this->rs)   &&   (deleters) )
    delete this->rs;

  this->rs = rs;
  deleters = delWhenFinished;

  Record* tmp = rs->prepRecToSend();

  Send(*tmp);
  delete tmp;
  Send( MCS_OK( MSG_NEW_RECORDSET ) );
}


mcs::UserThread::~UserThread()
{
  if ( (this->rs)   &&   (deleters) )
    delete rs;

  if (env->cl_work   &&   env->cl_clean_logout)
    rmDir(wpath());

  if (sock) {
    delete sock;
    sock = NULL;
  }

#if ENABLE_MYSQL
  if (env->cl_have_db)
    try { if (query) delete query;  }
    catch (Event &e) {}
#endif

  //printMallinfo();

  Log(MCS_OK( MSG_THREAD_DESTROY, "UserThread" ));
}



ClientInfo& mcs::UserThread::info()
{
  return linfo;
}



RetValue mcs::UserThread::hk_exec(CommandParser* cmd, bool& cmd_executed)
{
    RetValue ret = OK;
    cmd_executed = false;

    if (env->server->cb_exec)
	ret = (*(env->server->cb_exec))(cmd, cmd_executed);

    if (env->server->cbwa_exec)
	(*(env->server->cbwa_exec))();

    return ret;
}

void mcs::UserThread::hk_postexec(CommandParser* cmd, RetValue ret)
{
    if (env->server->cb_postexec)
	(*(env->server->cb_postexec))(cmd, ret);

    if (env->server->cbwa_postexec)
	(*(env->server->cbwa_postexec))();
}

RetValue mcs::UserThread::hk_connect()
{
    RetValue ret = OK;

    if (env->server->cb_connect)
	ret = (*(env->server->cb_connect))();

    if (env->server->cbwa_connect)
	(*(env->server->cbwa_connect))();

  return ret;
}

void mcs::UserThread::hk_disconnect()
{
    if (env->server->cb_disconnect)
        (*(env->server->cb_disconnect))();
}


RetValue mcs::UserThread::hk_auth(int& grants, bool& loginok)
{
    RetValue ret = OK;
    grants = MCS_GRANT_ALL;
    loginok = true;

    if (env->server->cb_auth)
	ret = (*(env->server->cb_auth))(grants, loginok);

    if (env->server->cbwa_auth)
	(*(env->server->cbwa_auth))();

    return ret;
}


Server* mcs::UserThread::parent()
{
  return env->server;
}


string mcs::UserThread::userName()
{
  string ret;
  ret = user;
  return ret;
}



int mcs::UserThread::userID()
{
  return luserid;
}


void mcs::UserThread::send2OtherThread(Data& d, int dest)
{
  d.setSourceID(userID());
  d.setDestID(dest);
  env->server->dispatch.addField(d);
}

