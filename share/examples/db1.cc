#include <mcs.hh>
using namespace mcs;

int main() {
  //Connect to the database server
  DBConn db;
  db.connect("mcstest", "mcstest", "test","localhost");

  //Execute a query through the opened connection
  Query qry(&db);
  qry.prepare("SELECT table_schema, table_name FROM INFORMATION_SCHEMA.TABLES");
  qry.execute();

  //Loop through the resulting record set
  while (! qry.eof()) {  //(\label{code:db1_line})

    //Get a reference to current record
    Record& rec = qry.rec();


    //For each field print its value
    for (int i=0; i<rec.count(); i++)
      cout << rec[i].sval() << "\t";
    cout << endl;

    //Move to next record
    qry.setNext();
  }

  //New query using query() and metarec() to print fields info
  qry.query("SELECT * FROM INFORMATION_SCHEMA.TABLES");

  int nfields = qry.metarec().count();
  string s = "\n# ";
  for (int i=0; i<nfields; i++)
    s += qry.metarec()[i].name() +" ("+
             Types2Str(qry.metarec()[i].type(), qry.metarec()[i].isUnsigned()) +") \t";

  cout << s << endl;
}
