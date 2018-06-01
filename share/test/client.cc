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

#include <mcs.hh>
using namespace mcs;


#include "test.h"


//Print all messages received from the server.
void printMsg(Client* cli)
{
  while (cli->msg.count() > 0) {
    cout << "\t" << cli->code.pop().sval() << " - "
	 << cli->msg.pop().sval()  << endl;
  }
}


//Execute a command, pront server messages and check if an error
//occurred and eventually exit the program.
void execCommand(Client* cli, string cmd, Data* data = NULL)
{
  bool b = cli->exec(cmd, data);
  printMsg(cli);
  if (! b)
    exit(1);
}


void spawn(string s)
{
  cout << "Spawning program: " << s << endl;
  int i = system(s.c_str());
  i = WEXITSTATUS(i);
  if (i != 0)
    ThrowErr("Spawned program exit with status " +  itos(i));
}


int main(int argc, char* argv[])
{
  Client* cli;
  string bl = " " ;
  string s, r;
  vector<string> vv;
  int i;
  unsigned int ui;

  Record v;

  try {

    //Open the configuration file
    Conf cnf("mcs.conf");

    //In these simple examples the client and server programs are in
    //the same directory so the client program can access the same
    //configuration file as the server.

    //Retrieve the server port and the "use_ssl" flag.
    int port = cnf.ival("test", "PORT");
    bool use_ssl = false;
    if (cnf.ival("test", "use_ssl") == 1)
      use_ssl = true;

    //Connect to the MCS server.
    cli = new Client("./tmp", "localhost", port, false, use_ssl);

    //Print received server messages.
    printMsg(cli);

    //Perform authentication.
    cli->login(TEST_USER, TEST_PASS, TEST_DBNA);
    printMsg(cli);

    //Execute the CID command.
    execCommand(cli, MCS_CMD_CID);

    //This should be the first and only client connected, so the
    //Client identifier should be 0.
    if (cli->aux[0].ival() != 0)
      ThrowErr(itos(cli->aux.count()));

    //Execute a query on the database server.
    execCommand(cli, MCS_CMD_QUERY + string(" select user()"));

    //Read the result of the query using the RecordSet interface of
    //the Client class. Check returned data are correct.
    if (cli->nRows() != 1)
      ThrowErr(itos(cli->nRows()));

    if (cli->metarec().count() != 1)
      ThrowErr(itos(cli->metarec().count()));

    if (cli->metarec()[0].name() != "user()")
      ThrowErr(cli->metarec()[0].name());

    if (cli->metarec()[0].type() != STRING)
      ThrowErr(itos(cli->metarec()[0].type()));

    if (cli->metarec()[0].isUnsigned())
      ThrowErr(itos(cli->metarec()[0].isUnsigned()));

    if (cli->metarec()[0].maxLength() != 77)
      ThrowErr(itos(cli->metarec()[0].maxLength()));

    if (cli->rec()[0].sval() != "mcstest@localhost")
      ThrowErr(cli->rec()[0].sval());

    if (cli->rec()[0].length() != 17)
      ThrowErr(itos(cli->rec()[0].length()));

    //Retrieve query result as an ASCII file.
    execCommand(cli, MCS_CMD_SENDQUERYRES);

    //Compare received file with correct one.
    LOG("Comparing received file");
    s = "diff ./tmp/query.out aux/query1.cmp";
    spawn(s);

    //Execute a custom command (see test_server.cc)
    execCommand(cli, "TCMD");

    //Print result on standard output.
    Record& r = cli->rec();
    for (i=0; i< r.count(); i++)
      cout << r[i].sval() << endl;


    //Retrieve list of database tables.
    execCommand(cli, MCS_CMD_TABLELIST);


    //Check that TEST_TBL and TEST_TBL_blob are present. Note that
    //also the table list is available through the RecordSet
    //interface.
    bool table_test = false;
    bool table_test_blob = false;

    do {
      s = cli->rec()[0].sval();
      cout << s << endl;
      if (s == TEST_TBL)                   table_test      = true;
      if (s == string(TEST_TBL) + "_blob") table_test_blob = true;
    }
    while (cli->setNext());

    if (! table_test)
      ThrowErr("Table " + string(TEST_TBL) + " not found!");

    if (! table_test_blob)
      ThrowErr("Table " + string(TEST_TBL) + "_blob not found!");

    goto llaabb;

    //Check TEST_TBL structure.
    execCommand(cli, MCS_CMD_TABLEINFO + bl + TEST_TBL);

    if (cli->nRows() != 6)
      ThrowErr(itos(cli->nRows()));

    do {
      cout << cli->rec()["COLUMN_NAME"].print() << endl;
      cout << cli->rec()["COLUMN_TYPE"].print() << endl;
      cout << cli->rec()["IS_NULLABLE"].print() << endl;
      cout << cli->rec()["COLUMN_KEY"].print() << endl;
      //cout << cli->rec()["COLUMN_DEFAULT"].print() << endl;
      cout << cli->rec()["EXTRA"].print() << endl;
      switch (cli->pos()) {
      case 0:
	s = cli->rec()["COLUMN_NAME"].sval();     if (s != "c") ThrowErr("");
	if (strncmp((const char*) cli->rec()["COLUMN_TYPE"].buffer(), "char(10)", cli->rec()["COLUMN_TYPE"].length())) ThrowErr("");
	s = cli->rec()["IS_NULLABLE"].sval();     if (s != "YES") ThrowErr("");
	s = cli->rec()["COLUMN_KEY"].sval();      if (s != "") ThrowErr("");
	if (! cli->rec()["COLUMN_DEFAULT"].isNull()) ThrowErr("");
	s = cli->rec()["EXTRA"].sval();           if (s != "") ThrowErr("");
	break;
	
      case 1:
	s = cli->rec()["COLUMN_NAME"].sval();     if (s != "i") ThrowErr("");
	if (strncmp((const char*) cli->rec()["COLUMN_TYPE"].buffer(), "int(11)", cli->rec()["COLUMN_TYPE"].length())) ThrowErr("");
	s = cli->rec()["IS_NULLABLE"].sval();     if (s != "YES") ThrowErr("");
	s = cli->rec()["COLUMN_KEY"].sval();      if (s != "") ThrowErr("");
	if (! cli->rec()["COLUMN_DEFAULT"].isNull()) ThrowErr("");
	s = cli->rec()["EXTRA"].sval();           if (s != "") ThrowErr("");
	break;
	
      case 2:
	s = cli->rec()["COLUMN_NAME"].sval();     if (s != "f") ThrowErr("");
	if (strncmp((const char*) cli->rec()["COLUMN_TYPE"].buffer(), "float", cli->rec()["COLUMN_TYPE"].length())) ThrowErr("");
	s = cli->rec()["IS_NULLABLE"].sval();     if (s != "YES") ThrowErr("");
	s = cli->rec()["COLUMN_KEY"].sval();      if (s != "") ThrowErr("");
	if (! cli->rec()["COLUMN_DEFAULT"].isNull()) ThrowErr("");
	s = cli->rec()["EXTRA"].sval();           if (s != "") ThrowErr("");
	break;
	
      case 3:
	s = cli->rec()["COLUMN_NAME"].sval();     if (s != "d") ThrowErr("");
	if (strncmp((const char*) cli->rec()["COLUMN_TYPE"].buffer(), "datetime", cli->rec()["COLUMN_TYPE"].length())) ThrowErr("");
	s = cli->rec()["IS_NULLABLE"].sval();     if (s != "YES") ThrowErr("");
	s = cli->rec()["COLUMN_KEY"].sval();      if (s != "") ThrowErr("");
	if (! cli->rec()["COLUMN_DEFAULT"].isNull()) ThrowErr("");
	s = cli->rec()["EXTRA"].sval();           if (s != "") ThrowErr("");
	break;
	
      case 4:
	s = cli->rec()["COLUMN_NAME"].sval();     if (s != "m") ThrowErr("");
	if (strncmp((const char*) cli->rec()["COLUMN_TYPE"].buffer(), "mediumint(9)", cli->rec()["COLUMN_TYPE"].length())) ThrowErr("");
	s = cli->rec()["IS_NULLABLE"].sval();     if (s != "YES") ThrowErr("");
	s = cli->rec()["COLUMN_KEY"].sval();      if (s != "") ThrowErr("");
	if (! cli->rec()["COLUMN_DEFAULT"].isNull()) ThrowErr("");
	s = cli->rec()["EXTRA"].sval();           if (s != "") ThrowErr("");
	break;
	
      case 5:
	s = cli->rec()["COLUMN_NAME"].sval();     if (s != "id") ThrowErr("");
	if (strncmp((const char*) cli->rec()["COLUMN_TYPE"].buffer(), "int(10) unsigned", cli->rec()["COLUMN_TYPE"].length())) ThrowErr("");
	s = cli->rec()["IS_NULLABLE"].sval();     if (s != "NO") ThrowErr("");
	s = cli->rec()["COLUMN_KEY"].sval();      if (s != "PRI") ThrowErr("");
	if (! cli->rec()["COLUMN_DEFAULT"].isNull()) ThrowErr("");
	s = cli->rec()["EXTRA"].sval();           if (s != "auto_increment") ThrowErr("");
	break;
      }
    }
    while (cli->setNext());

  llaabb:

    //Execute a query on the database server and save result as an
    //ASCII file on the server's work directory.
    execCommand(cli, MCS_CMD_QUERY + bl + " -" + MCS_OPT_SAVEQUERYASCII + bl +
	       " SELECT c, i, f, d, m, id FROM " + TEST_TBL);

    //Retrieve the file just created.
    execCommand(cli, MCS_CMD_GET);

    //Comparing recevied file.
    if (sizeof(int*) == 8)
      //64 bit machine
      s = "diff tmp/out aux/query2_64.cmp";
    else
      //32 bit machine
      s = "diff tmp/out aux/query2_32.cmp";
    spawn(s);


    //Execute a query on the database server and save result as a FITS
    //file on the server's work directory.
    execCommand(cli, MCS_CMD_QUERY + bl + " -" + MCS_OPT_SAVEQUERYFITS + bl +
	       " select * from " + TEST_TBL);

    //Retrieve the file just created.
    execCommand(cli, MCS_CMD_GET);

    //Comparing recevied file.
    //s = "diff tmp/out aux/query3.cmp";
    //spawn(s);



    //Copy a file on the server.
    spawn("cp " + string(argv[0]) + " tmp/PUTtest");
    execCommand(cli, MCS_CMD_PUT + string(" PUTtest"));

    //Retrieving the same file
    execCommand(cli, MCS_CMD_GET + string(" PUTtest"));

    //Comparing received file
    s = "diff " + string(argv[0]) + " tmp/PUTtest";
    spawn(s);

    //Execute a SQL script on the server side (see the "srv" directory)..
    execCommand(cli, MCS_CMD_EXEC + string(" sqltest ") +
		TEST_TBL + string(" c"));

    //Check result set.
    if (cli->metarec().count() != 8)
      ThrowErr("");

    s = cli->metarec()[0].name(); if (s != "c") ThrowErr(s);
    s = cli->metarec()[1].name(); if (s != "i") ThrowErr(s);
    s = cli->metarec()[2].name(); if (s != "f") ThrowErr(s);
    s = cli->metarec()[3].name(); if (s != "d") ThrowErr(s);
    s = cli->metarec()[4].name(); if (s != "m") ThrowErr(s);
    s = cli->metarec()[5].name(); if (s != "id") ThrowErr(s);
    s = cli->metarec()[6].name(); if (s != "ts") ThrowErr(s);
    s = cli->metarec()[7].name(); if (s != "len") ThrowErr(s);

    do {
      switch (cli->pos()) {
      case 0:
	ui = cli->rec()[7].uival(); if (ui != 5) ThrowErr(itos(ui)); break;
      case 1:
	ui = cli->rec()[7].uival(); if (ui != 3) ThrowErr(itos(ui)); break;
      case 2:
	ui = cli->rec()[7].uival(); if (ui != 5) ThrowErr(itos(ui)); break;
      case 3:
	ui = cli->rec()[7].uival(); if (ui != 6) ThrowErr(itos(ui)); break;
      case 4:
	ui = cli->rec()[7].uival(); if (ui != 3) ThrowErr(itos(ui)); break;
      case 5:
	ui = cli->rec()[7].uival(); if (ui !=10) ThrowErr(itos(ui)); break;
      case 6:
	ui = cli->rec()[7].uival(); if (ui != 6) ThrowErr(itos(ui)); break;
      }
    }
    while (cli->setNext());


    //Create a Data object to be sent to the server.
    Data aa(12);

    //Sent the Data object to the server, the "-loop" option is needed
    //to let the server put the received Data object in the "send"
    //queue, so that it can be retrieved form the client.
    execCommand(cli, MCS_CMD_PUTDATA + bl + "-loop", &aa);

    //Once an object is on the "send" queue on the server it is
    //automatically trasferred to the client at the end of a command
    //execution, so the Data object just sent should have already be
    //returned.
    Data bb = cli->recv.pop();

    //Compare sent and received Data objects.
    compareData(aa, bb);

    //Closing connection.
    execCommand(cli, MCS_CMD_CLOSECLIENT);

    if (cli->isConnected())
      ThrowErr("Connection still opened.");

    LOG("Issuing a command after connection have been closed...");
    //Issuing a command after connection have been closed, an
    //exception is expected to be thrown.
    try {
      cli->exec(MCS_CMD_CID);
      printMsg(cli);
    }
    catch(Event e) {
      if (e.type() != ERROR) ThrowErr(itos(e.type()));
      if (e.code() != MSG_NOT_CONNECTED) throw e;
    }

    sleep_ms(2000);
  }
  catch(Event e) {
    cerr << e.where() << ": " << e.msg() << endl;
    exit(2);
  }
  catch(...) {
    cout << "Unknown error" << endl;
    exit(3);
  }

  exit(0);
}
