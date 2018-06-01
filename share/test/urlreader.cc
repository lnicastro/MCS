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

#define LEN 100

#define MODE 2

int main(int argc, char* argv[])
{
  int i, fd, size;
  unsigned int sum;
  string fn;
  char buf[LEN];

  URLReader url;

  try {

      if (argc == 1) {
	  url.Download("http://ross.iasfbo.inaf.it/MCS/urlreader_test", "aux/urlreader_test");
	  return system ("diff aux/urlreader_test aux/urlreader_test.cmp");
      }

    for (i=1; i<argc; i++) {

	//cout << "START: " << argv[i] << endl;

#if MODE == 1
      fn = url.OpenAsFifo(argv[i]);
      fd = open(fn.c_str(), O_RDONLY);
#endif

#if MODE == 2
      fd = url.OpenAsFD(argv[i]);
#endif

#if MODE != 3
      sum = 0;
      while ((size = read(fd, buf, LEN))) {
	cout.write(buf, size);
	sum += size;
      }
#endif

#if MODE == 3
      url.Download(argv[i], "test");
#endif

      url.Close();  //Eventually throw errors
      //cout << "END: " << argv[i] << " (size: " << sum << ")" << endl;
    }
  }
  catch(Event& e) {
    cerr <<  e.msg() << endl;
    return 1;
  }
}
