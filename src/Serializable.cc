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




//--------------------------------------------------------
void mcs::Serializable::init(int type, unsigned int lmaxChunksize)
{
  this->type = type;
  buf = NULL;
  stream = NULL;

  lsize = 0;
  from = NULL;
  filename = "";
  freeAfterUse = false;

  maxChunksize = lmaxChunksize;
  if (maxChunksize == 0)
    throw MCS_ERROR(MSG_SIZE_CHUNK, maxChunksize);

  switch(type) {
  case MCS_SERIAL_UNKNOWN:
  case MCS_SERIAL_FILENAME:
  case MCS_SERIAL_BUFFER:
    break;
  default:
    throw MCS_ERROR(MSG_INVALID_SWITCH, type);
  }

  serialize_end();
}


mcs::Serializable::Serializable(unsigned int lmaxChunksize) {
    init(MCS_SERIAL_UNKNOWN, lmaxChunksize);
}


mcs::Serializable::Serializable(void* from, unsigned int size,
				bool freeAfterUse, unsigned int lmaxChunksize)
{
    init(MCS_SERIAL_BUFFER, lmaxChunksize);
    this->lsize = size;
    this->from = (char*) from;
    this->freeAfterUse = freeAfterUse;
}


mcs::Serializable::Serializable(string filename, unsigned int lmaxChunksize)
{
  init(MCS_SERIAL_FILENAME, lmaxChunksize);
  this->filename = filename;
  File_Dir_Exist(filename, lsize);
}


mcs::Serializable::Serializable(int type, unsigned int lmaxChunksize)
{
  init(type, lmaxChunksize);
}


mcs::Serializable::~Serializable() {
  serialize_end();
}


char* mcs::Serializable::nextChunk_unknown(char* userdata, char* buf,
					   unsigned int& chunksize, bool firstTime)
{
  throw MCS_ERROR(MSG_METHOD_MUST_BE_OVERLOADED, "Serializable::nextChunk_unknown");
}

char* mcs::Serializable::serialize_unknown()
{
  return NULL;
}

bool mcs::Serializable::serialize_buffer(char*& from, unsigned int& size)
{
  from = this->from;
  size = this->lsize;
  return freeAfterUse;
}

string mcs::Serializable::serialize_filename()
{
  return filename;
}



void mcs::Serializable::serialize()
{
  //if (buf)  //Maybe from getEntireBuffer
  //  free(buf);
  //buf = NULL;

  switch (type) {
  case MCS_SERIAL_UNKNOWN:
    userdata = serialize_unknown();
    buf = (char*) malloc(maxChunksize);
    break;
  case MCS_SERIAL_BUFFER:
    freeAfterUse = serialize_buffer(from, lsize);
    buf = from;
    break;
  case MCS_SERIAL_FILENAME:
    filename = serialize_filename();
    if (! File_Dir_Exist(filename, lsize))
      throw MCS_ERROR(MSG_CANT_OPEN_FILE, filename.csz);

    stream = new ifstream(filename.c_str(), ios::binary);
    buf = (char*) malloc(maxChunksize);
    break;
  }
}


void mcs::Serializable::serialize_end()
{
// LN
  if (type != MCS_SERIAL_BUFFER) {
    if (buf)
      free(buf);
  }
  buf = NULL;

  if (stream)
    delete stream;
  stream = NULL;

  if (type == MCS_SERIAL_BUFFER)
    if (freeAfterUse)
      if (from) {
	free(from);
	from = NULL;
      }

  firstTime = true;
}


void* mcs::Serializable::nextChunk(unsigned int& chunksize)
{
  unsigned int size_to_send; //To be used only if type == MCS_SERIAL_BUFFER

  if (firstTime)
    serialize();

  switch (type) {
  case MCS_SERIAL_BUFFER:
    size_to_send = lsize - (buf - from);
    chunksize = ( size_to_send < maxChunksize   ?   size_to_send   :   maxChunksize );

    if (! firstTime) {
	buf += chunksize;
	size_to_send = lsize - (buf - from);
	chunksize = ( size_to_send < maxChunksize   ?   size_to_send   :   maxChunksize );
    }

    if (size_to_send == 0) {
      buf = NULL; //Here buf is just a cursor, it must not be freed
      serialize_end();
    }
    break;

  case MCS_SERIAL_FILENAME:
    if (stream->eof())
      serialize_end();
    else {
      stream->read(buf, maxChunksize);
      chunksize = stream->gcount();
    }
    break;
	
  case MCS_SERIAL_UNKNOWN:
    chunksize = maxChunksize;
    if (! (userdata = nextChunk_unknown(userdata, buf, chunksize, firstTime)))
      serialize_end();
    break;
  }

  if (buf) //serialize_end() hasn't been called
    firstTime = false;

  return buf;
}


void* mcs::Serializable::getEntireBuffer(unsigned int& size)
{
  unsigned int chunk;
  void* tmp;
  Buffer abuf(DONT_FREE); //*

  while(( tmp = nextChunk(chunk) ))
    abuf(chunk) << tmp;

  size = abuf.size();
  return abuf[0];
  //buf = abuf.buffer();  //* = will be freed in the next call to serialize
  //return buf;
}


bool mcs::Serializable::knowSize()
{
    return (lsize != 0);
}

unsigned int mcs::Serializable::nChunk()
{
  unsigned int nchunk = (unsigned int) (floor((((float) lsize)/maxChunksize)));
  nchunk++;

  return nchunk;
}

unsigned int mcs::Serializable::size()
{
    return lsize;
}


unsigned int mcs::Serializable::maxChunkSize()
{
    return maxChunksize;
}


