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

#include "mcs.hh"
using namespace mcs;

#if ENABLE_MYSQL


//--------------------------------------------------------------------
mcs::DBConn::DBConn()
{
  MCS_DEBUG_SETUP(0, "DBConn");

  lconnInitialized = false;
  lconnOpened = false;
}


mcs::DBConn::~DBConn()
{
  close();
}

void mcs::DBConn::connect(string user, string pass, string db, string host)
{
  MYSQL* ret;

  close();

  luser = user;
  lpass = pass;
  ldb   = db  ;
  lhost = host;

  lconn = mysql_init(lconn);
  if (! lconn)
      throw MCS_ERROR(MSG_CANT_ALLOCATE_MYSQL);

  lconnInitialized = true;
  mysql_options(lconn, MYSQL_OPT_CONNECT_TIMEOUT, "5");

  ret = mysql_real_connect(lconn, lhost.c_str(), luser.c_str(), lpass.c_str(),
			   ldb.c_str(), 0, MYSQL_SOCK, 0);
  if (ret)
      lconnOpened = true;
  else
      throw MCS_ERROR(MSG_CANT_OPEN_MYSQL_CONNECTION, mysql_error(lconn));
}


void mcs::DBConn::close()
{
  if (lconnInitialized) {

    //To prevent SIGSEGV in mysql_close()
    //unsigned int n, f;
    //MYSQL_RES* res = mysql_list_processes(lconn);
    //vector<string> v = Query::printResultSet(res, n, f);
    //mysql_free_result(res);
    //------------------------------------

    mysql_close(lconn);
    lconnInitialized = false;
    lconnOpened = false;
  }
  lconn = NULL;
}



bool mcs::DBConn::isOpen()
{
  return lconnOpened;
}


unsigned long mcs::DBConn::id()
{
  unsigned long lid = mysql_thread_id(lconn);
  return lid;
}

DBConn* mcs::DBConn::newDBConn()
{
  DBConn* dbc = NULL;
  try {
    dbc = new DBConn();
    dbc->connect(luser, lpass, ldb, lhost);

    if (mysql_errno(dbc->lconn))
      throw MCS_ERROR(MSG_CANT_OPEN_MYSQL_CONNECTION, mysql_error(dbc->lconn));
    return dbc;
  }
  catch (Event& e) {
    if (dbc) delete dbc;
    throw;
  }
}












//--------------------------------------------------------------------
mcs::Query::Query(DBConn* conn, bool handleNewDBConn) : RecordSet()
{
  MCS_DEBUG_SETUP(0, "Query");

  lhandleNewDBConn = handleNewDBConn;
  if (lhandleNewDBConn) {
    try {
      ldbc = conn->newDBConn();
    }
    catch (Event& e) {
      delete this;
      throw;
    }
  }
  else
    ldbc = conn;

  lconn = ldbc->lconn;  //Friendness of Query to DBConn class
  ldbc->id();

  gotStmtInitialized = false;
  gotResult = false;

  nlparam = 0;
  lbparam  = NULL;
  lbrec = NULL;

  laffectedRows = 0;

  readTableList();
}


mcs::Query::~Query()
{
  close();
  if (lhandleNewDBConn) delete ldbc;
}





//Implementation of RecorSet's virtual methods
Record* mcs::Query::newRecord()
{
  return myrec;
}


bool mcs::Query::fetch(unsigned int newpos, bool random)
{
  if (random)
    mysql_stmt_data_seek(lstmt, newpos);

  int ret = mysql_stmt_fetch(lstmt);

  if (ret == 1)
    throw MCS_ERROR(MSG_MYSQL_ERROR, mysql_stmt_error(lstmt));

  return ! (ret == MYSQL_NO_DATA);
}





unsigned int mcs::Query::nAffectedRows()
{
  return laffectedRows;
}


Record& mcs::Query::param()
{
    return lparam;
}


bool mcs::Query::gotRecordSet()
{
  return gotResult;
}






void mcs::Query::prepare(string SQL)
{
  string s;
  int i, n, numfields;
  MYSQL_RES* res = NULL;
  MYSQL_FIELD* fields;
  Types type;

  string s2 = SQL;

  this->SQL = s2;

  if (SQL.empty())
    throw MCS_ERROR(MSG_EMPTY_STRING);

  close();

  lstmt = mysql_stmt_init(lconn);
  if (!lstmt)
    throw MCS_ERROR(MSG_OUT_OF_MEMORY);
  gotStmtInitialized = true;

  if (mysql_stmt_prepare(lstmt, SQL.c_str(), SQL.length()))
    throw MCS_ERROR(MSG_MYSQL_ERROR, mysql_stmt_error(lstmt));


  //Allocate binding structures for parameters
  nlparam = mysql_stmt_param_count(lstmt);
  if (nlparam > 0) {
    lbparam = (MYSQL_BIND*) malloc(sizeof(MYSQL_BIND) * nlparam);

    /*  ****PARAM_METADATA****
	res = mysql_stmt_param_metadata(lstmt);
	
	if (res) {
	
	}
    */

  }


  //Allocate binding structures for records in the result
  n = mysql_stmt_field_count(lstmt);
  if (n > 0) {
    res = mysql_stmt_result_metadata(lstmt);
    if (res) {

      gotResult = true;
      lbrec = (MYSQL_BIND*) malloc(sizeof(MYSQL_BIND) * n);

      //Will be destroyed by the underlying RecordSet object
      myrec = new Record(false);

      fields    = mysql_fetch_fields(res);
      numfields = mysql_num_fields(res);
	
      for (i=0; i<numfields; i++) {
	if (! MYSQL2Types(fields[i].type, type))
	  throw MCS_ERROR(MSG_TYPE_NOT_HANDLED, i, fields[i].type);
	
	Data* d = new Data(&(lbrec[i]),
			   type,
			   fields[i].name,
			   (VarLenType(type) ? fields[i].length : 1),
			   ((bool) (fields[i].flags & UNSIGNED_FLAG)),
			   fields[i].flags
			   );
	
	myrec->addField(d);
      }
      mysql_free_result(res);

      RecordSet::init(MCS_RS_USEMETAREC | MCS_RS_RANDOM, 0, myrec);
    }
  }
}



void mcs::Query::bind()
{
  if (mysql_stmt_bind_param(lstmt, lbparam))
      throw MCS_ERROR(MSG_BIND_FAILED, mysql_stmt_error(lstmt));
}





Data& mcs::Query::lookup(string field, string table, string where)
{
  SQL = string("SELECT ") + field + string(" FROM ") + table;

  if (! where.empty())
    SQL += string(" WHERE ") + where ;

  SQL += string(" LIMIT 1");

  query(SQL, true);

  if (nRows() != 1)
    throw MCS_ERROR(MSG_NO_RESULT);

  return rec()[0];
}





void mcs::Query::execute(bool StoreResult)
{
  string s;

  if (! lstmt)
    throw MCS_ERROR(MSG_STMT_NOT_YET_PREPARED);

  if (mysql_stmt_execute(lstmt))
    throw MCS_ERROR(MSG_MYSQL_ERROR, mysql_stmt_error(lstmt));

  if (gotResult) {
    if (mysql_stmt_bind_result(lstmt, lbrec))
      throw MCS_ERROR(MSG_BIND_FAILED, mysql_stmt_error(lstmt));

    if (StoreResult) {
      if (mysql_stmt_store_result(lstmt))
	throw MCS_ERROR(MSG_MYSQL_ERROR, mysql_stmt_error(lstmt));

      RecordSet::init(MCS_RS_USEMETAREC | MCS_RS_RANDOM | MCS_RS_KNOW_NROWS,
		      mysql_stmt_num_rows(lstmt), myrec);
    }
    else
      RecordSet::init(MCS_RS_USEMETAREC | MCS_RS_RANDOM,
		      0, myrec);

    myrec->emptyName();
    startFetch();
  }
  else {
    lastid = mysql_insert_id(lconn);
    laffectedRows = mysql_stmt_affected_rows(lstmt);
  }
}


void mcs::Query::query(string SQL, bool StoreResult)
{
  string scmp;

  SQL = trim(SQL);
  scmp = SQL.substr(0, 6);

  if ((strcasecmp(scmp.c_str(), "SELECT") == 0)   ||
      (strcasecmp(scmp.c_str(), "INSERT") == 0)   ||
      (strcasecmp(scmp.c_str(), "REPLAC") == 0)   ||
      (strcasecmp(scmp.c_str(), "UPDATE") == 0)   ||
      (strcasecmp(scmp.c_str(), "DELETE") == 0)
    ) {
    prepare(SQL);
    execute(StoreResult);
  }
  else {
    simpleQuery(SQL);
  }
}




void mcs::Query::close()
{
  if (lbparam) {
    free(lbparam) ;	
    nlparam = 0;
    lbparam  = NULL;
    lparam.clear();
  }


  if (gotResult) {
    mysql_stmt_free_result(lstmt);
    free(lbrec);
    lbrec = NULL;
    gotResult = false;
    RecordSet::init(0);
  }


  if (gotStmtInitialized) {
    mysql_stmt_close(lstmt);
    lstmt = NULL;
    gotStmtInitialized = false;
  }

  laffectedRows = 0;
}



void mcs::Query::readTableList()
{
  MYSQL_RES* res;
  MYSQL_ROW record;
  unsigned int ntbl, i;

  tableList.clear();

  res = mysql_list_tables(lconn, "%");
  if (res) {
    ntbl = mysql_num_rows(res);
    for (i=0; i<ntbl; i++) {
      record = mysql_fetch_row(res);
      tableList.push_back(record[0]);
    }

    mysql_free_result(res);
  }
}


vector<string> mcs::Query::tableInfo(string tbl)
{
  unsigned int i, j;
  string s = "SHOW COLUMNS FROM " + tbl;
  try {
    return simpleQuery(s, i, j);
  }
  catch (...) {
    throw MCS_ERROR(MSG_CANT_GET_INFO_TABLE, tbl.csz);
  }
}


vector<string> mcs::Query::printResultSet(unsigned int& nrows,
					  unsigned int& nfields,
					  MYSQL_RES* res)
{
  static string tab  = "\t";
  static string null = "NULL";
  string s;
  unsigned int i, j, f;
  vector<string> v;

  if (res) {  //Result coming from simpleQuery
    MYSQL_FIELD* fields;
    MYSQL_ROW record;

    nrows = 0;
    if (res) {
      nrows  = mysql_num_rows(res);
      f      = mysql_num_fields(res);  nfields = f;
      fields = mysql_fetch_fields(res);
      s = "#";
      for (j=0; j<f; j++)
	s += fields[j].name + tab;
      v.push_back(s);

      for (i=0; i<nrows; i++) {
	record = mysql_fetch_row(res);
	s = "";
	for (j=0; j<f; j++)
	  if (record[j])
	    s+=string(record[j]) + tab;
	  else
	    s+=null + tab;
	
	v.push_back(s);
      }
    }
  }


  else { //Result coming from prepare, execute
    if (! gotResult)
      throw MCS_ERROR(MSG_NO_RESULT);

    nfields = metarec().count();

    s = "#";
    for (i=0; i<nfields; i++)
      s += metarec()[i].name() + tab;
    v.push_back(s);

    setFirst();
    do {
      s = "";
      for (int i=0; i<rec().count(); i++)
	if (rec()[i].isNull())
	  s += null + tab;
	else
	  s += rec()[i].sval() + tab;

      v.push_back(s);
    }
    while (setNext());

    nrows = v.size();
  }

  return v;

}


vector<string> mcs::Query::simpleQuery(string SQL)
{
  unsigned int i, j;
  return simpleQuery(SQL, i, j);
}


vector<string> mcs::Query::simpleQuery(string SQL,
				       unsigned int& nrows,
				       unsigned int& nfields)
{
  MYSQL_RES* res;
  vector<string> v;

  this->SQL = SQL;
  close();
  if (mysql_query(lconn, SQL.c_str()))
      throw MCS_ERROR(MSG_MYSQL_ERROR, mysql_error(lconn), mysql_errno(lconn));

  res = mysql_store_result(lconn);
  if (res) {
    v = printResultSet(nrows, nfields, res);
//MYSQL_ROW record;
//mysql_data_seek(res, 0);
//record = mysql_fetch_row(res);
//v.push_back(record[0]);

    mysql_free_result(res);
  }
  return v;
}


//#if ENABLE_CFITSIO
//void mcs::Query::Result2Fits(string fn, string HDUName, bool compressed)
//{
//  fitsfile *fptr;
//  bool fitsfileOpened = false;
//  bool memAllocated   = false;
//  int i;
//  int status = 0;
//  int  j, fitstype;
//  string s;
//
//  char** ttype = NULL;
//  char** tform = NULL;
//  char*  buf   = NULL;
//  char* p1;
//  char* p2;
//
//  try {
//    if (! gotResult)
//      throw MCS_ERROR(MSG_NO_RESULT);
//
//    if (rec().count() == 0)
//      throw MCS_ERROR(MSG_NO_FIELDS);
//
//    if (HDUName.empty())
//      HDUName = "QUERY";
//
//    fn = "!" + fn;
//    if (compressed) fn += ".gz";
//
//    fits_create_file(&fptr, fn.c_str(), &status);
//    CHECK_FITS_ERROR;
//
//    fitsfileOpened = true;
//
//#define FIELD_NAME_LEN 50
//#define FIELD_TYPE_LEN 10
//    ttype = (char**) malloc(sizeof(char*) * rec().count());
//    tform = (char**) malloc(sizeof(char*) * rec().count());
//    buf = (char*)   malloc((FIELD_NAME_LEN + FIELD_TYPE_LEN + 2) *
//			   rec().count());
//    memset(buf, 0, (FIELD_NAME_LEN + FIELD_TYPE_LEN + 2) * rec().count());
//
//    p1 = buf;
//    p2 = buf + FIELD_NAME_LEN * rec().count();
//    memAllocated = true;
//
//    for (i=0; i<rec().count(); i++) {
//      ttype[i] = p1;
//      p1 = stpcpy(p1, rec()[i].name().c_str());
//      p1++;
//
//      if (! Types2S_FITS(rec()[i].type(), rec()[i].maxLength(),
//		     rec()[i].isUnsigned(), s))
//	  throw MCS_ERROR(MSG_TYPE_NOT_HANDLED, i, rec()[i].type());
//
//      tform[i] = p2;
//      p2 = stpcpy(p2, s.c_str());
//      p2++;
//    }
//
//    fits_create_tbl(fptr, BINARY_TBL, 0, rec().count(),
//		    ttype, tform,
//		    NULL, (char*) HDUName.c_str(), &status);
//    CHECK_FITS_ERROR;
//
//    i = 0;
//    while (! eof()) {
//      unsigned char lluc;
//      short int llsi;
//      int lli;
//      long long int llli;
//      char* llp;
//      char* aux;
//
//      for (j=0; j<rec().count(); j++) {
//	Types2FITS(rec()[j].type(), rec()[j].isUnsigned(), fitstype);
//	
//	switch (fitstype) {
//	case TBYTE:
//	  lluc = (unsigned char) rec()[j].lval();
//	  llp = (char*) &lluc;     break;
//	case TSHORT:
//	  llsi = (short int) rec()[j].lval();
//	  llp = (char*) &llsi;     break;
//	case TLONG:
//	  lli = (int) rec()[j].lval();
//	  llp = (char*) &lli;      break;
//	case TLONGLONG:
//	  llli= rec()[j].lval();
//	  llp = (char*) &llli;      break;	
//	case TSTRING:
//	  aux = (char*) rec()[j].buffer();
//	  llp = (char*) &aux;
//	  break;
//	default:
//	  llp = (char*) rec()[j].buffer();
//	}
//	
//	fits_write_col(fptr, fitstype, j+1, i+1, 1, 1, llp, &status);
//	CHECK_FITS_ERROR;
//      }
//      setNext();
//      i++;
//    }
//
//    free(ttype);
//    free(tform);
//    free(buf);
//    memAllocated = false;
//    fits_close_file(fptr, &status);
//    CHECK_FITS_ERROR;
//
//    fitsfileOpened = false;
//  }
//  catch (Event& e) {
//    if (memAllocated) {
//      free(ttype);
//      free(tform);
//      free(buf);
//    }
//    if (fitsfileOpened) fits_close_file(fptr, &status);
//  }
//}
//#endif


vector<string> mcs::Query::ExecutionDetails(string pre)
{
  vector<string> buf;
  int i;
  string nl = "\n";

  buf.push_back( pre + "Prepared query: " + SQL );
  buf.push_back( pre + "Number of parameters: " + itos(param().count()) );

  for (i=0; i<param().count(); i++)
    buf.push_back( "Param #" + itos(i) + string(" value: ") + lparam[i].sval() );

  if (gotResult) {
    buf.push_back( pre + "SELECT-LIKE query: " + SQL );
    buf.push_back( pre + "Number of records: " + itos(nRows())   );
    buf.push_back( pre + "Number of fields:  " + itos(nFields()) );
    buf.push_back( pre + "Fields detail follows:" );

    for (i=0; i<nFields(); i++) {
      buf.push_back( pre + "Field #" + itos(i) + ":" );
      buf.push_back( pre + "  Name      : " + metarec()[i].name() );
      buf.push_back( pre + "  Type      : " + Types2Str(rec()[i].type(), false) );
      buf.push_back( pre + "  Unsigned  : " + btos(rec()[i].isUnsigned()) );
      buf.push_back( pre + "  Max length: " + itos(rec()[i].maxLength()) );
      buf.push_back( pre + "  Length    : " + itos(rec()[i].length()) );
    }
  }
  else {
    buf.push_back( pre + "INSERT-LIKE query" );
    buf.push_back( pre + "Number of affected rows: " + itos(nAffectedRows())   );
  }

  return buf;
}



void mcs::Query::customFillBuffer(char* buf, unsigned int& chunksize, bool firstTime)
{
  int i;
  string det = "";
  string data, s;
  string null = "NULL";
  string tab  = "\t";
  string nl   = "\n";

  MCS_DEBUG(chunksize << " " << firstTime);

  if (firstTime) {
      if (! gotResult)
	  throw MCS_ERROR(MSG_NO_RESULT);

      det = vtos(ExecutionDetails());
      if (det.length() > chunksize)	
	  throw MCS_ERROR(MSG_NOT_ENOUGH_SPACE, det.length(), chunksize);

      memcpy(buf, det.c_str(), det.length());
      chunksize = det.length();
      return;
  }

  else {
      if (eof()) {
	  chunksize = 0;
	  return;
      }

      data = "";
      while (! eof()) {
	  s = "";
	  for (i=0; i<nFields(); i++) {
	      if (rec()[i].isNull())
		  s += null;
	      else
		  s += rec()[i].sval();
	      s += tab;
	  }
	  s += nl;
	
	  if (data.length() + s.length() < chunksize) {
	      data += s;
	      setNext();
	  }
	  else
	      break;
      }

      memcpy(buf, data.c_str(), data.length());
      chunksize = data.length();
      return;
  }
}



void mcs::Query::Result2Ascii(string fn)
{
  unsigned int chunk = MCS_DEFAULTCHUNKSIZE;
  ofstream of(fn.c_str());
  bool firstTime = true;

  if (! of.is_open())
      throw MCS_ERROR(MSG_CANT_OPEN_FILE, fn.csz);

  char* p = (char*) malloc(chunk);
  while (chunk) {
    customFillBuffer(p, chunk, firstTime);
    of.write(p, chunk);
    firstTime = false;
  }
  free(p);

  of.close();
}



void mcs::Query::prepare_with_parameters(int op, char** fields, int nfields,
										 string table, string where, int nrec)
{
  int i;
  string s = "";
  for (i=0; i<nfields; i++) {
    if (i>0) s += " " ;
    s += fields[i];
  }
  prepare_with_parameters(op, s, table, where, nrec);
}


void mcs::Query::parseFieldList(int op, string& fields, string& values,
				int& autoIncr)
{
  int i;
  bool first = true;
  autoIncr = -1;

  fields = "";
  values = "";

  for (i=0; i<nFields(); i++) {
    if (metarec()[i].isAutoIncrement())
      autoIncr = i;

    if (! first) {
      fields += ", ";
      values += ", ";
    }
    first = false;

    fields += "`"+ metarec()[i].name() +"`";
    values += "?";

    if (op == MCS_PAB_UPDATE)
      fields += " = ?";
  }
}



void mcs::Query::prepare_with_parameters(int op, string fields, string table, string where, int nrec)
{
  int i, autoIncr;
  string sql, comma_fields, marks = "";
  string tab_quoted = table;
  Record tmprec(false);

  if (tab_quoted.find("`") == std::string::npos)
    tab_quoted = "`"+ table +"`";

  fields = trim(fields);

  comma_fields = "*";
  if (fields != "*")
    comma_fields = subst(fields, " +", ", ");

  sql = "SELECT " + comma_fields + " FROM " + tab_quoted;
  if (! where.empty())
    sql += " WHERE " + where;
  sql += " LIMIT 1";

  query(sql);


  if (eof())
    tmprec = metarec();
  else
    tmprec = rec();

  parseFieldList(op, fields, marks, autoIncr);

  switch(op) {
  case MCS_PAB_INSERT:
	  sql = "INSERT INTO " + tab_quoted + "(" + fields + ") VALUES ";
	  marks = "(" + marks + ")";
	  for (int irec=0; irec<nrec; irec++) {
		  sql += marks;
		  if (irec == 0) {marks = ", " + marks;}
	  }
	  break;
  case MCS_PAB_REPLACE:
    sql = "REPLACE INTO " + tab_quoted + "(" + fields + ") VALUES (" + marks + ")";
    break;
  case MCS_PAB_UPDATE:
    sql = "UPDATE " + tab_quoted + " SET " + fields + " WHERE " + where;
    break;
  default:
    throw MCS_ERROR(MSG_UNEXPECTED);
  }

  prepare(sql);

  for (int irec=0; irec<nrec; irec++) {
	  for (i=0; i<tmprec.count(); i++) {
		  Data* d = new Data(&(lbparam[i + irec*tmprec.count()]),
							 tmprec[i].type(),
							 (char*) tmprec[i].name().c_str(),
							 tmprec[i].maxLength(),
							 tmprec[i].isUnsigned());
		  
		  lparam.addField(d);
	  
		  memcpy(lparam[i].buffer(), tmprec[i].buffer(), lparam[i].maxLength());
	  }
  }
  bind();
}



long long int mcs::Query::last_id()
{
  return lastid;
}










//--------------------------------------------------------
mcs::Table::Table(DBConn* db, string table, string fieldkey) :
  Query(db, true), newrec(false)
{
  MCS_DEBUG_SETUP(0, "Table");

  ltable = trim(table);
  lfieldkey = trim(fieldkey);

  loadTable();
}


mcs::Table::~Table()
{}


void mcs::Table::loadTable()
{
  query("SELECT * FROM " + ltable, true);

  //Check for lfieldkey
  posfieldkey = metarec().posWhoseNameIs(lfieldkey);

  newrec = rec();
  newrec.setNull();
}


Record& mcs::Table::newRec()
{
  return newrec;
}



void mcs::Table::insert()
{
  insert_or_update(MCS_PAB_INSERT);
}



void mcs::Table::update()
{
  insert_or_update(MCS_PAB_UPDATE);
}


void mcs::Table::replace()
{
  insert_or_update(MCS_PAB_REPLACE);
}


void mcs::Table::insert_or_update(int op)
{
  string where = "";
  int i;

  //Dynamic_Array<int> origmap(false);
  //origmap = rec().getFieldMap();
  //rec().setFieldMap();

  if (op == MCS_PAB_UPDATE) { //Data are written in current record
    newrec = rec();
    where = lfieldkey + " = " + rec()[posfieldkey].sval();
  }

  prepare_with_parameters(op, "*", ltable, where);

  //Copy values from newrec in the parameters
  for (i=0; i<param().count(); i++) {
    if ( param()[i].isAutoIncrement()   ||
         newrec[i].isNull()
         )
      param()[i].setNull();
    else
      memcpy(param()[i].buffer(), newrec[i].buffer(), param()[i].maxLength());
  }

  execute();
  loadTable();

  //rec().setFieldMap(origmap);
}



#endif
