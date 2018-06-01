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

#include <unistd.h>

//See test.h for auxiliary functions and macros
#include "test.h"


mcs::Pipe lpipe;

#define READ_BUFSIZE 20
class ReadingThread : public Thread
{
public:
    void run() {
	int fd, n;
	char buf[READ_BUFSIZE];
	
	lpipe.createNamed();
	fd = lpipe.openRead();
	
	while ((n = read(fd, buf, READ_BUFSIZE))) {
	    cout << "Read |";
	    cout.write(buf, n);
	    cout << "|" << endl;
	}
	
	lpipe.closeRead();
    }
};




#define N_WRITE 100
class WritingThread : public Thread
{
public:
    void run() {
	int fd, i, n;
	char buf[10];
	
	while (! lpipe.isReady()) sleep_ms(1);
	fd = lpipe.openWrite();

	for (i=0; i<N_WRITE; i++) {
	    if (! lpipe.consumerHasGone()) {
		n = sprintf(buf, "%03d - ", i);

		cout << "Write |";
		cout.write(buf, n);
		cout << "|" << endl;

		write(fd, buf, n);
	    }

	    if (i % 10)
		sleep_ms(1);
	}

	lpipe.closeWrite();
    }
};




int main()
{
  try {
      ReadingThread rr;
      WritingThread ww;

      rr.start();
      ww.start();

      cout << "Waiting for child thread termination..." << endl;

      while (rr.state() != MCS_STATE_END)
	  sleep_ms(500);

      while (ww.state() != MCS_STATE_END)
	  sleep_ms(500);
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
