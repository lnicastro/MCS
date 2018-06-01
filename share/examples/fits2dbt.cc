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

/*!
  \file fits2dbt.cc

  \brief Example of FITS binary table insertion into a DB.

  Usage:
  \code
  fits2dbt [-H] file1.fits ... fileN.fits
  -h: print this help
  -H: create additional DB table with header content
  -l: just list header content (no DB table is written)
  -e: select which extension (HDU starting from 1) you want to import (def. last one)
  -q: do not show info about header(s) and column types
  -S: insert one record at a time (very low performance)
  -v: be verbose
  -d DBnane: output database is 'DBnane' (def. DEF_DBNA)
  -e HDU: select which extension (HDU starting from 1) you want to import (def. last one)
  -p Password: MySQL user password is 'Password' (def. DEF_PASS) 
  -s Server: send query to DB server 'Server' (def. DEF_HOST)
  -t Table: output table name is 'Table' (def. filename_HDUn)
  -u User: MySQL user name is 'User' (def. DEF_USER)
  \endcode

  \details See the FITS reader class in Readers.cc

  \note
  Only create table for the selected or the last HDU (could select multiple)!
*/

#include <mcs.hh>
using namespace mcs;

#define DEF_HOST "localhost"
#define DEF_USER "root"
#define DEF_PASS "R0zz02"
#define DEF_DBNA "test"
//#define DEF_TBL  "mcstest"

static string VERID="Ver 0.1d, 03-07-2017, LN@INAF-IASF";

// Global variables
char my_host[32]=DEF_HOST, my_user[32]=DEF_USER, my_pass[32]=DEF_PASS,
	my_db[32]=DEF_DBNA;



/* Trim leading and trailing quote (') and spaces */
std::string trimfs( std::string s ) {
	string ss = s;
	ss.erase( 0, ss.find_first_not_of( " '\t\n" ) );
	ss.erase( ss.find_last_not_of( " '\t\n" ) + 1);
	return ss;
}

/* The file name without the dir. path and the extension (e.g. ".fits") */
std::string fclean( std::string fullname, std::string fext ) {
	string fname;
	size_t sl = fullname.rfind("/");
	if (sl != string::npos)
		fname = fullname.substr(sl+1);
	else
		fname = fullname;

	sl = fname.find(fext);

	//if (sl != string::npos)
	//return fname.erase(sl);
	//else
	//return fname;

	if (sl != string::npos)
		fname.erase(sl);

	// Additionally replace spaces and + or - signes with _
	//fname.find(" +-")
	while (1) {
		sl = fname.find("+", 0);
		if (sl != string::npos)
			fname.replace(sl, 1, "_");
		else break;
	}
	while (1) {
		sl = fname.find("-", 0);
		if (sl != string::npos)
			fname.replace(sl, 1, "_");
		else break;
	}
	while (1) {
		sl = fname.find(" ", 0);
		if (sl != string::npos)
			fname.replace(sl, 1, "_");
		else break;
	}
	//fname.replace(fname.find(" ", 0), 1, "_");

	return fname;
}

void
usage(char *name) {
	cout << name <<"  "<<  VERID << "\n\n"
		 <<"Usage: "<< name <<" [Options] file1.fits ... fileN.fits\n"
		 <<"  Options:\n"
		 <<"  -h: print this help\n"
		 <<"  -D: print data for the selected HDU (no DB table is written)\n"
		 <<"  -H: create additional DB table with header content\n"
		 <<"  -l: just list header content (no DB table is written)\n"
		 <<"  -q: do not show info about header(s) and column types\n"
		 <<"  -S: insert one record at a time (very low performance)\n"
		 <<"  -v: be verbose\n"
		 <<"  -d DBnane: output database is 'DBnane' (def. "<< DEF_DBNA <<")\n"
		 <<"  -e HDU: select which extension (HDU starting from 1) you want to import (def. last one)\n"
		 <<"  -p Password: MySQL user password is 'Password' (def. "<< DEF_PASS <<")\n"
		 <<"  -s Server: send query to DB server 'Server' (def. "<< DEF_HOST <<")\n"
		 <<"  -t Table: output table is 'Table' (def. filename_HDUn)\n"
		 <<"  -u User: MySQL user name is 'User' (def. "<< DEF_USER <<")\n\n"

		 <<"Note: if no HDU is selected, then only create table for the last HDU.\n\n";
	exit(0);
}


int main(int argc, char* argv[])
{
	FITSReader fits;
	int i, j, hdu, nflds=0;
	bool kwds=false, hdr_tab=false, list_only=false, be_quite=false, verbose=false,
		tab_given=false, tab_written=false, print_data=false, Slow=false;
	unsigned short hdu_sel=0;
	string tabname, tabname_h;

	char c;


	DBConn db;
	Query* qry;

	if (argc == 1) usage(argv[0]);

	/* Keywords section */
	while (--argc > 0 && (*++argv)[0] == '-')
		{
			kwds=1;
			while (kwds && (c = *++argv[0]))
				{
					switch (c)
						{
						case 'h':
							usage(argv[0]);
							break;
						case 'D':
							print_data = true;
							break;
						case 'H':
							hdr_tab = true;
							break;
						case 'l':
							list_only = true;
							break;
						case 'q':
							be_quite = true;
							break;
						case 'S':
							Slow = true;
							break;
						case 'v':
							verbose = true;
							break;
						case 'd':
							if (argc < 2) usage(argv[0]);
							sscanf(*++argv,"%s",my_db);
							--argc;
							kwds=0;
							break;
						case 'e':
							if (argc < 2) usage(argv[0]);
							sscanf(*++argv,"%hu", &hdu_sel);
							--argc;
							kwds=0;
							break;
						case 'p':
							if (argc < 2) usage(argv[0]);
							sscanf(*++argv,"%s",my_pass);
							--argc;
							kwds=0;
							break;
						case 's':
							if (argc < 2) usage(argv[0]);
							sscanf(*++argv,"%s",my_host);
							--argc;
							kwds=0;
							break;
						case 't':
							if (argc < 2) usage(argv[0]);
							tabname = string(*++argv);
							tab_given = true;
							--argc;
							kwds=0;
							break;
						case 'u':
							if (argc < 2) usage(argv[0]);
							sscanf(*++argv,"%s",my_user);
							--argc;
							kwds=0;
							break;
						default:
							cerr << argv[0] << ": Illegal option `" << c << "'.\n\n";
							usage(argv[0]);
						}
				}
		}

	try {
		// Connect to the DB
		db.connect(my_user, my_pass, my_db, my_host);
		qry = new Query(&db);

		// Loop on input list of files
		for (i=0; i<argc; i++) {
			if (! be_quite) cout  << endl << "Reading: " << argv[i] << endl;

			fits.open(argv[i]);

			for (hdu=1; hdu<=fits.HDUCount(); hdu++) {
				fits.selectHDU(hdu);
				if (! be_quite) cout << endl << "HDU #" << fits.currentHDU() << endl;

				//Print header
				if (verbose) for (j=0; j<fits.header.count(); j++) {
						cout <<
							fits.header[j].name() << " = " <<
							fits.header[j].sval() << " / " <<
							fits.header_comments[j].sval() << endl;
					}

				nflds = fits.metarec().count();
				if (! be_quite) cout << "There are "<< nflds << " fields.\n";
				//Print field names
				if (verbose)
					cout << fits.metarec().asStringNames() << endl;

				//Print field types
				if (verbose)
					cout << fits.metarec().asStringTypes() << endl;


				if (! list_only && (hdu == hdu_sel || hdu == fits.HDUCount())) {

					//Print data only
					if (print_data) {
						while (! fits.eof()) {
							cout << fits.rec().asString() << endl;
							fits.setNext();
						}
						break;
					}

					// Only create table for the last HDU (can be changed)!

					long int nrows = fits.header["NAXIS2"].ival();
					if (! be_quite)
						cout <<"There are "<< nrows <<" rows\n";

					string kname, ktype, flds;
					if (!tab_given)
						tabname = fclean(string(argv[i]), ".") +"_HDU"+ itos(hdu);
					//if (verbose) cout <<"Writing to table: "<< my_db <<"."<< tabname <<"\n";
					cout <<"Writing to table: "<< my_db <<"."<< tabname <<"\n";
					Types type;

					// Header table if requested

					if (hdr_tab) {
						tabname_h = tabname +"_hdr";
						if (verbose) cout <<"Keywords table name: "<< tabname_h <<"\n";
						qry->query("CREATE TABLE IF NOT EXISTS `"+ tabname_h +"` (keyword CHAR(8), value VARCHAR(72), comment VARCHAR(72))");

						qry->prepare_with_parameters(MCS_PAB_INSERT, "*", tabname_h);

						//Store header
						for (j=0; j<fits.header.count(); j++) {
							qry->param()[0] = fits.header[j].name();
							qry->param()[1] = fits.header[j].sval();
							qry->param()[2] = fits.header_comments[j].sval();

							qry->execute();
						}

					}

					kname = fits.metarec().field(0).name();
					flds = "(`"+ kname +"` ";

					if (fits.metarec().field(0).type() == STRING)
						ktype = "VARCHAR("+ itos(fits.metarec().field(0).length()) +")";
					else {
						type = fits.metarec().field(0).type();
						ktype = Types2MYSQLStr(type, fits.metarec().field(0).isUnsigned());
					}

					flds += ktype;

					for (j=1; j<fits.metarec().count(); j++) {
						kname = fits.metarec().field(j).name();
						flds += ", `"+ kname +"` ";
						//cout <<j<<" "<<kname<<" "<<ktype<<"**"<< endl;
						if (fits.metarec().field(j).type() == STRING)
							ktype = "VARCHAR("+ itos(fits.metarec().field(j).length()) +")";
						else {
							type = fits.metarec().field(j).type();
							ktype = Types2MYSQLStr(type, fits.metarec().field(j).isUnsigned());
						}
						flds += ktype;
					}

					flds += ")";

					// Create DB table
					qry->query("CREATE TABLE IF NOT EXISTS `"+ tabname +"` "+ flds);

					long int nCache = 65000 / fits.metarec().count();
					if (nCache < 1) { nCache = 1; }
					if (Slow) { nCache = 1; }
					cout << "Using NCACHE=" << nCache << endl;
					qry->prepare_with_parameters(MCS_PAB_INSERT, "*", tabname, "", nCache);

					// Insert the data
					while (! fits.eof()) {
						long int toBeWritten = nrows - fits.pos();
						if (toBeWritten < nCache) {
							nCache = toBeWritten;
							if (Slow) { nCache = 1; }
							cout << "Using NCACHE=" << nCache << endl;
							qry->prepare_with_parameters(MCS_PAB_INSERT, "*", tabname, "", nCache);
						}

						for (long int iCache=0; iCache < nCache; iCache++) {
							//cout << fits.rec().asString() << endl;
							for (j=0; j<nflds; j++) {
								//if (j==20){
								//cout<<j<<" "<<fits.metarec().field(j).name()<<" "<<fits.rec()[j].sval() <<endl;
								//cout<<"isNull: "<<fits.rec()[j].isNull()<<endl;
								////cout<<fits.rec()[j].print()<<endl;
								//}
								if (fits.rec()[j].isNull())
									//qry->param()[j] = '\0';
									qry->param()[j + iCache*nflds].setNull();
								else
									qry->param()[j + iCache*nflds] = fits.rec()[j].sval();
							}
							fits.setNext();
						}
						qry->execute();
					}

					delete qry;
					db.close();
					tab_written = true;

					break;  // stop reading any further HDU
				}  // end if selected hdu
			}  // end for each HDU

			fits.close();
		}  // end for each input file
	}
	catch(Event& e) {
		cerr << e.msg() << endl;
	}

	if (tab_written && ! be_quite) {
		cout <<"Table "<< tabname <<" written into DB "<< my_db <<"\n";
		if (hdr_tab)
			cout <<"together with header keywords table "<< tabname_h <<"\n";
	}
}
