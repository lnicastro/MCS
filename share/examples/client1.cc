#include <mcs.hh>
using namespace mcs;

int main(int argc, char *argv[]) {
  //Connect to the MCS server.
  Client cli("./", "localhost", 6523);

  //Perform authentication.
  cli.login("mcstest", "mcstest", "test");

  //Upload a file
  cli.exec("PUT myfile");

  //Download a file
  cli.exec("GET myfile");

  //Execute a query on remote database
  cli.exec("QRY SELECT * FROM mcstest");

  //Retrieve the result set.
  cli.exec("QRES");

  //Loop through the resulting record set
  while (! cli.eof()) {  //(\label{code:client1_line})

    //Get a reference to current record
    Record& rec = cli.rec();

    //For each field print its value
    for (int i=0; i<rec.count(); i++)
      cout << rec[i].sval() << "\t";
    cout << endl;

    //Move to next record
    cli.setNext();
  }
}
