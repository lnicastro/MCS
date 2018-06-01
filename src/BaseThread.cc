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
RetValue mcs::BaseThread::Log(Event e)
{
  char buf[MCS_COMMBUFSIZE];
  char* p = buf;
  string str;


  const char *format="%Y%m%d %H%M%S ";   //Format time string
  const int lenfmt=strlen(format)+3;     //Three more for century and NULL
  time_t tp=time(NULL);
  p+=strftime(p, lenfmt, format, localtime(&tp));   //Time-stamp
  p+=sprintf(p, "%s ", tID);          //Thread id

  str = buf;
  str += e.msg();

  if (e.type() != OK) {
      str += " [";
      switch (e.type()) {
	  case WARN:
	      str += "W"; break;
	  case ERROR:
	      str += "E"; break;
	  case FATAL:
	      str += "F"; break;
	  default:
	      break;
      }
      str += " " + e.where() + "]";
  }

  if (env->cl_logstdout) cout << str << endl;
  if (env->cl_logfile) if (env->flog) (*(env->flog)) << str << endl;

  if ((env->local)                       &&
      (e.code() != MSG_THREAD_CREATE)    &&
      (e.code() != MSG_THREAD_DESTROY)   &&
      (id() > 0)                         ) {

      env->local->hk_log((UserThread*) this, e);
  }

  return e.type();
}


mcs::BaseThread::BaseThread(Thread* parent, int ID) : Thread(ID, parent)
{
  sprintf(tID, "%03d", ID);
}


mcs::BaseThread::BaseThread(Thread* parent, const char *ltID) : Thread(-1, parent)
{
  sprintf(tID, "%3s", ltID);
}


mcs::BaseThread::~BaseThread()
{}

const char* mcs::BaseThread::tid()
{
  return tID;
}




Env* mcs::BaseThread::env;





//--------------------------------------------------------
#define FT_TEMPL_BATCH   "#BATCH"
#define FT_TEMPL_SQL     "#SQL"
#define FT_TEMPL_SCRIPT  "#SCRIPT"
#define FT_TEMPL_BIN     "\177ELF"



int mcs::BaseThread::fileType(string fn)
{
  char buffer[10];
  buffer[9] = '\0';

  ifstream fin;
  fin.open(fn.csz);
  if (!(fin.is_open()))
    throw MCS_ERROR(MSG_CANT_OPEN_FILE, fn.csz);

  fin.read(buffer, 9);
  fin.close();
  if (0)
    ;
  else if (! memcmp(buffer, FT_TEMPL_BATCH,  strlen(FT_TEMPL_BATCH)))
    return MCS_FT_BATCH;
  else if (! memcmp(buffer, FT_TEMPL_SQL,    strlen(FT_TEMPL_SQL)))
    return MCS_FT_SQL;
  else if (! memcmp(buffer, FT_TEMPL_SCRIPT, strlen(FT_TEMPL_SCRIPT)))
    return MCS_FT_SCRIPT;
  else if (! memcmp(buffer, FT_TEMPL_BIN, strlen(FT_TEMPL_BIN)))
    return MCS_FT_BIN;

  return MCS_FT_UNKNOWN;
}



int mcs::BaseThread::chkExt(string& s)
{
  int i;
  string fn = env->cnf->sval(env->appname + "_EXTERNAL" , s, "");

  if (! fn.empty()) {
    i = fileType(fn);  //Check file type
    if (i == MCS_FT_UNKNOWN)
      throw MCS_ERROR(MSG_UNKNOWN_FILE_TYPE, fn.csz);
  }
  else
    throw MCS_WARN(MSG_EXT_NOT_AVAILABLE, s.c_str());

  s = fn;
  return i;
}



int mcs::BaseThread::spawn(string fn, string pars,
			   string wpath, string thrID,
			   string user, string pass, string dbname,
			   string fout, string ferr)
{
  //We need to check for redirection operators '<', '>' and pipes
  //'|'. If one of these operators are present in the command line
  //we'll throw an error.

  static string bl = " ";
  string tmp;
  tmp = fn + bl + pars;

  if (strchr(tmp.c_str(), '>'))
    throw MCS_WARN(MSG_CHARACTER_NOT_ALLOWED, ">");
	
  if (strchr(tmp.c_str(), '<'))
    throw MCS_WARN(MSG_CHARACTER_NOT_ALLOWED, "<");

  if (strchr(tmp.c_str(), '|'))
    throw MCS_WARN(MSG_CHARACTER_NOT_ALLOWED, "|");

  int i = fileType(fn);   //Check file type
  if ((i != MCS_FT_SCRIPT)   &&
      (i != MCS_FT_BIN))
    throw MCS_ERROR(MSG_CANT_EXECUTE_FILE, fn);


  tmp  = BINDIR + string("/mcsexec ");        //external spawner, pars:
  tmp += thrID + bl;                          //client id
  tmp += user + bl + pass + bl + dbname + bl; //username, password, dbname,
  tmp += wpath + bl;                          //path to work directory

  if (i == MCS_FT_SCRIPT) //In this case the real executable is the shell
    tmp += "/bin/bash ";

  tmp += fn + bl + pars;                      //filename of executable
  tmp += " >  " + fout ;                      //redirect stdout
  tmp += " 2> " + ferr;                       //redirect stderr

  i=system(tmp.csz);
  if (WIFEXITED(i)) {
    i = WEXITSTATUS(i);
  }
  else
    throw MCS_ERROR( MSG_EXT_BIN_UNEXPECTED );

  return i;
}


