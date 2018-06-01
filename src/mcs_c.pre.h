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


#ifdef WANT_HEAD
/*!
  \file mcs_c.h

  \brief Declare the C wrappers to the MCS library.

  These wrappers are available as function in all interfaces.
 */

###ifndef DEF_MCS_C_H
###define DEF_MCS_C_H

###ifndef DOXYGEN_SKIP
//The following are needed to avoid redefinition's warning
###undef PACKAGE_BUGREPORT
###undef PACKAGE_NAME
###undef PACKAGE_STRING
###undef PACKAGE_TARNAME
###undef PACKAGE_VERSION

//The following is needed for ENABLE_MYSQL
###include "mcs_config.h"
###endif //DOXYGEN_SKIP

###include "mcstypes.hh"
#endif // WANT_HEAD



#ifdef WANT_CODE
###include "mcs.hh"
using namespace mcs;
#endif // WANT_CODE



#include "mcsifd.h"



#ifdef WANT_DESC
#include "mcstypes.hh"
#endif // WANT_DESC


###ifdef IFD_EXTRA
###undef IFD_EXTRA
###endif
###define IFD_EXTRA


//--------------------------------------------------------------------
//Wrapper for Data class

//!
IFD_CONSTRUCTOR(Data, (type, maxLength, (bool) isunsigned), \
                ARG(Types, type) ARG(unsigned short int, maxLength) ARG(short, isunsigned));

//!
IFD_COPY_CONSTRUCTOR(Data);

//!
IFD_DESTRUCTOR(Data);

//!Wrapper to Data::name
IFD_WRAP(char*,                  Data, name       , This->name().c_str());

//IFD_WRAP(unsigned int,           Data, flags      , This->flags()       );
IFD_WRAP(Types,                  Data, type       , This->type()        );
IFD_WRAP(unsigned short int,     Data, maxLength  , This->maxLength()   );
IFD_WRAP(unsigned short int,     Data, length     , This->length()      );
IFD_WRAP(short,                  Data, isUnsigned , This->isUnsigned()  );
IFD_WRAP(short,                  Data, isNull     , This->isNull()      );
IFD_WRAP(int,                    Data, ival       , This->ival()        );
IFD_WRAP(unsigned int,           Data, uival      , This->uival()       );
IFD_WRAP(long long int,          Data, lval       , This->lval()        );
IFD_WRAP(unsigned long long int, Data, ulval      , This->ulval()       );
IFD_WRAP(float,                  Data, fval       , This->fval()        );
IFD_WRAP(double,                 Data, dval       , This->dval()        );
IFD_WRAP(char*,                  Data, sval       , This->sval().c_str());
IFD_WRAP(unsigned int,           Data, tval       , This->tval()        );


//IFD_WRAP_VOID(                    Data, tsval      , This->tval(t), ARG(struct tm*, t));

IFD_WRAP_VOID(                   Data, setNull    , This->setNull());

IFD_WRAP_VOID(                   Data, setival    , This->setival(val) , ARG(int, val));
IFD_WRAP_VOID(                   Data, setuival   , This->setuival(val), ARG(unsigned int, val));

IFD_WRAP_VOID(                   Data, setlval    , This->setlval(val) , ARG(long long int, val));
IFD_WRAP_VOID(                   Data, setulval   , This->setulval(val), ARG(unsigned long long int, val));

IFD_WRAP_VOID(                   Data, setdval    , This->setdval(val), ARG(double, val));

IFD_WRAP_VOID(                   Data, setsval    , This->setsval(val), ARG(char*, val));

IFD_WRAP_VOID(                   Data, setblob    , This->setblob(lbuf, size), \
				  ARG(void*, lbuf) ARG(unsigned int, size));

//IFD_WRAP_VOID(                   Data, settmstrval, This->settimeval(val),
//				  ARG(struct tm*, val));

IFD_WRAP_VOID(                   Data, settimeval , This->settimeval(val), ARG(unsigned int, val));

//IFD_WRAP_VOID(                   Data, parseTime  , This->parseTime(s, ts),
//				  ARG(char*, s)  ARG(struct tm*, ts));

IFD_WRAP(char*,                  Data, print      , This->print().c_str());

IFD_WRAP_VOID(                   Data, setTag     , This->setTag(tag), ARG(short, tag));

IFD_WRAP(short,                  Data, getTag     , This->getTag());






//--------------------------------------------------------------------
//Wrapper for Record class
IFD_CONSTRUCTOR(Record, ());

IFD_DESTRUCTOR(Record);

IFD_WRAP_VOID(                 Record, clear       , This->clear());
IFD_WRAP(unsigned int,         Record, count       , This->count());

IFD_WRAP_VOID(                 Record, addField    , This->addField(* ((Data*) d)), \
	  ARG(IFD_OBJP, d));

IFD_WRAP(IFD_OBJP,             Record, pop         , new Data(This->pop(x)), ARG(int, x));

IFD_WRAP(IFD_OBJP,             Record, field       , &(This->operator[](x)), ARG(int, x));

IFD_WRAP(int,                  Record, posWhoseNameIs, This->posWhoseNameIs(name), ARG(char*, name));

IFD_WRAP_VOID(                 Record, setFieldMap , This->setFieldMap(s), ARG(char*, s));




//--------------------------------------------------------------------
//Wrapper for RecordSet class
IFD_WRAP(short,  RecordSet, setFirst , This->setFirst());
IFD_WRAP(short,  RecordSet, setLast  , This->setLast());
IFD_WRAP(short,  RecordSet, setNext  , This->setNext());
IFD_WRAP(short,  RecordSet, setPrev  , This->setPrev());
IFD_WRAP(short,  RecordSet, setWhereS, This->setWhere(field, where), \
	  ARG(unsigned int, field)  ARG(char*, where));

IFD_WRAP(short,  RecordSet, setWhereI, This->setWhere(field, where), \
	  ARG(unsigned int, field)  ARG(int, where));

IFD_WRAP(short,  RecordSet, setPos   , This->setPos(newpos), ARG(unsigned int, newpos));

IFD_WRAP_VOID(   RecordSet, dump     , This->dump(fn), ARG(char*, fn) );

IFD_WRAP(short,  RecordSet, know_nRows, This->know_nRows());
IFD_WRAP(short,  RecordSet, eof, This->eof());
IFD_WRAP(short,  RecordSet, alwaysSameStructure, This->alwaysSameStructure());

IFD_WRAP(unsigned int, RecordSet, nRows,   This->nRows());
IFD_WRAP(unsigned int, RecordSet, nFields, This->nFields());
IFD_WRAP(unsigned int, RecordSet, pos,     This->pos());
IFD_WRAP(IFD_OBJP,     RecordSet, rec,     &(This->rec()));
IFD_WRAP(IFD_OBJP,     RecordSet, metarec, &(This->metarec()));






//--------------------------------------------------------------------
//Wrapper for DBConn, Query, Table classes

###if ENABLE_MYSQL
IFD_CONSTRUCTOR(DBConn, ());

IFD_DESTRUCTOR(DBConn);

IFD_WRAP_VOID(           DBConn, connect, \
			  This->connect(user, pass, db , host), \
			  ARG(char*, user)  ARG(char*, pass)  \
			  ARG(char*, db)  ARG(char*, host));


IFD_WRAP_VOID(           DBConn, close,            This->close());
IFD_WRAP(short,          DBConn, isOpen,           This->isOpen());
IFD_WRAP(IFD_OBJP,       DBConn, newDBConn,        This->newDBConn());





IFD_CONSTRUCTOR(Query, ((DBConn*) lconn, (bool) handleNewDBConn), \
		ARG(IFD_OBJP, lconn)  ARG(short, handleNewDBConn));


IFD_DESTRUCTOR(Query);


IFD_WRAP_VOID(Query, query,            This->query(query, (bool) StoreResult), \
	       ARG(char*, query)  ARG(short, StoreResult));

IFD_WRAP_VOID(Query, prepare,          This->prepare(query), \
	       ARG(char*, query));

IFD_WRAP_VOID(Query, prepare_with_parameters, \
	       This->prepare_with_parameters(op, string(fields), table, where), \
	       ARG(int, op)  ARG(char*, fields)  \
	       ARG(char*, table)  ARG(char*, where));


IFD_WRAP(unsigned int,   Query, nAffectedRows,     This->nAffectedRows());

IFD_WRAP(IFD_OBJP,       Query, param,            &(This->param()));

IFD_WRAP(IFD_OBJP,       Query, lookup,           &(This->lookup(field, table, where)),
	  ARG(char*, field)  ARG(char*, table)  ARG(char*, where));

//IFD_WRAP(short,           Query, resultRows,       This->resultRows());

IFD_WRAP(short   ,       Query, gotRecordSet,     This->gotRecordSet());

IFD_WRAP_VOID(           Query, execute,          This->execute(StoreResult), \
			  ARG(short, StoreResult));


IFD_WRAP_VOID(           Query, close,            This->close());






###undef  IFD_EXTRA
###define IFD_EXTRA DBConn* conn = (DBConn*) lconn;
IFD_CONSTRUCTOR(Table, (conn, table, fieldkey), \
		ARG(IFD_OBJP, lconn)  ARG(char*, table)  ARG(char*, fieldkey));
###undef  IFD_EXTRA
###define IFD_EXTRA

IFD_DESTRUCTOR(Table);

IFD_WRAP_VOID(           Table, loadTable,         This->loadTable());

IFD_WRAP(IFD_OBJP,       Table, newRec,            &(This->newRec()));

IFD_WRAP_VOID(           Table, insert,            This->insert());
IFD_WRAP_VOID(           Table, update,            This->update());
###endif  //ENABLE_MYSQL




//--------------------------------------------------------------------
//Wrapper for Client class
IFD_CONSTRUCTOR(Client, (path, server, port, false, ssl, timeout), \
		ARG(char*, path)  ARG(char*, server)  ARG(int, port)  ARG(int, ssl) \
                ARG(unsigned int, timeout));

IFD_DESTRUCTOR(Client);

IFD_WRAP(short,   Client, login,         This->login(user, pass, db), \
	  ARG(char*, user)  ARG(char*, pass)  ARG(char*, db));

IFD_WRAP(short,   Client, exec,          This->exec(cmd, (Data*) data), \
	  ARG(char*, cmd)  ARG(IFD_OBJP, data));



IFD_WRAP(IFD_OBJP, Client, code,          &(This->code));
		    			
IFD_WRAP(IFD_OBJP, Client, msg,           &(This->msg));
		    			
IFD_WRAP(IFD_OBJP, Client, out,           &(This->out));
		    			
IFD_WRAP(IFD_OBJP, Client, recv,          &(This->recv));
		    			
IFD_WRAP(IFD_OBJP, Client, aux,           &(This->aux));
		    			




//--------------------------------------------------------------------
//Wrapper Conf class
IFD_CONSTRUCTOR(Conf, (filename), ARG(char*, filename));

IFD_DESTRUCTOR(Conf);

IFD_WRAP(char*,           Conf, sval,                 \
	  This->sval(section, key, false).c_str(),     \
	  ARG(char*, section)  ARG(char*, key));

IFD_WRAP(int,             Conf, ival,                 \
	  This->ival(section, key, false),             \
	  ARG(char*, section)  ARG(char*, key));



#ifdef WANT_HEAD
###endif // DEF_MCS_C_H
#endif // WANT_HEAD
