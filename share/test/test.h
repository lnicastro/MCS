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

#include <math.h>

#define TEST_USER "mcstest"
#define TEST_PASS "mcstest"
#define TEST_DBNA "test"
#define TEST_TBL  "mcstest"


#define LOG(A) cout << __LINE__ << ": " << A << endl

string sERR;


void mythrow(int line, string s) {
    throw Event("", line, ERROR, MSG_ERROR, s.c_str());
}

#define ThrowErr(S...)  mythrow(__LINE__, ## S)


struct blob {
    int ii;
    float ff;
    char cc[10];
};


bool compareFloat(float f1, float f2)
{
    float f = abs((int) rint(f1 - f2));
    if (f < 0.00001)
	return true;
    else
	return false;
}


bool compareDouble(double f1, double f2)
{
    double f = abs((int) rint(f1 - f2));
    if (f < 0.00000000000001)
	return true;
    else
	return false;
}

bool compareData(Data& d1, Data& d2)
{
    int i;

    if (d1.name() != d2.name())
	ThrowErr (d1.name() + string(" != ") + d2.name());


    if (d1.type() != d2.type())
	ThrowErr(Types2Str(d1.type(), false) + string(" != ") +
		 Types2Str(d2.type(), false));

    //if (d1.flags() != d2.flags())
    //ThrowErr(itos(d1.flags()) + string(" != ") + itos(d2.flags()));

    if (d1.dim(1) != d2.dim(1))
	ThrowErr(itos(d1.dim(1)) + string(" != ") + itos(d2.dim(1)));

    if (d1.dim(2) != d2.dim(2))
	ThrowErr(itos(d1.dim(2)) + string(" != ") + itos(d2.dim(2)));

    if (d1.dim(3) != d2.dim(3))
	ThrowErr(itos(d1.dim(3)) + string(" != ") + itos(d2.dim(3)));

    if (d1.dim(4) != d2.dim(4))
	ThrowErr(itos(d1.dim(4)) + string(" != ") + itos(d2.dim(4)));

    if (d1.dim(5) != d2.dim(5))
	ThrowErr(itos(d1.dim(5)) + string(" != ") + itos(d2.dim(5)));

    if (d1.arraySize() != d2.arraySize())
	ThrowErr(itos(d1.arraySize()) + string(" != ") + itos(d2.arraySize()));


    if (d1.maxLength() != d2.maxLength())
	ThrowErr(itos(d1.maxLength()) + string(" != ") + itos(d2.maxLength()));

    if (d1.length() != d2.length())
	ThrowErr(itos(d1.length()) + string(" != ") + itos(d2.length()));

    if (d1.isUnsigned() != d2.isUnsigned())
	ThrowErr(itos(d1.isUnsigned()) + string(" != ") + itos(d2.isUnsigned()));

    if (d1.isNull() != d2.isNull())
	ThrowErr(itos(d1.isNull()) + string(" != ") + itos(d2.isNull()));

    d1(); //Reset arrpos to 0
    d2();

    if (d1.type() == TIME)
      for (unsigned int i=0; i<d1.arraySize(); i++) {
	string s1 = d1(i).sval();
	string s2 = d2(i).sval();

	if (s1 != s2)
	  ThrowErr(s1 + string(" != ") + s2);
      }
    else
      if ((i = memcmp(d1.buffer(), d2.buffer(),
		      d1.maxLength() * d1.arraySize()))) {
	
	cout << endl << endl;
	cout << d1.print() << endl << endl;
	cout << d2.print() << endl << endl;
	
	ThrowErr("Buffer differs: " + itos(i));
      }

    /*
    if (d1.type() != BLOB)
      if (d1.sval() != d2.sval())
	ThrowErr(d1.sval() + string(" != ") + d2.sval());

//    cout << hexDump((const char*) d1.buffer(), d1.maxLength())
//	 << endl << endl
//	 << hexDump((const char*) d2.buffer(), d2.maxLength()) << endl;

    if (d1.type() != TIME)
      if ((i = memcmp(d1.buffer(), d2.buffer(), d1.maxLength())))
	ThrowErr("Buffer differs: " + itos(i));
    */

    return true;
}
