#include <mcs.hh>
using namespace mcs;

/*
  This example shows how to use the Data class in three different situations:

  - as a data holder, to perform conversion between different data types;
  - as a multi-dimensional array;
  - to access a database field;
 */

int main() {

  time_t tt = 0;  //In UTC
  struct tm tm;

  localtime_r(&tt, &tm);
  gmtime_r(&tt, &tm);



  tt = 100;

  try {
    //Simple usage as a data holder
    Data di(INT);        //Data object with base type INT

    //Store some values
    di = 1;         //store 1
    di = 2.7;       //store 2, note that the value is truncated
    di = "3";       //store 3

    try {
      di = "mcs";     //store a string, we expect an error
    }
    catch (Event e)
    { cerr << e.where() << ": " << e.msg() << endl; }

    //Retrieve values into different data type variables
    int          ii = di.ival();   cout << ii << endl;
    unsigned int ui = di.uival();  cout << ui << endl;
    float        ff = di.fval();   cout << ff << endl;
    string       ss = di.sval();   cout << ss << endl;

    char cc[10];
    di.cval(cc, 10);               cout << cc << endl;

    /*
      The time_t type represent a datetime as the number of seconds elapsed
      since 00:00:00 of 01-01-1970 UTC. Actually our INT Data object contains
      a value of zero, so a conversion to a datetime value will contain
      exactly 00:00:00 of 01-01-1970 UTC.
    */
    time_t       tt = di.tval();   cout << ctime(&tt) << endl;

    struct tm tm;
    di.tval(&tm);                  cout << asctime(&tm) << endl;

    //Now set the value to current datetime.
    di.settimenow();
    di.tval(&tm);                  cout << asctime(&tm) << endl;

    /*
      You may have noticed that the value is different from your computer
      clock, that is because by default UTC is used. To change this behaviour
      use setTimeLocal().
    */
    di.setTimeMode(LOCAL);
    di.settimenow();
    di.tval(&tm);                  cout << asctime(&tm) << endl;


    Data df(FLOAT);  //Data object with base type FLOAT
    df = 2.7;        //store 2.7
    df = "1.2e2";    //store 120
    ss = df.sval();                cout << ss << endl;
    df.tval(&tm);                  cout << asctime(&tm) << endl;


    Data dt(TIME);    //Data object with base type TIME

    dt = "00:30:15";  //Insert only a time value
    dt.tval(&tm);                  cout << asctime(&tm) << endl;

    dt = "2005-12-01"; //Insert only a date value
    dt.tval(&tm);                  cout << asctime(&tm) << endl;

    dt = "2005-12-01 00:30:15"; //Insert a datetime value
    dt.tval(&tm);                  cout << asctime(&tm) << endl;

    ss = dt.sval();                cout << ss << endl;

    //Number of seconds elapsed since 00:00:00 of 01-01-1970.
    ii = dt.tval();                cout << ii << endl;


    Data ds(STRING, 30);    //Data object with base type STRING


  }
  catch(Event e) {
    cerr << e.where() << ": " << e.msg() << endl;
    exit(1);
  }


  ////Assign a date/time data.
  //d = "2007-03-19 13:30";
  //time_t t = d.tval();
}
