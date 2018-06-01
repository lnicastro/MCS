#include <mcs.hh>
using namespace mcs;

/*
  This example shows how to use the CommandParser class to perform command
  line parsing. Command lines can be entered on standard input. Try these
  command lines:

      cmd one two three
      cmd one -p two
      cmd -q=abc  -s="string with blanks"

  The escape character is \, it can be used with itself and with doble quotes:

      cmd "blah blah" -1="escape character: \\ \""
      cmd par1 -e "bye bye"  \\a -sss="_\\_\"_"
   */


int main() {
  CommandParser cp;
  int i;
  char cmdline[200];

  try {
    while (cin.getline(cmdline, 200)) {
      //Parse command line.
      cp.parseCmd(cmdline);

      cout << "Command line is: "  << cp.cline()         << endl;
      cout << "Command: "          << cp.cmd()           << endl;


      //Tokens are strings separated by one or more blanks
      cout << "Number of tokens (" << cp.tokenc() << ")" << endl;
      for (i=0; i<cp.tokenc(); i++)
	cout << "  token " << i << ":" << cp.token(i) << "|" << endl;


      //Arguments are all those tokens that doesn't begin with a "-".
      cout << endl << "Arguments (" << cp.argc() << "): " << endl;

      for (i=0; i<cp.argc(); i++)
	cout << "  arg " << i << ":" << cp.arg(i).sval() << " " << endl;


      //Options are all those tokens that begin with a "-". An option can have an
      //argument assigned with an "=" character.
      cout << endl << "Options (" << cp.optc() << ") :" << endl;

      for (i=0; i<cp.optc(); i++) {
	cout << "  opt " << i << ":" << cp.opt(i);
	if (! cp.optarg(i).isNull())
	  cout << " (" << cp.optarg(i).sval() << ")";
	cout << endl;
      }
      cout << endl << endl;
    }
  }
  catch(Event e) {
    cerr << e.where() << ": " << e.msg() << endl;
    exit(1);
  }
}
