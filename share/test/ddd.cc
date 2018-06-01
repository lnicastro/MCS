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

#include "test.h"

int main()
{
    DBConn db;
    Query* qry;

    try {
	db.connect(TEST_USER, TEST_PASS, TEST_DBNA, "localhost");

	qry = new Query(&db);

	qry->query("CREATE TABLE IF NOT EXISTS pippo (c CHAR(10), i INT, f FLOAT)");
	qry->query("SELECT * FROM pippo");
	qry->query("DROP TABLE pippo");
    }
    catch (Event e) {
	cerr << e.msg() << endl;
    }
}
