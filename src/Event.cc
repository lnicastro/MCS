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


void mcs::Event::init(string atFile, unsigned int atLine,
		      RetValue type, int code)
{
  if (! flInitialized) {
    memset(vmsg, 0, sizeof(vmsg));

    if (custom_init_vmsg)
	(*custom_init_vmsg)(vmsg);

    init_vmsg();

    flInitialized = true;
  }

  latFile  = atFile;
  latLine  = atLine;
  lcode    = code  ;
  lsubcode = 0     ;
  ltype    = type  ;

  buf[0] = '\0';
}



mcs::Event::Event(string atFile, unsigned int atLine, RetValue type, int code,
		  string s1, string s2, string s3)
{
    init(atFile, atLine, type, code);
    sprintf(buf, vmsg[code], s1.c_str(), s2.c_str(), s3.c_str());
    lmsg = buf;
}

mcs::Event::Event(string atFile, unsigned int atLine, RetValue type, int code,
		  string s1, int i1)
{
    init(atFile, atLine, type, code);
    sprintf(buf, vmsg[code], s1.c_str(), i1);
    lmsg = buf;
    lsubcode = i1;
}

mcs::Event::Event(string atFile, unsigned int atLine, RetValue type, int code,
		  int i1, string s1)
{
    init(atFile, atLine, type, code);
    sprintf(buf, vmsg[code], i1, s1.c_str());
    lmsg = buf;
}

mcs::Event::Event(string atFile, unsigned int atLine, RetValue type, int code,
		  int i1, int i2)
{
    init(atFile, atLine, type, code);
    sprintf(buf, vmsg[code], i1, i2);
    lmsg = buf;
    lsubcode = i2;
}



mcs::Event::~Event()
{}

int mcs::Event::code()
{
  return lcode;
}



RetValue mcs::Event::type()
{
  return ltype;
}

string mcs::Event::msg()
{
  return lmsg;
}


string mcs::Event::where()
{
  string lerror;
  char buf[30];

  sprintf(buf, "%d", latLine);
  lerror  = latFile + ":" + string(buf);

  sprintf(buf, " (%d) ", lcode);

  return lerror;
}


string mcs::Event::file()
{
    return latFile;
}


unsigned int mcs::Event::line()
{
    return latLine;
}


string mcs::Event::codemsg()
{
    char buf[5];
    sprintf(buf, "#%03d", code());
    return (buf + msg());
}



int mcs::Event::subcode()
{
    return lsubcode;
}

Event& mcs::Event::subcode(int subcode)
{
  lsubcode = subcode;
  return *this;
}

string mcs::Event::static_vmsg(unsigned int i)
{
    string ret = "";

    if ((i>=0)   &&   (i < MCS_VMSG_SIZE))
        if (vmsg[i])
            ret = string(vmsg[i]);

    return ret;
}


const char* mcs::Event::vmsg[MCS_VMSG_SIZE];
bool mcs::Event::flInitialized = false;
void (*mcs::Event::custom_init_vmsg) (const char* vmsg[MCS_VMSG_SIZE]) = NULL;






#ifndef DOXYGEN_SKIP
#if ENABLE_DEBUG
pthread_t mcs::Debug::thrs[MCS_DEBUG_NTHR];
bool mcs::Debug::thrmap[MCS_DEBUG_NTHR];
int mcs::Debug::dummy = mcs::Debug::init();

int mcs::Debug::init()
{
  int i;
  for (i=0; i<MCS_DEBUG_NTHR; i++) {
    thrs[i] = 0;
    thrmap[i] = false;
  }
  return 0;
}

mcs::Debug::Debug()
{
  active = false;
  pThis = 0;
}

mcs::Debug::~Debug()
{}


void mcs::Debug::setDebug(string lclassName, void* lpthis,
			  bool lactive)
{
  className = lclassName;
  pThis = lpthis;
  active = lactive;
}

void mcs::Debug::setDebug()
{ active = true; }


bool mcs::Debug::getDebugFlag()
{ return active; }


#define DEBUG_MAXLENGTH 80
string mcs::Debug::debug(const char* file, int line, const char* function)
{
  int i;
  bool found = false;
  for (i=0; i<MCS_DEBUG_NTHR; i++) {
    if (thrmap[i]   &&   thrs[i] == pthread_self()) {
      found = true;
      break;
    }
  }

  if (! found) {
    for (i=0; i<MCS_DEBUG_NTHR; i++)
      if (! thrmap[i]) {
	thrmap[i] = true;
	thrs[i] = pthread_self();
	break;
      }
  }


  char buf[DEBUG_MAXLENGTH];
  sprintf(buf, "*** THR %02d, %p, %s:%04d %s:%s ", i, pThis, file, line,
	  className.c_str(), function);
  buf[DEBUG_MAXLENGTH - 1] = 0;
  return string(buf);
}


#endif //ENABLE_DEBUG
#endif //! DOXYGEN_SKIP
