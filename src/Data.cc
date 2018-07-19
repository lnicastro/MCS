// ----------------------------------------------------------------------^
// Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Giorgio Calderone
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

//CFITSIO include
#if ENABLE_CFITSIO
#include <fitsio.h>
#endif  //ENABLE_CFITSIO




void copyTimeStruct(MYSQL_TIME mysql, struct tm* tm) {
  tm->tm_sec   = mysql.second;
  tm->tm_min   = mysql.minute;
  tm->tm_hour  = mysql.hour;
  tm->tm_mday  = mysql.day;
  tm->tm_mon   = mysql.month - 1;
  tm->tm_year  = mysql.year - 1900;

}

void copyTimeStruct(struct tm tm, MYSQL_TIME* mysql) {
  mysql->second = tm.tm_sec ;
  mysql->minute = tm.tm_min ;
  mysql->hour   = tm.tm_hour;
  mysql->day    = tm.tm_mday;
  mysql->month  = tm.tm_mon + 1;
  mysql->year   = tm.tm_year + 1900;
}








const char* mcs::DateTime::parseTime(const char* s, struct tm* tm)
{
  int i;
  const char* ret = NULL;

  static const char* templ[] = {
    "%x%n%X",
    "%x%n%T",
    "%x%n%r",
    
    "%D%n%X",
    "%D%n%T",
    "%D%n%r",
    
    "%F%n%X",
    "%F%n%T",
    "%F%n%r",
    
    "%x",    //The date using the locale's date format.
    "%D",    //Equivalent to `%m/%d/%y'
    "%F",    //Equivalent to `%Y-%m-%d'
    
    "%X",    //The time using the locale's time format.
    "%T",    //Equivalent to the use of `%H:%M:%S' in this place.
    "%r",    //Complete time using the AM/PM format of the current locale.
    
    "%s",    //number of seconds elapsed since epoch (1970-01-01 00:00:00 UTC).
  };

  static const int ntempl = sizeof(templ) / sizeof(char*);



  memset(tm, '\0', sizeof (*tm));

  for (i=0; i<ntempl; i++) {
    ret = strptime(s, templ[i], tm);

    if (ret != NULL) {
      ret = templ[i];

      if (i < 15) {
	if (i > 11) {  //Only time
	  tm->tm_mday  = 1;
	  tm->tm_mon   = 0;
	  tm->tm_year  = 70;
	  tm->tm_isdst = 0;
	}
	else if (i > 8) {  //Only date
	  tm->tm_sec  = 0;
	  tm->tm_min  = 0;
	  tm->tm_hour = 0;
	}
      }
      break;
    }
  }

  return ret;
}




void mcs::DateTime::to_MYSQL_TIME()
{
  if (mysql) {
    struct tm tm;
    switch(timemode) {
    case UTC:    gmtime_r(&time, &tm); break;
    case LOCAL:  localtime_r(&time, &tm);
    }

    copyTimeStruct(tm, mysql);
  }
}




time_t mcs::DateTime::getTime() const
{
  time_t ret = time;

  if (mysql) {
    struct tm tm;
    memset(&tm, '\0', sizeof(tm));
    copyTimeStruct(*mysql, &tm);

    switch(timemode) {
    case UTC:   ret = timegm(&tm);      break;
    case LOCAL: ret = my_timelocal(&tm);
    }
  }

  return ret;
}




mcs::DateTime::DateTime()
{
  timemode = UTC;
  mysql = NULL;
}


void mcs::DateTime::setMysqlBuffer(MYSQL_TIME* mysql)
{ this->mysql = mysql; }


void mcs::DateTime::setTimeMode(enum TimeMode tm)
{
    timemode = tm;
    to_MYSQL_TIME();
}


void mcs::DateTime::now()
{
  time = ::time(NULL);
  to_MYSQL_TIME();
}


void mcs::DateTime::settval(time_t t)
{
  time = t;
  to_MYSQL_TIME();
}




time_t mcs::my_timelocal(struct tm* tm) {
  bool h2 = (tm->tm_hour == 2);
  time_t time = timelocal(tm);
  
  //check for DST-related issues
  if (tm->tm_isdst == 1) {
    struct tm tm_before;
    time_t t;

    t = time-3600;
    localtime_r(&t, &tm_before);

    if (h2   &&   (tm_before.tm_isdst == 0)) 
      cout << "DST Warning: given datetime does not exists!" << endl;

    if (tm_before.tm_isdst == 1)
      time -= 3600;
  }
  else {
    struct tm tm_after;
    time_t t;

    t = time+3600;
    localtime_r(&t, &tm_after);

    if (h2   &&   (tm_after.tm_isdst == 0)) 
      cout << "DST Warning: given datetime is ambiguous!" << endl;
  }

  return time;
}



void mcs::DateTime::setsval(string s)
{
  struct tm tm;
  parseTime(s.c_str(), &tm);

  switch(timemode) {
  case UTC:    time = timegm(&tm);        break;
  case LOCAL:  time = my_timelocal(&tm);  break;
  }

  to_MYSQL_TIME();
}



void mcs::DateTime::settmval(struct tm& ltm)
{
  //Note: the timelocal function does not use tm_isdst flag to compute
  //return value, thus we use our wrapper my_timelocal to
  //eventually take care of DST issues
  struct tm tm = ltm;
  switch(timemode) {
      case UTC:   time = timegm(&tm);         break;
      case LOCAL: time = my_timelocal(&tm);   break;
  }

  to_MYSQL_TIME();
}



time_t mcs::DateTime::tval() const
{
  return getTime();
}


struct tm mcs::DateTime::tmval() const
{
  struct tm tm;
  time_t time = getTime();

  switch(timemode) {
      case UTC:   gmtime_r(&time, &tm);      break;
      case LOCAL: localtime_r(&time, &tm);   break;
  }

  return tm;
}


//!Retrieve a string representation of the datetime value.
string mcs::DateTime::sval() const
{
  struct tm tm;
  char buf[30];
  time_t time = getTime();

  switch(timemode) {
      case UTC:   gmtime_r(&time, &tm);      break;
      case LOCAL: localtime_r(&time, &tm);   break;
  }

  strftime(buf, 30, "%F %T", &tm);

  return string(buf);
}

















string mcs::Types2Str(Types type, bool isunsigned)
{
  string ret="";
  if (isunsigned) ret = "UNSIGNED ";

  switch (type) {
  case TINY:
    ret += "TINY";   break;
  case SMALL:
    ret += "SMALL";  break;
  case MEDIUM:
    ret += "MEDIUM"; break;
  case INT  :
    ret += "INT";    break;
  case BIGINT:
    ret += "BIGINT"; break;
  case FLOAT:
    ret = "FLOAT";  break;
  case DOUBLE:
    ret = "DOUBLE"; break;
  case STRING:
    ret = "STRING"; break;
  case TIME:
    ret = "TIME";   break;
  case TINY_BLOB:
    ret = "TINY_BLOB"; break;
  case BLOB:
    ret = "BLOB"  ; break;
  case POINTER:
    ret = "PTR"  ; break;
  }

  return ret;
}


bool mcs::VarLenType(Types type)
{
  switch(type) {
  case STRING:
    return true;         break;
  case TINY_BLOB:
    return true;         break;
  case BLOB:
    return true;         break;
  default:
    return false;        break;
  }
}


bool mcs::IntType(Types type)
{
  if (
      (type == TINY)       ||
      (type == SMALL)      ||
      (type == MEDIUM)     ||
      (type == INT)        ||
      (type == BIGINT)
      )
    return true;
  else
    return false;
}


bool mcs::FloatType(Types type)
{
  if (
      (type == FLOAT)   ||
      (type == DOUBLE)
      )
    return true;
  else
    return false;
}



#if ENABLE_MYSQL
string mcs::MYSQL2Str(enum_field_types type)
{
  string ret;

  switch (type) {
  case MYSQL_TYPE_TINY:
    ret = "TINYINT"; 	break;
  case MYSQL_TYPE_SHORT:
    ret = "SMALLINT";	break;
  case MYSQL_TYPE_INT24:
    ret = "MEDIUMINT";	break;
  case MYSQL_TYPE_LONG:
    ret = "INT";        break;
  case MYSQL_TYPE_LONGLONG:
    ret = "BIGINT";	break;
  case MYSQL_TYPE_FLOAT:
    ret = "FLOAT";	break;
  case  MYSQL_TYPE_DOUBLE:
    ret = "DOUBLE";	break;
  case MYSQL_TYPE_STRING:
    ret = "CHAR";       break;
  case MYSQL_TYPE_VAR_STRING:
    ret = "CHAR";       break;
  case MYSQL_TYPE_TIME:
    ret = "TIME";       break;
  case MYSQL_TYPE_DATE:
    ret = "TIME";       break;
  case MYSQL_TYPE_DATETIME:
    ret = "TIME";       break;
  case MYSQL_TYPE_TIMESTAMP:
    ret = "TIME";       break;
  case MYSQL_TYPE_TINY_BLOB:
    ret = "TINYBLOB";   break;
  case MYSQL_TYPE_BLOB:
    ret = "BLOB";       break;
  default:
    ret ="";
  }

  return ret;
}


bool mcs::MYSQL2Types(enum_field_types mtype, Types& type)
{
  switch (mtype) {
  case MYSQL_TYPE_TINY:
    type = TINY; 	 break;
  case MYSQL_TYPE_SHORT:
    type = SMALL;	 break;
  case MYSQL_TYPE_INT24:
    type = MEDIUM;  	 break;	
  case MYSQL_TYPE_LONG:
    type = INT;          break;
  case MYSQL_TYPE_LONGLONG:
    type = BIGINT;	 break;
  case MYSQL_TYPE_FLOAT:
    type = FLOAT;	 break;
  case  MYSQL_TYPE_DOUBLE:
    type = DOUBLE;	 break;
  case MYSQL_TYPE_STRING:
    type = STRING;       break;
  case MYSQL_TYPE_VAR_STRING:
    type = STRING;       break;
  case MYSQL_TYPE_TIME:
    type = TIME;         break;
  case MYSQL_TYPE_DATE:
    type = TIME;         break;
  case MYSQL_TYPE_DATETIME:
    type = TIME;         break;
  case MYSQL_TYPE_TIMESTAMP:
    type = TIME;         break;
  case MYSQL_TYPE_TINY_BLOB:
    type = TINY_BLOB;    break;
  case MYSQL_TYPE_BLOB:
    type = BLOB;         break;
  default:
    return false;        break;
  }

  return true;
}


bool mcs::Types2MYSQL(Types& type, enum_field_types& mtype)
{
  switch (type) {
  case TINY:
    mtype = MYSQL_TYPE_TINY; 	  break;
  case SMALL:		
    mtype = MYSQL_TYPE_SHORT;	  break;
  case MEDIUM:		
    mtype = MYSQL_TYPE_INT24;	  break;
  case INT:		
    mtype = MYSQL_TYPE_LONG;      break;
  case BIGINT:		
    mtype = MYSQL_TYPE_LONGLONG;  break;
  case FLOAT:		
    mtype = MYSQL_TYPE_FLOAT;	  break;
  case DOUBLE:		
    mtype = MYSQL_TYPE_DOUBLE;	  break;
  case STRING:		
    mtype = MYSQL_TYPE_STRING;    break;
  case TIME:
    mtype = MYSQL_TYPE_DATETIME;  break;
  case TINY_BLOB:
    mtype = MYSQL_TYPE_TINY_BLOB; break;
  case BLOB:
    mtype = MYSQL_TYPE_BLOB;      break;
  default:
    return false;                 break;
  }

  return true;
}

string mcs::Types2MYSQLStr(Types& type, bool isunsigned)
{
  enum enum_field_types mtype;
  string ret="";

  if (Types2MYSQL(type, mtype)) {
    ret = MYSQL2Str(mtype);
  }

  if (isunsigned)
    ret += " UNSIGNED";

  return ret;
}


#if ENABLE_CFITSIO
bool mcs::FITS2Types(int fits, Types& dbt, bool& isunsigned)
{
  isunsigned = false;
  switch (fits) {
  case TLOGICAL:
    dbt = TINY;    isunsigned = true ; break;
  case TBYTE:
    dbt = TINY;    isunsigned = true ; break;
  case TSBYTE:
    dbt = TINY;                      ; break;
  case TSHORT:
    dbt = SMALL;                     ; break;
  case TUSHORT:
    dbt = SMALL;   isunsigned = true ; break;
  case TINT:
    dbt = INT;                       ; break;
  case TLONG:
    dbt = INT;                       ; break;
  case TUINT:
    dbt = INT;     isunsigned = true ; break;
  case TULONG:
    dbt = INT;     isunsigned = true ; break;
  case TLONGLONG:
    dbt = BIGINT;                    ; break;
  case TFLOAT:
    dbt = FLOAT;                       break;
  case TDOUBLE:
    dbt = DOUBLE;                      break;
  case TSTRING:
    dbt = STRING;                      break;
  default:
    return false;
  }

  return true;
}


bool mcs::Types2FITS(Types dbt, bool isunsigned, int& fits)
{
  switch (dbt) {
  case TINY:
    if (isunsigned)   fits = TBYTE;
    else              fits = TSHORT;
    break;
  case SMALL:
    if (isunsigned)   fits = TLONG;
    else              fits = TSHORT;
    break;
  case MEDIUM:
    if (isunsigned)   fits = TLONGLONG;
    else              fits = TLONG;
    break;
  case INT  :
    if (isunsigned)   fits = TLONGLONG;
    else              fits = TLONG;
    break;
  case BIGINT:
    if (isunsigned)   return false;
    else              fits = TLONGLONG;
    break;
  case FLOAT:
    fits = TFLOAT;  break;
  case DOUBLE:
    fits = TDOUBLE; break;
  case STRING:
    fits = TSTRING; break;
  case TIME: //Substitute blanks with 't'
    fits = TSTRING; break;
  default:
    return false;
  }

  return true;
}


bool mcs::Types2S_FITS(Types dbt, int len, bool isunsigned, string& fits)
{
  switch (dbt) {
//  case TINY:
//    if (isunsigned)   fits = "1B";
//    else              fits = "1S";
//    break;
//  case SMALL:
//    if (isunsigned)   fits = "1U";
//    else              fits = "1I";
//    break;
//  case INT  :
//    if (isunsigned)   fits = "1V";
//    else              fits = "1J";
//    break;
  case TINY:
    if (isunsigned)   fits = "1B";
    else              fits = "1I";
    break;
  case SMALL:
    if (isunsigned)   fits = "1J";
    else              fits = "1I";
    break;
  case MEDIUM  :
    if (isunsigned)   fits = "1K";
    else              fits = "1J";
  case INT  :
    if (isunsigned)   fits = "1K";
    else              fits = "1J";
    break;
  case BIGINT:
    if (isunsigned)   return false;
    else              fits = "1K";
    break;
  case FLOAT:
    fits = "1E";
    break;
  case DOUBLE:
    fits = "1D";
    break;
  case STRING:
    fits = itos(len) + "A";
    break;
  case TIME:
    fits = "20A";
    break; //TODO: Implement
  default:
    return false;
  }

  return true;
}
#endif
#endif




unsigned int mcs::Data::howManyDim()
{ return ldimspec & 15; }



unsigned int mcs::Data::varyingDim()
{ return ldimspec >> 4; }



string buildDimSpec(int ldimspec, const unsigned short int ldim[MCS_DATA_NDIM])
{
  string s = "";
  int i;
  int howManyDim = ldimspec & 15;
  int varyingDim = ldimspec >> 4;

  for (i=0; (i<howManyDim)   ||   (i<varyingDim); i++) {
    if (i > 0) s += "x";
    s += itos(ldim[i]);
  }

  if ((i > 0)   &&
      (i == varyingDim))
    s += "*";

  return s;
}


//NOTE: If this method is called on a variable length based Data
//object the initial "maxLength" given in init() will be overrided
void mcs::Data::resize(string dimSpec)
{
  string err = "";
  unsigned int i;
  unsigned char howManyDim, varyingDim;
  howManyDim = 0;
  varyingDim = 0;

  if (dimSpec == "")
    return;

#if ENABLE_MYSQL
  //Multi-dimensional array are a VOTable feature, they're not allowed
  //on the DB side.
  if (lbind)
    throw MCS_ERROR( MSG_CANT_BIND_AND_HAVE_DIMSPEC );
#endif
  
  //Initialize all dim-related variables
  for (i=0; i<MCS_DATA_NDIM; i++)
    ldim[i] = 1;

  ldimspec = 0;
  arrsize = 1;
  arrpos = 0;

  //Split the spec at "x"
  vector<string> values = split(dimSpec, "x");
  if (values.size() > MCS_DATA_NDIM)
    throw MCS_ERROR( MSG_TOO_MANY_DIMSPEC, MCS_DATA_NDIM);

  //Analyze all dimension specs
  for (i=0; i<values.size(); i++) {
    string s = values[i];

    //In a dimension spec there can be a '*', meaning that effective
    //size will be determined during data reading.
    if (strchr(s.c_str(), '*')) {

      if (i != values.size() - 1)
	throw MCS_ERROR( MSG_VARYING_MUST_BE_LAST );

      //So this dimension can change its size, set varyingDim...
      varyingDim = i + 1;

      //If a number is present then it will be the maximum size,
      //else the size is completely dynamic.
      //Remove the '*'
      s.replace(s.find("*", 0), 1, " ");

      ldim[i] = mcs::stoi(s, 1);
    }
    else
      ldim[i] = mcs::stoi(s);

    howManyDim++;
  }


  if (VarLenType(ltype)) {
    //If the type is a STRING then the first dimension is to be taken
    //as "maxLength" (see VOTable docs)
    if (ltype == STRING) {
      lmaxlength = ldim[0];

      //Shift all other dimensions
      for (i=0; i<MCS_DATA_NDIM-2; i++)
	ldim[i] = ldim[i+1];

      ldim[MCS_DATA_NDIM-1] = 1;
      howManyDim--;

      lmaxlength++;  //Need to allocate space for NULL character
    }
    else
      lmaxlength = 1; //Lengths are completely driven by dim spec
  }

  reallocBuffer();

  if (ltype == STRING)  //See analogous note in Data::init
    lmaxlength--;

  llength = lmaxlength;

  ldimspec = (varyingDim << 4) + howManyDim;

  mult[0] = 1;
  for (i=1; i<MCS_DATA_NDIM-1; i++)
    mult[i] = mult[i-1] * ldim[i-1];
}



Data& mcs::Data::operator()(const int i1,
			    const int i2,
			    const int i3,
			    const int i4,
			    const int i5,
			    const int i6,
			    const int i7,
			    const int i8,
			    const int i9,
			    const int i10,
			    const int i11,
			    const int i12,
			    const int i13,
			    const int i14,
			    const int i15)
{
  array(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15);
  return *this;
}


unsigned int mcs::Data::array(unsigned short int i1,
			      unsigned short int i2,
			      unsigned short int i3,
			      unsigned short int i4,
			      unsigned short int i5,
			      unsigned short int i6,
			      unsigned short int i7,
			      unsigned short int i8,
			      unsigned short int i9,
			      unsigned short int i10,
			      unsigned short int i11,
			      unsigned short int i12,
			      unsigned short int i13,
			      unsigned short int i14,
			      unsigned short int i15)
{
  if (ldim[0] <= i1)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 1, i1);

  if (ldim[1] <= i2)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 2, i2);

  if (ldim[2] <= i3)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 3, i3);

  if (ldim[3] <= i4)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 4, i4);

  if (ldim[4] <= i5)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 5, i5);

  if (ldim[5] <= i6)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 6, i6);

  if (ldim[6] <= i7)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 7, i7);

  if (ldim[7] <= i8)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 8, i8);

  if (ldim[8] <= i9)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 9, i9);

  if (ldim[9] <= i10)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 10, i10);

  if (ldim[10] <= i11)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 11, i11);

  if (ldim[11] <= i12)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 12, i12);

  if (ldim[12] <= i13)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 13, i13);

  if (ldim[13] <= i14)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 14, i14);

  if (ldim[14] <= i15)
      throw MCS_ERROR(MSG_WRONG_DIM_SPEC, 15, i15);

  arrpos =
    i1  * mult[ 0] +
    i2  * mult[ 1] +
    i3  * mult[ 2] +
    i4  * mult[ 3] +
    i5  * mult[ 4] +
    i6  * mult[ 5] +
    i7  * mult[ 6] +
    i8  * mult[ 7] +
    i9  * mult[ 8] +
    i10 * mult[ 9] +
    i11 * mult[10] +
    i12 * mult[11] +
    i13 * mult[12] +
    i14 * mult[13] +
    i15 * mult[14];


  //i1 +
  //  i2  * ldim[0 ] +
  //  i3  * ldim[0 ] * ldim[1 ] +
  //  i4  * ldim[0 ] * ldim[1 ] * ldim[2 ] +
  //  i5  * ldim[0 ] * ldim[1 ] * ldim[2 ] * ldim[3 ] +
  //  i6  * ldim[0 ] * ldim[1 ] * ldim[2 ] * ldim[3 ] * ldim[4 ] +

    return arrpos;
}




unsigned short int mcs::Data::dim(int d)
{
  // 1 <= d <= MCS_DATA_NDIM
  if (d < 1)
    return 0;
  else if (d > MCS_DATA_NDIM)
    return 0;
  else
    return ldim[d-1];
}


unsigned int mcs::Data::arraySize()
{
  return arrsize;
}




void mcs::Data::resizeVaryingDim(unsigned short int newsize)
{
  if (newsize == 0)
    return;

  if (varyingDim() == 0)
    throw MCS_ERROR( MSG_NO_DIM_ALLOWED_TO_CHANGE );

  //Set new dimension
  ldim[varyingDim() - 1] = newsize;

  reallocBuffer();
}



void mcs::Data::reallocBuffer()
{
  unsigned int old_bufsize = bufsize;

  //Calculate new array size
  arrsize = 1;
  for (int i=0; i<MCS_DATA_NDIM; i++)
    arrsize *= ldim[i];

  bufsize = lmaxlength * arrsize;

  if (buf == NULL) { //First time
    buf = (char*) malloc(bufsize);
    memset(buf, 0, bufsize);
  }
  else {
    buf = (char*) realloc(buf, bufsize);

    if (bufsize > old_bufsize) {
      //Set newly allocated memory to zero
      memset(buf + old_bufsize, 0, bufsize - old_bufsize);
    }
  }
}



void mcs::Data::init(MYSQL_BIND* bind, Types type, const char* name,
		     unsigned short int maxLength, bool isunsigned,
		     unsigned int flags)
{
  MCS_DEBUG_SETUP(0, "Data");

  buf = NULL;
  bufsize = 0;
  ltype = type;
  lname = string(name);
  lisunsigned = isunsigned;
  lisnull = false;
#if ENABLE_MYSQL
  lautoincr = (bool) (flags & AUTO_INCREMENT_FLAG);
#else
  lautoincr = false;
#endif
  lflags = flags;
  tag = 0;
  setSourceID(MCS_ID_UNKNOWN);
  setDestID(MCS_ID_UNKNOWN);


  //By default this object has no dimensions and no dimension should be varying
  ldimspec = 0;
  arrsize = 1;
  arrpos = 0;
  for (int i=0; i<MCS_DATA_NDIM; i++)
    ldim[i] = 1;


  switch (ltype) {
  case TINY:
      lmaxlength  = sizeof(char);            break;
  case SMALL:
      lmaxlength  = sizeof(short int);       break;
  case MEDIUM:
      lmaxlength  = sizeof(int);             break;
  case INT  :
      lmaxlength  = sizeof(int);             break;
  case BIGINT:
       lmaxlength = sizeof(long long int);  break;
  case FLOAT:
      lmaxlength  = sizeof(float);           break;
  case DOUBLE:
      lmaxlength  = sizeof(double);          break;
  case STRING:
      lmaxlength  = sizeof(char) * maxLength;
      lmaxlength++;  //Need to allocate space for NULL character
      break;
  case TIME:
    /*
      sizeof(MYSQL_TIME) is 36 on a 32bit processor, while it is 40 on
      a 64bit. This is due to the member <unsigned long second_part>
      of the structure, which will (not yet used) contain the
      fractional part of a second.
     */
    lmaxlength    = sizeof(MYSQL_TIME);
    break;
  case TINY_BLOB:
      lmaxlength  = sizeof(char) * maxLength;break;
  case BLOB:
      lmaxlength  = sizeof(char) * maxLength;break;
  case POINTER:
      lmaxlength  = sizeof(void*);           break;
  default:
      throw MCS_ERROR(MSG_TYPE_NOT_HANDLED, 0, ltype);
  }

  reallocBuffer();

  if (ltype == STRING)
    lmaxlength--; //Consider only the buffer without the trailing NULL.
                  //That's because the NULL is needed only on C side,
                  //not on mysql or user side.

  if (ltype == TIME)
    dt.setMysqlBuffer((MYSQL_TIME*) buf);


  llength = lmaxlength;

#if ENABLE_MYSQL
  enum_field_types mytype;
  Types2MYSQL(ltype, mytype);

  lbind = bind;
  if (lbind) {
    memset(lbind, 0, sizeof(MYSQL_BIND));
    lbind->buffer = buf;

    lbind->buffer_type =
	(mytype == MYSQL_TYPE_INT24 ? MYSQL_TYPE_LONG : mytype);

    lbind->buffer_length = lmaxlength;
    lbind->is_unsigned = lisunsigned;
    lbind->is_null = &lisnull;
    lbind->length = &llength;
  }
#endif
}



mcs::Data::Data() : Serializable(MCS_SERIAL_BUFFER)
{
    init(NULL, STRING);
    setNull();
    setTag(0);
}


mcs::Data::Data(MYSQL_BIND* bind, Types type, const char* name,
		unsigned short int maxLength, bool isunsigned,
		unsigned int flags, unsigned char tag)
  : Serializable(MCS_SERIAL_BUFFER)

{
  init(bind, type, name, maxLength, isunsigned, flags);
  setTag(tag);
  MCS_DEBUG("-> " << sval());
}


mcs::Data::Data(Types type, unsigned short int maxLength, bool isunsigned,
		string dimSpec)
  : Serializable(MCS_SERIAL_BUFFER)
{
    MCS_DEBUG("-> ");
    init(NULL, type, "", maxLength, isunsigned, 0);
    resize(dimSpec);
    MCS_DEBUG("<- ");
}



mcs::Data::Data(const Data& from)
  : Serializable(MCS_SERIAL_BUFFER)
{
  MCS_DEBUG("-> ");
  //cout << endl << endl << "Copy Constructor" << endl << endl;

  init(0,
       from.ltype,
       (char*) from.lname.c_str(),
       from.lmaxlength,
       from.lisunsigned,
       from.lflags);

  resize(buildDimSpec(from.ldimspec, from.ldim));

  memcpy(buf, from.buf, lmaxlength * arrsize);
  llength = from.llength;
  setTag(from.tag);
  lisnull = from.lisnull;
  lautoincr = from.lautoincr;
  MCS_DEBUG("<-");
}


mcs::Data::Data(int v, unsigned char tag)
  : Serializable(MCS_SERIAL_BUFFER)
{
  init(NULL, INT, "", 0, false, 0);
  setival(v);
  setTag(tag);
  MCS_DEBUG("-> " << sval());
}


mcs::Data::Data(long long int v, unsigned char tag)
  : Serializable(MCS_SERIAL_BUFFER)
{
  init(NULL, BIGINT, "", 0, false, 0);
  setlval(v);
  setTag(tag);
  MCS_DEBUG("-> " << sval());
}


mcs::Data::Data(double v, unsigned char tag)
  : Serializable(MCS_SERIAL_BUFFER)
{
  init(NULL, DOUBLE, "", 0, false, 0);
  setdval(v);
  setTag(tag);
  MCS_DEBUG("-> " << sval());
}


mcs::Data::Data(string v, unsigned char tag)
  : Serializable(MCS_SERIAL_BUFFER)
{
  init(NULL, STRING, "", v.length(), false, 0);
  if (v.length())
    setsval(v);
  else
    setNull();
  setTag(tag);
  MCS_DEBUG("-> " << sval());
}

mcs::Data::Data(struct tm v, unsigned char tag)
  : Serializable(MCS_SERIAL_BUFFER)
{
  init(NULL, TIME, "", 0, false, 0);
  settimeval(v);
  setTag(tag);
  MCS_DEBUG("-> " << sval());
}

mcs::Data::Data(time_t v, unsigned char tag)
  : Serializable(MCS_SERIAL_BUFFER)
{
  init(NULL, TIME, "", 0, false, 0);
  settimeval(v);
  setTag(tag);
  MCS_DEBUG("-> " << sval());
}



mcs::Data::~Data()
{
  if (buf)
    free(buf);
}

void mcs::Data::emptyName()
{ lname = ""; }


const char* mcs::Data::ifmt = "%d";
const char* mcs::Data::lfmt = "%ld";
const char* mcs::Data::ffmt = "%f";
const char* mcs::Data::dfmt = "%lf";
const char* mcs::Data::dtfmt = "%04d-%02d-%02d %02d:%02d:%02g";
const char* mcs::Data::dafmt = "%04d-%02d-%02d";
const char* mcs::Data::tmfmt = "%02d:%02d:%02g";


string mcs::Data::name() { return lname; }
void mcs::Data::setName(string name) { lname = name; }
//unsigned int mcs::Data::flags() { return lflags; }
Types mcs::Data::type() { return (Types) ltype; }
unsigned short int mcs::Data::maxLength() { return lmaxlength; }
unsigned short int mcs::Data::length() { return (unsigned short int) llength; }
bool mcs::Data::isUnsigned() { return lisunsigned; }
bool mcs::Data::isNull() { return lisnull; }
bool mcs::Data::isAutoIncrement() { return lautoincr; }
void* mcs::Data::buffer() const {
  unsigned int pos = arrpos;
  return (buf + pos * (lmaxlength + (ltype == STRING ?   1  :  0)));
}



int mcs::Data::ival() const
{
  char* buf = (char*) buffer();
  int ret;

  switch (ltype) {
  case TINY:
    if (lisunsigned)  ret = (*((unsigned char*         ) buf));
    else              ret = (*((char*                  ) buf));
    break;
  case SMALL:							
    if (lisunsigned)  ret = (*((unsigned short int*    ) buf));
    else              ret = (*((short int*             ) buf));
    break;
  case MEDIUM  :
    if (lisunsigned)  ret = (*((unsigned int*          ) buf));
    else              ret = (*((int*                   ) buf));
    break;
  case INT  :							
    if (lisunsigned)  ret = (*((unsigned int*          ) buf));
    else              ret = (*((int*                   ) buf));
    break;
  case BIGINT:							
    if (lisunsigned)  ret = (*((unsigned long long int*) buf));
    else              ret = (*((long long int*         ) buf));
    break;
  case FLOAT:
    ret = (int) rintf(*((float*) buf));
    break;
  case DOUBLE:
    ret = (int) rint(*((double*) buf));
    break;
  case STRING:
    if (sscanf(buf, ifmt, &ret) != 1)
	throw MCS_ERROR(MSG_CONVERSION_STRING_INT);
    break;
  case TIME:
    return (int) tval();
      break;
  case TINY_BLOB:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  case BLOB:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  case POINTER:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  }


  switch (ltype) {
  case TINY:
    if (lisunsigned)  ret = (*((unsigned char*         ) buf));
    else              ret = (*((char*                  ) buf));
    break;
  case SMALL:							
    if (lisunsigned)  ret = (*((unsigned short int*    ) buf));
    else              ret = (*((short int*             ) buf));
    break;
  case MEDIUM  :
    if (lisunsigned)  ret = (*((unsigned int*          ) buf));
    else              ret = (*((int*                   ) buf));
    break;
  case INT  :							
    if (lisunsigned)  ret = (*((unsigned int*          ) buf));
    else              ret = (*((int*                   ) buf));
    break;
  case BIGINT:							
    if (lisunsigned)  ret = (*((unsigned long long int*) buf));
    else              ret = (*((long long int*         ) buf));
    break;
  case FLOAT:
    ret = (int) rintf(*((float*) buf));
    break;
  case DOUBLE:
    ret = (int) rint(*((double*) buf));
    break;
  case STRING:
    if (sscanf(buf, ifmt, &ret) != 1)
	throw MCS_ERROR(MSG_CONVERSION_STRING_INT);
    break;
  case TIME:
    return (int) tval();
      break;
  case TINY_BLOB:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  case BLOB:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  case POINTER:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  }

  return ret;
}


unsigned int mcs::Data::uival() const
{
  char* buf = (char*) buffer();
  unsigned int ret;

  switch (ltype) {
  case TINY:
    if (lisunsigned)  ret = (*((unsigned char*         ) buf));
    else              ret = (*((char*                  ) buf));
    break;
  case SMALL:							
    if (lisunsigned)  ret = (*((unsigned short int*    ) buf));
    else              ret = (*((short int*             ) buf));
    break;
  case MEDIUM  :							
    if (lisunsigned)  ret = (*((unsigned int*          ) buf));
    else              ret = (*((int*                   ) buf));
    break;
  case INT  :							
    if (lisunsigned)  ret = (*((unsigned int*          ) buf));
    else              ret = (*((int*                   ) buf));
    break;
  case BIGINT:							
    if (lisunsigned)  ret = (*((unsigned long long int*) buf));
    else              ret = (*((long long int*         ) buf));
    break;
  case FLOAT:
    ret = (unsigned int) rintf(*((float*) buf));
    break;
  case DOUBLE:
    ret = (unsigned int) rint(*((double*) buf));
    break;
  case STRING:
    if (sscanf(buf, ifmt, &ret) != 1)
      throw MCS_ERROR(MSG_CONVERSION_STRING_INT);
    break;
  case TIME:
    return (unsigned int) tval();
  case TINY_BLOB:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  case BLOB:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  case POINTER:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
      break;
  }

  return ret;
}


long long int mcs::Data::lval() const
{
  char* buf = (char*) buffer();
  long long int ret;

  switch (ltype) {
      case TINY  :
      case SMALL :
      case MEDIUM:
      case INT   :
	if (lisunsigned)   ret = uival();
	else               ret =  ival();
	break;
      case BIGINT:
	if (lisunsigned)
	  ret = (*((unsigned long long int*) buf));
	else
	  ret = (*((long long int*) buf));
	break;
      case FLOAT :  ret = (long long int) rintf(*((float*) buf)); break;
      case DOUBLE:  ret = (long long int) rint(*((double*) buf)); break;
      case TIME  :  ret = (long long int) tval();                 break;
      case STRING:
	if (sscanf(buf, lfmt, &ret) != 1)
	  throw MCS_ERROR(MSG_CONVERSION_STRING_INT);
	break;
      case TINY_BLOB:
      case BLOB:
      case POINTER:
	throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
  }
  return ret;
}


unsigned long long int mcs::Data::ulval() const
{
  char* buf = (char*) buffer();
  unsigned long long int ret;
  //Same as preceding routine

    switch (ltype) {
      case TINY  :
      case SMALL :
      case MEDIUM:
      case INT   :
	if (lisunsigned)   ret = uival();
	else               ret =  ival();
	break;
      case BIGINT:
	if (lisunsigned)
	  ret = (*((unsigned long long int*) buf));
	else
	  ret = (*((long long int*) buf));
	break;
      case FLOAT :  ret = (long long int) rintf(*((float*) buf)); break;
      case DOUBLE:  ret = (long long int) rint(*((double*) buf)); break;
      case TIME  :  ret = (long long int) tval();                 break;
      case STRING:
	if (sscanf(buf, lfmt, &ret) != 1)
	  throw MCS_ERROR(MSG_CONVERSION_STRING_INT);
	break;
      case TINY_BLOB:
      case BLOB:
      case POINTER:
	throw MCS_ERROR(MSG_CONVERSION_BLOB_INT);
  }

  return ret;
}


float mcs::Data::fval() const
{
  char* buf = (char*) buffer();
  float ret;

  switch (ltype) {
      case TINY  :
      case SMALL :
      case MEDIUM:
      case INT   :
      case BIGINT:
      case TIME  :
	if (lisunsigned) ret = ulval();
	else             ret = lval();
	break;
      case FLOAT:   ret =          (*((float*) buf)); break;
      case DOUBLE:  ret = (float) (*((double*) buf)); break;
      case STRING:
	if (sscanf(buf, ffmt, &ret) != 1)
	  throw MCS_ERROR(MSG_CONVERSION_STRING_FLOAT);
	break;
      case TINY_BLOB:
      case BLOB:
      case POINTER:
	throw MCS_ERROR(MSG_CONVERSION_BLOB_FLOAT);
  }

  return ret;
}


double mcs::Data::dval() const
{
  char* buf = (char*) buffer();
  double ret;

  switch (ltype) {
      case TINY  :
      case SMALL :
      case MEDIUM:
      case INT   :
      case BIGINT:
      case TIME  :
	if (lisunsigned) ret = ulval();
	else             ret = lval();
	break;
      case FLOAT:   ret = fval();             break;
      case DOUBLE:  ret = (*((double*) buf)); break;
      case STRING:
	if (sscanf(buf, dfmt, &ret) != 1)
	  throw MCS_ERROR(MSG_CONVERSION_STRING_FLOAT);
	break;
      case TINY_BLOB:
      case BLOB:
      case POINTER:
      throw MCS_ERROR(MSG_CONVERSION_BLOB_FLOAT);
  }

  return ret;
}





//void mcs::Data::MySQL_TIME_2_time_t(MYSQL_TIME* mtime, time_t* t)
//{
//  struct tm tm;
//  memset(&tm, '\0', sizeof(tm));
//
//  tm.tm_sec   = mtime->second;
//  tm.tm_min   = mtime->minute;
//  tm.tm_hour  = mtime->hour;
//  tm.tm_mday  = mtime->day;
//  tm.tm_mon   = mtime->month - 1;
//  tm.tm_year  = mtime->year - 1900;
//
//  switch (dbtimemode) {
//      case GMT:
//	*t = timegm(&tm);
//	break;
//      case LOCAL:
//	*t = timelocal(&tm);
//	break;
//  }
//}
//
//void mcs::Data::time_t_2_MySQL_TIME(time_t* t, MYSQL_TIME* mtime)
//{
//  struct tm tm;
//  time_t lt = *t;
//  int fl = 0;
//
//  while ( (fl == 0)                       ||
//	 ((fl == 1)  &&  (tm.tm_isdst))
//        ) {
//    switch (dbtimemode) {
//	case GMT:
//	  gmtime_r(&lt, &tm);
//	  break;
//	case LOCAL:
//	  localtime_r(&lt, &tm);
//	  break;
//    }
//    if (tm.tm_isdst) lt -= 3600;
//    fl++;
//  }
//
//  mtime->second = tm.tm_sec ;
//  mtime->minute = tm.tm_min ;
//  mtime->hour   = tm.tm_hour;
//  mtime->day    = tm.tm_mday;
//  mtime->month  = tm.tm_mon + 1;
//  mtime->year   = tm.tm_year + 1900 ;
//}






string mcs::Data::sval(bool addWhiteSpaces) const
{
  char* buf = (char*) buffer();
  char lbuf[30];
  long long int l;
  unsigned long long int ul;
  float f;
  double d;
  string ret;

  if (ltype == TINY     ||
      ltype == SMALL    ||
      ltype == MEDIUM   ||
      ltype == INT
      ) {
    l = lval();
    sprintf(lbuf, ifmt, l);
  }

  else if (ltype == BIGINT) {
    if (lisunsigned) {
      l = lval();
      sprintf(lbuf, lfmt, l);
    }
    else {
      ul = ulval();
      sprintf(lbuf, lfmt, ul);
    }
  }
  else if (ltype == FLOAT)  {
    f = fval();
    sprintf(lbuf, ffmt, f);
  }
  else if (ltype == DOUBLE) {
    d = dval();
    sprintf(lbuf, dfmt, d);
  }
  else if (ltype == TIME) {
    ret = (string) dt;
    //time_t t;
    //struct tm tm;
    //MySQL_TIME_2_time_t((MYSQL_TIME*) buf, &t);
    //localtime_r(&t, &tm);
    //strftime(lbuf, 30, "%F %T", &tm);
  }
  else if (ltype == STRING) {
    ret = string(buf); //, lmaxlength);
    if (! addWhiteSpaces)
      ret = trim(ret);
  }
  else if ((ltype == BLOB)   ||   (ltype == TINY_BLOB)) {
      ret = hexDump(buf, llength);
  }
  if (ltype != STRING   &&   ltype != TIME)
    ret = string(lbuf);

  ret = trim(ret);
  return ret;
}


int mcs::Data::cval(char* c, int maxlength) const
{
    string s = sval();
    int l = s.length();

    if (l < maxlength) {
      memcpy(c, s.c_str(), l+1);
      return l;
    }
    else
      return 0;
}



void* mcs::Data::pval() const
{
  void* p;
  memcpy(&p, buf, sizeof(void*));
  return p;
}

void mcs::Data::setpval(void* p)
{
  memcpy(buf, &p, sizeof(void*));
}




/*
  Notes on TIME data type.

  MySQL handle 4 field type: MYSQL_TYPE_TIME, MYSQL_TYPE_DATE,
  MYSQL_TYPE_DATETIME and MYSQL_TYPE_TIMESTAMP. In any case data is returned
  as a MySQL_TIME structure in local time zone. Also upon writing on DB data
  must be in local time zone.

  When converting to/from other data types we assume that:
  - strings express datetime in local time zone;
  - numbers express the number of seconds elapsed since 1970-01-01 00:00:00
    UTC.


  When a Data(TIME) object is serialized data are converted to MCS_TIME. It is
  convenient because it is only 64 bit long, but it has enough room to handle
  all dates in the year range 1900 +/- 32768, that is from 00:00:00 1 Jan
  30867BC to 23:59:59 31 Dec 34668AD, with microsecond resolution. Furthermore
  it refers to UTC, so it can be converted to local time zone when
  unserializing the object.
*/
typedef int64_t MCS_TIME;

#define MCS_TIME_MICROSEC 0xFFFFF  // 20 bit,  0 tot, microsecond resolution, not yet used.
#define MCS_TIME_SEC         0x3F  //  6 bit, 20 tot
#define MCS_TIME_MIN         0x3F  //  6 bit, 26 tot
#define MCS_TIME_HOUR        0x1F  //  5 bit, 32 tot
#define MCS_TIME_DAY         0x1F  //  5 bit, 37 tot
#define MCS_TIME_MONTH        0xF  //  4 bit, 42 tot
#define MCS_TIME_YEAR      0xFFFF  // 16 bit, 46 tot
#define MCS_TIME_FLAGS        0x3  //  2 bit, 52 tot, not yet used
                                   // 64 bit, 64 tot

MCS_TIME struct_tm2MCS_TIME(struct tm* ct)
{
  //Convert to UTC
  time_t t = timelocal(ct);
  gmtime_r(&t, ct);

  MCS_TIME mt =
    (((MCS_TIME) ct->tm_sec ) << 20) +
    (((MCS_TIME) ct->tm_min ) << 26) +
    (((MCS_TIME) ct->tm_hour) << 32) +
    (((MCS_TIME) ct->tm_mday) << 37) +
    (((MCS_TIME) ct->tm_mon ) << 42) +
    (((MCS_TIME) ct->tm_year) << 46)  ;

  return mt;
}


int MCS_TIME2struct_tm(MCS_TIME mt, struct tm* ct)
{
  ct->tm_sec   = (int)  ((mt >> 20)  &  MCS_TIME_SEC  );
  ct->tm_min   = (int)  ((mt >> 26)  &  MCS_TIME_MIN  );
  ct->tm_hour  = (int)  ((mt >> 32)  &  MCS_TIME_HOUR );
  ct->tm_mday  = (int)  ((mt >> 37)  &  MCS_TIME_DAY  );
  ct->tm_mon   = (int)  ((mt >> 42)  &  MCS_TIME_MONTH);
  ct->tm_year  = (int)  ((mt >> 46)  &  MCS_TIME_YEAR );

  //Convert to local time zone
  time_t t = timegm(ct);
  localtime_r(&t, ct);
  return 0;
}




#define DATETIME_PARSE_NTEMPL  16

static const char* templ[DATETIME_PARSE_NTEMPL] = {
  "%x%n%X",
  "%x%n%T",
  "%x%n%r",

  "%D%n%X",
  "%D%n%T",
  "%D%n%r",

  "%F%n%X",
  "%F%n%T",
  "%F%n%r",

  "%x",                   //The date using the locale's date format.
  "%D",                   //Equivalent to `%m/%d/%y'
  "%F",                   //Equivalent to `%Y-%m-%d'

  "%X",                   //The time using the locale's time format.
  "%T",                   //Equivalent to the use of `%H:%M:%S' in this place.
  "%r",                   //The complete time using the AM/PM format of the current locale.

  "%s",                   //The number of seconds since the epoch, i.e., since 1970-01-01 00:00:00 UTC.
};


void mcs::Data::parseTime(string s, struct tm* tm)
{
  int i;
  const char* ret = NULL;

  memset(tm, '\0', sizeof(*tm));

  for (i=0; i<DATETIME_PARSE_NTEMPL; i++) {
    ret = strptime(s.c_str(), templ[i], tm);
    if (ret != NULL) break;
  }

  if (ret == NULL)
    throw MCS_ERROR(MSG_CONVERSION_DATETIME, s.c_str());

//  int ret;
//  bool fl_riporto = false;
//
//  s = trim(s); // leading and trailing blanks
//  s = subst(s, "  +", ""); // multiple blanks
//  s = subst(s, "[^0-9:\\- ]", ""); //non-expected chars
//
//  const char* p = s.c_str();
//  float sec;
//
//  ret = sscanf(p, dtfmt,
//	       &(ts->tm_year), &(ts->tm_mon), &(ts->tm_mday),
//	       &(ts->tm_hour), &(ts->tm_min), &sec);
//
//
//  if (ret == 6) {  //Date and time
//    ts->tm_year -= 1900;
//    ts->tm_mon--;
//  }
//  else {
//    ret = sscanf(p, dafmt,
//		 &(ts->tm_year), &(ts->tm_mon), &(ts->tm_mday));
//    if (ret == 3) {  //Only date
//      ts->tm_year -= 1900;
//      ts->tm_mon--;
//    }
//    else {
//      ret = sscanf(p, tmfmt, &(ts->tm_hour),
//		   &(ts->tm_min), &sec);
//      if (ret != 3)
//	throw MCS_ERROR(MSG_CONVERSION_DATETIME, s.c_str());
//    }
//  }
//
//  sec = rint(sec);
//  if (sec == 60) {
//    fl_riporto = true;
//    sec = 59;
//  }
//  ts->tm_sec = (int) sec;
//
//  if (
//    ((ts->tm_mon  < 0)   ||   (ts->tm_mon  > 11))             ||
//    ((ts->tm_mday < 1)   ||   (ts->tm_mday > 31))             ||
//    ((ts->tm_hour < 0)   ||   (ts->tm_hour > 23))             ||
//    ((ts->tm_min  < 0)   ||   (ts->tm_min  > 59))             ||
//    ((ts->tm_sec  < 0)   ||   (ts->tm_sec  > 59))
//    )
//    throw MCS_ERROR(MSG_CONVERSION_DATETIME, s.c_str());
//
//  //ts->tm_isdst = 0;
//  ts->tm_gmtoff = 0;
//  time_t t = timelocal(ts); //Normalize
//
//  if (fl_riporto) {
//    t++;  //Add one second
//    localtime_r(&t, ts);
//  }
}


void mcs::Data::tval(struct tm* ts) const
{
  if (ltype == STRING) {
    parseTime(sval(), ts);
  }
  else if (ltype == TIME) {
    *ts = dt;
    //time_t t;
    //MySQL_TIME_2_time_t((MYSQL_TIME*) buf, &t);
    //localtime_r(&t, ts);
  }
  else if ((ltype == BLOB)   ||   (ltype == TINY_BLOB))
    throw MCS_ERROR(MSG_CONVERSION_BLOB_DATETIME);
  else {
    time_t tt = (time_t) dval();
    localtime_r(&tt, ts);
  }
}


time_t mcs::Data::tval() const
{
  struct tm ts;
  tval(&ts);
  return timelocal(&ts);
}


void mcs::Data::settimenow()
{ dt.now(); }


void mcs::Data::setTimeMode(enum TimeMode tm)
{ dt.setTimeMode(tm); }



void mcs::Data::settimeval(struct tm tm)
{
  char* buf = (char*) buffer();
  time_t t;

  lisnull = false;

  //switch (dbtimemode) {
  //    case GMT:
  //	t = timegm(&tm);
  //	break;
  //    case LOCAL:
  	t = timelocal(&tm);
  //	break;
  //}

  switch (ltype) {
      case TINY:
      case SMALL:
      case MEDIUM:
      case TINY_BLOB:
      case BLOB:
      case POINTER:
	throw MCS_ERROR(MSG_CONVERSION_TIME_LINT);
	break;
      case INT:
      case BIGINT:
	setival( t );
	break;
      case FLOAT:
      case DOUBLE:
	setdval( t );
	break;
      case STRING:
	if (lmaxlength < 30)
	  throw MCS_ERROR(MSG_CONVERSION_STRING_TOO_LONG);

	strftime(buf, 30, "%F %T", &tm);
	break;
      case TIME:
	dt = tm;
	//time_t_2_MySQL_TIME(&t, (MYSQL_TIME*) buf);
  }
}


void mcs::Data::settimeval(time_t tt)
{
  struct tm ts;
  localtime_r(&tt, &ts);
  settimeval(ts);
}
















void mcs::Data::setNull(bool null)
{
  lisnull = null;
}


void mcs::Data::setival(int v)
{
  setlval(v);
}

void mcs::Data::setuival(unsigned int v)
{
  setulval(v);
}

void mcs::Data::setlval(long long int v)
{
  char* buf = (char*) buffer();
  lisnull = false;
  bool us = lisunsigned;

  switch (ltype) {
  case TINY:
    if (us) (*((unsigned char*)          buf)) = (unsigned char) v;
    else    (*((char*)                   buf)) = (char)          v;
    break;
  case SMALL:
    if (us) (*((unsigned short int*)     buf)) = (unsigned short int) v;
    else    (*((short int*)              buf)) = (short int)          v;
    break;
  case MEDIUM  :
    if (us) (*((unsigned int*)           buf)) = (unsigned int) v;
    else    (*((int*)                    buf)) = (int)          v;
    break;
  case INT  :
    if (us) (*((unsigned int*)           buf)) = (unsigned int) v;
    else    (*((int*)                    buf)) = (int)          v;
    break;
  case BIGINT:
    if (us) (*((unsigned long long int*) buf)) = (unsigned long long int) v;
    else    (*((long long int*)          buf)) = (long long int)          v;
    break;
  case FLOAT:
    (*((float*) buf)) = v; break;
  case DOUBLE:
    (*((double*) buf)) = v; break;
  case STRING:
    memset(buf, 32, lmaxlength);
    snprintf(buf, lmaxlength, lfmt, v);
    llength = strlen(buf);
    break;
  case TINY_BLOB:
      throw MCS_ERROR(MSG_CONVERSION_INT_BLOB); break;
  case BLOB:
      throw MCS_ERROR(MSG_CONVERSION_INT_BLOB); break;
  case POINTER:
      throw MCS_ERROR(MSG_CONVERSION_INT_BLOB); break;
  case TIME:
    time_t tt = (time_t) v;
    dt = tt;
    //time_t_2_MySQL_TIME(&tt, (MYSQL_TIME*) buf);
    break;
  }

  MCS_DEBUG("-> " << sval());
}





void mcs::Data::setulval(unsigned long long int v)
{
  char* buf = (char*) buffer();
  lisnull = false;
  bool us = lisunsigned;

  switch (ltype) {
  case TINY:
    if (us) (*((unsigned char*)          buf)) = (unsigned char) v;
    else    (*((char*)                   buf)) = (char)          v;
    break;
  case SMALL:
    if (us) (*((unsigned short int*)     buf)) = (unsigned short int) v;
    else    (*((short int*)              buf)) = (short int)          v;
    break;
  case MEDIUM  :
    if (us) (*((unsigned int*)           buf)) = (unsigned int) v;
    else    (*((int*)                    buf)) = (int)          v;
    break;
  case INT  :
    if (us) (*((unsigned int*)           buf)) = (unsigned int) v;
    else    (*((int*)                    buf)) = (int)          v;
    break;
  case BIGINT:
    if (us) (*((unsigned long long int*) buf)) = (unsigned long long int) v;
    else    (*((long long int*)          buf)) = (long long int)          v;
    break;
  case FLOAT:
    (*((float*) buf)) = v; break;
  case DOUBLE:
    (*((double*) buf)) = v; break;
  case STRING:
    memset(buf, 32, lmaxlength);
    snprintf(buf, lmaxlength, lfmt, v);
    llength = strlen(buf);
    break;
  case TINY_BLOB:
      throw MCS_ERROR(MSG_CONVERSION_INT_BLOB); break;
  case BLOB:
      throw MCS_ERROR(MSG_CONVERSION_INT_BLOB); break;
  case POINTER:
      throw MCS_ERROR(MSG_CONVERSION_INT_BLOB); break;
  case TIME:
    time_t tt = (time_t) v;
    dt = tt;
    //time_t_2_MySQL_TIME(&tt, (MYSQL_TIME*) buf);
    break;
  }

  MCS_DEBUG("-> " << sval());
}







void mcs::Data::setdval(double v)
{
  char* buf = (char*) buffer();
  lisnull = false;
  bool us = lisunsigned;

  switch (ltype) {
  case TINY:
    if (us) (*((unsigned char*)          buf)) = (unsigned char) nearbyint(v);
    else    (*((char*)                   buf)) = (char)          nearbyint(v);
    break;
  case SMALL:
    if (us) (*((unsigned short int*)     buf)) = (unsigned short int) nearbyint(v);
    else    (*((short int*)              buf)) = (short int)          nearbyint(v);
    break;
  case MEDIUM  :
    if (us) (*((unsigned int*)           buf)) = (unsigned int) nearbyint(v);
    else    (*((int*)                    buf)) = (int)          nearbyint(v);
    break;
  case INT  :
    if (us) (*((unsigned int*)           buf)) = (unsigned int) nearbyint(v);
    else    (*((int*)                    buf)) = (int)          nearbyint(v);
    break;
  case BIGINT:
    if (us) (*((unsigned long long int*) buf)) = (unsigned long long int) nearbyint(v);
    else    (*((long long int*)          buf)) = (long long int)          nearbyint(v);
    break;
  case FLOAT:
    (*((float*) buf)) = v; break;
  case DOUBLE:
    (*((double*) buf)) = v; break;
  case STRING:
    memset(buf, 32, lmaxlength);
    snprintf(buf, lmaxlength, dfmt, v);
    llength = strlen(buf);
    break;
  case TINY_BLOB:
      throw MCS_ERROR(MSG_CONVERSION_FLOAT_BLOB); break;
  case BLOB:
      throw MCS_ERROR(MSG_CONVERSION_FLOAT_BLOB); break;
  case POINTER:
      throw MCS_ERROR(MSG_CONVERSION_FLOAT_BLOB); break;
  case TIME:
    time_t tt = (time_t) nearbyint(v);
    dt = tt;
    //time_t_2_MySQL_TIME(&tt, (MYSQL_TIME*) buf);
    break;
  }

  MCS_DEBUG("-> " << sval());
}


void mcs::Data::setcval(const char* v)
{
    setsval(string(v));
    MCS_DEBUG("-> " << sval());
}


void mcs::Data::setsval(string v)
{
  char* buf = (char*) buffer();
  int ret=-1000;
  bool us = lisunsigned;
  lisnull = false;

  switch (ltype) {
  case TINY:
    if (us) ret=sscanf(v.c_str(), "%hhd", ((unsigned char*)          buf));
    else    ret=sscanf(v.c_str(), "%hhd", ((char*)                   buf));
    break;
  case SMALL:
    if (us) ret=sscanf(v.c_str(), "%hd" , ((unsigned short int*)     buf));
    else    ret=sscanf(v.c_str(), "%hd" , ((short int*)              buf));
    break;
  case MEDIUM  :
    if (us) ret=sscanf(v.c_str(), "%d" , ((unsigned int*)           buf));
    else    ret=sscanf(v.c_str(), "%d" , ((int*)                    buf));
    break;
  case INT  :
    if (us) ret=sscanf(v.c_str(), "%d" , ((unsigned int*)           buf));
    else    ret=sscanf(v.c_str(), "%d" , ((int*)                    buf));
    break;
  case BIGINT:
    if (us) ret=sscanf(v.c_str(), "%lld", ((unsigned long long int*) buf));
    else    ret=sscanf(v.c_str(), "%lld", ((long long int*)          buf));
    break;
  case FLOAT:
    ret=sscanf(v.c_str(), "%f" , ((float*)  buf)); break;
  case DOUBLE:
    ret=sscanf(v.c_str(), "%lf", ((double*) buf)); break;
  case TIME:
    dt = v;
    //struct tm ts;
    //time_t t;
    //parseTime(v, &ts);
    //
    //switch (dbtimemode) {
    //	case GMT:
    //	  t = timegm(&ts);
    //	  break;
    //	case LOCAL:
    //	  t = timelocal(&ts);
    //	  break;
    //}
    //time_t_2_MySQL_TIME(&t, (MYSQL_TIME*) buf);
    break;
  case STRING:
    if (v.length() > lmaxlength)
	throw MCS_ERROR(MSG_CONVERSION_STRING_TOO_LONG);
    else {
      memset(buf, 32, lmaxlength);
      llength = v.length();
      memcpy(buf, v.c_str(), llength);
    }
    break;
  case TINY_BLOB:
      throw MCS_ERROR(MSG_CONVERSION_STRING_BLOB); break;
  case BLOB:
      throw MCS_ERROR(MSG_CONVERSION_STRING_BLOB); break;
  case POINTER:
      throw MCS_ERROR(MSG_CONVERSION_STRING_BLOB); break;
  }

  if (ret!=-1000   &&   ret!=1)
      throw MCS_ERROR(MSG_CONVERSION_NAN);

  MCS_DEBUG("-> " << sval());
}



void mcs::Data::setblob(void* lbuf, unsigned int size)
{
  char* buf = (char*) buffer();

  lisnull = false;
  if ((ltype != TINY_BLOB)   &&   (ltype != BLOB))
    throw MCS_ERROR(MSG_NOT_A_BLOB);

  if (size > lmaxlength)
    throw MCS_ERROR(MSG_NOT_ENOUGH_SPACE, size, lmaxlength);

  memcpy(buf, lbuf, size);
}



long long int mcs::Data::MinValue(Types ltype, bool flunsigned)
{
  int nbit;
  switch (ltype) {
  case TINY:
    nbit =  8;   break;
  case SMALL:
    nbit = 16;   break;
  case MEDIUM:
    nbit = 24;   break;
  case INT:
    nbit = 32;   break;
  case BIGINT:
    nbit = 64;   break;
  default:
    return 0;
  }
  if (flunsigned)
    return 0;
  else
    return (long long int) (-1 * (pow(2.0, nbit) / 2));
}


long long int mcs::Data::MaxValue(Types ltype, bool flunsigned)
{
  int nbit;
  switch (ltype) {
  case TINY:
    nbit =  8;   break;
  case SMALL:
    nbit = 16;   break;
  case MEDIUM:
    nbit = 24;   break;
  case INT:
    nbit = 32;   break;
  case BIGINT:
    nbit = 64;   break;
  default:
    return 0;
  }
  if (flunsigned)
    return (long long int) (pow(2.0, nbit) - 1);
  else
    return (long long int) ((pow(2.0, nbit) / 2) - 1);
}




Data& mcs::Data::operator=(Data& d)
{
  //cout << endl << endl << "Operator assignment" << endl << endl;
  if (this == &d)
    return *this; //Self assignment

  if (d.isNull()) {
    setNull();
    return *this;
  }

  switch(d.type()) {
      case TINY:
      case SMALL:
      case MEDIUM:
	setival( d.ival() );
	break;
      case INT:
	if (d.isUnsigned())
	  setdval( d.lval() );
	else
	  setival( d.ival() );
        break;
      case BIGINT:
	if (d.isUnsigned())
	  setdval( d.ulval() );
	else
	  setdval( d.lval() );
	break;
      case FLOAT:
      case DOUBLE:
	setdval( d.dval() );
	break;
      case STRING:
	setsval( d.sval() );
	break;
      case TIME:
	settimeval( d.tval() );
	break;
      case TINY_BLOB:
      case BLOB:
	setblob( d.buffer(), d.length() );
      case POINTER:
	setpval(d.pval());
	break;
  }

  return *this;
}




unsigned int mcs::Data::objSize()
{
    //For (bits): ltype (4), lisunsigned (1), lisnul (1), autoincrement(1)
    unsigned int size = 1;

    if (VarLenType(ltype)) {
	size += sizeof(lmaxlength);
	size += sizeof(unsigned short int); //llength
    }
    size += sizeof(tag);
    size += lname.length()+1; //NULL character

    //Data buffer
    if (ltype == TIME)
      size += sizeof(MCS_TIME);
    else
      size += lmaxlength;

    return size;
}



bool mcs::Data::serialize_buffer(char*& buf, unsigned int& size)
{
  Buffer abuf(DONT_FREE); //*

  unsigned char tmp_mix = 0;

  tmp_mix += (unsigned char) ltype;

  if (lisunsigned)
    tmp_mix += 16;
  if (lisnull)
    tmp_mix += 32;
  if (lautoincr)
    tmp_mix += 64;

  if (ldimspec > 1) //Send also ldimspec
    tmp_mix += 128;

  abuf(sizeof(tmp_mix)) << &tmp_mix;

  if (ldimspec > 1) {
    abuf(sizeof(ldimspec)) << &ldimspec;

    unsigned short int dim;
    for (unsigned int i=0; (i<howManyDim())   ||   (i<varyingDim()); i++) {
      dim = ldim[i];
      abuf(sizeof(dim)) << &dim;
    }
  }

  if (VarLenType(ltype)) {
    abuf(sizeof(lmaxlength)) << &lmaxlength;

    unsigned short int tmp_llength = llength;
    abuf(sizeof(tmp_llength)) << &tmp_llength;
  }

  abuf(sizeof(tag)) << &tag;
  abuf(lname.length()+1) << lname.c_str();

  if (ltype == TIME) { /*
			 In this case we trasform data in a MCS_TIME
			 value, so it will be shorter. It also solve a
			 compatibility problem between different
			 platform when MYSQL_TIME has different size.
			*/
    //struct tm ts;
    //char* ll = this->buf;
    //for (unsigned int i=0; i<arrsize; i++) {
    //  MySQL_TIME_2_struct_tm((MYSQL_TIME*) ll, &ts);
    //  ll += lmaxlength;
    //
    //  MCS_TIME mt = struct_tm2MCS_TIME(&ts);
    //  abuf(sizeof(MCS_TIME)) << &mt;
    //}
    time_t t;
    char* ll = this->buf;
    for (unsigned int i=0; i<arrsize; i++) {
      //MySQL_TIME_2_time_t((MYSQL_TIME*) ll, &t);
      //ll += lmaxlength;

      dt.setMysqlBuffer((MYSQL_TIME*) ll);
      t = dt;
      abuf(sizeof(time_t)) << &t;
    }
  }
  else
    abuf(lmaxlength * arrsize) << this->buf;

  size = abuf.size();
  buf = abuf[0];
  return true;  //* = will be freed when not needed anymore
}



mcs::Data::Data(void* llbuf) : Serializable(MCS_SERIAL_BUFFER)
{
  if (! llbuf) {
      init(NULL, TINY);
      setNull();
      return;
  }
  char* lbuf = (char*) llbuf;

  unsigned char tmp_mix;
  memcpy(&tmp_mix, lbuf, sizeof(tmp_mix));
  lbuf += sizeof(tmp_mix);

  ltype           =  (Types) (tmp_mix  &  15);
  lisunsigned     =   (bool) (tmp_mix  &  16);
  bool lisnull    =   (bool) (tmp_mix  &  32); //This value must be set AFTER init()
  bool lautoincr  =   (bool) (tmp_mix  &  64); //This value must be set AFTER init()
  bool hasDimSpec =  ((bool) (tmp_mix  &  128));
  string dimspec = "";

  if (hasDimSpec) {
    memcpy(&ldimspec, lbuf, sizeof(ldimspec));
    lbuf += sizeof(ldimspec);

    for (unsigned int i=0; i<MCS_DATA_NDIM; i++) {
      if ((i<howManyDim())   ||   (i<varyingDim())) {
	memcpy(&(ldim[i]), lbuf, sizeof(unsigned short int));
	lbuf += sizeof(unsigned short int);
      }
      else
	ldim[i] = 1;
    }

    dimspec = buildDimSpec(ldimspec, ldim);
  }


  lmaxlength = 0;
  unsigned short int tmp_llength = 0;
  if (VarLenType(ltype)) {
      memcpy(&lmaxlength , lbuf, sizeof(lmaxlength) );
      lbuf += sizeof(lmaxlength) ;

      memcpy(&tmp_llength, lbuf, sizeof(tmp_llength));
      lbuf += sizeof(tmp_llength);
      llength = tmp_llength;
  }


  unsigned char tmp_ltag;
  memcpy(&tmp_ltag , lbuf, sizeof(tag) ); lbuf += sizeof(tmp_ltag) ;

  string tmp_lname = string(lbuf);
  lbuf += tmp_lname.length()+1;

  init(NULL,
       ltype,
       (char*) tmp_lname.c_str(),
       lmaxlength,
       lisunsigned,
       0);

  resize(dimspec);

  if (ltype == TIME) { //See note in Data::serialize_buffer
    //MCS_TIME mt;
    //char* ll = this->buf;
    //
    //for (unsigned int i=0; i<arrsize; i++) {
    //  memcpy(&mt, lbuf, sizeof(MCS_TIME));
    //  lbuf += sizeof(MCS_TIME);
    //
    //  struct tm ts;
    //  MCS_TIME2struct_tm(mt, &ts);
    //  Struct_tm_2_MySQL_TIME(&ts, (MYSQL_TIME*) ll);
    //  ll += lmaxlength;
    //}

    time_t t;
    char* ll = this->buf;

    for (unsigned int i=0; i<arrsize; i++) {
      memcpy(&t, lbuf, sizeof(time_t));
      lbuf += sizeof(size_t);

      dt = t;
      //time_t_2_MySQL_TIME(&t, (MYSQL_TIME*) ll);
      ll += lmaxlength;
    }
  }
  else
    memcpy(buf, lbuf, lmaxlength * arrsize);

  if (VarLenType(ltype))
      llength = tmp_llength;

  setTag(tmp_ltag);
  this->lisnull   = lisnull;
  this->lautoincr = lautoincr;
}


mcs::Data::Data(void* lbuf, unsigned int size, unsigned char tag)
  : Serializable(MCS_SERIAL_BUFFER)
{
    if (size < 256)
	init(NULL, TINY_BLOB, "", size, false, 0);
    else if (size < 65536)
	init(NULL, BLOB, "", size, false, 0);
    else
	throw MCS_ERROR(MSG_BLOB_TOO_BIG, size);

    setblob(lbuf, size);
    setTag(tag);
}

string mcs::Data::print()
{
    string s;
    char* p;
    unsigned int i;

    s = string( "N=") + name() +
	string(" T=") + Types2Str(type(), isUnsigned()) +
	string(" L=") + itos(maxLength()) +
	string(" l=") + itos(length()) +
	string(" n=") + itos(isNull()) +
	string(" D=") ;

    if ((type() == BLOB)   ||   (type() == TINY_BLOB)) {
	p = (char*) buffer();
	for (i=0; i<length(); i++)
	    s += itos(p[i]) + string(" ");
    }
    else
	s += sval();

    return s;
}

void mcs::Data::setTag(unsigned char ltag)
{
    tag = ltag;
}

unsigned char mcs::Data::getTag()
{
    return tag;
}


int mcs::Data::getSourceID()
{
  return id_source;
}

void mcs::Data::setSourceID(int id)
{
  id_source = id;
}

int mcs::Data::getDestID()
{
  return id_dest;
}

void mcs::Data::setDestID(int id)
{
  id_dest = id;
}
