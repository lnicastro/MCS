#include <mcs.hh>
using namespace mcs;

int main() {
  Data d(STRING, 20);

  //Assign an integer.
  d = 12;
  int i = d.ival();

  //Assigning a floating point number.
  d = 1.2;
  float f = d.fval();

  //Assign a date/time data.
  d = "2007-03-19 13:30";
  time_t t = d.tval();

  //Assign a string
  d = "My name is Giorgio";
  string s = d.sval();
}
