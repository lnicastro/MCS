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


int main(int argc, char* argv[])
{
  Client* cli;
  int i;

  try {

    //Open the configuration file
    Conf cnf("mcs.conf");

    //Retrieve the server port and the "use_ssl" flag.
    int port = cnf.ival("test", "PORT");
    bool use_ssl = false;
    if (cnf.ival("test", "use_ssl") == 1)
      use_ssl = true;

    i = 1;
    for (;;) {
      cout << "Attempt " << i++ << endl;

      //Connect to the MCS server.
      cli = new Client("./tmp", "localhost", port, false, use_ssl);

      //Print received server messages.
      printMsg(cli);

      //Perform authentication.
      cli->login(TEST_USER, TEST_PASS, TEST_DBNA);
      printMsg(cli);

      //Execute the CID command.
      execCommand(cli, MCS_CMD_CID);

      //Closing connection.
      execCommand(cli, MCS_CMD_CLOSECLIENT);

      delete cli;

      //sleep_ms(10);
    }

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
