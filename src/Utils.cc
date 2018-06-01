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


int mcs::extractCode(const char* msg)
{
  int ret;
  ret = 0;
  ret += (msg[1] - '0') * 100;
  ret += (msg[2] - '0') * 10 ;
  ret += (msg[3] - '0')      ;
  return ret;
}




bool mcs::File_Dir_Exist(string fn, unsigned int& size)
{
  DIR *dp;
  ifstream f;
  size = 0;

  if (fn.empty()) return false;
  dp = opendir(fn.csz);
  if (dp) {
    closedir(dp);
    return true;
  }
  else {
    f.open(fn.csz);
    if (f.is_open()) {
      f.seekg(0, ios::end);  // Seek the end of file (to get the size)
      size = f.tellg();      // Size of file
      f.seekg(0, ios::beg);  // Seek the start of file
      f.close();
      return true;
    }
    else
      return false;
  }
}


string mcs::Pwd()
{
  char* p;
  string s;
  p=getcwd(NULL, 0);
  s=p;
  free(p);
  return s;
}


string mcs::itos(int i)
{
  char buf[20];
  sprintf(buf, "%d", i);
  return string(buf);
}


int mcs::stoi(string s)
{
  int i;
  if (sscanf(s.c_str(), "%d", &i) == 1)
    return i;
  else
    throw MCS_ERROR(MSG_CONVERSION_STRING_INT);
}


int mcs::stoi(string s, int errval)
{
  int i;
  if (sscanf(s.c_str(), "%d", &i) == 1)
    return i;
  else
    return errval;
}


string mcs::dtos(double f)
{
  char buf[20];
  sprintf(buf, "%f", f);
  return string(buf);
}


string mcs::btos(bool b)
{
  if (b)
    return string("true");
  else
    return string("false");
}


string mcs::vtos(vector<string> vec)
{
  unsigned int i;
  string buf = "";
  string nl = "\n";

  for (i=0; i<vec.size(); i++)
    buf += vec[i] + nl;

  return buf;
}


string mcs::subst(string s, string what, string with, int op)
{
    string s2 = "(" + what + string(")");
    string templ = s2;

    if (op & MCS_SUBST_LEADING)
	templ = "^" + s2;

    if (op & MCS_SUBST_TRAILING) {
	if (op & MCS_SUBST_LEADING)
	    templ += "|" + s2;
	templ += "$";
    }
    pcrecpp::RE re(templ,
		   pcrecpp::RE_Options().set_dollar_endonly(true));

    if (op & MCS_SUBST_QUOTE_WITH) {
	pcrecpp::RE re("\\\\");
	re.GlobalReplace("\\\\\\\\", &with);
    }
    re.GlobalReplace(with, &s);
    return s;
}


string mcs::trim(string s)
{
    return subst(s, " +", "", MCS_SUBST_LEADING | MCS_SUBST_TRAILING);
}


string mcs::chomp(string s)
{
    return subst(s, "\n+", "", MCS_SUBST_TRAILING);
}


string mcs::remTabs(string s)
{
    return subst(s, "\t", "");
}


string mcs::remLeading(string& s, const char* p)
{
    s = subst(s, p, "", MCS_SUBST_LEADING);
    return s;
}


string mcs::remTrailing(string& s, const char* p)
{
    s = subst(s, p, "", MCS_SUBST_TRAILING);
    return s;
}


std::vector<std::string> mcs::split(string s, string sep)
{
    vector<string> v;

    //First characters must not be equal top sep
    pcrecpp::RE pre("^\\Q" + sep + "\\E+");
    pre.GlobalReplace("", &s);

    pcrecpp::StringPiece input(s);
    pcrecpp::RE re("([^\\Q" + sep + "\\E]+)\\Q" + sep + "\\E*");
    //pcrecpp::RE re("(\\S+) *");

    while (re.Consume(&input, &s))
	v.push_back(s);

    return v;
}


string mcs::hexDump(const void* pvoid, unsigned int size)
{
  const char* buf = (const char*) pvoid;
  string s = "";
  unsigned int i, j, count;
  char lbuf[40];

  i = 0;
  while (i < size) {
    count = size - i;
    if (count > 8)
      count = 8;

    sprintf(lbuf, "%06d ", i);
    s += lbuf;

    for (j=0; j<count; j++) {
      sprintf(lbuf, "%02X", buf[i+j]);
      s += lbuf;
    }

    s += " ";

    for (j=0; j<count; j++) {
      if (isprint(buf[i+j]))
	sprintf(lbuf, "%c", buf[i+j]);
      else
	sprintf(lbuf, ".");

      s += lbuf;
    }

    s += "\n";
    i += count;	
  }
  return s;
}


mode_t mcs::read_umask()
{
  mode_t mask = umask (0);
  umask(mask);
  return mask;
}



bool mcs::mkDir(string path, mode_t perm, enum ThrowExceptions throwexc)
{
  if (perm == MCS_MKDIR_UMASK)
    perm = (S_IRWXU | S_IRWXG | S_IRWXO) & ~read_umask();

  if (mkdir(path.csz, perm))
    if (throwexc)
      throw MCS_ERROR(MSG_CANT_MKDIR, path.csz);
    else
      return false;
  else
    return true;
}


bool mcs::rmDir(string path, enum ThrowExceptions throwexc)
{
  string s;
  //TODO: custom "rmdir"
  s = "rm -rf " + path;

  if (system(s.csz))
    if (throwexc)
      throw MCS_ERROR(MSG_CANT_RMDIR, path.csz);
    else
      return false;
  else
    return true;
}


void mcs::ls2Record(string fn, Record& v)
{
  DIR *dp;
  struct dirent *ep;
  string s, r;
  ifstream f;

  if (fn.empty()) return;

  dp = opendir (fn.c_str());
  if (dp) {
    while ((ep = readdir (dp))) {
      r=ep->d_name;
      if ( r != "."   &&   r != ".." )
	  ls2Record(fn + string("/") + r, v);
    }
    closedir (dp);
  }
  else {
    f.open(fn.c_str());
    if (f.is_open()) {
	Data d(NULL, STRING, "FILE", fn.length());
	d = fn;
	v.addField(&d);
	f.close();
    }
  }
}




//This routine is to be used inside this file, not by users
static char* appendFileNameToPath(char* OLDNAME, char* NEWNAME)
{
  char* buf;
  char* p;

  p = strrchr(OLDNAME, '/');
  if (p == NULL)
    p = OLDNAME;
  else {
    p++;
  }

  //Plus two: one to eventually add a slash, one for NULL
  buf = (char*) malloc(strlen(NEWNAME) + strlen(p) + 2);

  strcpy(buf, NEWNAME);

  if (buf[strlen(buf) - 1] != '/') { //If last character is not a slash
    buf[strlen(buf) + 1] = 0;
    buf[strlen(buf)] = '/';
  }

  strcpy(buf + strlen(buf), p);
  return buf;
}




#define BUFSIZE 16384
int mcs::copy(char* OLDNAME, char* NEWNAME)
{
  FILE *inf, *outf;
  int nread;
  char buf[BUFSIZE];

  if ((outf = fopen(NEWNAME, "wb")) == NULL) {
    if (errno == EISDIR) {
      char* b = appendFileNameToPath(OLDNAME, NEWNAME);
      int ret = copy(OLDNAME, b);
      free(b);
      return ret;
    }
    else {
      fprintf(stderr, "Error opening file %s: %s\n", NEWNAME,
	      strerror(errno));
      return -1;
    }
  }
  else {
    if ((inf = fopen(OLDNAME, "rb")) == NULL) {
      fclose(outf);
      fprintf(stderr, "Error opening file %s: %s\n", OLDNAME,
	      strerror(errno));
      return -1;
    }


    while (! feof(inf)) {
      nread = fread(buf, sizeof(char), BUFSIZE, inf);
      fwrite(buf, sizeof(char), nread, outf);
    }

    fclose(inf);
    fclose(outf);

    //Success
    return 0;
  }
}


int mcs::move(char* OLDNAME, char* NEWNAME)
{
  char* buf;
  int ret;

  ret = rename(OLDNAME, NEWNAME);

  if (ret == -1)  //An error occurred
    switch(errno) {

    case EISDIR: //NEWNAME is a directory but the OLDNAME isn't.
      //So append the filename to the NEWNAME directory and retry
      buf = appendFileNameToPath(OLDNAME, NEWNAME);
      ret = move(OLDNAME, buf);
      free(buf);
      return ret;


    case EBUSY: //This error can happen if NEWNAME is one of: ".", ".."
      if (NEWNAME[strlen(NEWNAME) - 1] != '/') { //That's a directory
	buf = appendFileNameToPath(OLDNAME, NEWNAME);
	ret = move(OLDNAME, buf);
	free(buf);
	return ret;
      }


    case EXDEV:  //Different filesystems, must copy, then delete
      ret = copy(OLDNAME, NEWNAME);
      if (ret == 0) {
	if (unlink(OLDNAME) == -1) {
	  fprintf(stderr, "Error deleting file %s: %s\n", OLDNAME,
		  strerror(errno));
	  return -1;
	}
      }
      return ret;



    default:
      fprintf(stderr, "Error renaming %s ==> %s: %s\n", OLDNAME, NEWNAME,
	      strerror(errno));
      return -1;
    }


  //Success
  return 0;
}





const char* mcs::B64_Codec::cb64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char* mcs::B64_Codec::cd64="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";


mcs::B64_Codec::B64_Codec()
{
  bufAllocated = NULL;
  precDataCount = 0;
  blocksout = 0;
}

mcs::B64_Codec::~B64_Codec()
{
  freeBuffer();
}


void mcs::B64_Codec::requireSpace(unsigned int Length)
{
  if (Length < 0)
    ; //ERROR

  if ((! bufAllocated)   ||
      (bufsize < Length)) {
    bufsize = Length;
    bufAllocated = (char*) realloc(bufAllocated, bufsize);
    memset(bufAllocated, 0, bufsize);
  }
  this->buf_out = bufAllocated;
}

void mcs::B64_Codec::freeBuffer()
{
  if (bufAllocated)
    free(bufAllocated);

  bufAllocated = NULL;
  bufsize = 0;
}


bool mcs::B64_Codec::eob()
{
  if (precDataCount)
    return false;
  else
    return ((bool) (pin >= EOIB));
}

unsigned char mcs::B64_Codec::readData()
{
  if (precDataCount) {
    unsigned char ret = precData[0];
    precData[0] = precData[1];
    precData[1] = precData[2];
    precData[2] = precData[3];
    precDataCount--;
    return ret;
  }
  else
    return *pin++;
}

void mcs::B64_Codec::writeData(unsigned char d)
{ *pout++ = d; }

char* mcs::B64_Codec::buffer()
{ return buf_out; }

unsigned int mcs::B64_Codec::bufUsed()
{ return pout - buf_out; }


unsigned int mcs::B64_Codec::encode(char* buf_in, int Length, char* par_buf_out,
				    unsigned int linesize)
{
  unsigned int rescue = 0;
  unsigned char in[3], out[4];
  int i, len = 0;

  int origLength = Length;  //Save "Length" because we are going to
			    //change it.

  //Number of bytes being read (those from precedent call:
  //precDataCount, plus actual buffer: Length) must be a multiple of 3
  //bytes.
  if (Length) {
    rescue = (precDataCount + Length) % 3; //"rescue" bytes need to be
                                           //saved for a later call.
    Length -= rescue;
  }

  //Setup input buffer
  EOIB = buf_in + Length;
  pin = buf_in;

  //Setup output buffer
  if (par_buf_out) {
    freeBuffer();
    buf_out = par_buf_out;
  }
  else {
    requireSpace(origLength * 2);
    buf_out = bufAllocated;
  }
  pout = buf_out;


  //Set linesize
  if (linesize < MCS_B64_MIN_LINE)
    linesize = MCS_B64_MIN_LINE;
  this->linesize = linesize;


  //Encode
  while (! eob()) {
    len = 0;
    for(i=0;  i<3;  i++) {
      if (! eob()) {
	in[i] = (unsigned char) readData();
	len++;
      }
      else
	in[i] = 0;
    }
    if (len) {
      encodeblock(in, out, len);
      for(i=0;  i<4;  i++) {
	writeData(out[i]);
      }
      blocksout++;
    }
    if (blocksout >= (linesize/4)) {
      writeData('\r');
      writeData('\n');
      blocksout = 0;
    }
  }


  if (Length) {  //Save bytes from input buffer for a later call
    for (unsigned int i=0; i<rescue; i++)
      precData[i] = readData();

    precDataCount = rescue;
  }
  else {  //Or if we are at end of file simply write a newline
    writeData('\r');
    writeData('\n');

    precDataCount = 0;  //Setup for new encoding
    blocksout = 0;
  }

  return pout - buf_out;
}



unsigned int mcs::B64_Codec::decode(char* buf_in, int Length, char* par_buf_out)
{
  unsigned char in[4], out[3], v, d;
  int i, len;

  //Setup input buffer
  EOIB = buf_in + Length;
  pin = buf_in;

  //Setup output buffer
  if (par_buf_out) {
    freeBuffer();
    buf_out = par_buf_out;
  }
  else {
    requireSpace(Length);
    buf_out = bufAllocated;
  }
  pout = buf_out;


  //Decode data
  len = 0;
  while (! eob()) {
    d = v = readData();

    //Check if the character is allowed
    v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
    if ((v != 0)     &&
	(v != '$')        ) {
      in[len] = v - 62;

      if (precDataCount == 0)
	precData[len] = d;

      len++;
    }

    //If we have a complete 4 byte block
    if (len == 4) {
      decodeblock(in, out);
      for(i=0; i<len-1; i++)
	writeData(out[i]);

      len = 0;
    }
  }

  if (len > 0) {  //If some byte hasn't been decoded yet
    precDataCount = len; //...save it for a later call

    if (Length == 0) { //Or if we are at the end of file
      decodeblock(in, out); //..decode it!
      for(i=0; i<len-1; i++)
	writeData(out[i]);

      precDataCount = 0;  //Setup for new encoding
    }
  }

  return pout - buf_out;
}

void mcs::B64_Codec::encodeblock( unsigned char in[3], unsigned char out[4], int len )
{
  out[0] = cb64[ in[0] >> 2 ];
  out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
  out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
  out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

void mcs::B64_Codec::decodeblock( unsigned char in[4], unsigned char out[3] )
{
  out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
  out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
  out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}




//returns 0 if timeout, 1 if ready, -1 if error
int mcs::Select(int fd, unsigned int sec_timeout, unsigned int usec_timeout, int op)
{
    fd_set set;
    struct timeval timeout;
    int ret;

    FD_ZERO (&set);
    FD_SET (fd, &set);
    timeout.tv_sec  = sec_timeout;
    timeout.tv_usec = usec_timeout;

    if (op == MCS_SELECT_READ)
	ret = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
    else
	ret =  select(FD_SETSIZE, NULL, &set, NULL, &timeout);

    return ret;
}



//returns 0 if timeout, number of fd ready, -1 if error
int mcs::Select(int fd[], int nfd, unsigned int sec_timeout, unsigned int usec_timeout, int op)
{
    fd_set set;
    struct timeval timeout;
    int i, ret;

    FD_ZERO (&set);
    for (i=0; i<nfd; i++) {
	FD_SET (fd[i], &set);
	fd[i] = 0;
    }

    timeout.tv_sec  = sec_timeout;
    timeout.tv_usec = usec_timeout;

    if (op == MCS_SELECT_READ)
	ret = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
    else
	ret = select(FD_SETSIZE, NULL, &set, NULL, &timeout);

    for (i=0; i<nfd; i++)
	fd[i] = FD_ISSET(i, &set);

    return ret;
}



