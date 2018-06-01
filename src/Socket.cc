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

#include "fcntl.h"


#define MCS_SOCKET_START     "DATA-START %d (%d)"
#define MCS_SOCKET_SENDING   "DATA-CHUNK %d"
#define MCS_SOCKET_STOP      "DATA-STOP"




//--------------------------------------------------------
mcs::NetInterface::NetInterface(string name)
{
  struct if_nameindex* ni;
  unsigned int i=0;
  bool found = false;

  ni = if_nameindex();
  while (ni[i].if_index) {
    names.push_back(string(ni[i].if_name));
    i++;
  }
  if_freenameindex(ni);


  for (i=0; i<names.size(); i++)
    if (name == names[i]) {
      lindex = i;
      found = true;
      break;
    }
  if (! found)
      throw MCS_FATAL(MSG_NET_INTERFACE_NAME_NOT_EXISTS, name.csz);

  if (! isup())
      throw MCS_FATAL(MSG_NET_INTERFACE_NOT_ACTIVE, name.csz);
}


mcs::NetInterface::~NetInterface()
{}


string mcs::NetInterface::name(int index)
{
  if (index == -1)
    return names[lindex];

  if ((unsigned int) index >= names.size())
      throw MCS_FATAL(MSG_NET_INTERFACE_ID_NOT_EXISTS, index, names.size());


  return names[index];
}

unsigned int mcs::NetInterface::index()
{
  return lindex;
}

unsigned int mcs::NetInterface::count()
{
  return names.size();
}


int mcs::NetInterface::req(int ioctl_num, struct ifreq *ifr)
{
  int sock, ret;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  strncpy(ifr->ifr_name, names[lindex].c_str(), IFNAMSIZ-1);
  ifr->ifr_name[IFNAMSIZ-1] = 0;

  ret = ioctl(sock, ioctl_num, ifr);
  close(sock);
  if (ret == -1)
      throw MCS_FATAL(MSG_RETRIEVING_INTERFACE_FLAGS, names[lindex].csz);

  return ret;
}


int mcs::NetInterface::getflags()
{
  struct ifreq ifr;
  req(SIOCGIFFLAGS, &ifr);
  return ifr.ifr_flags;
}


bool mcs::NetInterface::isup()
{
  return ((getflags() & IFF_UP) ? true : false);
}


void mcs::NetInterface::str_sockaddr(struct sockaddr* sa)
{
  struct ifreq ifr;

  ifr.ifr_addr.sa_family = AF_INET;
  req(SIOCGIFADDR, &ifr);

  sa->sa_family = ifr.ifr_addr.sa_family;
  memcpy(sa->sa_data, ifr.ifr_addr.sa_data, 14);
}


void mcs::NetInterface::str_sockaddr_in(struct sockaddr_in* sin)
{
  struct sockaddr sa;
  str_sockaddr(&sa);
  memcpy(sin, &sa, sizeof(struct sockaddr_in));
}


string mcs::NetInterface::ipaddress(int index)
{
  string s;
  struct sockaddr_in sin;

  int actIndex = lindex;
  if (index != -1 ) {
  if ((unsigned int) index >= names.size())
      throw MCS_FATAL(MSG_NET_INTERFACE_ID_NOT_EXISTS, index, names.size());

    lindex = index;
  }

  s = "(not available)";
  try {
    str_sockaddr_in(&sin);
    s = inet_ntoa(sin.sin_addr);
  }
  catch (Event e) {}

  lindex = actIndex;
  return s;
}









//--------------------------------------------------------
mcs::HostInfo::HostInfo(string host)
{
  struct hostent* he;

  he = gethostbyname(host.c_str());
  if (he == NULL)
      throw MCS_FATAL(MSG_RETRIEVING_HOSTNAME, hstrerror(h_errno));

  host = he->h_name;
  ipaddr = inet_ntoa(*(struct in_addr*)he->h_addr);
  populate_sockaddr_in();
}


mcs::HostInfo::HostInfo(int sockfd)
{
  int ret;
  socklen_t len;

  len = sizeof(sin);
  ret = getpeername(sockfd, (struct sockaddr*)&sin, &len);
  if (ret == -1)
      throw MCS_FATAL(MSG_RETRIEVING_PEERNAME, strerror(errno));

  struct hostent* he;
  he = gethostbyaddr(&(sin.sin_addr), sizeof(struct in_addr), AF_INET);

//  if (he == NULL)
//      throw MCS_FATAL(MSG_RETRIEVING_HOST_INFO, hstrerror(errno));

  if (he != NULL) {
      host = he->h_name;
      ipaddr = inet_ntoa(*(struct in_addr*)he->h_addr);
      populate_sockaddr_in();
  }
}

mcs::HostInfo::~HostInfo()
{}


void mcs::HostInfo::populate_sockaddr_in()
{
  struct in_addr addr;
  inet_aton(ipaddr.c_str(), &addr);

  sin.sin_family = AF_INET;
  sin.sin_port = htons(0);
  sin.sin_addr.s_addr = addr.s_addr;
  memset(&(sin.sin_zero), '\0', 8);
}

string mcs::HostInfo::hostname() { return host; }
string mcs::HostInfo::ipaddress() { return ipaddr; }










//--------------------------------------------------------
mcs::Socket::Socket(string host, unsigned short int port,
		    unsigned int readTimeout, unsigned int writeTimeout,
		    bool use_ssl) : HostInfo(host)
{
  MCS_DEBUG_SETUP(0, "Socket");

  this->port    = port;
  this->use_ssl = use_ssl;
#ifdef HAVE_OPENSSL_SSL_H
  this->ssl_ctx = NULL;
#endif //HAVE_OPENSSL_SSL_H

  set_struct_timeval(readTimeout , &readto);
  set_struct_timeval(writeTimeout, &writeto);

  sockfd = socketToHost(port);

#ifdef HAVE_OPENSSL_SSL_H
  if (use_ssl) {
    SSL_library_init();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

    ssl_ctx = SSL_CTX_new(SSLv23_method());

    ssl=SSL_new(ssl_ctx);
    sbio=BIO_new_socket(sockfd, BIO_NOCLOSE);
    SSL_set_bio(ssl, sbio, sbio);

    int ret = SSL_connect(ssl);
    if (ret <= 0)
      throw MCS_ERROR( MSG_SSL_CONNECT_ERROR );

    //ret = SSL_get_error(ssl, ret);
    //switch (ret) {
    //case SSL_ERROR_NONE:
    //  break;
    //case SSL_ERROR_ZERO_RETURN:     cout << "qui1" << endl; break;
    //case SSL_ERROR_WANT_READ:       cout << "qui2" << endl; break;
    //case SSL_ERROR_WANT_WRITE:	    cout << "qui3" << endl; break;
    //case SSL_ERROR_WANT_CONNECT:    cout << "qui4" << endl; break;
    //case SSL_ERROR_WANT_ACCEPT:	    cout << "qui5" << endl; break;
    //case SSL_ERROR_WANT_X509_LOOKUP:cout << "qui6" << endl; break;
    //case SSL_ERROR_SYSCALL:	    cout << "qui7" << endl; break;
    //case SSL_ERROR_SSL:             cout << "qui8" << endl; break;
    //}
    //cout << ERR_error_string(ret, NULL) <<  endl;
    //
    //ERR_print_errors_fp(stderr);
    //throw MCS_ERROR( MSG_SSL_CONNECT_ERROR );
  }
#endif //HAVE_OPENSSL_SSL_H
}


mcs::Socket::Socket(int sockfd,
		    unsigned int readTimeout,
		    unsigned int writeTimeout,
		    void* ssl_ctx) : HostInfo(sockfd)
{
  MCS_DEBUG_SETUP(0, "Socket");

  this->port    = 0;
  this->use_ssl = (ssl_ctx != NULL);
#ifdef HAVE_OPENSSL_SSL_H
  this->ssl_ctx = NULL;
#endif //HAVE_OPENSSL_SSL_H

  set_struct_timeval(readTimeout , &readto);
  set_struct_timeval(writeTimeout, &writeto);

  this->sockfd       = sockfd;


#ifdef HAVE_OPENSSL_SSL_H
  if (use_ssl) {
    SSL_CTX* ctx = (SSL_CTX*) ssl_ctx;

    sbio=BIO_new_socket(sockfd, BIO_NOCLOSE);
    ssl=SSL_new(ctx);
    SSL_set_bio(ssl, sbio, sbio);

    if(SSL_accept(ssl) <= 0)
      throw MCS_ERROR( MSG_SSL_ACCEPT_ERROR );
  }
#endif //HAVE_OPENSSL_SSL_H
}



int mcs::Socket::socketToHost(unsigned short port)
{
  int ret;
  //int sockfd;
  long socketflags;

  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  sin.sin_port = htons(port);

  // Set non-blocking
  socketflags = fcntl(sockfd, F_GETFL, NULL);
  fcntl(sockfd, F_SETFL, socketflags | O_NONBLOCK);

  ret = connect(sockfd, (struct sockaddr *)&sin, sizeof(struct sockaddr));

  if (ret == -1) {
    if (errno == EINPROGRESS)
      chkSend(THROW);
    else
      throw MCS_FATAL(MSG_CANT_CONNECT_TO_HOST, strerror(errno));
  }

  // Set to blocking mode again...
  fcntl(sockfd, F_SETFL, socketflags);

  return sockfd;
}


mcs::Socket::~Socket()
{
    Socket::Close();
}


void mcs::Socket::Close()
{
  if (sockfd) {
#ifdef HAVE_OPENSSL_SSL_H
    if (use_ssl) {
      SSL_shutdown(ssl);
      SSL_free(ssl);

      if (ssl_ctx)
	SSL_CTX_free (ssl_ctx);
    }
#endif //HAVE_OPENSSL_SSL_H

    close(sockfd);
    sockfd = 0;
  }
}


bool mcs::Socket::chkSend(enum ThrowExceptions throwexc)
{
  bool b = false;
  int ret;
  struct timeval timeout;
  timeout.tv_sec = writeto.tv_sec;
  timeout.tv_usec = writeto.tv_usec;

  FD_ZERO(&fds);
  FD_SET(sockfd, &fds);

  ret = select(sockfd+1, NULL, &fds, NULL, &timeout);
  if (ret == -1) { //Error
      if (throwexc)
	  throw MCS_FATAL(MSG_CALLING_SELECT, strerror(errno));

  } else if (ret == 0) { //Timeout
    if (throwexc)
	throw MCS_FATAL(MSG_TIME_OUT);
  }

  else {  //Should be ok
    if (FD_ISSET(sockfd, &fds))
      b = true;
    else //We didn't get an error nor a timeout, but the socket is not in the list???
	throw MCS_FATAL(MSG_UNEXPECTED);
  }

  return b;
}


bool mcs::Socket::chkRecv(bool chkDataAvailable, enum ThrowExceptions throwexc)
{
  bool b = false;
  int ret;
  struct timeval timeout;

  if (chkDataAvailable) {
      timeout.tv_sec  = 0;
      timeout.tv_usec = 0;
  }
  else {
      timeout.tv_sec  = readto.tv_sec;
      timeout.tv_usec = readto.tv_usec;
  }

  FD_ZERO(&fds);
  FD_SET(sockfd, &fds);

#ifdef HAVE_OPENSSL_SSL_H
  if (use_ssl) {
    ret = SSL_pending(ssl);
    if (ret > 0)
      return true;
  }
#endif //HAVE_OPENSSL_SSL_H

  ret = select(sockfd+1, &fds, NULL, NULL, &timeout);
  if (ret == -1) { //Error
    if (throwexc)
	throw MCS_FATAL(MSG_CALLING_SELECT, strerror(errno));
  }
  else if (ret == 0) { //Timeout
    if (throwexc)
	throw MCS_FATAL(MSG_TIME_OUT);
  }
  else { //Should be ok
    if (FD_ISSET(sockfd, &fds)) {
      char a;
      ret = recv(sockfd, &a, 1, MSG_PEEK);
      if (ret == -1) {  //Error
	if (throwexc)
	    throw MCS_FATAL(MSG_CALLING_RECV, strerror(errno));
      }
      else if (ret == 0) { //Connection closed by peer
	if (throwexc)
	    throw MCS_FATAL(MSG_CLOSED_BY_PEER);
      }
      else {
	b = true;
      }
    }
    else //We didn't get an error nor a timeout but the socket isn't in the list
	throw MCS_FATAL(MSG_UNEXPECTED);
  }

  return b;
}


void mcs::Socket::sendChunk(void* p, unsigned int size)
{
  string s;
  char buf[40];
  unsigned int resp;

  sprintf(buf, MCS_SOCKET_SENDING, size);
  print(buf);
	
  s = getline();
  if ((sscanf(s.c_str(), "%d", &resp) == 1)   &&   (resp == size) ) {
    write(p, size);
  }
  else
      throw MCS_WARN(MSG_SEND_ABORT_BY_RECEIVER);
}


unsigned int mcs::Socket::recvChunk(void* p, unsigned int maxsize)
{
    string s;
    char buf[60];
    unsigned int chunksize;

    s = getline();
    if (s == MCS_SOCKET_STOP)
	return 0;

    if (sscanf(s.c_str(), MCS_SOCKET_SENDING, &chunksize) != 1)
	throw MCS_FATAL(MSG_PROTOCOL, s.c_str());
	
    if (chunksize > maxsize)
	throw MCS_FATAL(MSG_NOT_ENOUGH_SPACE, chunksize, maxsize);

    sprintf(buf, "%d", chunksize);
    print(buf);

    read(p, chunksize);
    return chunksize;
}


unsigned int mcs::Socket::recvChunk(Buffer* abuf)
{
    string s;
    char buf[60];
    unsigned int chunksize;

    s = getline();
    if (s == MCS_SOCKET_STOP)
	return 0;

    if (sscanf(s.c_str(), MCS_SOCKET_SENDING, &chunksize) != 1)
	throw MCS_FATAL(MSG_PROTOCOL, s.c_str());
	
    abuf->resize(chunksize);
    char* p = (*abuf)[0];

    sprintf(buf, "%d", chunksize);
    print(buf);

    read(p, chunksize);
    return chunksize;
}




unsigned int mcs::Socket::read(void* buf, unsigned int count)
{
  unsigned int ret, lcount;
  char* p = (char*) buf;
  chkRecv();

  lcount = 0;
  while (count>0) {

#ifdef HAVE_OPENSSL_SSL_H
    if (use_ssl) {
      ret = SSL_read(ssl, p ,count);
      switch(SSL_get_error(ssl, ret)) {
      case SSL_ERROR_NONE:
	break;
      //case SSL_ERROR_ZERO_RETURN:
      //	goto shutdown;
      //case SSL_ERROR_SYSCALL:
      //	fprintf(stderr,
      //		"SSL Error: Premature close\n");
      //	goto done;
      default:
	throw MCS_FATAL(MSG_CLOSED_BY_PEER);
	//berr_exit("SSL read problem");
      }
    }
    else
#endif //HAVE_OPENSSL_SSL_H
      ret = recv(sockfd, (void*) p, count, 0);

    if (ret == 0)
	throw MCS_FATAL(MSG_CLOSED_BY_PEER);

    count -= ret;
    p += ret;
    lcount += ret;
  }

  return lcount;
}


unsigned int mcs::Socket::write(void* buf, unsigned int count)
{
  unsigned int ret;

  chkSend();

#ifdef HAVE_OPENSSL_SSL_H
  if (use_ssl) {
    ret = SSL_write(ssl, buf, count);
    switch(SSL_get_error(ssl, ret)) {
    case SSL_ERROR_NONE:
      break;
    default:
      throw MCS_FATAL(MSG_CALLING_SEND, "SSL write problem");
    }
  }
  else
#endif //HAVE_OPENSSL_SSL_H
    ret = send(sockfd, buf, count, 0);

  if (ret != count)
      throw MCS_FATAL(MSG_CALLING_SEND, strerror(errno));

  return ret;
}


string mcs::Socket::getline()
{
  char p = 0;
  string s = "";

  while (p != '\n') {
    read(&p, 1);
    s += p;
  }

  remTrailing(s, "\n");
  return s;
}


void mcs::Socket::print(string s)
{
  s += "\n";
  write((void*) s.c_str(), s.length());
}


void   mcs::Socket::sendData(Serializable* from)
{
  void* p;
  char buf[40];
  unsigned int chunksize;

  MCS_DEBUG_ENTER(NOARGS);

  if (from->knowSize())
      sprintf(buf, MCS_SOCKET_START, from->size(), from->maxChunkSize());
  else
      sprintf(buf, MCS_SOCKET_START, 0, from->maxChunkSize());
  print(buf);

  while ((p = from->nextChunk(chunksize))) {
    if (chunksize > 0)
      sendChunk(p, chunksize);
  }

  sprintf(buf, MCS_SOCKET_STOP);
  print(buf);
  MCS_DEBUG_LEAVE(NOARGS);
}


unsigned int mcs::Socket::recvData(char** buffer, unsigned int maxsize)
{
  unsigned int size, chunksize, accum, tmp;
  string s;

  s = getline();
  if (sscanf(s.c_str(), MCS_SOCKET_START, &size, &chunksize) != 2)
    throw MCS_FATAL(MSG_PROTOCOL, s.c_str());

  Buffer* abuf;
  if (*buffer)
    abuf = new Buffer(*buffer, maxsize);
  else
    abuf = new Buffer(DONT_FREE);

  accum = 0;
  while ((tmp = recvChunk(abuf)))
    accum += tmp;

  if (! (*buffer))
    *buffer = (*abuf)[0];

  return accum;

//  unsigned int size, chunksize, accum, tmp;
//  char* p;
//  string s;
//
//  s = getline();
//  if (sscanf(s.c_str(), MCS_SOCKET_START, &size, &chunksize) != 2)
//    throw MCS_FATAL(MSG_PROTOCOL, s.c_str());
//
//  if (! (*buffer)) {
//    if (size > 0) {
//      *buffer = (char*) malloc(size);
//      maxsize = size;
//    }
//    else {
//      *buffer = (char*) malloc(chunksize);
//      maxsize = chunksize;
//    }
//  }
//  else {
//    if (chunksize > maxsize)
//      throw MCS_FATAL(MSG_NOT_ENOUGH_SPACE, chunksize, maxsize);
//  }
//
//  accum = 0;
//  p = *buffer;
//  while ((tmp = recvChunk(p, maxsize))) {
//    p += tmp;
//    accum += tmp;
//    if (accum > maxsize)
//      *buffer = (char*) realloc(*buffer, maxsize + chunksize);
//  }
//
//  return accum;
}



unsigned int mcs::Socket::recvData(string filename)
{
  ofstream out(filename.c_str(), ios::binary);
  if (! out.is_open())
      throw MCS_ERROR(MSG_CANT_OPEN_FILE, filename.c_str());

  return recvData(out);
}


unsigned int mcs::Socket::recvData(ofstream& out)
{
  unsigned int size, chunksize, accum, tmp;
  char* p;
  string s;

  s = getline();
  if (sscanf(s.c_str(), MCS_SOCKET_START, &size, &chunksize) != 2)
      throw MCS_FATAL(MSG_PROTOCOL, s.c_str());

  p = (char*) malloc (chunksize);
  accum = 0;
  while ((tmp = recvChunk(p, chunksize))) {
    out.write(p, tmp);
    accum += tmp;
  }

  free(p);

  return accum;
}



unsigned int mcs::Socket::recvData(int filedes)
{
  unsigned int size, chunksize, accum, tmp;
  char* p;
  string s;

  s = getline();
  if (sscanf(s.c_str(), MCS_SOCKET_START, &size, &chunksize) != 2)
      throw MCS_FATAL(MSG_PROTOCOL, s.c_str());

  p = (char*) malloc (chunksize);
  accum = 0;
  while ((tmp = recvChunk(p, chunksize))) {
      ::write(filedes, p, tmp);
      accum += tmp;
  }

  free(p);

  return accum;
}




void mcs::Socket::set_struct_timeval(unsigned int millisec, struct timeval* time)
{
  float fsec = ((float) millisec) / 1000.0;
  int   isec = (int) rintf(fsec);
  time->tv_sec = isec;
  time->tv_usec = 0;
}









//--------------------------------------------------------
mcs::ServerSocket::ServerSocket(string interface, unsigned short int port,
				bool use_ssl, string sslcert, string sslpriv) :
  NetInterface(interface)
{
  int ret;
  struct sockaddr_in sin;

  this->use_ssl = use_ssl;

  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  str_sockaddr_in(&sin);
  sin.sin_port = htons(port);

  ret = 1;
  ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(int));
  ret = bind(sockfd, (struct sockaddr *)&sin, sizeof(struct sockaddr));

  if (ret == -1)
      throw MCS_FATAL(MSG_CALLING_BIND, strerror(errno));

  ret = listen(sockfd, 10);
  if (ret == -1)
      throw MCS_FATAL(MSG_CALLING_LISTEN, strerror(errno));


  //Eventually initialize the SSL library and a SSL context object
#ifdef HAVE_OPENSSL_SSL_H
  ssl_ctx = NULL;

  if (use_ssl) {
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_crypto_strings();

    ssl_ctx=SSL_CTX_new(SSLv23_server_method());


    if (! SSL_CTX_use_certificate_file(ssl_ctx, sslcert.c_str(),
				       SSL_FILETYPE_PEM))
      throw MCS_FATAL ( MSG_UNEXPECTED );

    if (! SSL_CTX_use_PrivateKey_file(ssl_ctx, sslpriv.c_str(),
				      SSL_FILETYPE_PEM))
      throw MCS_FATAL ( MSG_UNEXPECTED );

    if (! SSL_CTX_check_private_key(ssl_ctx))
      throw MCS_FATAL ( MSG_UNEXPECTED );
  }
#endif //HAVE_OPENSSL_SSL_H
}


mcs::ServerSocket::~ServerSocket()
{
  close(sockfd);

#ifdef HAVE_OPENSSL_SSL_H
  if (use_ssl)
    if (ssl_ctx)
      SSL_CTX_free (ssl_ctx);
#endif //HAVE_OPENSSL_SSL_H
}



void* mcs::ServerSocket::getSSLContext()
{
#ifdef HAVE_OPENSSL_SSL_H
  if (use_ssl)
    return ssl_ctx;
  else
    return NULL;
#else
  return NULL;
#endif //HAVE_OPENSSL_SSL_H
}



bool mcs::ServerSocket::acceptConnection(int& newsock, unsigned int millisec)
{
  struct sockaddr_in sin;
  socklen_t sin_size = sizeof(struct sockaddr_in);

  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(sockfd, &fds);

  struct timeval timeout;
  Socket::set_struct_timeval(millisec, &timeout);

  int ret = select(FD_SETSIZE, &fds, NULL, NULL, &timeout);
  if (ret == -1) { //Error
      throw MCS_FATAL(MSG_CALLING_SELECT, strerror(errno));
  }
  else if (ret == 0) { //Timeout
      return false;
  }
  else { //Should be ok
    if (FD_ISSET(sockfd, &fds))
	newsock = accept(sockfd, (struct sockaddr *)&sin, &sin_size);
    else //We didn't get an error nor a timeout but the socket isn't in the list
	throw MCS_FATAL(MSG_UNEXPECTED);
  }

  return true;
}
