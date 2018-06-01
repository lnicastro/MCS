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
mcs::CommandParser::CommandParser()
{
    MCS_DEBUG_SETUP(0, "CommandParser");

    int i;
    for (i=0; i<MCS_CMD_MAX_TOKENS; i++) {
	md_args[i] = NULL;
	md_optargs[i] = NULL;
    }
}

mcs::CommandParser::~CommandParser()
{
    clearArgs();
    MCS_DEBUG_LEAVE(NOARGS);
}


string mcs::CommandParser::clean(string c)
{

    c = subst(c, "\t+|\n+|\r+", " ");
    c = subst(c, ";.*", "", MCS_SUBST_TRAILING);
    c = trim(c);
    return c;
}

void mcs::CommandParser::clearArgs()
{
    MCS_DEBUG_ENTER(NOARGS);
    int i;
    for (i=0; i<MCS_CMD_MAX_TOKENS; i++) {
	if (md_args[i]) delete md_args[i];
	if (md_optargs[i]) delete md_optargs[i];
	md_args[i] = NULL;
	md_optargs[i] = NULL;
    }
    args.clear();
    opts.clear();
    optargs.clear();
    tokens.clear();
    rest.clear();
    MCS_DEBUG_LEAVE(NOARGS);
}


string mcs::CommandParser::cline()
{
    return origcmdline;
}


string mcs::CommandParser::cmd()
{
    return lcmd;
}

string mcs::CommandParser::allargs()
{
    unsigned int i;
    string s;
    for (i=0; i<args.size(); i++) {
	if (i > 0) s+= " ";
	s += args[i];
    }
    return s;
}

int mcs::CommandParser::argc()
{
    return args.size();
}


int mcs::CommandParser::tokenc()
{
    return tokens.size();
}


string mcs::CommandParser::token(int i)
{
    unsigned int ui = i;

    if (i < 0)
	throw MCS_ERROR(MSG_INDEX_LT_ZERO, i);

    if (ui >= tokens.size())
	throw MCS_ERROR(MSG_INDEX_OUT_RANGE, ui, tokens.size());

    return tokens[ui];
}


string mcs::CommandParser::line_afterToken(int i)
{
    unsigned int ui = i;

    if (i < 0)
	throw MCS_ERROR(MSG_INDEX_LT_ZERO, i);

    if (ui >= rest.size())
	throw MCS_ERROR(MSG_INDEX_OUT_RANGE, ui, rest.size());

    return rest[ui];
}


Data& mcs::CommandParser::arg(int i)
{
    unsigned int ui = i;

    if (i < 0)
	throw MCS_ERROR(MSG_INDEX_LT_ZERO, i);

    if (ui >= args.size())
	throw MCS_ERROR(MSG_INDEX_OUT_RANGE, ui, args.size());

    return *md_args[ui];
}


string mcs::CommandParser::sarg(int i)
{
    return arg(i).sval();
}


string mcs::CommandParser::opt(int i)
{
    unsigned int ui = i;

    if (i < 0)
	throw MCS_ERROR(MSG_INDEX_LT_ZERO, i);

    if (ui >= opts.size())
	throw MCS_ERROR(MSG_INDEX_OUT_RANGE, ui, opts.size());

    return opts[ui];
}


int mcs::CommandParser::optc()
{
    return opts.size();
}


Data& mcs::CommandParser::optarg(int i)
{
    unsigned int ui = i;

    if (i < 0)
	throw MCS_ERROR(MSG_INDEX_LT_ZERO, i);

    if (ui >= opts.size())
	throw MCS_ERROR(MSG_INDEX_OUT_RANGE, ui, opts.size());

    return *md_optargs[ui];
}


bool mcs::CommandParser::cmpCmd(string cmd)
{
    cmd = clean(cmd);
    if (cmd.length() != lcmd.length())
      return false;
    else
      return (! strncasecmp(cmd.c_str(),
			    lcmd.c_str(),
			    lcmd.length()));
}


bool mcs::CommandParser::givenOpt(string opt, int& i)
{
    unsigned int ui;

    opt = clean(opt);
    for (ui=0; ui< opts.size(); ui++)
	if (opts[ui] == opt) {
	    i = ui;
	    return true;
	}
    return false;
}

bool mcs::CommandParser::givenOpt(string opt)
{
    int i;
    return givenOpt(opt, i);
}


void mcs::CommandParser::parseCmd(int argc, char* argv[])
{
  string cmd;
  int i;

  for (i=0; i<argc; i++)
    cmd += argv[i] + string(" ");

  parseCmd(cmd);
}

void mcs::CommandParser::parseCmd(string c)
{
  unsigned int ui;
  string opt, s;
  vector<string> lpar;
  const char* p;
  bool quoted = false;
  bool escape = false;

  clearArgs();
  origcmdline = clean(c);
  c = origcmdline;
  lcmd = "";

  p = c.c_str();
  s = "";
  while (*p) {
      if ((! escape)   &&   (*p == '\\'))
	  escape = true;
      else {
	  if ((! escape)   &&   (*p == '"'))
	      quoted = ! quoted;
	  else if ((! quoted)   &&   (*p == ' ')) {
	      s = trim(s);
	      if (! s.empty()) {
		  args.push_back(s);
		  tokens.push_back(s);
		  rest.push_back(string(p));
	      }
	      s = "";
	  }
	  else
	      s += *p;

	  escape = false;
      }
      p++;
  }
  s = trim(s);
  if (! s.empty()) {
      args.push_back(s);
      tokens.push_back(s);
      rest.push_back("");
  }

  if (args.size() >= 1) {
      lcmd = args[0];
      args.erase(args.begin());
  }

  pcrecpp::RE reopt("^-(\\w+)");
  pcrecpp::RE reoptwarg("^-(\\w+)=(.+)");
  ui = 0;
  while (ui < args.size()) {
      s = "";
      if (reopt.FullMatch(args[ui], &opt)          ||
	  reoptwarg.FullMatch(args[ui], &opt, &s) )  {
	  opts.push_back(opt);
	  optargs.push_back(s);
	  md_optargs[opts.size()-1] = new Data(s);
	  args.erase(args.begin() + ui);
      }
      else {
	  md_args[ui] = new Data(args[ui]);
	  ui++;
      }
  }

  /* //DEBUG
  int i;
  cout << endl << __LINE__ << " " << cmd() << "|" << endl << endl;
  cout << "Arguments: " << argc() << endl;
  for (i=0; i<argc(); i++)
      cout << i << " " << arg(i).sval() << "|" << endl;

  cout << endl << "Options: " << optc() << endl;
  for (i=0; i<optc(); i++)
      cout << i << " "
	   << this->opt(i) << " "
	   << optarg(i).sval() << "|" << endl;
  */
}



vector<string> mcs::CommandParser::replPars_onFile(string fn)
{
    vector<string> v;
    ifstream fin;
    string s, line;

    fin.open(fn.csz);
    if (!(fin.is_open()))
	throw MCS_ERROR(MSG_CANT_OPEN_FILE, fn.csz);
    else {
	line = "";
	while(getline(fin, s)) {
	    s = replPars(s);
	    if (! s.empty()) {
		if (! line.empty()) line += " " ;
		line += s;
		if (s[s.length()-1] == ';') {  //the line is complete
		    line = line.substr(0, line.length()-1); //removes ";"
		    v.push_back(trim(line));
		    line = "";
		}
	    }
	}
    }

    fin.close();
    return v;
}

//Used to parse external script
//Must be used one line at a time
string mcs::CommandParser::replPars(string s)
{
  char expr[10];
  unsigned int i;
  string r;

  s = trim(s);
  if (! s.empty()) {
      s = subst(s, "\n+"   , "", MCS_SUBST_TRAILING);  //Trailing newlines
      s = subst(s, "#.*$"  , "" );                 //Comments   --> null
      s = subst(s, "\t+"   , " ");                 //tabs       --> space
      s = subst(s, ";\\s*$", ";");                 //";" spaces --> ";"

      if (! s.empty()) {
	  for (i=0; i<args.size(); i++) {   //Arguments
	      sprintf(expr, "\\$%d", i);
	      r = md_args[i]->sval();
	      s = subst(s, expr, r, MCS_SUBST_QUOTE_WITH);
	  }
	  s = trim(s);
      }
  }

  return s;
}
