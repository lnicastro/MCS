#include <mcs.hh>
using namespace mcs;

int main() {
  //Connect to the database server
  DBConn db;
  db.connect("generic", "password", "RossImgs","localhost");

  //Execute a query through the opened connection
  Query qry(&db);
  qry.prepare("SELECT * FROM PubObslog WHERE date_obs>'2011-01-31' AND date_obs<'2011-02-01'");
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
}
