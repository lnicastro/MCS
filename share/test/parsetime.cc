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
#include <time.h>
#include <assert.h>
#include <iostream>
using namespace std;


#include <mcs.hh>
using namespace mcs;


#define DATETIME_PARSE_NTEMPL  16




/*
void sub1(string s, bool isDST, bool isUTC)
{
  const char* c;
  struct tm tm;
  time_t t;
  char buf[30];
  enum TimeMode timemode = LOCAL;

  //Parsing
  cout << "Parsing: " << s
       << " (isDST=" << isDST << ", isUTC=" << isUTC << ")" << endl;
  //c = DateTime::parseTime(s.c_str(), &tm);
  if (c)
    cout << "Format used: " << c << endl;
  else {
    cout << "Parse failed!" << endl;
    return;
  }

  tm.tm_isdst = isDST;//   &&  (! isUTC);

  //Output as time_t (reference value)
  t = (isUTC   ?   timegm(&tm)   :   timelocal(&tm) );
  cout << "Seconds since epoch: " << t << endl;

  //Output struct tm (local)
  localtime_r(&t, &tm);
  strftime(buf, 30, "%F %T (%Z)", &tm); //as a string
  cout << "Local: " << buf << ", dst=" << tm.tm_isdst << endl;

  //Output and struct tm (UTC)
  gmtime_r(&t, &tm);
  strftime(buf, 30, "%F %T (%Z)", &tm); //as a string
  cout << "UTC  : " << buf << ", dst=" << tm.tm_isdst << endl;

  //To MySQL (remove DST effect)
  time_t lt = t;
  int fl = 0;
  while ( (fl == 0)                       ||
	 ((fl == 1)  &&  (tm.tm_isdst))
        ) {
    switch (timemode) {
	case UTC:
	  gmtime_r(&lt, &tm);
	  break;
	case LOCAL:
	  localtime_r(&lt, &tm);
	  break;
    }
    if (tm.tm_isdst) lt -= 3600;
    fl++;
  }
  strftime(buf, 30, "%F %T", &tm); //as a string
  cout << "MySQL: " << buf << endl;

  //Go back to time_t
  tm.tm_isdst = 0;
  switch (timemode) {
      case UTC:
	lt = timegm(&tm);
	break;
      case LOCAL:
	lt = timelocal(&tm);
	break;
  }
  assert(t == lt);
  cout << endl << endl;
}



void sub(DateTime& dt)
{
  time_t t;
  struct tm tm;
  string s;
  MYSQL_TIME mt;

  dt.setTimeMode(LOCAL);
  t  = dt;
  tm = dt;
  s  = (string) dt;

  cout << "Seconds since epoch: " << t << endl;
  cout << "Local: " << s << endl;

  dt.setTimeMode(UTC);
  s  = (string) dt;
  cout << "UTC: " << s << endl;

  dt.to_MYSQL_TIME(&mt);
  dt.from_MYSQL_TIME(&mt);

}
*/


void chkStructTM(struct tm a, struct tm b)
{
  assert(a.tm_sec    == b.tm_sec   );
  assert(a.tm_min    == b.tm_min   );
cout<<"hour a: "<<a.tm_hour<<"\n---\n"
    <<"hour b: "<<b.tm_hour<<"\n===\n";
  assert(a.tm_hour   == b.tm_hour  );
  assert(a.tm_mday   == b.tm_mday  );
  assert(a.tm_mon    == b.tm_mon   );
  assert(a.tm_year   == b.tm_year  );
  assert(a.tm_wday   == b.tm_wday  );
  assert(a.tm_yday   == b.tm_yday  );
  assert(a.tm_isdst  == b.tm_isdst );
  assert(a.tm_gmtoff == b.tm_gmtoff);
  assert(a.tm_zone   == b.tm_zone  );
}


void chkMySQL(MYSQL_TIME* a, MYSQL_TIME* b)
{
  assert(a->second == b->second);
  assert(a->minute == b->minute);
cout<<"My_hour a: "<<a->hour<<"\n---\n"
    <<"My_hour b: "<<b->hour<<"\n===\n";
  assert(a->hour   == b->hour  );
  assert(a->day    == b->day   );
  assert(a->month  == b->month );
  assert(a->year   == b->year  );
}




int main()
{
  //The DateTime object to test
  DateTime dt;

  //Various datetime format
  struct tm tm;
  time_t t;
  string s;

  MYSQL_TIME mt1;
  MYSQL_TIME mt2;
  dt.setMysqlBuffer(&mt1); //Set internal pointer

  //Get some info about local time zone.
  t = 0;
  localtime_r(&t, &tm);

  int localgmtoff = tm.tm_gmtoff;
  char* localTZ = (char *)tm.tm_zone;

  gmtime_r(&t, &tm);
  char* gmtTZ = (char *)tm.tm_zone;

  t = 18316800;
  localtime_r(&t, &tm);
  char* localTZ_dst = (char *)tm.tm_zone;



  //Test 1: epoch
  dt.setTimeMode(LOCAL);
  dt = "1970-01-01";

  t = dt;
  assert(t == -1 * localgmtoff);

  memset(&tm, 0, sizeof(struct tm));
  tm.tm_sec  = 0;
  tm.tm_min  = 0;
  tm.tm_hour = 0;
  tm.tm_mday = 1;
  tm.tm_mon  = 0;
  tm.tm_year = 70;
  tm.tm_wday = 4;  //Thursday
  tm.tm_yday = 0;  //First day of the year
  tm.tm_isdst = 0;
  tm.tm_gmtoff = localgmtoff;
  tm.tm_zone = localTZ;
  chkStructTM(tm, dt);

  s = (string) dt;
  assert(! strcmp(s.c_str(), "1970-01-01 00:00:00"));

  memset(&mt2, 0, sizeof(MYSQL_TIME));
  mt2.second = 0;
  mt2.minute = 0;
  mt2.hour   = 0;
  mt2.day    = 1;
  mt2.month  = 1;
  mt2.year   = 1970;
  chkMySQL(&mt1, &mt2);

  //Convert to UTC
  dt.setTimeMode(UTC);
  t = dt;
  assert(t == -1 * localgmtoff);  //There's no difference in time_t values
				  //because they always refer to epoch

  memset(&tm, 0, sizeof(struct tm));
  tm.tm_sec  = 0;
  tm.tm_min  = 0;
  tm.tm_hour = 23;
  tm.tm_mday = 31;
  tm.tm_mon  = 11;
  tm.tm_year = 69;
  tm.tm_wday = 3;   //Wednesday
  tm.tm_yday = 364; //Last day of the year
  tm.tm_isdst = 0;
  tm.tm_gmtoff = 0;
  tm.tm_zone = gmtTZ;
  chkStructTM(tm, dt);

  s = (string) dt;
  assert(! strcmp(s.c_str(), "1969-12-31 23:00:00"));

  memset(&mt2, 0, sizeof(MYSQL_TIME));
  mt2.second = 0;
  mt2.minute = 0;
  mt2.hour   = 23;
  mt2.day    = 31;
  mt2.month  = 12;
  mt2.year   = 1969;
  chkMySQL(&mt1, &mt2);







  //Test 2: test DST
  dt.setTimeMode(LOCAL);
  dt = "1970-08-01 02:00:00"; //Correspond to time_t 18324000

  t = dt;
  assert(t == 18324000 - 3600 - localgmtoff); //One more hour to account for DST

  memset(&tm, 0, sizeof(struct tm));
  tm.tm_sec  = 0;
  tm.tm_min  = 0;
  tm.tm_hour = 2;
  tm.tm_mday = 1;
  tm.tm_mon  = 7;
  tm.tm_year = 70;
  tm.tm_wday = 6;  //Saturday
  tm.tm_yday = 212;
  tm.tm_isdst = 1;
  tm.tm_gmtoff = localgmtoff + 3600;
  tm.tm_zone = localTZ_dst;
  chkStructTM(tm, dt);

  s = (string) dt;
  assert(! strcmp(s.c_str(), "1970-08-01 02:00:00"));

  memset(&mt2, 0, sizeof(MYSQL_TIME));
  mt2.second = 0;
  mt2.minute = 0;
  mt2.hour   = 1; //IMPORTANT: MySQL time never account for DST!
  mt2.day    = 1;
  mt2.month  = 8;
  mt2.year   = 1970;
  chkMySQL(&mt1, &mt2);

  //Convert to UTC
  dt.setTimeMode(UTC);
  t = dt;
  assert(t == 18324000 - 3600 - localgmtoff);

  memset(&tm, 0, sizeof(struct tm));
  tm.tm_sec  = 0;
  tm.tm_min  = 0;
  tm.tm_hour = 0; //IMPORTANT: also UTC never account for DST!
  tm.tm_mday = 1;
  tm.tm_mon  = 7;
  tm.tm_year = 70;
  tm.tm_wday = 6;  //Saturday
  tm.tm_yday = 212;
  tm.tm_isdst = 0;
  tm.tm_gmtoff = 0;
  tm.tm_zone = gmtTZ;
  chkStructTM(tm, dt);

  s = (string) dt;
  assert(! strcmp(s.c_str(), "1970-08-01 00:00:00"));

  memset(&mt2, 0, sizeof(MYSQL_TIME));
  mt2.second = 0;
  mt2.minute = 0;
  mt2.hour   = 0;
  mt2.day    = 1;
  mt2.month  = 8;
  mt2.year   = 1970;
  chkMySQL(&mt1, &mt2);




  //Test 3: insert datetime as a time_t
  dt.setTimeMode(UTC);

  dt = 0;
  s = (string) dt;
  assert(! strcmp(s.c_str(), "1970-01-01 00:00:00"));

  dt.setTimeMode(LOCAL);
  s = (string) dt;
  assert(! strcmp(s.c_str(), "1970-01-01 01:00:00"));


  //Test 3: insert datetime as a struct tm, note that only the following 6
  //fields are used
  tm.tm_sec  = 1;
  tm.tm_min  = 2;
  tm.tm_hour = 3;
  tm.tm_mday = 4;
  tm.tm_mon  = 1;
  tm.tm_year = 60;

  dt = tm;
  s = (string) dt;
  assert(! strcmp(s.c_str(), "1960-02-04 03:02:01"));

  //Test 4: modify MySQL structure
  mt1.year = 1970;
  s = (string) dt;
  assert(! strcmp(s.c_str(), "1970-02-04 03:02:01"));


  cout << "Test successfull!" << endl;
}


