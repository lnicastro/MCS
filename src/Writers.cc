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

//CFITSIO include
#if ENABLE_CFITSIO
#include <fitsio.h>

#define CHECK_FITS_ERROR if (status) throw MCS_ERROR(MSG_FITS_ERROR, fitsError(status).c_str())

mcs::FITSWriter::FITSWriter()
{ fptr = NULL; }

mcs::FITSWriter::~FITSWriter()
{ close(); }


void mcs::FITSWriter::open(string fn, bool compressed)
{
  int status = 0;
  fitsfile *fptr;

  fn = "!" + fn;
  if (compressed) fn += ".gz";

  fits_create_file(&fptr, fn.c_str(), &status);
  CHECK_FITS_ERROR;
  this->fptr = fptr;
}

void mcs::FITSWriter::close()
{
  fitsfile *fptr = (fitsfile*) this->fptr;
  int status = 0;

  if (fptr) {
    fits_close_file(fptr, &status);
    CHECK_FITS_ERROR;
  }

  fptr = NULL;
}


void mcs::FITSWriter::newBinTable(Record& meta, string HDUName)
{
  fitsfile *fptr = (fitsfile*) this->fptr;
  int status = 0;
  int i;
  string s;

  char** ttype = NULL;
  char** tform = NULL;
  char*  buf   = NULL;
  char* p1;
  char* p2;
  Record& rec = *this;

  Record::operator=(meta);
  setNull();


#define FIELD_NAME_LEN 50
#define FIELD_TYPE_LEN 10
  ttype = (char**) malloc(sizeof(char*) * count());
  tform = (char**) malloc(sizeof(char*) * count());
  buf = (char*) malloc((FIELD_NAME_LEN + FIELD_TYPE_LEN + 2) *
		       count());
  memset(buf, 0, (FIELD_NAME_LEN + FIELD_TYPE_LEN + 2) * count());

  p1 = buf;
  p2 = buf + FIELD_NAME_LEN * count();

  for (i=0; i<count(); i++) {
    ttype[i] = p1;
    p1 = stpcpy(p1, rec[i].name().c_str());
    p1++;

    if (! Types2S_FITS(rec[i].type(), rec[i].maxLength(),
		       rec[i].isUnsigned(), s)) {
      free(ttype);
      free(tform);
      free(buf);
      throw MCS_ERROR(MSG_TYPE_NOT_HANDLED, i, rec[i].type());
    }

    tform[i] = p2;
    p2 = stpcpy(p2, s.c_str());
    p2++;
  }

  fits_create_tbl(fptr, BINARY_TBL, 0, count(),
		  ttype, tform,
		  NULL, (char*) HDUName.c_str(), &status);
  CHECK_FITS_ERROR;

  free(ttype);
  free(tform);
  free(buf);
}





void mcs::FITSWriter::newImage(Types bitpix,
			       unsigned int naxis, unsigned int naxes,
			       void* buf, string name)
{
  //fitsfile *fptr = (fitsfile*) this->fptr;
  //int status = 0;
  int bitpix_fits;

  switch(bitpix) {
  case TINY:
    bitpix_fits = BYTE_IMG;
    break;
  case SMALL:
    bitpix_fits = SHORT_IMG;
    break;
  case INT:
    bitpix_fits = LONG_IMG;
    break;
  case BIGINT:
    bitpix_fits = LONGLONG_IMG;
    break;
  case FLOAT:
    bitpix_fits = FLOAT_IMG;
    break;
  case DOUBLE:
    bitpix_fits = DOUBLE_IMG;
    break;
  default:
    throw; //ERROR
  }

  //fits_create_img(fptr, bitpix_fits, &naxis, &naxes, &status);
}

#endif  //ENABLE_CFITSIO
