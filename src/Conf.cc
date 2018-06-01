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
mcs::Conf::Conf(string filename) :
  lastval(STRING, 300)
{
  if (! filename.empty())
    open(filename);
}


mcs::Conf::~Conf()
{}


void mcs::Conf::open(string filename)
{
  ifstream in;
  string sect, s, key, val, comm;

  pcrecpp::RE re_brackets("\\[(\\w+[-+]?\\w+)\\]");
  pcrecpp::RE re_comments("(.*);(.*)");
  pcrecpp::RE re_equal_split("(.+)=(.+)");


  this->filename = filename;

  in.open(filename.c_str());
  if (!in)
    throw MCS_ERROR(MSG_CANT_OPEN_FILE, filename.csz);


  keys.clear();
  values.clear();
  sections.clear();
  comments.clear();


  while (getline(in, s)) {
    s = trim(s);
    s = remTabs(s);

    if (! s.empty()) {
      if (re_brackets.FullMatch(s, &val)) {  //This is a section
	sect = val;
      }
      else {
	if (re_equal_split.FullMatch(s)) {
	  re_equal_split.FullMatch(s, &key, &val);
	
	  comm = "";
	  if (re_comments.FullMatch(val)) {
	    s  = val;
	    re_comments.FullMatch(s, &val, &comm);
	  }
	
	  keys.push_back(trim(key));
	  values.push_back(trim(val));
	  sections.push_back(sect);
	  comments.push_back(trim(comm));
	}
      }
    }
  }
  in.close();

  //    unsigned int i;
  //    for (i=0; i<sections.size(); i++)
  //	cout << "-"  << sections[i] << "-\t"
  //	     << "-"  << keys[i]     << "-\t"
  //	     << "-"  << values[i]   << "-\t"
  //	     << "-"  << comments[i] << "-"  << endl;	
}






int mcs::Conf::index(string section, string key)
{
  unsigned int i;
  section = trim(section);
  key = trim(key);
  lastval.setNull();

  for (i=0; i<sections.size(); i++)
    if (sections[i] == section)
      if (keys[i] == key) {
	lastval = values[i];
	return (int) i;
      }

  return -1;
}


bool mcs::Conf::search(string section, string key, enum ThrowExceptions throwexc)
{
  if (index(section, key) > -1)
    return true;
  else
    if (throwexc)
      throw MCS_ERROR(MSG_ENTRY_UNAVAILABLE, section.csz, key.csz);
    else
      return false;
}



string mcs::Conf::sval(string section, string key)
{
  search(section, key, THROW);
  return lastval.sval();
}


int mcs::Conf::ival(string section, string key)
{
  search(section, key, THROW);
  return lastval.ival();
}


long long int mcs::Conf::lval(string section, string key)
{
  search(section, key, THROW);
  return lastval.lval();
}


Data& mcs::Conf::val(string section, string key)
{
  if (section.length() + key.length() > 1)
    search(section, key, THROW);
  return lastval;
}


string mcs::Conf::sval(string section, string key, string defval)
{
  if (search(section, key, DONT_THROW))
    return lastval.sval();
  else
    return defval;
}


int mcs::Conf::ival(string section, string key, int defval)
{
  if (search(section, key, DONT_THROW))
    return lastval.ival();
  else
    return defval;
}


long long int mcs::Conf::lval(string section, string key, int defval)
{
  if (search(section, key, DONT_THROW))
    return lastval.lval();
  else
    return defval;
}


void mcs::Conf::setval(string section, string key, string val, string comment)
{
  val = trim(val);
  comment = trim(comment);
  int i = index(section, key);
  if (i > -1) {
    values[i] = val;
    comments[i] = comment;
  }
  else {
    sections.push_back(section);
    keys.push_back(key);
    values.push_back(val);
    comments.push_back(comment);
  }
}


void mcs::Conf::setval(string section, string key, Data* val, string comment)
{
  setval(section, key, val->sval(), comment);
}


void mcs::Conf::setval(string section, string key, int val, string comment)
{
  setval(section, key, itos(val), comment);
}


void mcs::Conf::setval(string section, string key, long long int val, string comment)
{
  setval(section, key, itos(val), comment);
}


void mcs::Conf::save(string filename)
{
  ofstream out;
  string sect;
  unsigned int i;

  if (filename.empty())
    filename = this->filename;

  out.open(filename.c_str());
  if (!out)
    throw MCS_ERROR(MSG_CANT_OPEN_FILE, filename.csz);

  for (i=0; i<sections.size(); i++) {
    if (sections[i] != sect) {
      sect = sections[i];
      out << "[" << sect << "]" << endl;
    }

    out << keys[i] << " = " << values[i];

    if (! comments[i].empty())
      out << " ;" << comments[i];

    out << endl;
  }
  out.close();
}
