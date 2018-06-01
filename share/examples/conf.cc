#include <fstream>
#include <mcs.hh>
using namespace mcs;

/*
  This example shows how to use the Conf class to handle configuration
  files. Note that section and key names cannot contains blanks.
*/

int main(int argc, char *argv[]) {

  try {
    Conf conf;

    //Insert some section/key pairs
    conf.setval("Section_A", "Key1", "Val1 ciao", "Comment1");
    conf.setval("Section_A", "Key2", 123   , "Comment2");
    conf.setval("Section_B", "Key1", "Val2", "Comment3");
    conf.setval("Section_B", "Key2", 321   , "Comment4");

    //Save the file in its current state
    conf.save("test.conf");

    //Reopen the same file
    conf.open("test.conf");

    //Print some values
    cout << conf.sval("Section_A", "Key1") << endl;
    cout << conf.ival("Section_B", "Key2") << endl;

    //Search for a section/key pair
    bool found = conf.search("Section_C", "key");
    cout << found << endl;
  }
  catch(Event e) {
    cerr << e.where() << ": " << e.msg() << endl;
    exit(1);
  }
}
