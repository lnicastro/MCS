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
    Record qt(true);
    int i;

    Data aa1(NULL, INT, "aa1", 0, true, 34);
    aa1 = 6523;


    LOG("Testing class Record...");
    qt.addField(aa1);
    qt.addField(1);
    qt.addField(2);

    if (qt.count() != 3)
      ThrowErr("Wrong number of elements: " + itos(qt.count()));
    i = qt.pop().ival();

    if (i != 6523)
      ThrowErr("Wrong value: " + itos(i));

    if (qt.count() != 2)
      ThrowErr("Wrong number of elements: " + itos(qt.count()));
    i = qt.pop().ival();
    if (i != 1)
      ThrowErr("Wrong value: " + itos(i));

    if (qt.count() != 1)
      ThrowErr("Wrong number of elements: " + itos(qt.count()));
    i = qt.pop().ival();
    if (i != 2)
      ThrowErr("Wrong value: " + itos(i));

    if (qt.count() != 0)
      ThrowErr("Wrong number of elements: " + itos(qt.count()));


    //------------------------------------------------------------
    LOG("Dynamic_Array Vector...");
    Dynamic_Array<int> daint(false);
    i = 1;
    daint.push(i);

    i = 2;
    daint.push(i);

    cout << daint.count() << endl;
    for (i=0; i<daint.count(); i++)
      cout << i << " " << daint[i] << endl;

    //------------------------------------------------------------
    LOG("Testing Record...");
    Record* v = new Record();

    Data d_str(STRING, 10);
    d_str = "ciao";

    v->addField(aa1);
    v->addField(d_str);
    v->addField(aa1);


    if ((*v)[0].sval() != "6523") ThrowErr("");
    if ((*v)[1].sval() != "ciao") ThrowErr("");
    if ((*v)[2].sval() != "6523") ThrowErr("");


    for (int i=0; i<v->count(); i++)
      cout << (*v)[i].sval() << endl;

    LOG("Testing RecordSet...");
    RecordSet* rs = new RecordSet();
    rs->insert(v);
    rs->insert(*v);

    if (rs->nRows() != 2)
      ThrowErr("Wrong number of rows (should be 2");

    rs->setFirst();
    do {
      Record& vref = rs->rec();

      for (int i=0; i<vref.count(); i++)
	cout << vref[i].sval() << endl;
    }
    while (rs->setNext());


    //delete v;  Don't delete v becuase it will be deleted by rs
    delete rs;


    //Create an empty recordset.
    rs = new RecordSet();
    Record* rec = new Record();

    DBConn db;
    db.connect(TEST_USER, TEST_PASS, TEST_DBNA, "localhost");

    Query* q = new Query(db.newDBConn());
    q->prepare("SELECT * FROM mcstest");
    q->execute(true);

    while (! q->eof()) {
      cout << q->rec()[0].sval() << endl;
      rec->addField(q->rec()[0]);
      q->setNext();
    }

    delete q;
  }
  catch (Event e) {
    cerr << e.msg() << endl;
  }
}
