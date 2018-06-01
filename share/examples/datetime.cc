#include <string.h>
#include <stdio.h>
#include <mcs.hh>
using namespace mcs;

MYSQL_TIME buf;


//This routine is used to print the content of a DateTime object using
//different data type.
void printDateTime(DateTime& dt)
{
  time_t t;
  struct tm tm;
  string s;
  char dbuf[120];

  //Retrieve a time_t value, that is the number of seconds elapsed since epoch
  //(1970-01-01 00:00:00 UTC).
  t = dt;

  //Retrieve a struct tm.
  tm = dt;

  //Retrieve a string representation.
  s = (string) dt;


  sprintf(dbuf,
	  "%s\t%4d-%02d-%02d %02d:%02d:%02d  %1d   %4d-%02d-%02d %02d:%02d:%02d \t%ld",
	  s.c_str(), 
	  tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday,
	  tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_isdst, 
	  buf.year, buf.month, buf.day, buf.hour, buf.minute, buf.second, t);

  cout << dbuf << endl;
}




int main()
{
  DateTime dt;
  struct tm tm;

  //Insert a datetime as the number of seconds elapsed since epoch
  dt.setTimeMode(LOCAL);
  dt = 3600;
  printDateTime(dt);
  dt.setTimeMode(UTC);
  printDateTime(dt);
  
  //Insert a time value
  dt.setTimeMode(LOCAL);
  dt = "01:00:15";
  printDateTime(dt);
  dt.setTimeMode(UTC);
  printDateTime(dt);
  
  //Insert a date value
  dt.setTimeMode(LOCAL);
  dt = "2010-01-01";
  printDateTime(dt);
  dt.setTimeMode(UTC);
  printDateTime(dt);
    
  //Insert a datetime as a struct tm
  dt.setTimeMode(LOCAL);
  tm.tm_sec   = 0;
  tm.tm_min   = 0;
  tm.tm_hour  = 1;
  tm.tm_mday  = 1;
  tm.tm_mon   = 7;
  tm.tm_year  = 70;
  dt = tm;
  printDateTime(dt);
  dt.setTimeMode(UTC);
  printDateTime(dt);


  //Set current datetime
  dt.now();
  printDateTime(dt);
  dt.setTimeMode(UTC);
  printDateTime(dt);
  cout << endl;



  //Show how DST is handled
  for (int i=0; i<=3; i++) {
    switch (i) {
    case 0:   dt.setTimeMode(LOCAL);    break;
    case 1:   dt.setMysqlBuffer(&buf);  break;
    case 2:   
      memset(&buf, '\0', sizeof (buf));
      dt.setMysqlBuffer(NULL); 
      dt.setTimeMode(UTC); 
      break;
    case 3:   dt.setMysqlBuffer(&buf);  break;
    }


    //Just before DST
    dt = "2010-03-28 01:59:59";
    printDateTime(dt);

    //When DST become active the interval 2:00:00 -- 3:00:00 does not
    //exists. Inserting a value in this interval result in a warning.
    dt = "2010-03-28 02:00:00";
    printDateTime(dt);

    //First valid DST time
    dt = "2010-03-28 03:00:00";
    printDateTime(dt);
    
    //Valid DST time
    dt = "2010-08-01 01:00:00";
    printDateTime(dt);

    //Last valid DST time
    dt = "2010-10-31 01:59:59";
    printDateTime(dt);

    //datetime one second after can not be given using a string
    //representation, thus we use time_t value
    dt = ((time_t) dt) + 1;
    printDateTime(dt);

    //When DST become inactive the interval 2:00:00 -- 3:00:00 is
    //ambiguous. Inserting a value in this interval result in a
    //warning.
    dt = "2010-10-31 02:00:00";
    printDateTime(dt);

    //First valid non-DST time
    dt = "2010-10-31 03:00:00";
    printDateTime(dt);

    cout << endl;
  }
}
