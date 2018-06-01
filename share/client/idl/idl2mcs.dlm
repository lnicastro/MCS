MODULE idl2mcs
DESCRIPTION Interface to the MCS library
VERSION 0.3.3-alpha7
SOURCE Giorgio Calderone (gcalderone@ifc.inaf.it)
FUNCTION IFD_LAST_ERROR 0 0
FUNCTION IFD_GOT_ERROR 0 0
PROCEDURE IFD_RESET_ERROR 0 0
FUNCTION IFD_NULL 0 0
FUNCTION TYPES_TINY 0 0
FUNCTION TYPES_SMALL 0 0
FUNCTION TYPES_MEDIUM 0 0
FUNCTION TYPES_INT 0 0
FUNCTION TYPES_BIGINT 0 0
FUNCTION TYPES_FLOAT 0 0
FUNCTION TYPES_DOUBLE 0 0
FUNCTION TYPES_STRING 0 0
FUNCTION TYPES_TIME 0 0
FUNCTION TYPES_TINY_BLOB 0 0
FUNCTION TYPES_BLOB 0 0
FUNCTION TYPES_POINTER 0 0
FUNCTION NEW_DATA 4 4
FUNCTION COPY_DATA 1 1
PROCEDURE DEL_DATA 1 1
FUNCTION DATA_NAME 1 1
FUNCTION DATA_TYPE 1 1
FUNCTION DATA_MAXLENGTH 1 1
FUNCTION DATA_LENGTH 1 1
FUNCTION DATA_ISUNSIGNED 1 1
FUNCTION DATA_ISNULL 1 1
FUNCTION DATA_IVAL 1 1
FUNCTION DATA_UIVAL 1 1
FUNCTION DATA_LVAL 1 1
FUNCTION DATA_ULVAL 1 1
FUNCTION DATA_FVAL 1 1
FUNCTION DATA_DVAL 1 1
FUNCTION DATA_SVAL 1 1
FUNCTION DATA_TVAL 1 1
PROCEDURE DATA_SETNULL 1 1
PROCEDURE DATA_SETIVAL 2 2
PROCEDURE DATA_SETUIVAL 2 2
PROCEDURE DATA_SETLVAL 2 2
PROCEDURE DATA_SETULVAL 2 2
PROCEDURE DATA_SETDVAL 2 2
PROCEDURE DATA_SETSVAL 2 2
PROCEDURE DATA_SETBLOB 3 3
PROCEDURE DATA_SETTIMEVAL 2 2
FUNCTION DATA_PRINT 1 1
PROCEDURE DATA_SETTAG 2 2
FUNCTION DATA_GETTAG 1 1
FUNCTION NEW_RECORD 1 1
PROCEDURE DEL_RECORD 1 1
PROCEDURE RECORD_CLEAR 1 1
FUNCTION RECORD_COUNT 1 1
PROCEDURE RECORD_ADDFIELD 2 2
FUNCTION RECORD_POP 2 2
FUNCTION RECORD_FIELD 2 2
FUNCTION RECORD_POSWHOSENAMEIS 2 2
PROCEDURE RECORD_SETFIELDMAP 2 2
FUNCTION RECORDSET_SETFIRST 1 1
FUNCTION RECORDSET_SETLAST 1 1
FUNCTION RECORDSET_SETNEXT 1 1
FUNCTION RECORDSET_SETPREV 1 1
FUNCTION RECORDSET_SETWHERES 3 3
FUNCTION RECORDSET_SETWHEREI 3 3
FUNCTION RECORDSET_SETPOS 2 2
PROCEDURE RECORDSET_DUMP 2 2
FUNCTION RECORDSET_KNOW_NROWS 1 1
FUNCTION RECORDSET_EOF 1 1
FUNCTION RECORDSET_ALWAYSSAMESTRUCTURE 1 1
FUNCTION RECORDSET_NROWS 1 1
FUNCTION RECORDSET_NFIELDS 1 1
FUNCTION RECORDSET_POS 1 1
FUNCTION RECORDSET_REC 1 1
FUNCTION RECORDSET_METAREC 1 1
FUNCTION NEW_DBCONN 1 1
PROCEDURE DEL_DBCONN 1 1
PROCEDURE DBCONN_CONNECT 5 5
PROCEDURE DBCONN_CLOSE 1 1
FUNCTION DBCONN_ISOPEN 1 1
FUNCTION DBCONN_NEWDBCONN 1 1
FUNCTION NEW_QUERY 3 3
PROCEDURE DEL_QUERY 1 1
PROCEDURE QUERY_QUERY 3 3
PROCEDURE QUERY_PREPARE 2 2
PROCEDURE QUERY_PREPARE_WITH_PARAMETERS 5 5
FUNCTION QUERY_NAFFECTEDROWS 1 1
FUNCTION QUERY_PARAM 1 1
FUNCTION QUERY_LOOKUP 4 4
FUNCTION QUERY_GOTRECORDSET 1 1
PROCEDURE QUERY_EXECUTE 2 2
PROCEDURE QUERY_CLOSE 1 1
FUNCTION NEW_TABLE 4 4
PROCEDURE DEL_TABLE 1 1
PROCEDURE TABLE_LOADTABLE 1 1
FUNCTION TABLE_NEWREC 1 1
PROCEDURE TABLE_INSERT 1 1
PROCEDURE TABLE_UPDATE 1 1
FUNCTION NEW_CLIENT 6 6
PROCEDURE DEL_CLIENT 1 1
FUNCTION CLIENT_LOGIN 4 4
FUNCTION CLIENT_EXEC 3 3
FUNCTION CLIENT_CODE 1 1
FUNCTION CLIENT_MSG 1 1
FUNCTION CLIENT_OUT 1 1
FUNCTION CLIENT_RECV 1 1
FUNCTION CLIENT_AUX 1 1
FUNCTION NEW_CONF 2 2
PROCEDURE DEL_CONF 1 1
FUNCTION CONF_SVAL 3 3
FUNCTION CONF_IVAL 3 3
