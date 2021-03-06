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

  int i;
  //float f;
  //double d;
  //char* c;
  //time_t t;
  //blob b;

  void* buf;
  unsigned int size;
  bool fl;


  //The MCS library provide three class by which user can store data:
  //Data, Record and RecordSet. Each class correspond to a different
  //level of data complexity.

  //The first level correspond to the Data class, which can be used to
  //store single data (we will see a little further that this is not
  //exctly correct), that is a number, a string, a date/time, a binary
  //object, etc... One of the major feature of a Data object is that
  //it can be used "almost" like a dynamic type variable, mostly like
  //in other programming languages environments. For example you can
  //create a Data object in the following way:

  Data s(STRING, 20);

  //Now you can assign any type of data to the object:

  //Assign an integer.
  s = 12;

  //Assigning a floating point number.
  s = 1.2;

  //Assign a date/time data.
  s = "2007-03-19 13:30";

  //Assign a string
  s = "My name is Giorgio";

  //Check string conversion to double
  s = "654321.123";
  if (! compareDouble(654321.123, s.dval()))
    ThrowErr("Wrong string to double conversion!");

  //Note anyway that we said "almost" like a dynamic type variable, in
  //the sense that the type is not at all dynamic but when we assign a
  //value to the object it will perform all necessary conversions to
  //cast it to correct type. So there is always a so-called "base
  //type" of the Data object, in our case it is a buffer 20 character
  //long, as can be seen from the parameters used in the constructor.


  //this means that beyond the Data object there is a buffer 20
  //character long, as can be seen from the parameters used in the
  //constructor, so we would get an error if we try to assign a string
  //longer than 20 charatcters.

  //, it will
  //automatically perform all necessary conversions:


  //Note that the parameter "STRING, 20" means that beyond the Data
  //object there is a buffer of length 20.




  //Create a Data object with base type INT. This means that internal
  //buffer has the size of an integer variable.
  Data q(INT, 0, true); //The "true" parameter means that the integer
			//will be treated as an unsigned integer

  //Assign an integer value
  q = 89101008;

  //Check conversion to double
  if (! compareDouble(89101008, q.dval()))
    ThrowErr("Wrong value!");

  //Same check with another value
  q = 89104277;
  if (! compareDouble(89104277, q.dval()))
    ThrowErr("Wrong value!");

  i = 2^31-1;
  q = i;

  unsigned int ui = 2^32;
  q = ui;

  long long int ul = 2^63 - 1;
  q = ul;

  short int si = 2^15 - 1;
  q = si;

  unsigned short int usi = 2^16;
  q = usi;

  time_t t = 0;
  q = t;

  struct tm tm;
  q = tm;


  //Create a multidimensional Data object with base type INT. The
  //array has 4 dimensions with length respectively 3, 5, 4, dynamic
  //dimension.
  Data da(INT, 0, false, "3x5x4x*");

  //Check that each dimension has the right length. Note that
  //dimension index begin at 1.
  if (da.dim(1) != 3)
    ThrowErr("Wrong number for dim 1 (should be 3)");

  if (da.dim(2) != 5)
    ThrowErr("Wrong number for dim 2 (should be 5)");

  if (da.dim(3) != 4)
    ThrowErr("Wrong number for dim 3 (should be 4)");

  if (da.dim(4) != 1)
    ThrowErr("Wrong number for dim 4 (should be 1)");

  //Note that unused dimensions always have a length of 1.
  if (da.dim(5) != 1)
    ThrowErr("Wrong number for dim 5 (should be 1)");

  //Assign an integer value, this could have be done through the "="
  //operator, i.e. "d = 5;". The value will be assigned to the (0,0,0)
  //cell of the multi-dimensional array.
  da.setival(5);

  //Assign an integer value to the cell (1, 1, 1).
  da(1, 1, 1).setival(6);

  //Same assignment but with the assignment operator "=" and in the
  //cell (1, 1, 2).
  da(1, 1, 2) = 7;

  //Assign to the last element, remember the dimension specification:
  //"3x5x4x*".
  da(2, 4, 3) = 8;

  //Check all values are correct
  if (da().ival() != 5)
    ThrowErr("");

  if (da(1, 1, 1).ival() != 6)
    ThrowErr("");

  //Note that specifying an extra dimension with index 0 has no
  //effect.
  if (da(1, 1, 1, 0).ival() != 6)
    ThrowErr("");

  if (da(1, 1, 2, 0).ival() != 7)
    ThrowErr("");

  if (da(2, 1, 2, 0).ival() != 0)
    ThrowErr("");

  if (da(2, 4, 3).ival() != 8)
    ThrowErr("");

  //Resize the last dimension, which is a dynamic one.
  da.resizeVaryingDim(5);

  //Check that all values previously assigned are still present.
  if (da().ival() != 5)
    ThrowErr("");

  if (da(1, 1, 1).ival() != 6)
    ThrowErr("");

  if (da(1, 1, 1, 0).ival() != 6)
    ThrowErr("");

  if (da(1, 1, 2, 0).ival() != 7)
    ThrowErr("");

  if (da(2, 1, 2, 0).ival() != 0)
    ThrowErr("");

  if (da(2, 4, 3).ival() != 8)
    ThrowErr("");

  //Check that unassigned cells contains zeroes.
  for (i=1; i<5; i++)
    if (da(2, 4, 3, i).ival() != 0)
      ThrowErr("");

  //Assign a value in the fourth dimension, then check for
  //correctness.
  da(2, 4, 3, 4) = 10;
  if (da(2, 4, 3, 4).ival() != 10)
    ThrowErr("");

  //Test if boundary check is correctly performed.
  fl = false;
  try {
    da(2, 4, 4).ival();  //Note that the third dimension has a length
			//of 4 (indexes from 0 to 3) so an exception
			//should be thrown.
  }
  catch(...) {
    fl = true; //Correct.
  }

  if (! fl)
    ThrowErr("");


  //Test the copy constructor.
  Data d2 = da;
  compareData(da, d2);


  //Test serialization procedures:

  //Serialize the Data object in a dynamically allocated buffer,
  //return the size in the parameter and the pointer as return value.
  buf = da.getEntireBuffer(size);

  //Unserialize the Data object in a new object istance...
  Data d3(buf);

  //...and compare it to the first one.
  compareData(da, d3);

  //Remember to free the buffer.
  free(buf);

  //Same test with a binary object.

  //Populate a binary structure.
  struct blob blob;
  blob.ii = 11;
  blob.ff = 3.1415;
  strcpy(blob.cc, "blob");

  //Create a Data object with base type BLOB, large enough to store a
  //DBBLOBTest structure.
  Data d4(NULL, BLOB, "aa3", sizeof(struct blob));

  //Copy binary data into it
  d4.setblob(&blob, sizeof(struct blob));

  //Use the hexDump facility (declared in test.hh) to print the content
  //of a binary structure.
  //cout << hexDump(&blob, sizeof(struct blob)) << endl << endl;

  //Serialize.
  buf = d4.getEntireBuffer(size);

  //Unserialize.
  Data d5(buf);

  //Compare.
  compareData(d4, d5);
  free(buf);


  cout << "Test successfull!" << endl;
  }
  catch(Event e) {
      cerr << e.where() << ": " << e.msg() << endl;
      exit(1);
  }
  catch(...) {
      cout << "Unknown error" << endl;
      exit(2);
  }
}
