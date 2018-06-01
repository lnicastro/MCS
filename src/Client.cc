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
mcs::Client::Client(string path, string server, int port,
		    bool synchro, bool SSL,
		    unsigned int timeout) :
  Socket(server, port, timeout, timeout, SSL),
  RecordSet(),
  code(synchro),
  msg(synchro),
  out(synchro),
  recv(synchro),
  aux(synchro)
{
  connected = false;
  init(0);
  lpath=path;
  if (lpath.empty())
    lpath=Pwd();

  //Remove trailing slash
  remTrailing(lpath, "/");


  connected = true;
  writetofile = true;
  lrecord = NULL;
  exec("");  //Read welcome message
  exec(MCS_CMD_CID);

  unsigned int size;
  if (! File_Dir_Exist(lpath, size))
    throw MCS_ERROR(MSG_DIRECTORY_NOT_FOUND, lpath.csz);

}


mcs::Client::~Client()
{}


void mcs::Client::writeToFile(int des)
{
    writetofile = true;
    fileDescriptor = des;
}




bool mcs::Client::login(string user, string pass, string db)
{
  bool ret;
  string bl = " ";
  string cmd;

  cmd = MCS_CMD_USERNAME + bl + user;
  ret = exec(cmd);
  if (ret) {
      cmd = MCS_CMD_PASSWORD + bl + pass;
	  ret = exec(cmd);
      if (ret) {
	  if (! db.empty()) {
	      cmd = MCS_CMD_DBNAME + bl + db;
	      ret = exec(cmd);
	  }

      if (ret)
	ret = exec(MCS_CMD_DBCONNECT);
    }
  }
  return ret;
}


void mcs::Client::clearRecords()
{
  this->code.clear();
  this->msg.clear();
  this->out.clear();
  this->recv.clear();
  this->aux.clear();
}



bool sendRecord(Client* cli, Record* rec)
{
  int i;

  for (i=0; i<rec->count(); i++)
    if (! cli->exec(MCS_CMD_PUTDATA, &((*rec)[i])))
      return false;

  return true;
}


bool mcs::Client::exec(string cmd, Data* data)
{
  string s="", msg, filename, fn;
  int i, code=0;
  unsigned int ui;
  bool ret=true;
  unsigned int size = 0;
  static string bl = " ";
  CommandParser hc;
  char* buf;
  Record* newRecordSet = NULL;
  static bool batch = false;
  Serializable* sss;

  if (! connected)
      throw MCS_ERROR(MSG_NOT_CONNECTED);

  if (! batch) {
    clearRecords();
    if (lrecord) delete lrecord;
    lrecord = NULL;
  }

  batch = true;


  if (! cmd.empty()) {
      hc.parseCmd(cmd);

      if (hc.cmpCmd(MCS_CMD_PUT)) {
	  filename = lpath + string("/") + hc.arg(0).sval();
	  if (! (size =  File_Dir_Exist(filename, ui)))
	      throw MCS_ERROR(MSG_CANT_OPEN_FILE, filename.csz);
	
	  cmd = MCS_CMD_PUT + bl + hc.arg(0).sval() + bl + itos(size);
	  for (i=0; i<hc.optc(); i++)
	      cmd += " -" + hc.opt(i);
      }

      else if (hc.cmpCmd(MCS_CMD_PUTDATA)) {
	  if (! data)
	      throw MCS_ERROR(MSG_OBJECT_MISSING);
	
	  buf = (char *)data->getEntireBuffer(size);
	  cmd = MCS_CMD_PUTDATA + bl + itos(size);
	  for (i=0; i<hc.optc(); i++)
	      cmd += " -" + hc.opt(i);
      }
      print(cmd);
  }


  s = getline();
  while((s != string(MCS_PROMPT_OK))      &&
	(s != string(MCS_PROMPT_WARN))    &&
	(s != string(MCS_PROMPT_ERROR))   &&
	(code != MSG_BYE)) {

    if (s.csz[0] == *MCS_PRE) {
      code = extractCode(s.csz);
      msg = s.substr(4, 100000);
	
      this->code.addField(code);
      this->msg.addField(msg);

      if (code == MSG_BYE) {
	Socket::Close();
	connected = false;
	event(MCS_CLIENT_DISCONNECT, Data((void*) NULL));
	return true;
      }
	
      //Search for auxiliary field in the answer....
      vector<string> v = split(msg, MCS_SEP);

      //...and put them in the recv vector
      for (ui=0; ui<v.size(); ui++)
	if ((ui % 2) == 1)  //only items with an even index
	  aux.addField(v[ui]);


      if (code >= 100) {
	ret=false;
	event(MCS_CLIENT_ERROR, Data(msg), code);
      }


      switch (code) {
      case MSG_SENDFILE_START:
	fn = aux.field(aux.count()-1).sval();
	if (writetofile)
	  recvData(fileDescriptor);
	else
	  recvData(lpath + string ("/") + fn);
	
	event(MCS_CLIENT_FILE, Data(fn));
	break;
	
      case MSG_SENDDATA_START:
	buf = NULL;
	size = recvData((char**) &buf, 0);
	i = aux.field(aux.count()-1).ival();
	if (i == 1) {
	  Data d(buf);
	  this->recv.addField(d);
	  event(MCS_CLIENT_DATA, d);
	}
	else {
	  lrecord = new Record(buf);
	}
	
	free(buf);
	break;
	
      case MSG_RECVFILE_START:
	sss = new Serializable(filename, lchunksize);
	sendData(sss);
	delete sss;
	break;
	
      case MSG_RECVDATA_START:
	sss = new Serializable(buf, size, true, lchunksize);
	sendData(sss);
	delete sss;
	//free(buf); //It has been freed by the Serializable object
	break;
	
      case MSG_OUT:
	out.addField(s.substr(4, 100000));
	break;
	
      case MSG_NEW_RECORDSET:
	if (lrecord) {
	  newRecordSet = new Record(*lrecord);
	  delete lrecord;
	  lrecord = NULL;
	}
	break;


      }
    }
    else
      throw MCS_ERROR(MSG_PROTOCOL, s.csz);
	
    s = getline();
  }

  writetofile = false;
  if (newRecordSet) {
    Data d = newRecordSet->pop();
    if (d.getTag() & MCS_RS_INSERT)
      d.setTag( d.getTag() - MCS_RS_INSERT);

    init(d.getTag(), d.ival(), newRecordSet);
    delete newRecordSet;
    startFetch();
  }

  if (hc.cmpCmd(MCS_CMD_CID)) {
    lcid = aux[0].ival();
    lchunksize = aux[1].ival();
  }



  batch = false;
  return ret;
}



bool mcs::Client::fetch(unsigned int newpos, bool random)
{
  static string bl = " ";
  if (! exec(MCS_CMD_RECORD + bl + itos(newpos))) {
    if (code[code.count()-1].ival() == MSG_ALREADY_AT_END_OF_FILE)
      return false;
    else
      throw MCS_ERROR( MSG_FETCH_ERROR, msg[0].sval());
  }


  if (lrecord) {
    rec() = *lrecord;
    return true;
  }
  else
    return false;
}



void mcs::Client::event(int op, Data d, int code)
{}



bool mcs::Client::isConnected()
{
    return connected;
}



int mcs::Client::cid() { return lcid; }


int mcs::Client::chunksize() { return lchunksize; }






////--------------------------------------------------------
//mcs::ClientThread::ClientThread(string path, string server,
//				int port, bool SSL) :
//    Client(path, server, port, true, SSL) , Thread()
//{
//    start();
//}
//
//
//mcs::ClientThread::~ClientThread()
//{}
//
//
//bool mcs::ClientThread::exec(string cmd, Data* d)
//{
//    bool ret = false;
//    enter();
//
//    try {
//	ret = Client::exec(cmd, d);
//    }
//    catch (Event e) {
//	leave();
//	throw e;
//    }
//
//    leave();
//    return ret;
//}
//
//
//void mcs::ClientThread::run()
//{
//  for (;;) {
//    sleep_ms(500);
//
//    if (tryenter()) {
//      try {
//	if (chkRecv(true, false))
//	  Client::exec(MCS_CMD_NOP);
//      }
//      catch (Event e) {
//	leave();
//	cerr << e.where() << " " << e.msg() << endl;
//	exit();
//      }
//      leave();
//    }
//  }
//}
//
//
////Record& mcs::ClientThread::vec(int id)
////{
////  enter();
////
////  Record* ret = NULL;
////
////  try {
////    ret = &(Client::vec(id));
////  }
////  catch (Event e) {
////    leave();
////    throw e;
////  }
////
////  leave();
////  return *ret;
////}
