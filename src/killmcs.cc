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

void usage()
{
  cout << "killmcs: kills a MCS server" << endl;
  cout << "Giorgio Calderone <gcalderone@ifc.inaf.it>" << endl;
  cout << endl;
  cout << "Usage: killmcs [-h] appname [inipath]" << endl;
}


int main(int argc, char* argv[])
{
    Env* env = NULL;
    CommandParser cp;
    string s;

    try {
	cp.parseCmd(argc, argv);

	if (argc == 1         ||  //Invocation without any option or argument
	    cp.givenOpt("h")  ||  //Option -h has been given
	    cp.argc() == 0    ){  //No argument has been given
	    usage();
	    exit(0);
	}

	//At least one argument has been given
	string appname = cp.arg(0).sval();

	//Eventually get the second argument
	string inipath = (cp.argc() >= 2    ?   cp.arg(1).sval()   :   "");

	if (inipath.empty())
	    env = new Env(appname);
	else
	    env = new Env(appname, inipath);
	env-> server = NULL;
	
	if (chdir(env->path.c_str()) != 0)
	    throw MCS_ERROR(MSG_DIRECTORY_NOT_FOUND, env->path);

	s = env->path + "/mcs.pid";
	ifstream pidfile("mcs.pid");

	if (pidfile.fail())
	    throw MCS_ERROR(MSG_CANT_OPEN_FILE, s.c_str());

	pidfile >> s;
	pidfile.close();
	unlink("mcs.pid");

	s = "kill " + s;
	cout << s << endl;

	exit(system(s.c_str()));
    }
    catch (Event& e) {
	cerr << e.msg() << endl;
	exit(3);
    }


    if (env) delete env;
}
