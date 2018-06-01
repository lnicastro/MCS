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




void mcs::Env::chkTree()
{
  string s = path;
  unsigned int size;

  if (! File_Dir_Exist(s, size)) {
      if (cl_work)
	mkDir(s);
      else
	  throw MCS_ERROR(MSG_DIRECTORY_NOT_FOUND, s.csz);
  }
}


void mcs::Env::LoadConf()
{
  //General values                                                 Default values
  appvers            = cnf->sval("MCS_Generic", "APPVERS"        , "0.0"                     );
  path               = cnf->sval("MCS_Generic", "PATH"           , Pwd()                     );
//LN-darwin
#ifdef __APPLE__
  interface          = cnf->sval("MCS_Generic", "INTERFACE"      , "lo0"                     );
#else
  interface          = cnf->sval("MCS_Generic", "INTERFACE"      , "lo"                      );
#endif
//interface          = cnf->sval("MCS_Generic", "INTERFACE"      , "127.0.0.1"               );
  port               = cnf->ival("MCS_Generic", "PORT"           , MCS_DEFAULT_PORT          );
  timeout            = cnf->ival("MCS_Generic", "TIMEOUT"        , MCS_DEFAULT_CLIENT_TIMEOUT);
  max_users          = cnf->ival("MCS_Generic", "MAX_USERS"      , MCS_DEFAULT_MAX_USERS     );
  db_user            = cnf->sval("MCS_Generic", "DB_USER"        , ""                        );
  db_pass            = cnf->sval("MCS_Generic", "DB_PASS"        , ""                        );
  db_name            = cnf->sval("MCS_Generic", "DB_NAME"        , ""                        );
  cl_local_kills_mcs = cnf->ival("MCS_Generic", "local_kills_mcs", 0                         );
  cl_have_db         = cnf->ival("MCS_Generic", "have_db"        , 0                         );
  cl_custom_auth     = cnf->ival("MCS_Generic", "custom_auth"    , 0                         );
  cl_logfile         = cnf->ival("MCS_Generic", "logfile"        , 0                         );
  cl_work            = cnf->ival("MCS_Generic", "work"           , 1                         );
  cl_work_cid        = cnf->ival("MCS_Generic", "work_cid"       , 0                         );
  cl_logstdout       = cnf->ival("MCS_Generic", "logstdout"      , 1                         );
  cl_createlocal     = cnf->ival("MCS_Generic", "createlocal"    , 1                         );
  cl_restartlocal    = cnf->ival("MCS_Generic", "restartlocal"   , 0                         );
  cl_clean_logout    = cnf->ival("MCS_Generic", "clean_logout"   , 0                         );
  cl_read_grants     = cnf->ival("MCS_Generic", "read_grants"    , 0                         );
  cl_autoexec        = cnf->ival("MCS_Generic", "autoexec"       , 0                         );
  cl_use_ssl         = cnf->ival("MCS_Generic", "use_ssl"        , 0                         );
  cl_chunksize       = cnf->ival("MCS_Generic", "chunksize"      , MCS_DEFAULTCHUNKSIZE      );

  //Application specific values
  appvers            = cnf->sval(appname      , "APPVERS"        , appvers           );
  path               = cnf->sval(appname      , "PATH"           , path              );
  interface          = cnf->sval(appname      , "INTERFACE"      , interface         );
  port               = cnf->ival(appname      , "PORT"           , port              );
  timeout            = cnf->ival(appname      , "TIMEOUT"        , (int) timeout     );
  max_users          = cnf->ival(appname      , "MAX_USERS"      , max_users         );
  db_user            = cnf->sval(appname      , "DB_USER"        , db_user           );
  db_pass            = cnf->sval(appname      , "DB_PASS"        , db_pass           );
  db_name            = cnf->sval(appname      , "DB_NAME"        , db_name           );
  cl_local_kills_mcs = cnf->ival(appname      , "local_kills_mcs", cl_local_kills_mcs);
  cl_have_db         = cnf->ival(appname      , "have_db"        , cl_have_db        );
  cl_custom_auth     = cnf->ival(appname      , "custom_auth"    , cl_custom_auth    );
  cl_logfile         = cnf->ival(appname      , "logfile"        , cl_logfile        );
  cl_work            = cnf->ival(appname      , "work"           , cl_work           );
  cl_work_cid        = cnf->ival(appname      , "work_cid"       , cl_work_cid       );
  cl_logstdout       = cnf->ival(appname      , "logstdout"      , cl_logstdout      );
  cl_createlocal     = cnf->ival(appname      , "createlocal"    , cl_createlocal    );
  cl_restartlocal    = cnf->ival(appname      , "restartlocal"   , cl_restartlocal   );
  cl_clean_logout    = cnf->ival(appname      , "clean_logout"   , cl_clean_logout   );
  cl_read_grants     = cnf->ival(appname      , "read_grants"    , cl_read_grants    );
  cl_autoexec        = cnf->ival(appname      , "autoexec"       , cl_autoexec       );
  cl_use_ssl         = cnf->ival(appname      , "use_ssl"        , cl_use_ssl        );
  cl_chunksize       = cnf->ival(appname      , "chunksize"      , cl_chunksize      );
}

void mcs::Env::DumpConf()
{
  Conf* cnf;

  cnf = new Conf(inipath);

  cnf->setval(appname, "APPVERS"        , appvers           );
  cnf->setval(appname, "PATH"           , path              );
  cnf->setval(appname, "INTERFACE"      , interface         );
  cnf->setval(appname, "PORT"           , port              );
  cnf->setval(appname, "TIMEOUT"        , (int) timeout     );
  cnf->setval(appname, "MAX_USERS"      , max_users         );
  cnf->setval(appname, "DB_USER"        , db_user           );
  cnf->setval(appname, "DB_PASS"        , db_pass           );
  cnf->setval(appname, "DB_NAME"        , db_name           );
  cnf->setval(appname, "local_kills_mcs", cl_local_kills_mcs);
  cnf->setval(appname, "have_db"        , cl_have_db        );
  cnf->setval(appname, "custom_auth"    , cl_custom_auth    );
  cnf->setval(appname, "logfile"        , cl_logfile        );
  cnf->setval(appname, "work"           , cl_work           );
  cnf->setval(appname, "work_cid"       , cl_work_cid       );
  cnf->setval(appname, "logstdout"      , cl_logstdout      );
  cnf->setval(appname, "createlocal"    , cl_createlocal    );
  cnf->setval(appname, "restartlocal"   , cl_restartlocal   );
  cnf->setval(appname, "clean_logout"   , cl_clean_logout   );
  cnf->setval(appname, "read_grants"    , cl_read_grants    );
  cnf->setval(appname, "autoexec"       , cl_autoexec       );
  cnf->setval(appname, "use_ssl"        , cl_use_ssl        );
  cnf->setval(appname, "chunksize"      , cl_chunksize      );
  cnf->save();
  delete cnf;
}


mcs::Env::Env(string app, string inipath) {
  char* p;
  string s;

  flog=NULL;
  cnf = NULL;
  appname=app;
  server = NULL;
  local=NULL;
  srand(time(0));

  ssl_ctx = NULL;
  sslcert = "";
  sslpriv = "";


  //Getting host name
  localhost="localhost";
  if ((p=getenv("HOSTNAME")))
    localhost=p;

  this->inipath = inipath;
  cnf = new Conf(inipath);
  LoadConf();

  //Remove trailing slash
  remTrailing(path, "/");
  chkTree();

  //Eventually open log file
  if (cl_logfile) {
    char ts[20];
    const char *format="%Y%m%d%H%M%S";   //Format time string
    const int lenfmt=strlen(format)+3;   //Three more for century and NULL
    time_t tp=time(NULL);
    strftime(ts, lenfmt, format, localtime(&tp));   //Time-stamp

    s=path;
    s+=string("/mcs-") + appname + string("-");
    s+=ts;
    s+=".log";

    flog=new ofstream(s.csz, ios::app);
    if (!(flog->is_open()))
	throw MCS_ERROR(MSG_CANT_OPEN_FILE, s.csz);
  }

#ifdef HAVE_OPENSSL_SSL_H
  if (cl_use_ssl) {
    unsigned int ui;
    string s;
    sslcert = path + "/" + string(MCS_SSLCERT);
    sslpriv = path + "/" + string(MCS_SSLPRIV);

    if ((! File_Dir_Exist(sslcert, ui))   ||
	(! File_Dir_Exist(sslpriv, ui))      )
      throw MCS_ERROR(MSG_ERROR, "Can't found certificate and private key files, run \"mcscreatekey\"");
  }
#endif //HAVE_OPENSSL_SSL_H

  //DumpConf();
}



mcs::Env::~Env()
{
#if ENABLE_MYSQL
    if (cl_have_db)
	db.close();
#endif

  if (cnf)
    delete cnf;

  if (flog)
    if (flog->is_open())
      flog->close();

  //if (x509)
  //  delete x509;
  //DumpConf();
}

