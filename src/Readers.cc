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
//#undef ENABLE_CURL
//CURL includes
#if ENABLE_CURL
#include <curl/curl.h>
#else
#undef ENABLE_CURL
#endif

//CFITSIO include
#if ENABLE_CFITSIO
#include <fitsio.h>
//#include <fstream>
#endif  //ENABLE_CFITSIO


//Synchro Pipe::synchro;
int Pipe::filecount = 0;

Pipe::Pipe()
{
  pipefn = "";
  pipefd[0] = 0;
  pipefd[1] = 0;
  flcreated = false;
  named = false;
}

Pipe::~Pipe()
{
  //Assume that is always the thread who reads from the pipe that
  //manages the object creation/destruction
    //closeRead();
}

bool Pipe::isReady()
{ return flcreated; }

string Pipe::filename()
{ return pipefn; }


bool Pipe::consumerHasGone()
{
  if (pipefd[1])
    if (Select(pipefd[1], 0, 1, MCS_SELECT_WRITE) == -1)
      return true;

  return false;
}


void Pipe::create()
{
  if (flcreated)
    throw MCS_ERROR(MSG_PIPE_YET_OPENED);

  if (pipefd[0] + pipefd[1])
    throw MCS_ERROR(MSG_PIPE_YET_OPENED);

  if (pipe(pipefd) != 0)
    throw MCS_ERROR(MSG_CALLING_PIPE);

  flcreated = true;
  named = false;
}

string Pipe::createNamed()
{
  char filename[20];

  if (flcreated)
    throw MCS_ERROR(MSG_PIPE_YET_OPENED);

  if (pipefd[0] + pipefd[1])
    throw MCS_ERROR(MSG_PIPE_YET_OPENED);

  do {
    /*
      Due to the fact that a Pipe object is typically used by two
      concurrent threads, and that the FIFO file is not open just as
      it is created (instead it is opened in the openRead() method)
      it is not safe to use tempnam-like functions. So we provide our
      own mechanisms to solve filename conflicts.
    */
    synchro.enter();
    sprintf(filename, "/tmp/mcspipe%04d", filecount);
    filecount++;
    filecount = filecount % 10000;
    synchro.leave();

    if (mkfifo(filename, S_IREAD | S_IWRITE) == 0)
      break;

    sleep_ms(20);
  } while (1);

  pipefn = filename;
  flcreated = true;
  named = true;

  return pipefn;
}






int Pipe::openRead()
{
  if (! flcreated)
    throw MCS_ERROR(MSG_PIPE_NOT_CREATED);

  if (! pipefd[0]) {
      if (named) {
	  pipefd[0] = open(pipefn.c_str(), O_RDONLY);
	  unlink(pipefn.c_str()); //Remove the FIFO, it is no longer needed
	  if (pipefd[0] == -1) throw MCS_ERROR(MSG_CANT_OPEN_FILE, pipefn.c_str());
      }

      //Ensure the other side hand of the pipe has been opened
      while (Select(pipefd[0], 0, 100, MCS_SELECT_READ) != 1) ;
  }

  return pipefd[0];
}


int Pipe::openWrite()
{
  if (! flcreated)
    throw MCS_ERROR(MSG_PIPE_NOT_CREATED);

  if (! pipefd[1]) {
      if (named) {
	  pipefd[1] = open(pipefn.c_str(), O_WRONLY);
	  unlink(pipefn.c_str()); //Remove the FIFO, it is no longer needed
	  if (pipefd[1] == -1) throw MCS_ERROR(MSG_CANT_OPEN_FILE, pipefn.c_str());
      }

      //Ensure the other side hand of the pipe has been opened
      while (Select(pipefd[1], 0, 100, MCS_SELECT_WRITE) != 1) ;
  }

  return pipefd[1];
}


void Pipe::closeRead()
{
  if (! flcreated) return;  //Already closed
  if (named) unlink(pipefn.c_str()); //Remove the FIFO

  if (pipefd[0]) {
      //Close read side of the pipe
      close(pipefd[0]);
      pipefd[0] = 0;
  }

  pipefn = "";
  flcreated = false;
  named = false;
}

void Pipe::closeWrite()
{
  if (! flcreated) return;  //Already closed
  if (named) unlink(pipefn.c_str()); //Remove the FIFO

  if (pipefd[1]) {
      //Close write side of the pipe
      close(pipefd[1]);
      pipefd[1] = 0;
  }
}









#if ENABLE_CURL
bool mcs::URLReader::fl_curl_global_init = false;

mcs::URLReader::URLReader() : Pipe()
{
  if (! fl_curl_global_init) {
    fl_curl_global_init = true;
    curl_global_init(CURL_GLOBAL_ALL);
  }

  thr = NULL;
}


mcs::URLReader::~URLReader()
{ Close(); }


string mcs::URLReader::url()
{ return lurl; }

bool mcs::URLReader::chkLocal(string& url)
{
  bool local = (bool) (
		       (url.find("://") == string::npos)   ||
		       (url.find("file://") == 0)
		       );

  if (local) url = subst(url, "file://", "", MCS_SUBST_LEADING);
  return local;
}


int mcs::URLReader::OpenAsFD(string url)
{
  local = chkLocal(url);
  this->lurl = url;

  create();

  thr = new ThreadFunc(thread_run, this);
  thr->startDetached();
  return openRead();
}


const char* mcs::URLReader::OpenAsFifo(string url)
{
  local = chkLocal(url);
  this->lurl = url;

  string fn = createNamed();

  thr = new ThreadFunc(thread_run, this);
  thr->startDetached();

  return fn.c_str();
}



void mcs::URLReader::Download(string url, string fn)
{
  ofstream out(fn.c_str());

  if (! out.is_open())
    throw MCS_ERROR(MSG_CANT_OPEN_FILE, fn.csz);

  int fd = OpenAsFD(url);
  char buf[1024];
  int n;

  while ((n = read(fd, buf, 1024)))
    out.write(buf, n);

  out.close();
  Close();
}



void mcs::URLReader::Close()
{
  if (thr) {
    closeRead();
    lurl = "";

    while (thr->state() <= MCS_STATE_RUNNING)
      sleep_ms(20);

    if (thr->error())
    {
      Event e = *thr->error();
      delete thr;
      thr = NULL;
      throw e;
    }
    delete thr;
    thr = NULL;
  }
}



int mcs::URLReader::thread_run(void* p)
{
  URLReader* This = (URLReader*) p;
  This->thread_fetch();
  return 0;
}


void mcs::URLReader::thread_fetch()
{
  openWrite();  //Open file for writing, it is necessary here otherwise the
		//openRead() on the other thread won't return

  if (local) {
    int fd = open(lurl.c_str(), O_RDONLY);
    char buf[1024];
    int len;

    if (fd == -1) {
      closeWrite();
      throw MCS_ERROR(MSG_CANT_OPEN_FILE, lurl.c_str());
    }

    while ((len = read(fd, buf, 1024)))
      cb_write(buf, len, 1, this);

    closeWrite();
  }
  else {
    CURLcode res;
    CURL* curl = curl_easy_init();

    if (! curl)
      throw MCS_ERROR(MSG_CURL_INIT);

    res = curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    if (res != CURLE_OK) goto error;

    //Timeout in seconds
    res = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60);
    if (res != CURLE_OK) goto error;

    res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    if (res != CURLE_OK) goto error;

    res = curl_easy_setopt(curl, CURLOPT_URL, lurl.c_str());
    if (res != CURLE_OK) goto error;

    //res = curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, cb_progress);
    //if (res != CURLE_OK) goto error;

    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb_write);
    if (res != CURLE_OK) goto error;

    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    if (res != CURLE_OK) goto error;

    res = curl_easy_setopt(curl, CURLOPT_PRIVATE, this);
    if (res != CURLE_OK) goto error;

    res = curl_easy_perform(curl);
    if (res != 0) goto error;

    closeWrite();
    return;

  error:
    closeWrite();
    curl_easy_cleanup((CURL*) curl);
    throw MCS_ERROR(MSG_CURL_ERROR, curl_easy_strerror(res));
  }
}


size_t mcs::URLReader::cb_write(void *ptr, size_t size, size_t nmemb,
				void *This)
{
  URLReader* p = (URLReader*) This;

  if (p->consumerHasGone())  //The pipe has been closed on the consumer side
    return 0;  //This will abort CURL data fetching

  //The Write() method is virtual because some derived class may wants
  //to write in a different manner
  return p->Write(ptr, size, nmemb);
}


void mcs::URLReader::flush()
{ fsync(openWrite()); }


//In the base class version of the Write() method simply writes on
//output file descriptor.
unsigned int mcs::URLReader::Write(void *ptr, unsigned int size,
				   unsigned int nmemb)
{  return write(openWrite(), ptr, size * nmemb); }




int mcs::URLReader::Read(char* buf, int maxlen)
{ return read(openRead(), buf, maxlen); }


//Event* mcs::URLReader::thread_error()
//{
//  if (thr)
//    return thr->error();
//
//  return NULL;
//}


//int mcs::URLReader::cb_progress(void *clientp,
//				double dltotal, double dlnow,
//				double ultotal, double ulnow)
//{
//  cout << "Download statistics: " << dlnow << " : " << dltotal << endl;
//  return 0; //Continue reading
//}
#endif



#if ENABLE_CFITSIO
string mcs::fitsError(int status) {
  char buf[31];
  fits_get_errstatus(status, buf);
  return string(buf);
}


FITSReader::FITSReader() : RecordSet()
{ fptr = NULL; }


FITSReader::~FITSReader()
{}


#define FITSKEYLEN 70
#define CHECK_FITS_ERROR if (status) throw MCS_ERROR(MSG_FITS_ERROR, fitsError(status).c_str())


int FITSReader::HDUCount()
{
  int status = 0;
  fitsfile *fptr = (fitsfile*) this->fptr;

  fits_get_num_hdus(fptr, &nhdu, &status);

  return nhdu;
}


int FITSReader::currentHDU()
{
  int i;
  fitsfile *fptr = (fitsfile*) this->fptr;
  fits_get_hdu_num(fptr, &i);

  return i;
}

void FITSReader::selectHDU(string name, int extver)
{
  int status, i;
  fitsfile *fptr = (fitsfile*) this->fptr;

  status = 0;
  fits_movnam_hdu(fptr, ANY_HDU, (char*) name.c_str(), extver, &status);
  CHECK_FITS_ERROR;

  fits_get_hdu_num(fptr, &i);
  selectHDU(i);
}


bool FITSReader::selectNextHDU()
{
  int status = 0;
  int exttype = ANY_HDU;

  fitsfile *fptr = (fitsfile*) this->fptr;

  fits_movrel_hdu(fptr, 1, &exttype, &status);
  if (status == END_OF_FILE)
    return false;

  CHECK_FITS_ERROR;
  selectHDU(currentHDU());

  return true;
}


#include <json/json.h>

/*
  Trim input string for leading and trailing quote (') and spaces.

*/
std::string trimfs( std::string s ) {
  string ss = s;
  ss.erase( 0, ss.find_first_not_of( " '\t\n" ) );
  ss.erase( ss.find_last_not_of( " '\t\n" ) + 1);
  return ss;
}


void FITSReader::read_json_header(const string& json_string){

  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  bool parsingSuccessful = reader.parse( json_string, root );

  if ( !parsingSuccessful )
    {
      // report to the user the failure and their locations in the document.
      std::cout  << "Failed to parse configuration\n"
		 << reader.getFormattedErrorMessages();
      return;
    }

  // Get the value of the member of root named 'encoding', return 'UTF-8' if there is no
  // such member.
  //  std::string encoding = root.get("encoding", "UTF-8" ).asString();

  // Get the value of the member of root named 'encoding', return a 'null' value if
  // there is no such member.
  const Json::Value fitskeys = root["keywords"];

  unsigned int nkeys= fitskeys.size();
  string key, value;

  cout << "Found " << nkeys << " keys in JSON string" << endl;

  //  pthread_init()

  //cout << "XX"<<endl;

  //header_x.addField(new Data("tototot"));
  // header_comments.addField(new Data("tototot"));
  
  //cout << "XX"<<endl;


  //cout << "clear comments "<< header.size() << " and " << header_comments.size() << endl;
  
  if(header_comments.size())
    header_comments.clear();
  
  
  // if(header_x.size())
  //   header_x.clear();

  cout << "clear comments "<< header.size() << " and " << header_comments.size() << endl;

  header.clear();
  cout << "clear comments OK"<<endl;

  for ( int index = 0; index < nkeys; ++index ){  // Iterates over the sequence elements.


    key=fitskeys[index]["key"].asString();
    value=fitskeys[index]["value"].asString();

    value = trimfs(value);
    cout << "Adding " << index << " : " << key <<" = " << value << endl;

    Data* d = new Data(string(value));
    d->setName(string(key));
    header.addField(d);
    

    // d = new Data(string(head_comment));
    // d->setName(string(head_name));
    // header_comments.addField(d);

  } 
  
  cout << "Done loading JSON keys ! Record list: header count = " << header.count() << endl;
  
  for(int i=0;i<header.count();i++){
    cout << i << " : " << header[i].name()  << " value " << header[i].sval()<< endl;
  }




  //  sleep(5);

}

void FITSReader::selectHDU(int hdunum)
{
  int status;
  long int repeat, width;
  int i, j;
  int fits_type;
  Types type;
  bool flunsign;
  string s;
  char buf[FITSKEYLEN];
  char reqColName[5];
  char head_name[80];
  char head_val[80];
  char head_comment[80];
  Record meta;
  int nkeys;
  int hdutype;

  fitsfile *fptr = (fitsfile*) this->fptr;
  fits_get_num_hdus(fptr, &nhdu, &status);

  if ((hdunum <= 0)   ||   (hdunum > nhdu))
      throw; //ERROR

  status = 0;
  hdutype = ANY_HDU;
  fits_movabs_hdu(fptr, hdunum, &hdutype, &status);
  CHECK_FITS_ERROR;

  fits_get_hdrspace(fptr, &nkeys, NULL, &status);
  CHECK_FITS_ERROR;

  header.clear();
  header_comments.clear();

  for (i=1; i<=nkeys; i++) {
    fits_read_keyn(fptr, i, head_name, head_val, head_comment, &status);
    CHECK_FITS_ERROR;

    Data* d = new Data(string(head_val));
    d->setName(string(head_name));
    header.addField(d);

    d = new Data(string(head_comment));
    d->setName(string(head_name));
    header_comments.addField(d);
  }

  fits_get_hdu_type(fptr, &hdutype, &status);
  CHECK_FITS_ERROR;

  if ((hdutype == BINARY_TBL)   ||
      (hdutype == ASCII_TBL)       ) {
    //Number of records
    fits_get_num_rows(fptr, &nrows, &status);
    CHECK_FITS_ERROR;

    //Number of fields
    fits_get_num_cols(fptr, &ncols, &status);
    CHECK_FITS_ERROR;

    for (i=0; i< ncols; i++) {
      sprintf(reqColName, "%d", i+1);
      fits_get_colname(fptr, CASEINSEN, reqColName, buf, &j, &status);
      fits_get_coltype(fptr, j, &fits_type, &repeat, &width, &status);
      CHECK_FITS_ERROR;

      if (! FITS2Types(fits_type, type, flunsign))
	throw MCS_ERROR(MSG_TYPE_NOT_HANDLED, i, fits_type);

      if (! VarLenType(type))
	width = 0;

      Data* d = new Data(NULL, type, buf, width, flunsign);
      meta.addField(d);
    }

    i = MCS_RS_USEMETAREC | MCS_RS_KNOW_NROWS;
    if (local)
      i |= MCS_RS_RANDOM;

    init(i, nrows, &meta);
    startFetch();
  }
  else {  //Empty recordset
    i = MCS_RS_USEMETAREC | MCS_RS_KNOW_NROWS;
    init(i, 0, &meta);
  }
}

//static FILE* _origstdin_ = stdin;

//int fits_open_pipe(fitsfile** fptr, int filedes, int rwmode, int* status)
//{
//  if (filedes == 0)
//    stdin = _origstdin_;
//  else
//    _origstdin_ = fdopen(filedes, "r");
//
//  return fits_open_file(fptr, "-", READONLY, status);
//}


//void FITSReader::open(int fd)
//{
//  int status = 0;
//  fitsfile *fptr;
//
//  fits_open_pipe(&fptr, fd, READONLY, &status);
//  CHECK_FITS_ERROR;
//  this->fptr = fptr;
//
//  fits_get_num_hdus(fptr, &nhdu, &status);
//  CHECK_FITS_ERROR;
//
//  selectHDU(1);
//}


//static FILE* _origstdin_ = stdin;

void FITSReader::open(Buffer& buf)
{
  int status;
  fitsfile *fptr;

  memfile.set(buf[0], buf.size(), AUTO_FREE);

  memfilep = memfile[0];
  memfilesize = memfile.size();
  fits_open_memfile(&fptr, "mem.fits", READONLY,
		    &memfilep, &memfilesize,
		    0, NULL, &status);

  CHECK_FITS_ERROR;
  this->fptr = fptr;

  fits_get_num_hdus(fptr, &nhdu, &status);
  CHECK_FITS_ERROR;

  selectHDU(1);
}




void FITSReader::open(string fn)
{
  int status;
  fitsfile *fptr;

#if defined(ENABLE_CURL)
  local = URLReader::chkLocal(fn);
#else
  local = true;
#endif

  status = 0;
  //if (local) {
  if (local && this->ffile_is_compressed(fn)) {
    fits_open_file(&fptr, fn.c_str(), READONLY, &status);
  }
  else if (local) {
    fits_open_file(&fptr, fn.c_str(), READONLY, &status);
  }

#if defined(ENABLE_CURL)
  else {
    //The FITSReader can read also remote files (using its parent class
    //URLReader). Note that CFITSIO cannot read a FITS file as a
    //stream so actually it is entirely loaded into memory and then
    //parsed.

    //Open the remote file as a file descriptor
    URLReader url;
    int fd = url.OpenAsFD(fn.c_str());
    int n;
    char tmp[8192];

    //Load the FITS file into memory
    memfile.free();
    while ((n = read(fd, tmp, 8192)))
	memfile(n) << tmp;

    memfilep = memfile[0];
    memfilesize = memfile.size();
    fits_open_memfile(&fptr, "mem.fits", READONLY,
		      &memfilep, &memfilesize,
		      0, NULL, &status);

    ////Substitute the stdin stream with a new one wrapped around the new fd
    //stdin = fdopen(fd, "r");
    //
    ////Read the fits file into memory and parse it
    //fits_open_file(&fptr, "-", READONLY, &status);
    //
    ////Restore original stdin stream
    //stdin = _origstdin_;

    //Close the remote file descriptor. If an error occurred during timeout
    //an exception will be thrown here.
    url.Close();
  }
#endif

  CHECK_FITS_ERROR;
  this->fptr = fptr;

  fits_get_num_hdus(fptr, &nhdu, &status);
  CHECK_FITS_ERROR;

  selectHDU(1);
}



bool FITSReader::fetch(unsigned int newpos, bool random)
{
  int i, status = 0;
  fitsfile *fptr = (fitsfile*) this->fptr;

  if (newpos >= nrows)
    return false;

  for (i=0; i<ncols; i++) {
    void* p = rec()[i].buffer();
    int fitstype;
    int isnull;

    Types2FITS(rec()[i].type(), rec()[i].isUnsigned(), fitstype);

    fits_read_col(fptr, fitstype, i+1, newpos+1, 1, 1, NULL,
		  (VarLenType(rec()[i].type())  ?   &p   :   p),
		  &isnull, &status);
    CHECK_FITS_ERROR;

    //rec()[i].setNull(isnull);
    rec()[i].setNull(0);
    //if ( (fitstype == TFLOAT) && (isnan( *(float*)p) ) ) rec()[i].setNull(1);
    //if ( (fitstype == TDOUBLE) && (isnan( *(double*)p) ) ) rec()[i].setNull(1);
    if ( (fitstype == TFLOAT) && ! (isfinite( *(float*)p) ) ) rec()[i].setNull(1);
    if ( (fitstype == TDOUBLE) && ! (isfinite( *(double*)p) ) ) rec()[i].setNull(1);
  }

  return true;
}


void FITSReader::close() {
  int status = 0;
  fitsfile *fptr = (fitsfile*) this->fptr;

  if (fptr) {
    fits_close_file(fptr, &status);
    fptr = NULL;
    CHECK_FITS_ERROR;
  }
}

bool FITSReader::ffile_is_compressed(string fn) {
  char buf[2];

  try {
    std::ifstream in(fn.c_str(), ios_base::in | ios_base::binary);

    in.read(&buf[0], 2);

    in.close();

    if ( (memcmp(buf, "\037\213", 2) == 0) ||  /* GZIP  */
         (memcmp(buf, "\120\113", 2) == 0) ||  /* PKZIP */
         (memcmp(buf, "\037\036", 2) == 0) ||  /* PACK  */
         (memcmp(buf, "\037\235", 2) == 0) ||  /* LZW   */
         (memcmp(buf, "\037\240", 2) == 0) )   /* LZH   */
    {
      return true;  /* compressed file */
    } else {
      return false;  /* not a compressed file */
    }
  } catch(Event& e) {
    cerr <<"ffile_is_compressed: "<< e.msg() << endl;
    return false;
  }

}
#endif //ENABLE_CFITSIO






//FileReader::FileReader() : RecordSet()
//{
//  fd = 0;
//}
//
//FileReader::~FileReader()
//{
//  this->close();
//}
//
//void FileReader::open(string url)
//{
//  this->close();
//
//  if (master.count() == 0)
//    throw;
//
//  fd = input.OpenAsFD(url);
//  init(0);
//  startFetch();
//}
//
//
//bool FileReader::fetch(unsigned int newpos, bool random)
//{
//  if (sep.length() > 0) {   //Variable length record
//    ;
//
//  }
//  else {  //Fixed length record
//    size_t recsize = 0;
//    int i;
//    for (i=0; i<master.count(); i++)  //Compute record size
//      recsize += master[i].maxLength();
//
//    buf.chkAllocation(recsize);
//    unsigned int size = read(fd, buf[0], recsize);
//
//    if (size < recsize)
//      throw;
//
//    //Copy data into the current record object
//    rec() = master;
//    unsigned int cur = 0;
//    for (i=0; i<master.count(); i++) {
//      buf(cur, rec()[i].maxLength()) >> rec()[i].buffer();
//      cur += rec()[i].maxLength();
//    }
//  }
//
//  return true;
//}
//
//void FileReader::close()
//{
//  sep = "";
//  eor = "";
//  master.clear();
//}
//
//void FileReader::setMaster(Record& rec)
//{
//  sep = "";
//  eor = "";
//  master = rec;
//}
//
//void FileReader::setMaster(Record& rec, string sep)
//{
//  this->sep = sep;
//  eor = "";
//  master = rec;
//}
//
//void FileReader::setMaster(Record& rec, string sep, string eor)
//{
//  this->sep = sep;
//  this->eor = eor;
//  master = rec;
//}
