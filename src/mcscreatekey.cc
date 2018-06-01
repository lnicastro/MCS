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

int main(int argc, char* argv[])
{

#ifdef HAVE_OPENSSL_SSL_H
  unsigned int ui;
  string sslcert = string(MCS_SSLCERT);
  string sslpriv = string(MCS_SSLPRIV), s;

  if ((! mcs::File_Dir_Exist(sslcert, ui))   ||
      (! mcs::File_Dir_Exist(sslpriv, ui))      ) {

    //Create certificate and private key
    int i;
    string s;

    cout << "Creating SSL private key and certificate for MCS server..." << endl << endl;

    cout << "The private key can be protected by a password, this is needed if the " << endl;
    cout << sslpriv << " file can be read by other users." << endl;
    cout << "If you choose to provide a password then you must enter that password " << endl;
    cout << "each time you execute the server." << endl << endl;

    cout << "Protect the private key with a password (y/n) ? [y]" << endl;
    cin >> s;

    s = ( ((s == "") || (s[0] == 'y') || (s[0] == 'Y'))   ?  " -des3 "  :  "");

    s = "openssl genrsa " + s + " -out " + string(MCS_SSLPRIV) + " 2048";
    cout << endl << endl << s << endl;
    i = system(s.c_str());
    if (i != 0) {
      cerr << "Error executing openssl" << endl;
      exit(i);
    }

    s = "openssl req -new -x509 -batch -key " + string(MCS_SSLPRIV) +
      " -out " + string(MCS_SSLCERT);
    cout << endl << endl << s << endl;

    i = system(s.c_str());
    if (i != 0) {
      cerr << "Error executing openssl" << endl;
      exit(i);
    }
  }
  else {
    cout << "Files for private key and certificate already exists:" << endl;
    cout << "    " << sslpriv << endl;
    cout << "    " << sslcert << endl;
  }
#else //HAVE_OPENSSL_SSL_H
  cerr << "The SSL support has been disabled, try: ./configure --enable-openssl" << endl;
  exit(1);
#endif //HAVE_OPENSSL_SSL_H

  //openssl genrsa -des3 -out privkey.pem 2048
  //openssl req -new -x509 -key privkey.pem -out cacert.pem -days 1095
}
