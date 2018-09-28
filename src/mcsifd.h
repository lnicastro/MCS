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
  Giorgio Calderone - giorgio.calderone@inaf.it
*/

#define NOARGS
#define ARG(TYPE, NAME) ,TYPE NAME


//Object pointer type
###define IFD_OBJP   void*
###define IFD_ERR_LEN 200

#define IFD_OBJP   void*
#define IFD_ERR_LEN 200

struct ifd_status {
    int  error;
    char errormsg[IFD_ERR_LEN+1];
    void* data;
};

typedef struct ifd_status IFD_STATUS;
							


#ifdef WANT_HEAD  //---------------------------- Interface header

#define IFD_CONSTRUCTOR(CLASS, CALLARGS, ARGS...)                     \
IFD_OBJP new ## _ ## CLASS (void* dummy  ARGS)
							
#define IFD_COPY_CONSTRUCTOR(CLASS)                                   \
IFD_OBJP copy ## _ ## CLASS (IFD_OBJP obj)
							
#define IFD_DESTRUCTOR(CLASS)                                         \
void del ## _ ## CLASS (IFD_OBJP obj)

#define IFD_WRAP(RETTYPE, CLASS, METHOD, CALL, ARGS...)               \
RETTYPE CLASS ## _ ## METHOD (IFD_OBJP obj  ARGS)

#define IFD_WRAP_VOID(CLASS, METHOD, CALL, ARGS...)                   \
void  CLASS ## _ ## METHOD (IFD_OBJP obj  ARGS)

#define IFD_C_WRAP(RETTYPE, FUNCTION, CALL, ARGS...)                  \
RETTYPE FUNCTION (ARGS)

#define IFD_C_WRAP_VOID(FUNCTION, CALL, ARGS...)                      \
void  FUNCTION (ARGS)
#endif //WANT_HEAD













#ifdef WANT_DESC  //----------------------- Interface description
//gcc -E -DWANT_DESC mcs_c.h | grep -v '#' > pre.h

#define IFD_CONSTRUCTOR(CLASS, CALLARGS, ARGS...)                     \
function IFD_OBJP new ## _ ## CLASS (void* dummy  ARGS)
							
#define IFD_COPY_CONSTRUCTOR(CLASS)                                   \
function IFD_OBJP copy ## _ ## CLASS (IFD_OBJP obj)
							
#define IFD_DESTRUCTOR(CLASS)                                         \
function void del ## _ ## CLASS (IFD_OBJP obj)

#define IFD_WRAP(RETTYPE, CLASS, METHOD, CALL, ARGS...)               \
function RETTYPE CLASS ## _ ## METHOD (IFD_OBJP obj  ARGS)

#define IFD_WRAP_VOID(CLASS, METHOD, CALL, ARGS...)                   \
function void  CLASS ## _ ## METHOD (IFD_OBJP obj  ARGS)

#define IFD_C_WRAP(RETTYPE, FUNCTION, CALL, ARGS...)                  \
function RETTYPE FUNCTION (ARGS)

#define IFD_C_WRAP_VOID(FUNCTION, CALL, ARGS...)                      \
function void  FUNCTION (ARGS)
#endif  //WANT_DESC










//IFD routines
#ifndef WANT_CODE
//IFD_C_SIMPLE_WRAP(IFD_STATUS*, ifd_new_status , , );
//IFD_C_WRAP_VOID(        ifd_del_status ,, );
IFD_C_WRAP(char*      , ifd_last_error ,, );
IFD_C_WRAP(int        , ifd_got_error  ,, );
IFD_C_WRAP_VOID(        ifd_reset_error,, );
IFD_C_WRAP(IFD_OBJP   , ifd_null, , );
//IFD_C_WRAP_VOID(        ifd_set_error  , , ARG(char*, msg));
#endif //! WANT_CODE







#ifdef WANT_CODE //--------------------------------------Code
static IFD_STATUS status;

#define IFD_BEGIN_HANDLE_EXCEPTION(RETWHAT)                           \
if (status.error)                                                     \
  return RETWHAT;       			      	              \
try {							
    							
							
#define IFD_END_HANDLE_EXCEPTION                                      \
}							              \
catch(Event e) {				                      \
    strncpy(status.errormsg, e.msg().c_str(), IFD_ERR_LEN);           \
}							              \
catch(exception e) {					              \
    strncpy(status.errormsg, e.what(), IFD_ERR_LEN);                  \
}							              \
status.error = 1;                                                     \



#define IFD_CONSTRUCTOR(CLASS, CALLARGS, ARGS...)                     \
extern "C"                                                            \
IFD_OBJP new ## _ ## CLASS (void* dummy  ARGS)                        \
{                                                                     \
  IFD_BEGIN_HANDLE_EXCEPTION( (IFD_OBJP) 0)                           \
  IFD_EXTRA;                                                          \
  CLASS* This = new CLASS CALLARGS;                                   \
  return (IFD_OBJP) This;                                             \
  IFD_END_HANDLE_EXCEPTION                                            \
  return (IFD_OBJP) 0;                                                \
}


#define IFD_COPY_CONSTRUCTOR(CLASS)                                   \
extern "C"                                                            \
IFD_OBJP copy ## _ ## CLASS (IFD_OBJP obj)                            \
{                                                                     \
  IFD_BEGIN_HANDLE_EXCEPTION( (IFD_OBJP) 0)                           \
  CLASS* This = (CLASS*) obj;                                         \
  return (IFD_OBJP) new CLASS(*This);                                 \
  IFD_END_HANDLE_EXCEPTION                                            \
  return (IFD_OBJP) 0;                                                \
}							
							
							
#define IFD_DESTRUCTOR(CLASS)                                         \
extern "C"                                                            \
void del ## _ ## CLASS (IFD_OBJP obj)                                 \
{                                                                     \
  IFD_BEGIN_HANDLE_EXCEPTION(NOARGS);                                 \
  CLASS* This = (CLASS*) obj;                                         \
  delete This;                                                        \
  return;                                                             \
  IFD_END_HANDLE_EXCEPTION                                            \
  return;                                                             \
}


#define IFD_WRAP(RETTYPE, CLASS, METHOD, CALL, ARGS...)               \
extern "C"                                                            \
RETTYPE CLASS ## _ ## METHOD (IFD_OBJP obj  ARGS)                     \
{                                                                     \
  IFD_BEGIN_HANDLE_EXCEPTION( (RETTYPE) 0)                            \
  CLASS* This = (CLASS*) obj;                                         \
  IFD_EXTRA;                                                          \
  return (RETTYPE) CALL;                                              \
  IFD_END_HANDLE_EXCEPTION                                            \
  return (RETTYPE) 0;                                                 \
}							
							
							
#define IFD_WRAP_VOID(CLASS, METHOD, CALL, ARGS...)                   \
extern "C"                                                            \
void  CLASS ## _ ## METHOD (IFD_OBJP obj  ARGS)                       \
{                                                                     \
  IFD_BEGIN_HANDLE_EXCEPTION(NOARGS)                                  \
  CLASS* This = (CLASS*) obj;                                         \
  CALL;                                                               \
  return;                                                             \
  IFD_END_HANDLE_EXCEPTION                                            \
  return;                                                             \
}							
							



/*
#define IFD_C_WRAP(RETTYPE, FUNCTION, CALL, ARGS...)                  \
extern "C"                                                            \
RETTYPE FUNCTION (ARGS)                                               \
{                                                                     \
  return (RETTYPE) CALL;                                              \
}


//IFD routines
#define IFD_C_WRAP_VOID(FUNCTION, CALL, ARGS...)                      \
extern "C"                                                            \
RETTYPE FUNCTION (ARGS)                                               \
{                                                                     \
  CALL;                                                               \
  return;                                                             \
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
    return (status.error != 0   ?   1   :   0);
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


  IFD_OBJP ifd_null()
  {
    return (IFD_OBJP) 0;
  }
} //extern "C"

#endif //WANT_CODE

