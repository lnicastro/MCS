#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mcs_c.h>

static char* String_F2C(char* ss, int len)
{
    unsigned int i;
    char* p = malloc(len + 1);
    p[len] = 0;
    memcpy(p, ss, len);
    if (len > 0) {
      for (i=len-1; i>=0; i--) {
        if (p[i] != ' ')  break;
        else p[i] = 0;
      }
    }
    return p;
}


static int String_C2F(char* src, char*dst, int len)
{
    int i;

    for (i=0; i<len; i++ )
	       if (! src[i]) break;
        else dst[i] = src[i];

    for (; i<len; i++)
	       dst[i] = ' ';

    return 1;
}


//char* ifd_last_error();
void wf77_ifd_last_error_(char* RET, int* RET_len) {
  char* RETSTR =   ifd_last_error();
  String_C2F(RETSTR, RET, *RET_len);
}


//int ifd_got_error();
void wf77_ifd_got_error_(int* RET) {
  *RET =   ifd_got_error();
}


//void ifd_reset_error();
void wf77_ifd_reset_error_() {
  ifd_reset_error();
}


//void* ifd_null();
void wf77_ifd_null_(int** RET) {
  *RET =   ifd_null();
}


//void* new_Data(void* dummy, Types type, unsigned short int maxLength, short isunsigned);
void wf77_new_data_(int** dummy, int* type, int* maxLength, int* isunsigned, int** RET) {
  *RET =   new_Data(* ((void**) dummy), * ((Types*) type), * ((unsigned short int*) maxLength), * ((short*) isunsigned));
}


//void* copy_Data(void* obj);
void wf77_copy_data_(int** obj, int** RET) {
  *RET =   copy_Data(* ((void**) obj));
}


//void del_Data(void* obj);
void wf77_del_data_(int** obj) {
  del_Data(* ((void**) obj));
}


//char* Data_name(void* obj);
void wf77_data_name_(int** obj, char* RET, int* RET_len) {
  char* RETSTR =   Data_name(* ((void**) obj));
  if (! ifd_got_error())
    String_C2F(RETSTR, RET, *RET_len);
  else
    String_C2F("", RET, *RET_len);
}


//Types Data_type(void* obj);
void wf77_data_type_(int** obj, int* RET) {
  *RET =   Data_type(* ((void**) obj));
}


//unsigned short int Data_maxLength(void* obj);
void wf77_data_maxlength_(int** obj, int* RET) {
  *RET =   Data_maxLength(* ((void**) obj));
}


//unsigned short int Data_length(void* obj);
void wf77_data_length_(int** obj, int* RET) {
  *RET =   Data_length(* ((void**) obj));
}


//short Data_isUnsigned(void* obj);
void wf77_data_isunsigned_(int** obj, int* RET) {
  *RET =   Data_isUnsigned(* ((void**) obj));
}


//short Data_isNull(void* obj);
void wf77_data_isnull_(int** obj, int* RET) {
  *RET =   Data_isNull(* ((void**) obj));
}


//int Data_ival(void* obj);
void wf77_data_ival_(int** obj, int* RET) {
  *RET =   Data_ival(* ((void**) obj));
}


//unsigned int Data_uival(void* obj);
void wf77_data_uival_(int** obj, long long int* RET) {
  *RET =   Data_uival(* ((void**) obj));
}


//long long int Data_lval(void* obj);
void wf77_data_lval_(int** obj, long long int* RET) {
  *RET =   Data_lval(* ((void**) obj));
}


//unsigned long long int Data_ulval(void* obj);
void wf77_data_ulval_(int** obj, unsigned long long int* RET) {
  *RET =   Data_ulval(* ((void**) obj));
}


//float Data_fval(void* obj);
void wf77_data_fval_(int** obj, float* RET) {
  *RET =   Data_fval(* ((void**) obj));
}


//double Data_dval(void* obj);
void wf77_data_dval_(int** obj, double* RET) {
  *RET =   Data_dval(* ((void**) obj));
}


//char* Data_sval(void* obj);
void wf77_data_sval_(int** obj, char* RET, int* RET_len) {
  char* RETSTR =   Data_sval(* ((void**) obj));
  if (! ifd_got_error())
    String_C2F(RETSTR, RET, *RET_len);
  else
    String_C2F("", RET, *RET_len);
}


//unsigned int Data_tval(void* obj);
void wf77_data_tval_(int** obj, long long int* RET) {
  *RET =   Data_tval(* ((void**) obj));
}


//void Data_setNull(void* obj);
void wf77_data_setnull_(int** obj) {
  Data_setNull(* ((void**) obj));
}


//void Data_setival(void* obj, int val);
void wf77_data_setival_(int** obj, int* val) {
  Data_setival(* ((void**) obj), * ((int*) val));
}


//void Data_setuival(void* obj, unsigned int val);
void wf77_data_setuival_(int** obj, long long int* val) {
  Data_setuival(* ((void**) obj), * ((unsigned int*) val));
}


//void Data_setlval(void* obj, long long int val);
void wf77_data_setlval_(int** obj, long long int* val) {
  Data_setlval(* ((void**) obj), * ((long long int*) val));
}


//void Data_setulval(void* obj, unsigned long long int val);
void wf77_data_setulval_(int** obj, unsigned long long int* val) {
  Data_setulval(* ((void**) obj), * ((unsigned long long int*) val));
}


//void Data_setdval(void* obj, double val);
void wf77_data_setdval_(int** obj, double* val) {
  Data_setdval(* ((void**) obj), * ((double*) val));
}


//void Data_setsval(void* obj, char* val);
void wf77_data_setsval_(int** obj, char* val, int* val_flen) {
  char* C_val = String_F2C(val, *val_flen);
  Data_setsval(* ((void**) obj), C_val);
  free(C_val);
}


//void Data_setblob(void* obj, void* lbuf, unsigned int size);
void wf77_data_setblob_(int** obj, int** lbuf, long long int* size) {
  Data_setblob(* ((void**) obj), * ((void**) lbuf), * ((unsigned int*) size));
}


//void Data_settimeval(void* obj, unsigned int val);
void wf77_data_settimeval_(int** obj, long long int* val) {
  Data_settimeval(* ((void**) obj), * ((unsigned int*) val));
}


//char* Data_print(void* obj);
void wf77_data_print_(int** obj, char* RET, int* RET_len) {
  char* RETSTR =   Data_print(* ((void**) obj));
  if (! ifd_got_error())
    String_C2F(RETSTR, RET, *RET_len);
  else
    String_C2F("", RET, *RET_len);
}


//void Data_setTag(void* obj, short tag);
void wf77_data_settag_(int** obj, int* tag) {
  Data_setTag(* ((void**) obj), * ((short*) tag));
}


//short Data_getTag(void* obj);
void wf77_data_gettag_(int** obj, int* RET) {
  *RET =   Data_getTag(* ((void**) obj));
}


//void* new_Record(void* dummy);
void wf77_new_record_(int** dummy, int** RET) {
  *RET =   new_Record(* ((void**) dummy));
}


//void del_Record(void* obj);
void wf77_del_record_(int** obj) {
  del_Record(* ((void**) obj));
}


//void Record_clear(void* obj);
void wf77_record_clear_(int** obj) {
  Record_clear(* ((void**) obj));
}


//unsigned int Record_count(void* obj);
void wf77_record_count_(int** obj, long long int* RET) {
  *RET =   Record_count(* ((void**) obj));
}


//void Record_addField(void* obj, void* d);
void wf77_record_addfield_(int** obj, int** d) {
  Record_addField(* ((void**) obj), * ((void**) d));
}


//void* Record_pop(void* obj, int x);
void wf77_record_pop_(int** obj, int* x, int** RET) {
  *RET =   Record_pop(* ((void**) obj), * ((int*) x));
}


//void* Record_field(void* obj, int x);
void wf77_record_field_(int** obj, int* x, int** RET) {
  *RET =   Record_field(* ((void**) obj), * ((int*) x));
}


//int Record_posWhoseNameIs(void* obj, char* name);
void wf77_record_poswhosenameis_(int** obj, char* name, int* name_flen, int* RET) {
  char* C_name = String_F2C(name, *name_flen);
  *RET =   Record_posWhoseNameIs(* ((void**) obj), C_name);
  free(C_name);
}


//void Record_setFieldMap(void* obj, char* s);
void wf77_record_setfieldmap_(int** obj, char* s, int* s_flen) {
  char* C_s = String_F2C(s, *s_flen);
  Record_setFieldMap(* ((void**) obj), C_s);
  free(C_s);
}


//short RecordSet_setFirst(void* obj);
void wf77_recordset_setfirst_(int** obj, int* RET) {
  *RET =   RecordSet_setFirst(* ((void**) obj));
}


//short RecordSet_setLast(void* obj);
void wf77_recordset_setlast_(int** obj, int* RET) {
  *RET =   RecordSet_setLast(* ((void**) obj));
}


//short RecordSet_setNext(void* obj);
void wf77_recordset_setnext_(int** obj, int* RET) {
  *RET =   RecordSet_setNext(* ((void**) obj));
}


//short RecordSet_setPrev(void* obj);
void wf77_recordset_setprev_(int** obj, int* RET) {
  *RET =   RecordSet_setPrev(* ((void**) obj));
}


//short RecordSet_setWhereS(void* obj, unsigned int field, char* where);
void wf77_recordset_setwheres_(int** obj, long long int* field, char* where, int* where_flen, int* RET) {
  char* C_where = String_F2C(where, *where_flen);
  *RET =   RecordSet_setWhereS(* ((void**) obj), * ((unsigned int*) field), C_where);
  free(C_where);
}


//short RecordSet_setWhereI(void* obj, unsigned int field, int where);
void wf77_recordset_setwherei_(int** obj, long long int* field, int* where, int* RET) {
  *RET =   RecordSet_setWhereI(* ((void**) obj), * ((unsigned int*) field), * ((int*) where));
}


//short RecordSet_setPos(void* obj, unsigned int newpos);
void wf77_recordset_setpos_(int** obj, long long int* newpos, int* RET) {
  *RET =   RecordSet_setPos(* ((void**) obj), * ((unsigned int*) newpos));
}


//void RecordSet_dump(void* obj, char* fn);
void wf77_recordset_dump_(int** obj, char* fn, int* fn_flen) {
  char* C_fn = String_F2C(fn, *fn_flen);
  RecordSet_dump(* ((void**) obj), C_fn);
  free(C_fn);
}


//short RecordSet_know_nRows(void* obj);
void wf77_recordset_know_nrows_(int** obj, int* RET) {
  *RET =   RecordSet_know_nRows(* ((void**) obj));
}


//short RecordSet_eof(void* obj);
void wf77_recordset_eof_(int** obj, int* RET) {
  *RET =   RecordSet_eof(* ((void**) obj));
}


//short RecordSet_alwaysSameStructure(void* obj);
void wf77_recordset_alwayssamestructure_(int** obj, int* RET) {
  *RET =   RecordSet_alwaysSameStructure(* ((void**) obj));
}


//unsigned int RecordSet_nRows(void* obj);
void wf77_recordset_nrows_(int** obj, long long int* RET) {
  *RET =   RecordSet_nRows(* ((void**) obj));
}


//unsigned int RecordSet_nFields(void* obj);
void wf77_recordset_nfields_(int** obj, long long int* RET) {
  *RET =   RecordSet_nFields(* ((void**) obj));
}


//unsigned int RecordSet_pos(void* obj);
void wf77_recordset_pos_(int** obj, long long int* RET) {
  *RET =   RecordSet_pos(* ((void**) obj));
}


//void* RecordSet_rec(void* obj);
void wf77_recordset_rec_(int** obj, int** RET) {
  *RET =   RecordSet_rec(* ((void**) obj));
}


//void* RecordSet_metarec(void* obj);
void wf77_recordset_metarec_(int** obj, int** RET) {
  *RET =   RecordSet_metarec(* ((void**) obj));
}


//void* new_DBConn(void* dummy);
void wf77_new_dbconn_(int** dummy, int** RET) {
  *RET =   new_DBConn(* ((void**) dummy));
}


//void del_DBConn(void* obj);
void wf77_del_dbconn_(int** obj) {
  del_DBConn(* ((void**) obj));
}


//void DBConn_connect(void* obj, char* user, char* pass, char* db, char* host);
void wf77_dbconn_connect_(int** obj, char* user, int* user_flen, char* pass, int* pass_flen, char* db, int* db_flen, char* host, int* host_flen) {
  char* C_user = String_F2C(user, *user_flen);
  char* C_pass = String_F2C(pass, *pass_flen);
  char* C_db = String_F2C(db, *db_flen);
  char* C_host = String_F2C(host, *host_flen);
  DBConn_connect(* ((void**) obj), C_user, C_pass, C_db, C_host);
  free(C_user);
  free(C_pass);
  free(C_db);
  free(C_host);
}


//void DBConn_close(void* obj);
void wf77_dbconn_close_(int** obj) {
  DBConn_close(* ((void**) obj));
}


//short DBConn_isOpen(void* obj);
void wf77_dbconn_isopen_(int** obj, int* RET) {
  *RET =   DBConn_isOpen(* ((void**) obj));
}


//void* DBConn_newDBConn(void* obj);
void wf77_dbconn_newdbconn_(int** obj, int** RET) {
  *RET =   DBConn_newDBConn(* ((void**) obj));
}


//void* new_Query(void* dummy, void* lconn, short handleNewDBConn);
void wf77_new_query_(int** dummy, int** lconn, int* handleNewDBConn, int** RET) {
  *RET =   new_Query(* ((void**) dummy), * ((void**) lconn), * ((short*) handleNewDBConn));
}


//void del_Query(void* obj);
void wf77_del_query_(int** obj) {
  del_Query(* ((void**) obj));
}


//void Query_query(void* obj, char* query, short StoreResult);
void wf77_query_query_(int** obj, char* query, int* query_flen, int* StoreResult) {
  char* C_query = String_F2C(query, *query_flen);
  Query_query(* ((void**) obj), C_query, * ((short*) StoreResult));
  free(C_query);
}


//void Query_prepare(void* obj, char* query);
void wf77_query_prepare_(int** obj, char* query, int* query_flen) {
  char* C_query = String_F2C(query, *query_flen);
  Query_prepare(* ((void**) obj), C_query);
  free(C_query);
}


//void Query_prepare_with_parameters(void* obj, int op, char* fields, char* table, char* where);
void wf77_query_prepare_with_parameters_(int** obj, int* op, char* fields, int* fields_flen, char* table, int* table_flen, char* where, int* where_flen) {
  char* C_fields = String_F2C(fields, *fields_flen);
  char* C_table = String_F2C(table, *table_flen);
  char* C_where = String_F2C(where, *where_flen);
  Query_prepare_with_parameters(* ((void**) obj), * ((int*) op), C_fields, C_table, C_where);
  free(C_fields);
  free(C_table);
  free(C_where);
}


//unsigned int Query_nAffectedRows(void* obj);
void wf77_query_naffectedrows_(int** obj, long long int* RET) {
  *RET =   Query_nAffectedRows(* ((void**) obj));
}


//void* Query_param(void* obj);
void wf77_query_param_(int** obj, int** RET) {
  *RET =   Query_param(* ((void**) obj));
}


//void* Query_lookup(void* obj, char* field, char* table, char* where);
void wf77_query_lookup_(int** obj, char* field, int* field_flen, char* table, int* table_flen, char* where, int* where_flen, int** RET) {
  char* C_field = String_F2C(field, *field_flen);
  char* C_table = String_F2C(table, *table_flen);
  char* C_where = String_F2C(where, *where_flen);
  *RET =   Query_lookup(* ((void**) obj), C_field, C_table, C_where);
  free(C_field);
  free(C_table);
  free(C_where);
}


//short Query_gotRecordSet(void* obj);
void wf77_query_gotrecordset_(int** obj, int* RET) {
  *RET =   Query_gotRecordSet(* ((void**) obj));
}


//void Query_execute(void* obj, short StoreResult);
void wf77_query_execute_(int** obj, int* StoreResult) {
  Query_execute(* ((void**) obj), * ((short*) StoreResult));
}


//void Query_close(void* obj);
void wf77_query_close_(int** obj) {
  Query_close(* ((void**) obj));
}


//void* new_Table(void* dummy, void* lconn, char* table, char* fieldkey);
void wf77_new_table_(int** dummy, int** lconn, char* table, int* table_flen, char* fieldkey, int* fieldkey_flen, int** RET) {
  char* C_table = String_F2C(table, *table_flen);
  char* C_fieldkey = String_F2C(fieldkey, *fieldkey_flen);
  *RET =   new_Table(* ((void**) dummy), * ((void**) lconn), C_table, C_fieldkey);
  free(C_table);
  free(C_fieldkey);
}


//void del_Table(void* obj);
void wf77_del_table_(int** obj) {
  del_Table(* ((void**) obj));
}


//void Table_loadTable(void* obj);
void wf77_table_loadtable_(int** obj) {
  Table_loadTable(* ((void**) obj));
}


//void* Table_newRec(void* obj);
void wf77_table_newrec_(int** obj, int** RET) {
  *RET =   Table_newRec(* ((void**) obj));
}


//void Table_insert(void* obj);
void wf77_table_insert_(int** obj) {
  Table_insert(* ((void**) obj));
}


//void Table_update(void* obj);
void wf77_table_update_(int** obj) {
  Table_update(* ((void**) obj));
}


//void* new_Client(void* dummy, char* path, char* server, int port, int ssl, unsigned int timeout);
void wf77_new_client_(int** dummy, char* path, int* path_flen, char* server, int* server_flen, int* port, int* ssl, long long int* timeout, int** RET) {
  char* C_path = String_F2C(path, *path_flen);
  char* C_server = String_F2C(server, *server_flen);
  *RET =   new_Client(* ((void**) dummy), C_path, C_server, * ((int*) port), * ((int*) ssl), * ((unsigned int*) timeout));
  free(C_path);
  free(C_server);
}


//void del_Client(void* obj);
void wf77_del_client_(int** obj) {
  del_Client(* ((void**) obj));
}


//short Client_login(void* obj, char* user, char* pass, char* db);
void wf77_client_login_(int** obj, char* user, int* user_flen, char* pass, int* pass_flen, char* db, int* db_flen, int* RET) {
  char* C_user = String_F2C(user, *user_flen);
  char* C_pass = String_F2C(pass, *pass_flen);
  char* C_db = String_F2C(db, *db_flen);
  *RET =   Client_login(* ((void**) obj), C_user, C_pass, C_db);
  free(C_user);
  free(C_pass);
  free(C_db);
}


//short Client_exec(void* obj, char* cmd, void* data);
void wf77_client_exec_(int** obj, char* cmd, int* cmd_flen, int** data, int* RET) {
  char* C_cmd = String_F2C(cmd, *cmd_flen);
  *RET =   Client_exec(* ((void**) obj), C_cmd, * ((void**) data));
  free(C_cmd);
}


//void* Client_code(void* obj);
void wf77_client_code_(int** obj, int** RET) {
  *RET =   Client_code(* ((void**) obj));
}


//void* Client_msg(void* obj);
void wf77_client_msg_(int** obj, int** RET) {
  *RET =   Client_msg(* ((void**) obj));
}


//void* Client_out(void* obj);
void wf77_client_out_(int** obj, int** RET) {
  *RET =   Client_out(* ((void**) obj));
}


//void* Client_recv(void* obj);
void wf77_client_recv_(int** obj, int** RET) {
  *RET =   Client_recv(* ((void**) obj));
}


//void* Client_aux(void* obj);
void wf77_client_aux_(int** obj, int** RET) {
  *RET =   Client_aux(* ((void**) obj));
}


//void* new_Conf(void* dummy, char* filename);
void wf77_new_conf_(int** dummy, char* filename, int* filename_flen, int** RET) {
  char* C_filename = String_F2C(filename, *filename_flen);
  *RET =   new_Conf(* ((void**) dummy), C_filename);
  free(C_filename);
}


//void del_Conf(void* obj);
void wf77_del_conf_(int** obj) {
  del_Conf(* ((void**) obj));
}


//char* Conf_sval(void* obj, char* section, char* key);
void wf77_conf_sval_(int** obj, char* section, int* section_flen, char* key, int* key_flen, char* RET, int* RET_len) {
  char* C_section = String_F2C(section, *section_flen);
  char* C_key = String_F2C(key, *key_flen);
  char* RETSTR =   Conf_sval(* ((void**) obj), C_section, C_key);
  free(C_section);
  free(C_key);
  if (! ifd_got_error())
    String_C2F(RETSTR, RET, *RET_len);
  else
    String_C2F("", RET, *RET_len);
}


//int Conf_ival(void* obj, char* section, char* key);
void wf77_conf_ival_(int** obj, char* section, int* section_flen, char* key, int* key_flen, int* RET) {
  char* C_section = String_F2C(section, *section_flen);
  char* C_key = String_F2C(key, *key_flen);
  *RET =   Conf_ival(* ((void**) obj), C_section, C_key);
  free(C_section);
  free(C_key);
}


