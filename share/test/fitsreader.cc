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


int main(int argc, char* argv[])
{
  FITSReader fits;
  int i, j, hdu;

  try {
    for (i=1; i<argc; i++) {
      cout  << endl  << endl << "Reading: " << argv[i] << endl;

      fits.open(argv[i]);

      for (hdu=1; hdu<=fits.HDUCount(); hdu++) {
	fits.selectHDU(hdu);
	cout << endl << "HDU #" << fits.currentHDU() << endl;

	//Print header
	for (j=0; j<fits.header.count(); j++) {
	  cout <<
	    fits.header[j].name() << " = " <<
	    fits.header[j].sval() << " / " <<
	    fits.header_comments[j].sval() << endl;
	}
	
	cout <<"BITPIX="<< fits.header["BITPIX"].sval() << endl;

cout << "there are "<< fits.metarec().count()<< " fields."<<endl;
	//Print field names
	cout << fits.metarec().asStringNames() << endl;
	
	//Print field types
	cout << fits.metarec().asStringTypes() << endl;
	
	//Print data
/*
	while (! fits.eof()) {
	  cout << fits.rec().asString() << endl;
	  fits.setNext();
	}
*/
      }

      fits.close();
    }
  }
  catch(Event& e) {
    cerr << e.msg() << endl;
  }
}


