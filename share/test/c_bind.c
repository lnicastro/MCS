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

#include <stdio.h>
#include <time.h>

#include <mcs_c.h>

#define USER   "mcstest"
#define PASS   "mcstest"
#define DBNAME "test"


int main()
{
    int i, j, nrows;
    IFD_OBJP null = 0;
    null = ifd_null();

    char* query;

    printf("Testing class Data...\n");
    IFD_OBJP  p = new_Data(0, FLOAT, 0, 0);

    Data_setival(p, 6);
    printf("%d %s\n", ifd_got_error(), Data_print(p));

    Data_setsval(p, "5.7");
    printf("%d %s\n", ifd_got_error(), Data_print(p));

    Data_setsval(p, "ciao");
    printf("%d %s\n", ifd_got_error(), Data_print(p));

    if (ifd_got_error()) {
	printf("%s\n", ifd_last_error());
    }
    else {
	printf("An error is expected\n");
	return 1;
    }

    ifd_reset_error();
    del_Data(p);


    //-------------------------------------------------
    printf("\n\n\nTesting class DBConn...\n");
    ifd_reset_error();
    p = new_DBConn(0);

    printf("DB connection is open: %d\n", DBConn_isOpen(p));

    printf("Connecting...\n");
    DBConn_connect(p, USER, PASS, DBNAME, "");
    printf("DB connection is open: %d\n", DBConn_isOpen(p));

    if (ifd_got_error()) {
        printf("%s\n", ifd_last_error());
	return 1;
    }


    //-------------------------------------------------
    printf("\n\n\nTesting class Query...\n");
    ifd_reset_error();
    IFD_OBJP dbq = new_Query(0, p, 0);

    query = "SELECT * FROM mcstest";
    printf("Preparing query: %s\n", query);
    Query_prepare(dbq, query);

    printf("Executing query...\n");
    Query_execute(dbq, 1);

    printf("Is a SELECT-like query: %d\n", Query_gotRecordSet(dbq));

    if (Query_gotRecordSet(dbq)) {
	printf("Number of returned rows: %d\n", RecordSet_nRows(dbq));
	printf("Printing rows...\n");

	//-------------------------------------------------
	printf("\n\n\nTesting class Record...\n");

	RecordSet_setFirst(dbq);
	i = 0;
	while (! RecordSet_eof(dbq)) {
	    IFD_OBJP vec = RecordSet_rec(dbq);
	
	    if (i==0) { //First row, print head
		for (j=0; j<Record_count(vec); j++) {
		    IFD_OBJP d = Record_field(vec, j);
		    printf("%s\t", Data_name(d));
		}
		printf ("\n");
	    }

	    //print data
	    for (j=0; j<Record_count(vec); j++) {
		IFD_OBJP d = Record_field(vec, j);
		if (Data_isNull(d))
		  printf("(NULL)\t");
		else
		  printf("%s\t", Data_sval(d));
	    }
	    printf ("\n");
	
	    i++;
	    RecordSet_setNext(dbq);
	}
    }
    else
      printf("Number of affected rows: %d\n", Query_nAffectedRows(dbq));

    Query_close(dbq);


    if (ifd_got_error()) {
        printf("%s\n", ifd_last_error());
	return 1;
    }

    ifd_reset_error();
    del_Query(dbq);

    ifd_reset_error();
    del_DBConn(p);


    ifd_reset_error();
    printf("\n\n\nTesting class Client...\n");

    IFD_OBJP cnf = new_Conf(0, "mcs.conf");
    int port = Conf_ival(cnf, "test", "PORT");
    int ssl = Conf_ival(cnf, "test", "use_ssl");

    IFD_OBJP cli = new_Client(0, "", "localhost", port, ssl, 60000);

    printf("Login...\n");
    Client_login(cli, USER, PASS, DBNAME);

    if (ifd_got_error()) {
        printf("%s\n", ifd_last_error());
	return 1;
    }

    query = "qry SELECT * FROM mcstest";
    printf("Executing command: %s\n", query);
    Client_exec(cli, query, NULL);

    nrows = RecordSet_nRows(cli);
    printf("Number of rows: %d\n", nrows);

    printf("Print all result from query....\n");
    for (i=0; i<nrows; i++) {
      RecordSet_setPos(cli, i);
      p = RecordSet_rec(cli);

	if (i == 0) { //Print head
	    for (j=0; j<Record_count(p); j++) {
		if (Data_type(Record_field(p, j)) != BLOB) {
		    printf("%s\t", Data_name(Record_field(p, j)));
		}
	    }
	    printf("\n");
	}

	for (j=0; j<Record_count(p); j++) {
	    if (Data_type(Record_field(p, j)) != BLOB) {
		printf("%s\t", Data_sval(Record_field(p, j)));
	    }
	}
	printf("\n");
    }
    printf("\n");


    if (ifd_got_error()) {
        printf("%s\n", ifd_last_error());
	return 1;
    }


    ifd_reset_error();
    del_Client(cli);

    return 0;
}
