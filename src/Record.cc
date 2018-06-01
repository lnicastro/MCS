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



//--------------------------------------------------------------------
mcs::Buffer::Buffer(enum BufferFreeOnDestroy freeBuffer)
{
  buf = NULL;
  extbuffer = false;
  free();

  this->freebuffer = freeBuffer;
}


mcs::Buffer::Buffer(void* extbuf, unsigned int size,
		    enum BufferFreeOnDestroy freeBuffer)
{
  buf = NULL;
  extbuffer = false;
  free();

  buf = (char*) extbuf;
  bufsize = size;
  this->freebuffer = freeBuffer;
  extbuffer = true;
}


void mcs::Buffer::set(void* extbuf, unsigned int size,
		      enum BufferFreeOnDestroy freeBuffer)
{
  free();

  buf = (char*) extbuf;
  bufsize = size;
  this->freebuffer = freeBuffer;
  extbuffer = true;
}


mcs::Buffer::~Buffer()
{ free(); }


void mcs::Buffer::free()
{
  if ((freebuffer == AUTO_FREE)  &&  buf)
    std::free(buf);

  bufsize = 0;
  select = false;
  wstart = 0;
  wlen = 0;
}



void mcs::Buffer::resize(unsigned int size)
{
  if (extbuffer)
    if (bufsize < size)
      throw MCS_FATAL(MSG_NOT_ENOUGH_SPACE, size, bufsize);

  select = false;
  if (bufsize == 0) {
    buf = (char*) malloc(size);
    bufsize = size;
  }
  else if (bufsize < size) {
    buf = (char*) realloc(buf, size);
    bufsize = size;
  }
}




Buffer& mcs::Buffer::operator()(unsigned int len)
{
  wstart = -1;
  wlen = len;
  select = true;
  return *this;
}

Buffer& mcs::Buffer::operator()(unsigned int start, unsigned int len)
{
  wstart = start;
  wlen = len;
  select = true;
  return *this;
}


Buffer& mcs::Buffer::operator<<(const void* extbuf)
{
  if (select) {
    if (wstart == (unsigned int) -1) {  //Append to the end of the buffer
      unsigned int tt = bufsize;
      resize(bufsize + wlen);
      char* p = buf + tt;
      memcpy(p, extbuf, wlen);
    }
    else { //Write in the selected window
      resize(wstart + wlen);
      memcpy(buf + wstart, extbuf, wlen);
    }
    select = false;
  }
  else
    throw MCS_ERROR( MSG_NO_WINDOWS_SELECTED );

  return *this;
}


Buffer& mcs::Buffer::operator<<(istream& stream)
{
  if (select) {
    if (wstart == (unsigned int) -1)
      wstart = 0;

    stream.read(buf + wstart, wlen);
    select = false;
  }
  else {
      char tmp[2048];
      while (! stream.eof()) {
	  stream.read(tmp, 2048);
	  int n = stream.gcount(); //Bytes read
	  (*this)(n) << tmp;       //Fill the buffer
      }
  }

  return *this;
}





Buffer& mcs::Buffer::operator>>(void* extbuf)
{
  if (select) {
    if (wstart == (unsigned int) -1)
      wstart = 0;

    memcpy(extbuf, buf + wstart, wlen);
    select = false;
  }
  else {
    memcpy(extbuf, buf, bufsize);
  }

  return *this;
}


Buffer& mcs::Buffer::operator>>(ostream& stream)
{
  if (select) {
    if (wstart == (unsigned int) -1)
      wstart = 0;

    stream.write(buf + wstart, wlen);
    select = false;
  }
  else {
    stream.write(buf, bufsize);
  }

  return *this;
}


char* mcs::Buffer::operator[](unsigned int pos)
{
  if (pos > bufsize-1)
    throw MCS_ERROR( MSG_INVALID_POSITION, pos );

  return buf + pos;
}


unsigned int mcs::Buffer::size()
{
  select = false;
  return bufsize;
}













//--------------------------------------------------------------------
mcs::Record::Record(bool synchro) :
  Serializable(MCS_SERIAL_BUFFER), Synchro(), array(false), lmap(false)
{
  synchronize(synchro);
}


mcs::Record::~Record()
{}


mcs::Record::Record(void* llbuf, bool synchro) :
  Serializable(MCS_SERIAL_BUFFER), Synchro(), array(false), lmap(false)
{
  int i;
  char* lbuf = (char*) llbuf;
  unsigned short int count;

  synchronize(synchro);

  memcpy(&count, lbuf, sizeof(unsigned short int));
  lbuf += sizeof(unsigned short int);

  for (i=0; i<count; i++) {
    Data d(lbuf);
    addField(d);
    lbuf += d.objSize();
  }
}



mcs::Record::Record(Record& from) :
  Serializable(MCS_SERIAL_BUFFER), Synchro(), array(false), lmap(false)
{
  for (int i=0; i<from.count(); i++)
    addField(from[i]);

  setFieldMap(from);
}


Record& mcs::Record::operator=(Record& from)
{
  clear();
  for (int i=0; i<from.count(); i++)
    addField(from[i]);

  setFieldMap(from);
  return *this;
}



unsigned int mcs::Record::objSize()
{
    unsigned int size = 0;
    int i;

    size += sizeof(unsigned short int); //array.count()

    for (i=0; i<count(); i++) {
      size += array[i].objSize();  //Data objects
    }

    return size;
}





bool mcs::Record::serialize_buffer(char*& buf, unsigned int& size)
{
  unsigned short int lcount = count(), i;
  unsigned int chunk;
  Buffer abuf(DONT_FREE); //*

  abuf(sizeof(lcount)) << &lcount;
  for (i=0; i< lcount; i++) {
    void* tmp = array[i].getEntireBuffer(chunk);
    abuf(chunk) << tmp;
    free(tmp);
  }

  size = abuf.size();
  buf = abuf[0];
  return true; //* = will be freed when not needed anymore
}




//unsigned int mcs::Record::prepareBuffer(void** lbuf, unsigned int bufsize)
//{
//  unsigned int size = objSize();
//  int i;
//
//  MCS_CRITICAL_SECTION_BEGIN;
//
//  if (*lbuf == NULL) //Allocate the buffer
//    *lbuf = malloc(size);
//  else
//    if (bufsize < size)
//      throw MCS_ERROR(MSG_NOT_ENOUGH_SPACE, bufsize, size);
//
//  void* p = *lbuf;
//  unsigned short int lcount = count();
//  p = my_mempcpy(p, &lcount, sizeof(unsigned short int));
//
//  unsigned int left = size - (((char*) p) - ((char*) *lbuf));
//  for (i=0; i<lcount; i++) {
//    unsigned int s = array[i].prepareBuffer(&p, left);
//    (char*) p += s;
//    left -= s;
//  }
//
//  MCS_CRITICAL_SECTION_END;
//  return size;
//}



void mcs::Record::clear()
{
  MCS_CRITICAL_SECTION_BEGIN;
  array.clear();
  lmap.clear();
  MCS_CRITICAL_SECTION_END;
}


int mcs::Record::count()
{
  return array.count();
}

void mcs::Record::addField(Data* d)
{
  MCS_CRITICAL_SECTION_BEGIN;
  int i = array.count();  lmap.push(i);
  array.push(d);
  MCS_CRITICAL_SECTION_END;
}

void mcs::Record::addField(Data& d)
{
  MCS_CRITICAL_SECTION_BEGIN;
  int i = array.count();  lmap.push(i);
  array.push(d);
  MCS_CRITICAL_SECTION_END;
}

void mcs::Record::addField(string v, char tag)
{
  Data d(v, tag);
  addField(d);
}

void mcs::Record::addField(int v, char tag)
{
  Data d(v, tag);
  addField(d);
}

void mcs::Record::addField(long long int v, char tag)
{
  Data d(v, tag);
  addField(d);
}

void mcs::Record::addField(double v, char tag)
{
  Data d(v, tag);
  addField(d);
}


Data mcs::Record::pop(int x)
{
  MCS_CRITICAL_SECTION_BEGIN;

  for (int i=x; i<lmap.count(); i++)
    lmap[i] = lmap[i] - 1;
  lmap.pop(x);
  Data d = array.pop(x);

  MCS_CRITICAL_SECTION_END_RETURN(d);
}



Data mcs::Record::field(string name)
{
  MCS_CRITICAL_SECTION_BEGIN;

  Data d = array.peek( posWhoseNameIs(name) );

  MCS_CRITICAL_SECTION_END_RETURN(d);
}


Data mcs::Record::field(int pos)
{
  MCS_CRITICAL_SECTION_BEGIN;

  Data d = array.peek( lmap[ pos ] );

  MCS_CRITICAL_SECTION_END_RETURN(d);
}


Data& mcs::Record::operator[](string name)
{
  int pos = posWhoseNameIs(name);
  return array[ pos ];
}


Data& mcs::Record::operator[](int pos)
{
  pos = lmap[ pos ];
  return array[ pos ];
}






int mcs::Record::posWhoseNameIs(string name, enum ThrowExceptions throwexc)
{
  int i;
  bool found = false;

  for (i=0; i<count(); i++)
    if (array[i].name() == name) {
      found = true;
      break;
    }

  if (found)
    return i;
  else
    if (throwexc)
	throw MCS_ERROR(MSG_MISSING_FIELD, name.csz);
    else
      return -1;
}




void mcs::Record::setFieldMap(string s)
{
  unsigned int ui;
  s = trim(s);
  smap = s;

  if (s.empty()) {
    for (ui=0; ui< (unsigned int) array.count(); ui++)
      lmap[ui] = ui;
  }
  else {
    s += " ";
    vector<string> vmap = split(s);

    for (ui=0; ui<vmap.size(); ui++)
      lmap[ui] = posWhoseNameIs(vmap[ui]);
  }
}


void mcs::Record::setFieldMap(Record& rec)
{
  int i;
  for (i=0; i<lmap.count(); i++)
    lmap[i] = rec.lmap[i];
}


void mcs::Record::emptyName()
{
  for (int i=0; i<count(); i++)
    operator[](i).emptyName();
}



void mcs::Record::setNull()
{
  for (int i=0; i<count(); i++)
    operator[](i).setNull();
}



string mcs::Record::asString(string sep)
{
  string ret;

  for (int i=0; i<count(); i++) {
    ret += field(i).sval();

    if (i < count()-1)
      ret += sep;
  }

  return ret;
}

string mcs::Record::asStringNames(string sep)
{
  string ret;

  for (int i=0; i<count(); i++) {
    ret += field(i).name();

    if (i < count()-1)
      ret += sep;
  }

  return ret;
}


string mcs::Record::asStringTypes(string sep)
{
  string ret;

  for (int i=0; i<count(); i++) {
    ret += Types2Str(field(i).type(), field(i).isUnsigned());

    if (i < count()-1)
      ret += sep;
  }

  return ret;
}










//--------------------------------------------------------
mcs::RecordSet::RecordSet() :
  lmetarec(false), rs(false)
{
  clear();
  init(MCS_RS_INSERT);
}


mcs::RecordSet::~RecordSet() {
  clear();
}


void mcs::RecordSet::setFieldMap(string s)
{
  if (lusemetarec)
    lmetarec.setFieldMap(s);
  else
    throw; //ERROR
}




Record* mcs::RecordSet::newRecord()
{
  Record* rec = new Record(false);

  if (lusemetarec) {
    for (int i=0; i<lmetarec.count(); i++) {
      rec->addField( lmetarec[i] );
      (*rec)[i].setNull();
      (*rec)[i].emptyName();
    }
  }

  return rec;
}


void mcs::RecordSet::clear()
{
  lid = 0;
  lknow_nrows = false;
  laccum = false;
  lrandom = false;
  lnrows = 0;

  //Because the meta record will be filled before calling init()
  lusemetarec = true;

  leof = true;
  lpos = 0;
  current = 0;

  lmetarec.clear();
  rs.clear();
}



void mcs::RecordSet::init(unsigned char code, unsigned int nrows,
			  Record* meta, short int id)
{
  clear();

  lid = id;

  this->code = code;
  lknow_nrows      = code & MCS_RS_KNOW_NROWS;
  laccum           = code & MCS_RS_ACCUM;
  lrandom          = code & MCS_RS_RANDOM;
  lusemetarec      = code & MCS_RS_USEMETAREC;
  linsert          = code & MCS_RS_INSERT;
  lfetch = ! linsert;

  lnrows = nrows;

  if (meta)
    lmetarec = *meta;

  if (laccum) {
    lrandom = true;
    lknow_nrows = true;
  }

  if (linsert) {
    laccum = true;  //We always have all records
    lrandom = true;
    lknow_nrows = true;
    lnrows = 0;
  }
}



void mcs::RecordSet::insert(Record* rec) {
  if (lfetch)
    throw MCS_ERROR( MSG_INSERT_NOT_ALLOWED );

  rs.push(rec);
  lnrows++;
  leof = false;
}


void mcs::RecordSet::insert(Record& rec) {
  if (lfetch)
    throw MCS_ERROR( MSG_INSERT_NOT_ALLOWED );

  rs.push(rec);
  lnrows++;
  leof = false;
}


void mcs::RecordSet::startFetch() {
  if ((lfetch)   &&   (rs.count() == 0)) { //First time
    rs.push(newRecord()); //At least one record is needed

    if (laccum) {
      lnrows = 0;
      leof = false;  //See note in the startFetch() method
      while (fetch(lnrows, false)) {
	rs.push(newRecord());
	lnrows++;
	leof = false;
      }

      //laccum true implies lrandom=true and lknow_nrows=true
      //lrandom = true;
      //lknow_nrows = true;
      setFirst();
    }
    else {
      leof = false; //This is because inside fetch() a derived class
		    //must use the rec() method to access current
		    //record. If leof is true the rec() method would
		    //throw an exception. Anyway leof is not
		    //accessible in derived classes.
      leof = ! fetch(0, false);
    }
  }
}


bool mcs::RecordSet::internal_fetch(unsigned int newpos) {
  if (lpos != newpos) {
    if (lusemetarec)
      rs[current].emptyName();

    if (laccum) {
      current = lpos = newpos;
      leof = (lpos == lnrows); //(lpos == (lnrows-1));
    }
    else {
      leof = false;  //See note in the startFetch() method
      leof = ! fetch(newpos, ( newpos == (lpos+1) ? false : true) );
      lpos = newpos;
    }
  }

  return ! leof;
}


bool mcs::RecordSet::fetch(unsigned int newpos, bool random) {
  if (lfetch)
    throw MCS_ERROR(MSG_METHOD_MUST_BE_OVERLOADED, "RecordSet::fetch");

  return false;
}




bool mcs::RecordSet::know_nRows() { return lknow_nrows; }

unsigned int mcs::RecordSet::nRows() {
  if (! lknow_nrows)
    throw MCS_ERROR( MSG_DONT_KNOW_NROWS );

  return lnrows;
}




int mcs::RecordSet::nFields() {
  if (rs.count() > 0)
    return rs[current].count();
  else if (lusemetarec)
    return lmetarec.count();
  else
    return 0;
}


unsigned int mcs::RecordSet::pos() { return lpos; }

bool mcs::RecordSet::eof() { return leof; }

bool mcs::RecordSet::alwaysSameStructure() { return lusemetarec; }


Record& mcs::RecordSet::metarec()
{
  if (! lusemetarec)
    throw MCS_ERROR( MSG_NO_METAREC );

  return lmetarec;
}


Record& RecordSet::rec() {
  if (leof)
    throw MCS_ERROR( MSG_ALREADY_AT_END_OF_FILE );

  if (lusemetarec) {
    for (int i=0; i<lmetarec.count(); i++)
      rs[current][i].setName( lmetarec[i].name() );

    rs[current].setFieldMap( lmetarec);
  }

  return rs[current];
}





bool mcs::RecordSet::setNext() {
  if (leof)
    throw MCS_ERROR( MSG_ALREADY_AT_END_OF_FILE );

  return internal_fetch( lpos + 1 );
}


bool mcs::RecordSet::setPrev()
{
  if (! lrandom)
    throw MCS_ERROR( MSG_NOT_A_RANDOM_ACCESS_RECORDSET, "setPrev()" );

  if (lpos == 0)
    throw MCS_ERROR( MSG_ALREADY_AT_BEGIN_OF_FILE );

  return internal_fetch( lpos - 1 );
}


bool mcs::RecordSet::setFirst()
{
  if (! lrandom)
    throw MCS_ERROR( MSG_NOT_A_RANDOM_ACCESS_RECORDSET, "setFirst()" );

  return internal_fetch( 0 );
}



bool mcs::RecordSet::setLast() {
  if (! lrandom)
    throw MCS_ERROR( MSG_NOT_A_RANDOM_ACCESS_RECORDSET, "setLast()" );

  if (! lknow_nrows)
    throw MCS_ERROR( MSG_DONT_KNOW_NROWS );

  return internal_fetch( lnrows - 1 );
}


bool mcs::RecordSet::setPos(unsigned int newpos)
{
  if (! lrandom)
    throw MCS_ERROR( MSG_NOT_A_RANDOM_ACCESS_RECORDSET, "setPos()" );

  if (! lknow_nrows)
    throw MCS_ERROR( MSG_DONT_KNOW_NROWS );

  if (newpos >= lnrows)
    throw MCS_ERROR( MSG_INDEX_OUT_RANGE, newpos, lnrows );

  return internal_fetch( newpos );
}


bool mcs::RecordSet::setWhere(int field, string equalTo)
{
  if (field < 0)
    throw MCS_ERROR(MSG_INVALID_POSITION, field);

  if (field >= nFields())
    throw MCS_ERROR(MSG_INDEX_OUT_RANGE, field, nFields());

  if (! alwaysSameStructure())
    throw MCS_ERROR( MSG_NO_SAME_META_INFO );

  bool found = false;

  if (lrandom)
    setFirst();

  do {
    if (rec()[field].sval() == equalTo) {
      found = true;
      break;
    }
  }
  while (setNext());

  return found;
}



bool mcs::RecordSet::setWhere(int field, int equalTo)
{
  if (field < 0)
    throw MCS_ERROR(MSG_INVALID_POSITION, field);

  if (field >= nFields())
    throw MCS_ERROR(MSG_INDEX_OUT_RANGE, field, nFields());

  if (! alwaysSameStructure())
    throw MCS_ERROR( MSG_NO_SAME_META_INFO );

  bool found = false;

  if (lrandom)
    setFirst();

  do {
    if (rec()[field].ival() == equalTo) {
      found = true;
      break;
    }
  }
  while (setNext());

  return found;
}




void mcs::RecordSet::dump(string fn) {
  hk_dump(fn);
}

void mcs::RecordSet::hk_dump(string fn) {}


Record* mcs::RecordSet::prepRecToSend()
{
  Record* ret = new Record(false);
  long int nrows = (long int) lnrows;

  Data d(nrows);
  d.setTag(code);

  ret->addField(d);

  if (lusemetarec)
    for (int i=0; i<lmetarec.count(); i++)
      ret->addField(lmetarec[i]);

  return ret;
}


