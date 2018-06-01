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
#include <mcs.hh>
using namespace mcs;

/* Trim leading and trailing quote (') and spaces */
std::string trimfs( std::string s ) {
  string ss = s;
  ss.erase( 0, ss.find_first_not_of( " '\t\n" ) );
  ss.erase( ss.find_last_not_of( " '\t\n" ) + 1);
  return ss;
}



int main(int argc, char* argv[])
{
  FITSReader fits;
  int i, j;

  const int n_hdr_kwds=26;

  struct kwd_fld {
    string name;
    short type;
    string fldname;
   } kwd [n_hdr_kwds];

   string kwd_name[n_hdr_kwds] = {"CAT-RA","CAT-DEC","OBJECT","USERID","OBSTYPE","OBSID","PROPID",
      "FILTER1","INSTRUME","DATE-OBS","LST",  //string
      "CRVAL1","CRVAL2","AZIMUTH","ALTITUDE","MJD",  //DBL
      "EXPTIME","SEEING","AIRMASS","BACKGRD","STDDEV","MOONDIST","MOONFRAC",  //FLT
      "GRPUID","CONFIGID",  //INT
      "GRPNUMOB"  //SHORT
    };

    short kwd_type[n_hdr_kwds] = {
      STRING,STRING,STRING,STRING,STRING,STRING,STRING,STRING,STRING,STRING,STRING,
      DOUBLE,DOUBLE,DOUBLE,DOUBLE,DOUBLE,
      FLOAT,FLOAT,FLOAT,FLOAT,FLOAT,FLOAT,FLOAT,
      INT,INT,
      SMALL
    };

    string kwd_fldname[n_hdr_kwds] = {
      "ras","decs","object","pi_coi","obstype","obsID","propID",
      "filter","instrume","date_obs","lst",
      "radeg","decdeg","azdeg","eldeg","mjdobs",
      "exptime","seeing","airmass","dettotbg","detbgdev","moondist","moonphas",
      "grpuID","configID",
      "grpnumob"
    };

  for (i=0; i<n_hdr_kwds; i++) {
    kwd[i].name = kwd_name[i];
    kwd[i].type = kwd_type[i];
    kwd[i].fldname = kwd_fldname[i];

  }

  Record row;

// These fields are managed by table triggers
  //Data datein(NULL, TIME, (char *) "datein");
  //datein = time(NULL);
  //Data htmID_6(NULL, SMALL, (char *) "htmID_6", 0, true);
  //htmID_6 = 0;
  //row.addField(datein);
  //row.addField(htmID_6);

  string auto_fields = "serial datein htmID_6 ";

  int ik = 0;

  try {
    for (i=1; i<argc; i++) {
      cout  << endl  << endl << "Reading: " << argv[i] << endl;

      fits.open(argv[i]);

//Print header
    for (i=0; i<fits.header.count(); i++) {
	  cout <<
	    fits.header[i].name() << " = " <<
	    fits.header[i].sval() << " / " <<
	    fits.header_comments[i].sval() << endl;

        for (j=0; j<n_hdr_kwds; j++) {
          if (fits.header[i].name().compare(kwd[j].name) == 0) {
cout<<"Found "<< kwd[j].name<< endl;

            switch (kwd[j].type) {  // ignore types: all STRING
              case STRING:
              case DOUBLE:
              case FLOAT:
              case INT:
              case SMALL:
                row.addField(trimfs(fits.header[i].sval()));
// Standard Date format replacing the T with blank
                if ( (kwd[j].fldname.compare("date_obs") == 0) &&
                     (fits.header[i].sval().length() > 10) ) {
                  size_t tp = fits.header[i].sval().find("T");
                  if (tp > 0) {
                    string s = row[ik].sval(); 
cout << "tp="<<tp <<" "<< s << endl;
                    s.replace(tp,1," ");
cout << "tp="<<tp <<" "<< s << endl;
row[ik] = s;
                  }
                }

//cout << "row="<< row[ik].sval() <<endl;
                break;

              default:
                break;
            }

            row[ik].setName(kwd[j].fldname);
cout<<ik<<" "<<row[ik].name()<<" "<<row[ik].sval() << "\n";
            ik++;
            break;
          }
        }

      }
      cout <<"Transferred "<< ik <<" kwds"<< endl;
      cout <<"BITPIX="<< fits.header["BITPIX"].sval() << endl;
	
//Print field names
      cout << row.asStringNames() << endl;

//Print field types
      cout << row.asStringTypes() << endl;

//Print data
      for (i=0; i<row.count(); i++)
        cout << i <<" "<< row[i].name() <<" = "<< row[i].sval()<< endl;

        fits.close();
      }

// Here the data can be inserted into the DB table
/*
      string plist = auto_fields;

      db.connect(DB_USER, DB_PASS, DB_NAME, DB_HOST);
      query = new Query(&db, false);

      for (i=0; i<n_hdr_kwds; i++)
        plist += kwd[i].fldname + bl;
      for (i=n_hdr_kwds; i<n_hdr_kwds+n_extra_flds-1; i++)
        plist += row[i].name() + bl;
      plist += row[i].name();

      query->prepare_with_parameters(MCS_PAB_INSERT, plist, tabname);
      query->param()[0] = 0;
      query->param()[1] = 0;
      query->param()[2] = 0;

      for (i=0; i<n_hdr_kwds; i++){
        query->param()[kwd[i].fldname] = row[kwd[i].fldname].sval();
      }

      for (i=n_hdr_kwds; i<n_hdr_kwds+n_extra_flds; i++) {
        string fldn = row[i].name();
        query->param()[fldn] = row[fldn].sval();
      }
      query->execute();

      query->close();
      delete query;
      db.close();

*/

  }
  catch(Event& e) {
    cerr << e.msg() << endl;
  }

}
