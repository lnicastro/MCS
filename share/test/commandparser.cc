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


int main()
{
  try {

  //Test class CommandParser
  CommandParser hc;
  vector<string> vv;
  int i;
  unsigned int ui;

  //Set up a complex command with various features:
  // - multiple blanks and tabs;
  // - strings enclosed in double quotes;
  // - escaped characters;
  // - options with arguments;
  string s = " cid  par1 -p \"bye bye\" \t \\\\a -sss=\"_\\\\_\\\"_\"  ";;

  //Parse the command line.
  hc.parseCmd(s);

  //Print the entire command line.
  cout << "Command line is: " << hc.cline() << endl;

  //Print number of tokens.
  cout << "Tokens (" << hc.tokenc() << ")" << endl;

  //Print all tokens
  for (i=0; i<hc.tokenc(); i++)
    cout << "Token " << i << " " << hc.token(i) << "|" << endl;

  //The first token is assumed to be the "command".
  cout << endl << "Command: " << hc.cmd() << "|" << endl;

  //Arguments are all those tokens that doesn't begin with a "-"
  //character. Note that the string inside double quote are taken as a
  //single token.
  cout << endl << "Arguments (" << hc.argc() << "): " << hc.allargs() << endl;
  for (i=0; i<hc.argc(); i++)
    cout << i << " " << hc.arg(i).sval() << " " << endl;

  //Options are all those tokens that begin with a "-" character. An
  //option can have an optional argument assigned with an "="
  //character.
  cout << endl << "Options: " << hc.optc() << endl;
  for (i=0; i<hc.optc(); i++) {
    cout << i << " " << hc.opt(i);
    if (! hc.optarg(i).isNull())
      cout << " (" << hc.optarg(i).sval() << ")";
    cout << endl;
  }

  //Check for correctness of prasing.
  if (hc.argc() != 3)
    ThrowErr("Wrong number of arguments: " + itos(hc.argc()));

  if (hc.cmd() != "cid")
    ThrowErr(hc.cmd() + "|");

  if (hc.arg(0).sval() != "par1")
    ThrowErr(hc.arg(0).sval() + "|");

  if (hc.arg(1).sval() != "bye bye")
    ThrowErr(hc.arg(1).sval() + "|");

  if (hc.arg(2).sval() != "\\a")
    ThrowErr(hc.arg(2).sval() + "|");

  if (hc.optc() != 2)
    ThrowErr("Wrong number of options: " + itos(hc.optc()));

  if (hc.opt(0) != "p")
    ThrowErr(hc.opt(0));

  if (! hc.optarg(0).sval().empty())
    ThrowErr(hc.optarg(0).sval() + "|");

  if (hc.opt(1) != "sss")
    ThrowErr(hc.opt(1));

  if (hc.optarg(1).sval() != "_\\_\"_")
    ThrowErr(hc.optarg(1).sval() + "|");

  //The cmpCmd() method can be used to check if a particular command
  //has been given.
  if (! hc.cmpCmd("CID"))
    ThrowErr(hc.cmd());

  if (hc.cmpCmd("USR"))
    ThrowErr(hc.cmd());
  cout << endl << endl;

  //The CommandParser class can be used to parse a script and
  //substitute placeholders with the arguments given in the command
  //line. In this example the following substitutions will be made:

  //$0 --> 0 par1
  //$1 --> bye bye
  //$2 --> \a

  //Print the script as is.
  cout << endl << "Script: " << endl;
  system("cat aux/script");
  cout << "Script end. " << endl;

  //Replace parameters on the script, the result will be stored in the
  //resulting string vector, one row at a cell
  vv = hc.replPars_onFile("aux/script");

  //Print the script with the substitution performed.
  cout << endl << "Script parsed: " << endl;
  for (ui=0; ui<vv.size(); ui++)
    cout << vv[ui] << "|" << endl;

  //Check for correctness
  if (vv[0] != "line 1")                   ThrowErr(vv[0]);
  if (vv[1] != "line 2")                   ThrowErr(vv[1]);
  if (vv[2] != "line par1")                ThrowErr(vv[2]);
  if (vv[3] != "line bye bye")             ThrowErr(vv[3]);
  if (vv[4] != "line splitted in two \\a") ThrowErr(vv[4]);
  if (vv[5] != "line splitted in two \\a") ThrowErr(vv[5]);


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
