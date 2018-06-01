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


#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <string>
using namespace std;

#define REPLACE 1
#define N_ARGS 7

void printUsage(char* name)
{
    cerr << name
	 << " MCS_CID MCS_USER MCS_PASS MCS_DBNAME WORK_DIR PROGNAME [PARS...]"
	 << endl << endl;
}

void printArguments(int argc, char* argv[])
{
    int i;
    cerr << "Arguments: " << argc << endl;
    for (i=0; i<argc; i++)
	cerr << i << " " << argv[i] << "|" << endl;
}

int main(int argc, char* argv[])
{
    int ret, i;

    if (argc < N_ARGS) {
	cerr << "MCSEXEC: Wrong number of parameters" << endl << endl;
	printUsage(argv[0]);
	printArguments(argc, argv);
	exit(200);
    }
	
    string cid  = argv[1];
    string user = argv[2];
    string pass = argv[3];
    string dbna = argv[4];
    string wd   = argv[5];
    string path = argv[6];
    string prog = path;
    char* pc = (char*) strrchr(path.c_str(), '/');
    if (pc)
	prog = ++pc;

    int subargc = argc - N_ARGS;
    char** subargv = (char**) malloc((subargc+2) * sizeof(char*));

    subargv[0] = (char*) malloc(strlen(prog.c_str()) + 1);
    strcpy(subargv[0], prog.c_str());
    for (i=0; i<subargc; i++)
    {
	subargv[i+1] = (char*) malloc(strlen(argv[N_ARGS + i]) + 1);
	strcpy(subargv[i+1], argv[N_ARGS + i]);
    }
    *subargv[subargc+1] = '\0';


    ret = chdir(wd.c_str());
    if (ret != 0) {
	cerr << strerror(errno) << endl;
	exit(errno);
    }

    pc = getcwd(NULL, 0);
    ret = setenv("PWD", pc, REPLACE);  free(pc);

    ret = setenv("MCS_CID" , cid.c_str() , REPLACE);

    ret = setenv("MCS_USER", user.c_str(), REPLACE);

    ret = setenv("MCS_PASS", pass.c_str(), REPLACE);

    ret = setenv("MCS_DBNAME", dbna.c_str(), REPLACE);

    ret = execv(path.c_str(), subargv);
    if (ret == -1) {
	cerr << strerror(errno) << endl;
	exit(errno);
    }
}
