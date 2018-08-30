



struct ifd_status {
    int error;
    char errormsg[200];
    void* data;
};

typedef struct ifd_status IFD_STATUS;
function char* ifd_last_error ();
function int ifd_got_error ();
function void ifd_reset_error ();
function void* ifd_null ();




enum Types {
  TINY ,
  SMALL ,
  MEDIUM ,
  INT ,
  BIGINT ,
  FLOAT ,
  DOUBLE ,
  STRING ,
  TIME ,
  TINY_BLOB ,
  BLOB ,
  POINTER
};

typedef enum Types Types;









function void* new_Data (void* dummy ,Types type ,unsigned short int maxLength ,short isunsigned)
                                                                                           ;


function void* copy_Data (void* obj);


function void del_Data (void* obj);


function char* Data_name (void* obj );


function Types Data_type (void* obj );
function unsigned short int Data_maxLength (void* obj );
function unsigned short int Data_length (void* obj );
function short Data_isUnsigned (void* obj );
function short Data_isNull (void* obj );
function int Data_ival (void* obj );
function unsigned int Data_uival (void* obj );
function long long int Data_lval (void* obj );
function unsigned long long int Data_ulval (void* obj );
function float Data_fval (void* obj );
function double Data_dval (void* obj );

function char* Data_sval (void* obj );

function unsigned int Data_tval (void* obj );




function void Data_setNull (void* obj );

function void Data_setival (void* obj ,int val);
function void Data_setuival (void* obj ,unsigned int val);

function void Data_setlval (void* obj ,long long int val);
function void Data_setulval (void* obj ,unsigned long long int val);

function void Data_setdval (void* obj ,double val);

function void Data_setsval (void* obj ,char* val);

function void Data_setblob (void* obj ,void* lbuf ,unsigned int size)
                                               ;




function void Data_settimeval (void* obj ,unsigned int val);




function char* Data_print (void* obj );

function void Data_setTag (void* obj ,short tag);

function short Data_getTag (void* obj );
function void* new_Record (void* dummy );

function void del_Record (void* obj);

function void Record_clear (void* obj );
function unsigned int Record_count (void* obj );

function void Record_addField (void* obj ,void* d)
                    ;

function void* Record_pop (void* obj ,int x);

function void* Record_field (void* obj ,int x);

function int Record_posWhoseNameIs (void* obj ,char* name);

function void Record_setFieldMap (void* obj ,char* s);






function short RecordSet_setFirst (void* obj );
function short RecordSet_setLast (void* obj );
function short RecordSet_setNext (void* obj );
function short RecordSet_setPrev (void* obj );
function short RecordSet_setWhereS (void* obj ,unsigned int field ,char* where)
                                               ;

function short RecordSet_setWhereI (void* obj ,unsigned int field ,int where)
                                             ;

function short RecordSet_setPos (void* obj ,unsigned int newpos);

function void RecordSet_dump (void* obj ,char* fn);

function short RecordSet_know_nRows (void* obj );
function short RecordSet_eof (void* obj );
function short RecordSet_alwaysSameStructure (void* obj );

function unsigned int RecordSet_nRows (void* obj );
function unsigned int RecordSet_nFields (void* obj );
function unsigned int RecordSet_pos (void* obj );
function void* RecordSet_rec (void* obj );
function void* RecordSet_metarec (void* obj );
function void* new_DBConn (void* dummy );

function void del_DBConn (void* obj);

function void DBConn_connect (void* obj ,char* user ,char* pass ,char* db ,char* host)


                                      ;


function void DBConn_close (void* obj );
function short DBConn_isOpen (void* obj );
function void* DBConn_newDBConn (void* obj );





function void* new_Query (void* dummy ,void* lconn ,short handleNewDBConn)
                                                    ;


function void del_Query (void* obj);


function void Query_query (void* obj ,char* query ,short StoreResult)
                                                   ;

function void Query_prepare (void* obj ,char* query)
                          ;

function void Query_prepare_with_parameters (void* obj ,int op ,char* fields ,char* table ,char* where)


                                             ;


function unsigned int Query_nAffectedRows (void* obj );

function void* Query_param (void* obj );

function void* Query_lookup (void* obj ,char* field ,char* table ,char* where)
                                                           ;



function short Query_gotRecordSet (void* obj );

function void Query_execute (void* obj ,short StoreResult)
                             ;


function void Query_close (void* obj );






function void* new_Table (void* dummy ,void* lconn ,char* table ,char* fieldkey)
                                                                ;

function void del_Table (void* obj);

function void Table_loadTable (void* obj );

function void* Table_newRec (void* obj );

function void Table_insert (void* obj );
function void Table_update (void* obj );






function void* new_Client (void* dummy ,char* path ,char* server ,int port ,int ssl ,unsigned int timeout)

                                           ;

function void del_Client (void* obj);

function short Client_login (void* obj ,char* user ,char* pass ,char* db)
                                                      ;

function short Client_exec (void* obj ,char* cmd ,void* data)
                                        ;



function void* Client_code (void* obj );

function void* Client_msg (void* obj );

function void* Client_out (void* obj );

function void* Client_recv (void* obj );

function void* Client_aux (void* obj );







function void* new_Conf (void* dummy ,char* filename);

function void del_Conf (void* obj);


function char* Conf_sval (void* obj ,char* section ,char* key)

                                        ;


function int Conf_ival (void* obj ,char* section ,char* key)

                                        ;
