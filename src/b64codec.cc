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
#include <fcntl.h>
#include <unistd.h>

#include <mcs.hh>
using namespace mcs;


void usage()
{
  cout << "b64codec: A base 64 encoder/decoder based on MCS" << endl;
  cout << "Giorgio Calderone <gcalderone@ifc.inaf.it>" << endl;
  cout << endl;
  cout << "Usage: b64codec [-e|-d] [FILEIN [FILEOUT]]" << endl;
  cout << "Options:" << endl;
  cout << "    -e, --encode  Encode data (default)" << endl;
  cout << "    -d, --decode  Decode data" << endl;
  cout << "    -h, --help    Shows this help" << endl;
  cout << "" << endl;
  cout << "If no file name is given stdin and stdout will be used." << endl;
  cout << "If one file name is given it will be used for input and stdout for output." << endl;
  cout << "If two file names are given they will be used respectively for input and output." << endl;
  cout << "If FILEIN is - stdin will be used for input." << endl;
}



#define BUFSIZE 2048

int main(int argc, char* argv[])
{
  unsigned int ret;
  bool encode = true;  //Default
  string fin, fout;
  int fdin, fdout;
  char buf[BUFSIZE];

  //Parse arguments
  for (ret=1; ret<(unsigned int) argc; ret++) {
    if ((strcmp(argv[ret], "-e") == 0)    ||
	(strcmp(argv[ret], "--encode") == 0)       )
      encode = true;

    else if ((strcmp(argv[ret], "-d") == 0)   ||
	     (strcmp(argv[ret], "--decode") == 0)   )
      encode = false;

    else if ((strcmp(argv[ret], "-h") == 0)   ||
	     (strcmp(argv[ret], "--help") == 0)       ) {
      usage();
      return 0;
    }

    else {
      if (fin.empty())
	fin = argv[ret];

      else {
	if (fout.empty())
	  fout = argv[ret];
	else
	  break;
      }
    }
  }

  if (fin == "-")
    fin = "";

  if (fout == "-")
    fout = "";


  //Open input file, if needed
  if (fin.empty())
    fdin = 0;
  else {
    fdin = open(fin.c_str(), O_RDONLY);
    if (fdin <=0 ) {
      cerr << "Can't open file " << fin << " for input." << endl;
      return 2;
    }
  }


  //Open output file if needed
  if (fout.empty())
    fdout = 1;
  else {
    fdout = open(fout.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0);
    if (fdout <=0 ) {
      cerr << "Can't open file " << fout << " for output." << endl;
      cerr << strerror(errno) << endl;
      return 3;
    }
    fchmod(fdout, S_IREAD | S_IWRITE | S_IRGRP | S_IROTH);
  }



  //Do the job
  B64_Codec b64;
  while ((ret = read(fdin, buf, BUFSIZE)) > 0) {
    if (encode)
      ret = b64.encode(buf, ret);
    else
      ret = b64.decode(buf, ret);

    ret = write(fdout, b64.buffer(), b64.bufUsed());

    if (ret != b64.bufUsed()) {
      cerr << "Can't write on output file." << endl;
      return 4;
    }
  }
  if (encode)
    ret = b64.encode(buf, 0);
  else
    ret = b64.decode(buf, 0);
  ret = write(fdout, b64.buffer(), b64.bufUsed());


  //Cleanup
  if (! fin.empty())
    close(fdin);

  if (! fout.empty())
    close(fdout);

  return 0;
}
