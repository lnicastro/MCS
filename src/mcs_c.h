



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



/*!
  \file mcs_c.h

  \brief Declare the C wrappers to the MCS library.

  These wrappers are available as function in all interfaces.
 */

#ifndef DEF_MCS_C_H
#define DEF_MCS_C_H

#ifndef DOXYGEN_SKIP
//The following are needed to avoid redefinition's warning
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

//The following is needed for ENABLE_MYSQL
#include "mcs_config.h"
#endif //DOXYGEN_SKIP

#include "mcstypes.hh"


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


/*
  IFD - C/C++ to C Interface Descriptor
  Giorgio Calderone - gcalderone@ifc.inaf.it
*/





//Object pointer type
#define IFD_OBJP void*
#define IFD_ERR_LEN 200




struct ifd_status {
    int error;
    char errormsg[200];
    void* data;
};

typedef struct ifd_status IFD_STATUS;

//IFD routines

//IFD_C_SIMPLE_WRAP(IFD_STATUS*, ifd_new_status , , );
//IFD_C_WRAP_VOID(        ifd_del_status ,, );
char* ifd_last_error ();
int ifd_got_error ();
void ifd_reset_error ();
void* ifd_null ();
//IFD_C_WRAP_VOID(        ifd_set_error  , , ARG(char*, msg));


#ifdef IFD_EXTRA
#undef IFD_EXTRA
#endif
#define IFD_EXTRA


//--------------------------------------------------------------------
//Wrapper for Data class

//!
void* new_Data (void* dummy ,Types type ,unsigned short int maxLength ,short isunsigned);


//!
void* copy_Data (void* obj);

//!
void del_Data (void* obj);

//!Wrapper to Data::name
char* Data_name (void* obj );

//IFD_WRAP(unsigned int,           Data, flags      , This->flags()       );
Types Data_type (void* obj );
unsigned short int Data_maxLength (void* obj );
unsigned short int Data_length (void* obj );
short Data_isUnsigned (void* obj );
short Data_isNull (void* obj );
int Data_ival (void* obj );
unsigned int Data_uival (void* obj );
long long int Data_lval (void* obj );
unsigned long long int Data_ulval (void* obj );
float Data_fval (void* obj );
double Data_dval (void* obj );
char* Data_sval (void* obj );
unsigned int Data_tval (void* obj );


//IFD_WRAP_VOID(                    Data, tsval      , This->tval(t), ARG(struct tm*, t));

void Data_setNull (void* obj );

void Data_setival (void* obj ,int val);
void Data_setuival (void* obj ,unsigned int val);

void Data_setlval (void* obj ,long long int val);
void Data_setulval (void* obj ,unsigned long long int val);

void Data_setdval (void* obj ,double val);

void Data_setsval (void* obj ,char* val);

void Data_setblob (void* obj ,void* lbuf ,unsigned int size);


//IFD_WRAP_VOID(                   Data, settmstrval, This->settimeval(val),
//				  ARG(struct tm*, val));

void Data_settimeval (void* obj ,unsigned int val);

//IFD_WRAP_VOID(                   Data, parseTime  , This->parseTime(s, ts),
//				  ARG(char*, s)  ARG(struct tm*, ts));

char* Data_print (void* obj );

void Data_setTag (void* obj ,short tag);

short Data_getTag (void* obj );






//--------------------------------------------------------------------
//Wrapper for Record class
void* new_Record (void* dummy );

void del_Record (void* obj);

void Record_clear (void* obj );
unsigned int Record_count (void* obj );

void Record_addField (void* obj ,void* d);


void* Record_pop (void* obj ,int x);

void* Record_field (void* obj ,int x);

int Record_posWhoseNameIs (void* obj ,char* name);

void Record_setFieldMap (void* obj ,char* s);




//--------------------------------------------------------------------
//Wrapper for RecordSet class
short RecordSet_setFirst (void* obj );
short RecordSet_setLast (void* obj );
short RecordSet_setNext (void* obj );
short RecordSet_setPrev (void* obj );
short RecordSet_setWhereS (void* obj ,unsigned int field ,char* where);


short RecordSet_setWhereI (void* obj ,unsigned int field ,int where);


short RecordSet_setPos (void* obj ,unsigned int newpos);

void RecordSet_dump (void* obj ,char* fn);

short RecordSet_know_nRows (void* obj );
short RecordSet_eof (void* obj );
short RecordSet_alwaysSameStructure (void* obj );

unsigned int RecordSet_nRows (void* obj );
unsigned int RecordSet_nFields (void* obj );
unsigned int RecordSet_pos (void* obj );
void* RecordSet_rec (void* obj );
void* RecordSet_metarec (void* obj );






//--------------------------------------------------------------------
//Wrapper for DBConn, Query, Table classes

#if ENABLE_MYSQL
void* new_DBConn (void* dummy );

void del_DBConn (void* obj);

void DBConn_connect (void* obj ,char* user ,char* pass ,char* db ,char* host);





void DBConn_close (void* obj );
short DBConn_isOpen (void* obj );
void* DBConn_newDBConn (void* obj );





void* new_Query (void* dummy ,void* lconn ,short handleNewDBConn);



void del_Query (void* obj);


void Query_query (void* obj ,char* query ,short StoreResult);


void Query_prepare (void* obj ,char* query);


void Query_prepare_with_parameters (void* obj ,int op ,char* fields ,char* table ,char* where);





unsigned int Query_nAffectedRows (void* obj );

void* Query_param (void* obj );

void* Query_lookup (void* obj ,char* field ,char* table ,char* where);


//IFD_WRAP(short,           Query, resultRows,       This->resultRows());

short Query_gotRecordSet (void* obj );

void Query_execute (void* obj ,short StoreResult);



void Query_close (void* obj );






#undef IFD_EXTRA
#define IFD_EXTRA DBConn* conn = (DBConn*) lconn;
void* new_Table (void* dummy ,void* lconn ,char* table ,char* fieldkey);

#undef IFD_EXTRA
#define IFD_EXTRA

void del_Table (void* obj);

void Table_loadTable (void* obj );

void* Table_newRec (void* obj );

void Table_insert (void* obj );
void Table_update (void* obj );
#endif //ENABLE_MYSQL




//--------------------------------------------------------------------
//Wrapper for Client class
void* new_Client (void* dummy ,char* path ,char* server ,int port ,int ssl ,unsigned int timeout);



void del_Client (void* obj);

short Client_login (void* obj ,char* user ,char* pass ,char* db);


short Client_exec (void* obj ,char* cmd ,void* data);




void* Client_code (void* obj );

void* Client_msg (void* obj );

void* Client_out (void* obj );

void* Client_recv (void* obj );

void* Client_aux (void* obj );





//--------------------------------------------------------------------
//Wrapper Conf class
void* new_Conf (void* dummy ,char* filename);

void del_Conf (void* obj);

char* Conf_sval (void* obj ,char* section ,char* key);



int Conf_ival (void* obj ,char* section ,char* key);






#endif // DEF_MCS_C_H
