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
#include  <fcntl.h>


//#include  <mcsvo.hh>
#include  <votpp.hh>
using namespace mcs;
//using namespace vot;
using namespace votpp;

#include <fstream>

#define BUFLEN 100



void visitFits(FITSReader* fits)
{
  int j;

  do {
    cout << endl << "HDU #" << fits->currentHDU() << endl;

    //Print header
    for (j=0; j<fits->header.count(); j++) {
      cout <<
	fits->header[j].name() << " = " <<
	fits->header[j].sval() << " / " <<
	fits->header_comments[j].sval() << endl;
    }

    //Print field names
    cout << fits->metarec().asStringNames() << endl;

    //Print field types
    cout << fits->metarec().asStringTypes() << endl;

    //Print data
    while (! fits->eof()) {
      cout << fits->rec().asString() << endl;
      fits->setNext();
    }
  } while (fits->selectNextHDU());
}



int main(int argc, char* argv[])
{
  char buffer[BUFLEN];
  int nn, fdxml;

  if (argc < 2) {
    cerr << "Usage: " << basename(argv[0]) << " <VOTable>" << endl;
    return 1;
  }


  try {

    //Read a VOTable file and splits its XML part from its BINARY part.
    VOTableReaderSplit input;

    //In this case we want the binary part to be written in a file.
    input.saveBinaryStream("votablesplit.binstream");

    //Open the VOTable file and return a file descriptor
    fdxml = input.OpenAsFD(argv[1]);

    //Print the content of the XML part
    while ((nn = read(fdxml, buffer, BUFLEN)))
      cout.write((const char*) buffer, nn);

    //Check for errors
    if (input.error())
      cerr << input.error()->msg().c_str() << endl;

    input.Close();


    cout << endl << endl << endl << endl;

    //Do the same but now use the binary stream as input for a
    //FITSReader object

    //Pass an empty string
    input.saveBinaryStream();

    //Open the VOTable file and return a file descriptor
    fdxml = input.OpenAsFD(argv[1]);

    //Print the content of the XML part
    while ((nn = read(fdxml, buffer, BUFLEN))) {
      cout.write((const char*) buffer, nn);

      //If a stream is present
      if (strstr(buffer, "<STREAM")) {
	FITSReader* fits = new mcs::FITSReader();
	fits->open(input.binpipe()->getFD_read());
	visitFits(fits);
	fits->close();
	delete fits;
      }
    }
    //Check for errors
    if (input.error())
      cerr << input.error()->msg().c_str() << endl;

    input.Close();
  }
  catch (Event e) {
    cerr << e.where() << " " << e.msg() << endl;
    return 2;
  }
}

