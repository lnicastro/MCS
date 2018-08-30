



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

#include "mcs.hh"
using namespace mcs;





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

static IFD_STATUS status;

/*
#define IFD_C_WRAP(RETTYPE, FUNCTION, CALL, ARGS...)                  extern "C"                                                            RETTYPE FUNCTION (ARGS)                                               {
return (RETTYPE) CALL;
}
      \
}


//IFD routines
#define IFD_C_WRAP_VOID(FUNCTION, CALL, ARGS...)                      extern "C"                                                            RETTYPE FUNCTION (ARGS)                                               {
CALL;
return;
}
        \
}
*/

extern "C" {
  void ifd_reset_error()
  {
    status.error = 0;
    strncpy(status.errormsg, "", 1);
  }

  //IFD_STATUS* ifd_new_status()
  //{
  //    IFD_STATUS* ret = (IFD_STATUS*) malloc(sizeof(IFD_STATUS));
  //    ifd_reset_error(ret);
  //    return ret;
  //}

  //void ifd_del_status(IFD_STATUS* _st)
  //{
  //    if (_st)
  //	free(_st);
  //}


  int ifd_got_error()
  {
    return (status.error != 0 ? 1 : 0);
  }


  char* ifd_last_error()
  {
    return status.errormsg;
  }

  //void ifd_set_error(char* msg, IFD_STATUS* _st)
  //{
  //    _st->error = 1;
  //    strncpy(_st->errormsg, msg, IFD_ERR_LEN);
  //}


  void* ifd_null()
  {
    return (void*) 0;
  }
}
//extern "C"


#ifdef IFD_EXTRA
#undef IFD_EXTRA
#endif
#define IFD_EXTRA


//--------------------------------------------------------------------
//Wrapper for Data class

//!
extern "C" void* new_Data (void* dummy ,Types type ,unsigned short int maxLength ,short isunsigned) {
if (status.error) return (void*) 0;
try {
IFD_EXTRA;
Data* This = new Data (type, maxLength, (bool) isunsigned);
return (void*) This;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
}
                                                                                           ;

//!
extern "C" void* copy_Data (void* obj) {
if (status.error) return (void*) 0;
try {
Data* This = (Data*) obj;
return (void*) new Data(*This);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

//!
extern "C" void del_Data (void* obj) {
if (status.error) return ;
try {;
Data* This = (Data*) obj;
delete This;
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

//!Wrapper to Data::name
extern "C" char* Data_name (void* obj ) {
if (status.error) return (char*) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (char*) This->name().c_str();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (char*) 0;
};

//IFD_WRAP(unsigned int,           Data, flags      , This->flags()       );
extern "C" Types Data_type (void* obj ) {
if (status.error) return (Types) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (Types) This->type();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (Types) 0;
};
extern "C" unsigned short int Data_maxLength (void* obj ) {
if (status.error) return (unsigned short int) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (unsigned short int) This->maxLength();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned short int) 0;
};
extern "C" unsigned short int Data_length (void* obj ) {
if (status.error) return (unsigned short int) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (unsigned short int) This->length();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned short int) 0;
};
extern "C" short Data_isUnsigned (void* obj ) {
if (status.error) return (short) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (short) This->isUnsigned();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" short Data_isNull (void* obj ) {
if (status.error) return (short) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (short) This->isNull();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" int Data_ival (void* obj ) {
if (status.error) return (int) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (int) This->ival();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (int) 0;
};
extern "C" unsigned int Data_uival (void* obj ) {
if (status.error) return (unsigned int) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (unsigned int) This->uival();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned int) 0;
};
extern "C" long long int Data_lval (void* obj ) {
if (status.error) return (long long int) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (long long int) This->lval();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (long long int) 0;
};
extern "C" unsigned long long int Data_ulval (void* obj ) {
if (status.error) return (unsigned long long int) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (unsigned long long int) This->ulval();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned long long int) 0;
};
extern "C" float Data_fval (void* obj ) {
if (status.error) return (float) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (float) This->fval();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (float) 0;
};
extern "C" double Data_dval (void* obj ) {
if (status.error) return (double) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (double) This->dval();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (double) 0;
};

#undef IFD_EXTRA
#define IFD_EXTRA static string s; s = This->sval();
extern "C" char* Data_sval (void* obj ) {
if (status.error) return (char*) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (char*) s.c_str();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (char*) 0;
};
#undef IFD_EXTRA
#define IFD_EXTRA

extern "C" unsigned int Data_tval (void* obj ) {
if (status.error) return (unsigned int) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (unsigned int) This->tval();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned int) 0;
};


//IFD_WRAP_VOID(                    Data, tsval      , This->tval(t), ARG(struct tm*, t));

extern "C" void Data_setNull (void* obj ) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setNull();
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void Data_setival (void* obj ,int val) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setival(val);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};
extern "C" void Data_setuival (void* obj ,unsigned int val) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setuival(val);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void Data_setlval (void* obj ,long long int val) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setlval(val);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};
extern "C" void Data_setulval (void* obj ,unsigned long long int val) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setulval(val);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void Data_setdval (void* obj ,double val) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setdval(val);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void Data_setsval (void* obj ,char* val) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setsval(val);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void Data_setblob (void* obj ,void* lbuf ,unsigned int size) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setblob(lbuf, size);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
}
                                               ;

//IFD_WRAP_VOID(                   Data, settmstrval, This->settimeval(val),
//				  ARG(struct tm*, val));

extern "C" void Data_settimeval (void* obj ,unsigned int val) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->settimeval(val);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

//IFD_WRAP_VOID(                   Data, parseTime  , This->parseTime(s, ts),
//				  ARG(char*, s)  ARG(struct tm*, ts));

extern "C" char* Data_print (void* obj ) {
if (status.error) return (char*) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (char*) This->print().c_str();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (char*) 0;
};

extern "C" void Data_setTag (void* obj ,short tag) {
if (status.error) return ;
try {
Data* This = (Data*) obj;
This->setTag(tag);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" short Data_getTag (void* obj ) {
if (status.error) return (short) 0;
try {
Data* This = (Data*) obj;
IFD_EXTRA;
return (short) This->getTag();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};






//--------------------------------------------------------------------
//Wrapper for Record class
extern "C" void* new_Record (void* dummy ) {
if (status.error) return (void*) 0;
try {
IFD_EXTRA;
Record* This = new Record ();
return (void*) This;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void del_Record (void* obj) {
if (status.error) return ;
try {;
Record* This = (Record*) obj;
delete This;
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void Record_clear (void* obj ) {
if (status.error) return ;
try {
Record* This = (Record*) obj;
This->clear();
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};
extern "C" unsigned int Record_count (void* obj ) {
if (status.error) return (unsigned int) 0;
try {
Record* This = (Record*) obj;
IFD_EXTRA;
return (unsigned int) This->count();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned int) 0;
};

extern "C" void Record_addField (void* obj ,void* d) {
if (status.error) return ;
try {
Record* This = (Record*) obj;
This->addField(* ((Data*) d));
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
}
                    ;

extern "C" void* Record_pop (void* obj ,int x) {
if (status.error) return (void*) 0;
try {
Record* This = (Record*) obj;
IFD_EXTRA;
return (void*) new Data(This->pop(x));
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void* Record_field (void* obj ,int x) {
if (status.error) return (void*) 0;
try {
Record* This = (Record*) obj;
IFD_EXTRA;
return (void*) &(This->operator[](x));
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" int Record_posWhoseNameIs (void* obj ,char* name) {
if (status.error) return (int) 0;
try {
Record* This = (Record*) obj;
IFD_EXTRA;
return (int) This->posWhoseNameIs(name);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (int) 0;
};

extern "C" void Record_setFieldMap (void* obj ,char* s) {
if (status.error) return ;
try {
Record* This = (Record*) obj;
This->setFieldMap(s);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};




//--------------------------------------------------------------------
//Wrapper for RecordSet class
extern "C" short RecordSet_setFirst (void* obj ) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->setFirst();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" short RecordSet_setLast (void* obj ) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->setLast();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" short RecordSet_setNext (void* obj ) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->setNext();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" short RecordSet_setPrev (void* obj ) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->setPrev();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" short RecordSet_setWhereS (void* obj ,unsigned int field ,char* where) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->setWhere(field, where);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
}
                                               ;

extern "C" short RecordSet_setWhereI (void* obj ,unsigned int field ,int where) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->setWhere(field, where);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
}
                                             ;

extern "C" short RecordSet_setPos (void* obj ,unsigned int newpos) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->setPos(newpos);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};

extern "C" void RecordSet_dump (void* obj ,char* fn) {
if (status.error) return ;
try {
RecordSet* This = (RecordSet*) obj;
This->dump(fn);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" short RecordSet_know_nRows (void* obj ) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->know_nRows();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" short RecordSet_eof (void* obj ) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->eof();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" short RecordSet_alwaysSameStructure (void* obj ) {
if (status.error) return (short) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (short) This->alwaysSameStructure();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};

extern "C" unsigned int RecordSet_nRows (void* obj ) {
if (status.error) return (unsigned int) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (unsigned int) This->nRows();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned int) 0;
};
extern "C" unsigned int RecordSet_nFields (void* obj ) {
if (status.error) return (unsigned int) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (unsigned int) This->nFields();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned int) 0;
};
extern "C" unsigned int RecordSet_pos (void* obj ) {
if (status.error) return (unsigned int) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (unsigned int) This->pos();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned int) 0;
};
extern "C" void* RecordSet_rec (void* obj ) {
if (status.error) return (void*) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (void*) &(This->rec());
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};
extern "C" void* RecordSet_metarec (void* obj ) {
if (status.error) return (void*) 0;
try {
RecordSet* This = (RecordSet*) obj;
IFD_EXTRA;
return (void*) &(This->metarec());
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};






//--------------------------------------------------------------------
//Wrapper for DBConn, Query, Table classes

#if ENABLE_MYSQL
extern "C" void* new_DBConn (void* dummy ) {
if (status.error) return (void*) 0;
try {
IFD_EXTRA;
DBConn* This = new DBConn ();
return (void*) This;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void del_DBConn (void* obj) {
if (status.error) return ;
try {;
DBConn* This = (DBConn*) obj;
delete This;
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void DBConn_connect (void* obj ,char* user ,char* pass ,char* db ,char* host) {
if (status.error) return ;
try {
DBConn* This = (DBConn*) obj;
This->connect(user, pass, db , host);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
}


                                      ;


extern "C" void DBConn_close (void* obj ) {
if (status.error) return ;
try {
DBConn* This = (DBConn*) obj;
This->close();
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};
extern "C" short DBConn_isOpen (void* obj ) {
if (status.error) return (short) 0;
try {
DBConn* This = (DBConn*) obj;
IFD_EXTRA;
return (short) This->isOpen();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};
extern "C" void* DBConn_newDBConn (void* obj ) {
if (status.error) return (void*) 0;
try {
DBConn* This = (DBConn*) obj;
IFD_EXTRA;
return (void*) This->newDBConn();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};





extern "C" void* new_Query (void* dummy ,void* lconn ,short handleNewDBConn) {
if (status.error) return (void*) 0;
try {
IFD_EXTRA;
Query* This = new Query ((DBConn*) lconn, (bool) handleNewDBConn);
return (void*) This;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
}
                                                    ;


extern "C" void del_Query (void* obj) {
if (status.error) return ;
try {;
Query* This = (Query*) obj;
delete This;
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};


extern "C" void Query_query (void* obj ,char* query ,short StoreResult) {
if (status.error) return ;
try {
Query* This = (Query*) obj;
This->query(query, (bool) StoreResult);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
}
                                                   ;

extern "C" void Query_prepare (void* obj ,char* query) {
if (status.error) return ;
try {
Query* This = (Query*) obj;
This->prepare(query);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
}
                          ;

extern "C" void Query_prepare_with_parameters (void* obj ,int op ,char* fields ,char* table ,char* where) {
if (status.error) return ;
try {
Query* This = (Query*) obj;
This->prepare_with_parameters(op, string(fields), table, where);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
}


                                             ;


extern "C" unsigned int Query_nAffectedRows (void* obj ) {
if (status.error) return (unsigned int) 0;
try {
Query* This = (Query*) obj;
IFD_EXTRA;
return (unsigned int) This->nAffectedRows();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (unsigned int) 0;
};

extern "C" void* Query_param (void* obj ) {
if (status.error) return (void*) 0;
try {
Query* This = (Query*) obj;
IFD_EXTRA;
return (void*) &(This->param());
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void* Query_lookup (void* obj ,char* field ,char* table ,char* where) {
if (status.error) return (void*) 0;
try {
Query* This = (Query*) obj;
IFD_EXTRA;
return (void*) &(This->lookup(field, table, where));
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
}
                                                           ;

//IFD_WRAP(short,           Query, resultRows,       This->resultRows());

extern "C" short Query_gotRecordSet (void* obj ) {
if (status.error) return (short) 0;
try {
Query* This = (Query*) obj;
IFD_EXTRA;
return (short) This->gotRecordSet();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
};

extern "C" void Query_execute (void* obj ,short StoreResult) {
if (status.error) return ;
try {
Query* This = (Query*) obj;
This->execute(StoreResult);
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
}
                             ;


extern "C" void Query_close (void* obj ) {
if (status.error) return ;
try {
Query* This = (Query*) obj;
This->close();
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};






#undef IFD_EXTRA
#define IFD_EXTRA DBConn* conn = (DBConn*) lconn;
extern "C" void* new_Table (void* dummy ,void* lconn ,char* table ,char* fieldkey) {
if (status.error) return (void*) 0;
try {
IFD_EXTRA;
Table* This = new Table (conn, table, fieldkey);
return (void*) This;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
}
                                                                ;
#undef IFD_EXTRA
#define IFD_EXTRA

extern "C" void del_Table (void* obj) {
if (status.error) return ;
try {;
Table* This = (Table*) obj;
delete This;
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void Table_loadTable (void* obj ) {
if (status.error) return ;
try {
Table* This = (Table*) obj;
This->loadTable();
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" void* Table_newRec (void* obj ) {
if (status.error) return (void*) 0;
try {
Table* This = (Table*) obj;
IFD_EXTRA;
return (void*) &(This->newRec());
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void Table_insert (void* obj ) {
if (status.error) return ;
try {
Table* This = (Table*) obj;
This->insert();
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};
extern "C" void Table_update (void* obj ) {
if (status.error) return ;
try {
Table* This = (Table*) obj;
This->update();
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};
#endif //ENABLE_MYSQL




//--------------------------------------------------------------------
//Wrapper for Client class
extern "C" void* new_Client (void* dummy ,char* path ,char* server ,int port ,int ssl ,unsigned int timeout) {
if (status.error) return (void*) 0;
try {
IFD_EXTRA;
Client* This = new Client (path, server, port, false, ssl, timeout);
return (void*) This;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
}

                                           ;

extern "C" void del_Client (void* obj) {
if (status.error) return ;
try {;
Client* This = (Client*) obj;
delete This;
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};

extern "C" short Client_login (void* obj ,char* user ,char* pass ,char* db) {
if (status.error) return (short) 0;
try {
Client* This = (Client*) obj;
IFD_EXTRA;
return (short) This->login(user, pass, db);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
}
                                                      ;

extern "C" short Client_exec (void* obj ,char* cmd ,void* data) {
if (status.error) return (short) 0;
try {
Client* This = (Client*) obj;
IFD_EXTRA;
return (short) This->exec(cmd, (Data*) data);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (short) 0;
}
                                        ;



extern "C" void* Client_code (void* obj ) {
if (status.error) return (void*) 0;
try {
Client* This = (Client*) obj;
IFD_EXTRA;
return (void*) &(This->code);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void* Client_msg (void* obj ) {
if (status.error) return (void*) 0;
try {
Client* This = (Client*) obj;
IFD_EXTRA;
return (void*) &(This->msg);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void* Client_out (void* obj ) {
if (status.error) return (void*) 0;
try {
Client* This = (Client*) obj;
IFD_EXTRA;
return (void*) &(This->out);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void* Client_recv (void* obj ) {
if (status.error) return (void*) 0;
try {
Client* This = (Client*) obj;
IFD_EXTRA;
return (void*) &(This->recv);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void* Client_aux (void* obj ) {
if (status.error) return (void*) 0;
try {
Client* This = (Client*) obj;
IFD_EXTRA;
return (void*) &(This->aux);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};





//--------------------------------------------------------------------
//Wrapper Conf class
extern "C" void* new_Conf (void* dummy ,char* filename) {
if (status.error) return (void*) 0;
try {
IFD_EXTRA;
Conf* This = new Conf (filename);
return (void*) This;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (void*) 0;
};

extern "C" void del_Conf (void* obj) {
if (status.error) return ;
try {;
Conf* This = (Conf*) obj;
delete This;
return;
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return;
};


#undef IFD_EXTRA
#define IFD_EXTRA static string s; s = This->sval(section, key, "")
extern "C" char* Conf_sval (void* obj ,char* section ,char* key) {
if (status.error) return (char*) 0;
try {
Conf* This = (Conf*) obj;
IFD_EXTRA;
return (char*) s.c_str();
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (char*) 0;
}

                                        ;
#undef IFD_EXTRA
#define IFD_EXTRA


extern "C" int Conf_ival (void* obj ,char* section ,char* key) {
if (status.error) return (int) 0;
try {
Conf* This = (Conf*) obj;
IFD_EXTRA;
return (int) This->ival(section, key, -1);
}
catch(Event e) {
strncpy(status.errormsg, e.msg().c_str(), 200);
}
catch(exception e) {
strncpy(status.errormsg, e.what(), 200);
}
status.error = 1;
return (int) 0;
}

                                        ;
