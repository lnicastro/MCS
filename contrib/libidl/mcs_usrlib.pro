;+
; Library allowing simple MCS communication and DB tables data retrieval.
; See help for each routine. Note that the parameter "c_host" defines the
; hostname (def. 'localhost') and is defined in DBCONNECT.
;
; v0.3d: Work in progress...
;
; NOTES:
;   The default MySQL server hostname is 'localhost'. In DBCONNECT and
;   DBEXEC_QRY, DBEXEC_TABQRY, DBEXEC_SHOWQRY can use the keyword HOST to
;   change it.
;   The majority of the routines of this library use COMMON blocks to pass
;   information:
;
;     common DBptr, dbc, dbq
;     common DBinfo, c_host, c_user, c_password, c_dbname, c_query
;     common TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
;                     f_atype, f_itype, tab_stru
;
;   See routines help for returned output.
;
;   Typically users' main PRO do NOT need to include common blocks. It is left
;   to the user the choice to include "DBinfo" and "TABinfo" for convenience.
;
;   We use the word "Table" to identify the 2-d data returned by a SELECT query.
;   Otherwise we use DB table (or similar) to identify the database table on
;   which the query is performed.
;
; DEPENDENCIES:
;   The MCS package must be compiled with the IDL interface enabled
;   (--enable-idl or --with-idl) and installed.
;   See http://ross.iasfbo.inaf.it/mcs or https://github.com/gcalderone/MCS
;
;   MCS version required: 0.3.3-alpha2 or above.
;
; VERSION HISTORY:
;   0.2b: 17-Apr-2007
;   0.3a: 01-Aug-2008
;   0.3b: 23-Sep-2008
;   0.3c: 22-Mar-2009
;   0.3d: 01-Mar-2011
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;
;  18/07/2006: Default structure tag names are now "fN" where N is a sequencial
;              integer. This allows to have multiple (actual) columns with
;              the same name (e.g. in queries with multiple tables).
;              Can use (when querying a single table) the keyword "GET_FNAMES"
;              to get tag names = field names.
;  15/02/2007: Added c_host to the DBinfo structure.
;              Added HOST as optional parameter in dbconnect, dbexec_qry,
;              dbexec_tabqry and dbexec_showqry.
;              In dbexec_qry call dbconnect.
;  29/08/2008: Version 0.3a.
;              Major changes and more routines derived from Giorgio's personal
;              library. See routines header. In particular a SELECT query
;              will return by default an array of structures with tag names =
;              field names (it was "a structure of arrays" with tag names =
;              "fN", see above).
;  23/09/2008: Version 0.3b.
;              Changed DBQuery to manage all query types.
;              Changed structure with _null fields at the end.
;              Other minor changes.
;
;  22/03/2009: Version 0.3c.
;              Set is_rowstru in FieldInfo.

;  01/03/2011: Version 0.3d.
;              Manage TIMESTAMP field type (4 bytes "unsigned" type in
;              MCS/MySQL)
;  07/12/2014: Added missing "query = c_query" when USE_SAVED_INFO is set
;              in DBExec_ShowQry
;-


;---> Below, routines derived or copied from Giorgio's personal library


FUNCTION askPassword, prompt
;+
; NAME:
;       askPassword
;
; PURPOSE:
;   Ask a password on the IDL terminal hiding the characters being typed.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = askPassword('prompt')
;
; INPUT PARAMETERS:
;   PROMPT: A prompt to be printed on IDL terminal.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; RETURN VALUE:
;   The string typed without the final newline character.
;
; ERROR HANDLING:
;   None.
;
;
; MODIFICATION HISTORY:
;   Written by G. Calderone @ IFC-INAF, May 2008.
;-
  IF (N_PARAMS() GT 0) THEN $
    PRINT, FORMAT='($, %"%s")', prompt

  ret = ''
  c = 0
  WHILE (BYTE(c) NE 10) DO BEGIN
    c = GET_KBRD()
    ret = ret + c
    PRINT, FORMAT='($, %"*")'
  ENDWHILE
  PRINT, ''

;Remove last character: newline
  ret = STRMID(ret, 0, STRLEN(ret)-1)

  RETURN, ret
END


FUNCTION DBIsConnected
;+
; NAME:
;       DBISCONNECTED
;
; PURPOSE:
;   Check if a DB connection is already opened.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBIsConnected()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   True (1) if a connection is already opened, false (0) otherwise.
;
;
; MODIFICATION HISTORY:
;   Written by G. Calderone @ IFC-INAF, May 2008.
;-
COMMON DBptr, dbc, dbq

  IF (N_ELEMENTS(dbc) EQ 0) THEN RETURN, 0
  IF (dbc EQ ifd_null())    THEN RETURN, 0
  RETURN, 1

END


FUNCTION prepareStruct, rec
;+
; NAME:
;       prepareStruct
;
; NOTES:
;   This function is not supposed to be used by end-users.
;   Called by DBPrepare.
;
;
; MODIFICATION HISTORY:
;   Written by G. Calderone @ IFC-INAF, May 2008.
;  25/07/2008: Added unsigned integers full support and N-dim structure
;              management.
;  19/09/2008: LN, v0.3b: changed structure with _null fields at the end.
;  01/03/2011: LN, v0.3d: (unsigned) TIMESTAMP management.
;-
ON_ERROR, 2

  str = 'str = {'
  strnull = ''

  FOR i=0, Record_count(rec)-1 DO BEGIN
    data = Record_field(rec, i)
    name = Data_name(data)
    type = Data_type(data)
    is_uns = Data_isUnsigned(data)

; Manage unsigned
    IF is_uns THEN BEGIN
      CASE type OF
        Types_TINY(): type = '0U'
        Types_SMALL(): type = '0U'
        Types_MEDIUM(): type = '0UL'
        Types_INT(): type = '0UL'
        Types_BIGINT(): type = '0ULL'
        Types_TIME(): type = '""'
        ELSE: $
          MESSAGE, 'Unsupported MCS data type: ' + STRTRIM(type,1)
      ENDCASE
    ENDIF ELSE BEGIN

      CASE type OF
        Types_TINY(): type = '0'
        Types_SMALL(): type = '0'
        Types_MEDIUM(): type = '0L'
        Types_INT(): type = '0L'
        Types_BIGINT(): type = '0LL'
        Types_FLOAT(): type = '0.'
        Types_DOUBLE(): type = '0D'
        Types_STRING(): type = '""'
        Types_TIME(): type = '""'
        Types_TINY_BLOB(): type = '0B'
        Types_BLOB(): type = '0B'
        Types_POINTER(): type = '0L'
        ELSE: $
          MESSAGE, 'Unsupported MCS data type: ' + STRTRIM(type,1)
      ENDCASE
    ENDELSE


    IF (i GT 0) THEN strnull += ', '
    str += name + ':' + type + ', '
    strnull += name + '_null: 0b'
  ENDFOR

  str += strnull + '}'

  IF (KEYWORD_SET(debug)) THEN PRINT, str
  IF (NOT EXECUTE(str)) THEN $
    MESSAGE, 'Unexpected error'

  RETURN, str
END


FUNCTION DBPrepare, fields, table, $
                    INSERT=insert, UPDATE=update, REPLACE=replace, $
                    WHERE_IS=where_is, DEBUG=debug
;+
; NAME:
;       DBPrepare
;
; PURPOSE:
;   Create a prepared statement.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBPrepare('field1 field2 ...', 'table_name' $
;                      , /INSERT | /UPDATE | /REPLACE,   $
;                      , WHERE_IS='where clause')
;
; INPUT PARAMETERS:
;   FIELDS: list of fields (blank separated) to be modified by the prepared
;           statement;
;   TABLE:  table on which the prepared statement is to be executed.
;
; INPUT KEYWORDS:
;   INSERT:   use this keyword to create an INSERT prepared statement (default);
;   UPDATE:   use this keyword to create an UPDATE prepared statement;
;   REPLACE:  use this keyword to create a REPLACE prepared statement;
;   WHERE_IS: use this keyword to specify a WHERE clause that must be matched
;             by affected records (not used with /INSERT). This keyword must
;             not contains placeholders (?).
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   An anonymous structure whose field's name/type match the corresponding
;   name/type on DB side. This fields can be modified and used as in put
;   parameter to DBExecPar to execute the prepared statement.
;
; ERROR HANDLING:
;   Through a call to MESSAGE.
;
; NOTES:
;   The prepared statement acts on a single table or view at a time.
;
; SEE ALSO:
;   DBExecPar (to execute prepared statements).
;
;
; MODIFICATION HISTORY:
;   Written by G. Calderone @ IFC-INAF, May 2008.
;-
COMMON DBptr, dbc, dbq
ON_ERROR, 2

  IF (N_ELEMENTS(dbc) EQ 0) THEN BEGIN
    dbc = ifd_null()
    dbq = ifd_null()
  ENDIF

  IF (dbc EQ ifd_null()) THEN $
    MESSAGE, 'Connection is closed.'

  op = 1
  IF (KEYWORD_SET(update))  THEN op = 2
  IF (KEYWORD_SET(replace)) THEN op = 3

  IF (KEYWORD_SET(where_is)) THEN $
    Query_prepare_with_parameters, dbq, op, fields, table, where_is $
  ELSE $
    Query_prepare_with_parameters, dbq, op, fields, table, ''

;Check for errors
  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    MESSAGE, msg
  ENDIF

  RETURN, prepareStruct(Query_param(dbq))
END


FUNCTION DBExecPar, param, DEBUG=debug
;+
; NAME:
;       DBExecPar
;
; PURPOSE:
;   Executes a prepared statement.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBExecPar( par_struct )
;
; INPUT PARAMETERS:
;   PARAM: a structure (returned by DBPrepare) whose field's values will be
;          used for the execution of the prepared statement.
;          It can also be an array of structures or a structure whose
;          elements are arrays (one per field).
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; RETURN VALUE:
;   Number of affected records.
;
; ERROR HANDLING:
;   Through a call to MESSAGE.
;
; NOTES:
;   The prepared statement acts on a single table or view at a time.
;
; SEE ALSO:
;   DBPrepare (to create a prepared statements).
;
;
; MODIFICATION HISTORY:
;   Written by G. Calderone @ IFC-INAF, May 2008.
;  28/07/2008: LN, v0.3a: changed the PARAM structure and NULL value management.
;              Manages unsigned int/long.
;              Also allows array of structures as well as structure of arrays.
;  19/09/2008: LN, v0.3b: changed structure with _null fields at the end.
;-
COMMON DBptr, dbc, dbq
ON_ERROR, 2

  IF (N_ELEMENTS(dbc) EQ 0) THEN BEGIN
    dbc = ifd_null()
    dbq = ifd_null()
  ENDIF

  IF (dbc EQ ifd_null()) THEN $
    MESSAGE, 'Connection is closed.'

  accum = 0
  rec = Query_param(dbq)
  nf = Record_count(rec)

  IF (nf EQ 0) THEN $
    MESSAGE, 'No query has been prepared'

  names = TAG_NAMES(param)
  IF (N_ELEMENTS(names) NE nf*2) THEN $
    MESSAGE, 'Wrong parameter structure'

; Array of structures?
  is_arr_stru = N_ELEMENTS(param) GT 1
; Structure with arrays?
  is_stru_arr = N_ELEMENTS(param[0].(0)) GT 1

; Get only once IDL field types
  idl_types_id = INTARR(nf)

    FOR i=0, nf-1 DO BEGIN
        data = Record_field(rec, i)
; Check the XYZ_null field
        IF (param[0].(nf+i)[0]) THEN BEGIN
          Data_setNull, data
          CONTINUE
        ENDIF

        CASE SIZE(param[0].(i)[0], /TYPE) OF
            1: BEGIN  ; BYTE
               idl_types_id[i] = 1
               Data_setival, data, param[0].(i)[0]
               END
            2: BEGIN  ; INT
               idl_types_id[i] = 2
               Data_setival, data, param[0].(i)[0]
               END
            3: BEGIN  ; LONG
               idl_types_id[i] = 3
               Data_setival, data, param[0].(i)[0]
               END
           12: BEGIN  ; UINT
               idl_types_id[i] = 12
               Data_setuival, data, param[0].(i)[0]
               END
           13: BEGIN  ; ULONG
               idl_types_id[i] = 13
               Data_setuival, data, param[0].(i)[0]
               END
           14: BEGIN  ; LONG64
               idl_types_id[i] = 14
               Data_setlval, data, param[0].(i)[0]
               END
           15: BEGIN  ; ULONG64
               idl_types_id[i] = 15
               Data_setulval, data, param[0].(i)[0]
               END
            4: BEGIN  ; FLOAT
               idl_types_id[i] = 4
               Data_setdval, data, param[0].(i)[0]
               END
            5: BEGIN  ; DOUBLE
               idl_types_id[i] = 5
               Data_setdval, data, param[0].(i)[0]
               END
            7: BEGIN  ; STRING
               idl_types_id[i] = 7
               Data_setsval, data, param[0].(i)[0]
               END
            ELSE: $
              MESSAGE, 'Unsupported data type'
        ENDCASE

    ENDFOR

    Query_execute, dbq, 1

    ;Check for errors
    IF (ifd_got_error()) THEN BEGIN
      msg = ifd_last_error()
      ifd_reset_error
      del_Query, dbq
      MESSAGE, msg
    ENDIF

    accum += Query_nAffectedRows(dbq)

; After having processed the first record, go on with the rest
; disinguishing between array of structures and structure of arrays.
  IF (is_stru_arr) THEN BEGIN
    nrec = N_ELEMENTS(param[0].(0))

    FOR j=1, nrec-1 DO BEGIN
      FOR i=0, nf-1 DO BEGIN
        data = Record_field(rec, i)
        IF (param.(nf+i)[j]) THEN BEGIN
          Data_setNull, data
          CONTINUE
        ENDIF
        CASE idl_types_id[i] OF
            1: Data_setival, data, param.(i)[j]
            2: Data_setival, data, param.(i)[j]
            3: Data_setival, data, param.(i)[j]
           12: Data_setival, data, param.(i)[j]
           13: Data_setuival, data, param.(i)[j]
           14: Data_setlval, data, param.(i)[j]
           15: Data_setulval, data, param.(i)[j]
            4: Data_setdval, data, param.(i)[j]
            5: Data_setdval, data, param.(i)[j]
            7: Data_setsval, data, param.(i)[j]
        ENDCASE

      ENDFOR

      Query_execute, dbq, 1

      ;Check for errors
      IF (ifd_got_error()) THEN BEGIN
        msg = ifd_last_error()
        ifd_reset_error
        del_Query, dbq
        MESSAGE, msg
      ENDIF

      accum += Query_nAffectedRows(dbq)

    ENDFOR


  ENDIF ELSE IF (is_arr_stru) THEN BEGIN
    nrec = N_ELEMENTS(param)

    FOR j=1, nrec-1 DO BEGIN
      FOR i=0, nf-1 DO BEGIN
        data = Record_field(rec, i)
        IF (param[j].(nf+i)) THEN BEGIN
          Data_setNull, data
          CONTINUE
        ENDIF
        CASE idl_types_id[i] OF
            1: Data_setival, data, param[j].(i)
            2: Data_setival, data, param[j].(i)
            3: Data_setival, data, param[j].(i)
           12: Data_setival, data, param[j].(i)
           13: Data_setuival, data, param[j].(i)
           14: Data_setlval, data, param[j].(i)
           15: Data_setulval, data, param[j].(i)
            4: Data_setdval, data, param[j].(i)
            5: Data_setdval, data, param[j].(i)
            7: Data_setsval, data, param[j].(i)
        ENDCASE

      ENDFOR

      Query_execute, dbq, 1

      ;Check for errors
      IF (ifd_got_error()) THEN BEGIN
        msg = ifd_last_error()
        ifd_reset_error
        del_Query, dbq
        MESSAGE, msg
      ENDIF

      accum += Query_nAffectedRows(dbq)

    ENDFOR

  ENDIF

  RETURN, accum
END


;---> End Giorgio's library routines


PRO DBConnect, dbname, user, pass, HOST=host, NEWCONN=newconn
;+
; NAME:
;       DBCONNECT
;
; PURPOSE:
;   Open a DB connection and create a query communication channel.
;
; NOTES:
;   Execute queries with DBQUERY, get the result as a 2-D array
;   (table) with DBTABxyz or as an array of structures with TABSTRUCT.
;   Close/free resources with DBCLOSE.
;   If any of the input parameter is not given it will be interactively
;   requested from the terminal.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   DBConnect, 'db_name', 'username', 'password', HOST='hostname'
;
; OPTIONAL INPUT PARAMETERS:
;   DBNAME:   database name to connect to.
;   USER:     user name.
;   PASSWORD: password.
;
; INPUT KEYWORDS:
;   HOST:    hostname to connect to. If not given 'localhost' will be used.
;   NEWCONN: if not 0 then require a new connection i.e. the connection CANNOT
;            be already open.
;
; OUTPUT PARAMETERS:
;   None. Parameters in common block DBptr and DBinfo are set.
;
;   dbc: DB connection pointer as established via MCS.
;   dbq: Query pointer managed via MCS (used to send query and read result).
;
; OUTPUT KEYWORDS:
;   None.
;
; ERROR HANDLING:
;   The last MCS error is displayed via a call to MESSAGE.
;
; COMMON BLOCKS:
;   DBptr, DBinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  28/07/2008: LN, v0.3a: from Function to Pro;
;              changed keywords and input parameters.
;-
COMMON DBptr, dbc, dbq
COMMON DBinfo, c_host, c_user, c_password, c_dbname, c_query

  IF (N_ELEMENTS(dbc) EQ 0) THEN BEGIN
    dbc = ifd_null()
    dbq = ifd_null()
  ENDIF

  IF (dbc NE ifd_null()) THEN BEGIN
    IF (KEYWORD_SET(newconn)) THEN $
      MESSAGE, 'Connection is already opened.' $
    ELSE BEGIN
;      MESSAGE, 'Connection is already opened. Using it.', /INFO
      RETURN
    ENDELSE
  ENDIF

  IF NOT KEYWORD_SET(host) THEN $
    host = 'localhost'  ; Change as needed or pass as parameter

;print,"HOST=", host
  SWITCH N_PARAMS() OF
    0: BEGIN
        dbname = ''
        READ, dbname, PROMPT='Insert database name: '
    END
    1: BEGIN
        user = ''
        READ, user, PROMPT='Insert user name: '
    END
    2: pass = askPassword('Insert password for user ' + user +'@'+ host +': ')
    ELSE:
  ENDSWITCH

  c_host   = host
  c_dbname = dbname
  c_user   = user
  c_password = pass

;print, 'PASSWORD=',c_password

  dbc = new_DBConn(ifd_null())
  DBConn_connect, dbc, c_user, c_password, c_dbname, c_host
  dbq = new_Query(ifd_null(), dbc, 0)
;Check for errors
  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error

    del_Query, dbq
    del_DBConn, dbc
    dbq = ifd_null()
    dbc = ifd_null()

    MESSAGE, msg
  ENDIF

END


PRO DBClose
;+
; NAME:
;       DBCLOSE
;
; PURPOSE:
;   Free query resources and close DB connection
;   (deallocate DB connection and Query pointers).
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   DBClose
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; ERROR HANDLING:
;   The last MCS error is displayed via a call to MESSAGE.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  28/07/2008: LN, v0.3a: from Function to Pro.
;-
COMMON DBptr, dbc, dbq

  IF (N_ELEMENTS(dbc) EQ 0) THEN BEGIN
    dbc = ifd_null()
    dbq = ifd_null()
  ENDIF

  IF (dbc EQ ifd_null()) THEN $
    RETURN ;Connection is already closed

;Free resources
  Query_close, dbq

;Check for errors
  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    MESSAGE, msg
  ENDIF

;Destroy objects
  del_Query, dbq
  del_DBConn, dbc
  dbq = ifd_null()
  dbc = ifd_null()

END


PRO DBFree
;+
; NAME:
;       DBFREE
;
; PURPOSE:
;   Alias for DBCLOSE.
;-

  DBClose

END


FUNCTION DBQuery, query, NREC=nrec, EXEC_ONLY=exec_only
;+
; NAME:
;       DBQUERY
;
; PURPOSE:
;   Prepare and execute a generic query (passed as parameter or from the common
;   block DBinfo -> "c_query") on the communication channel "dbq".
;
; NOTES:
;   - If the connection is closed an error will occur.
;   - For SELECT queries this function is equivalent to DBQryStruct and the
;     result is an array of structures unless the kwd EXEC_ONLY is set, in
;     which case TabStruct has to used to get it.
;     TabStruct also allows to get a structure of arrays.
;     Use DBTABxyz to get a 2-D array (table) with all the elements of the same
;     type (string, float, etc.).
;   - To close/free resources use DBClose.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBQuery('SQL query')
;
; OPTIONAL INPUT PARAMETERS:
;   QUERY: SQL query to be executed. If missing then use "c_query" from DBinfo.
;
; INPUT KEYWORDS:
;   EXEC_ONLY: if set then, for a SELECT query, do not return the structure
;              with selected data (other routines can be used to retrieve
;              selected rows).
;
; OUTPUT PARAMETERS:
;   None (see NOTES). Set "c_query" in common DBptr if "query" parameter given.
;
; OUTPUT KEYWORDS:
;   NREC: number of records returned by query execution (if the query is
;         SELECT-like) or number of affected records.
;
; OUTPUTS:
;   For SELECT queries, returns an array (one element for each returned record)
;   of anonymous structures whose field's name/type match the corresponding
;   name/type on DB side, unless the kwd EXEC_ONLY is set, in which case it
;   returns 0 on success.
;   For other queries, returns 0 on success.
;
; ERROR HANDLING:
;   The last MCS error is displayed via a call to MESSAGE.
;
; COMMON BLOCKS:
;   DBptr, DBinfo, TABinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  28/07/2008: LN, v0.3a: changed keywords and input parameters.
;  19/09/2008: LN, v0.3b: added kwd EXEC_ONLY and common TABinfo.
;              Allows direct call also for SELECT queries returning data into
;              a structure (as from DBQryStruct).
;-
COMMON DBptr, dbc, dbq
COMMON DBinfo, c_host, c_user, c_password, c_dbname, c_query
COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

CATCH, errcatch
  IF errcatch NE 0 THEN BEGIN
    msg = !ERROR_STATE.MSG
    IF (ifd_got_error()) THEN BEGIN
      msg = 'MCS: ' + ifd_last_error() + STRING(10B) + msg
      ifd_reset_error
    ENDIF
    CATCH, /CANCEL
    MESSAGE, msg
  ENDIF

  IF (N_ELEMENTS(dbc) EQ 0) THEN BEGIN
    dbc = ifd_null()
    dbq = ifd_null()
  ENDIF

  IF (dbc EQ ifd_null()) THEN $
    MESSAGE, 'Connection is closed.'

  if N_PARAMS() NE 0 THEN $
    c_query = query

; Make sure query string is not an array
  c_query = STRTRIM(c_query[0],2)
  IF (STRLEN(c_query) EQ 0) THEN BEGIN
    MESSAGE, 'Query string is empty.', /INFO
    nrec = 0
    RETURN, 1
  ENDIF

  IF (STRCMP(c_query, 'SELECT', 6, /FOLD_CASE)) THEN $
    is_select = 1 $
  ELSE $
    is_select = 0

;  Query_prepare, dbq, c_query[0]
;  Query_execute, dbq, 1
  Query_query, dbq, c_query, 1

; Distinguish between SELECT and other query types
  IF (Query_gotRecordSet(dbq)) THEN $
    nrec = RecordSet_nRows(dbq) $
  ELSE $
    nrec = Query_nAffectedRows(dbq)

;Check for errors
  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    del_Query, dbq
    MESSAGE, msg
  ENDIF

  IF (is_select AND NOT KEYWORD_SET(exec_only)) THEN BEGIN
    IF (Query_gotRecordSet(dbq)) THEN BEGIN
      TabStruct
      IF (nrec GT 0) THEN BEGIN
        TabFill
        RETURN, tab_stru
      ENDIF
    ENDIF ELSE $
      MESSAGE, 'Query did not report a RecordSet from a SELECT query.'
  ENDIF

  RETURN, 0
END


FUNCTION DBExec_Qry, dbname, user, pass, query, HOST=host, NREC=nrec
;+
; NAME:
;       DBEXEC_QRY
;
; PURPOSE:
;   Open a DB connection and execute the query (passed as parameter or from
;   the common block DBinfo -> "c_query") on the communication channel "dbq".
;   It is equivalent to use DBCONNECT and DBQUERY in sequence.
;
; NOTES:
;   Connection is left open.
;   Can execute further queries with DBQUERY, get the result as
;   an array of structures or a structure of arrays with TABSTRUCT,
;   or as a 2-D array (table) with DBTABxyz.
;   Use DBCLOSE to close/free resources.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBExec_Qry('db_name', 'username', 'password', 'SQL query')
;
; OPTIONAL INPUT PARAMETERS:
;   DBNAME:   database name to connect to.
;   USER:     user name.
;   PASSWORD: password.
;   QUERY: SQL query to be executed. If missing then use "c_query" from DBinfo. 
;
; INPUT KEYWORDS:
;    HOST: hostname to connect to.
;          If not given 'localhost' will be used (see DBConnect).
;
; OUTPUT PARAMETERS:
;   None. Parameters in common block DBptr and DBinfo are set.
;
; OUTPUT KEYWORDS:
;   NREC: number of records returned by query execution (if the query is
;         SELECT-like) or number of affected records.
;
; OUTPUTS:
;   Returns 0 on success.
;
; ERROR HANDLING:
;   The last MCS error is displayed via a call to MESSAGE.
;
; COMMON BLOCKS:
;   DBptr, DBinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  28/07/2008: LN, v0.3a: changed keywords and input parameters.
;  19/09/2008: LN, v0.3b: changed call to DBConnect
;-
COMMON DBptr, dbc, dbq
COMMON DBinfo, c_host, c_user, c_password, c_dbname, c_query

CATCH, errcatch
  IF (errcatch NE 0) THEN BEGIN
    msg = !ERROR_STATE.MSG
    IF (ifd_got_error()) THEN BEGIN
      msg = 'MCS: ' + ifd_last_error() + STRING(10B) + msg
      ifd_reset_error
    ENDIF
    CATCH, /CANCEL
    MESSAGE, msg
  ENDIF

;  IF (KEYWORD_SET(host)) THEN $
;    c_host = host

; Check for undefined parameters: note the parameters order!
  np = 4
  SWITCH N_PARAMS() OF
    4: IF (SIZE(query, /TYPE) EQ 0) THEN $
         np--
    3: BEGIN
       IF (SIZE(pass, /TYPE) EQ 0) THEN $
         np--
    END
    2: BEGIN
       IF (SIZE(user, /TYPE) EQ 0) THEN $
         np--
    END
    1: BEGIN
       IF (SIZE(dbname, /TYPE) EQ 0) THEN $
         np--
       BREAK
    END
    0: np = 0
  ENDSWITCH

  SWITCH np OF
    0: DBConnect, HOST=host
    1: DBConnect, HOST=host
    2: DBConnect, dbname, HOST=host
    3: DBConnect, dbname, user, HOST=host
    ELSE: DBConnect, dbname, user, pass, HOST=host
  ENDSWITCH

  IF (N_ELEMENTS(dbc) EQ 0) THEN BEGIN
    dbc = ifd_null()
    dbq = ifd_null()
  ENDIF

  IF (dbc EQ ifd_null()) THEN $
    MESSAGE, 'Connection is closed.'

  if N_ELEMENTS(query) NE 0 THEN $
    c_query = query

; Make sure query string is not an array
  c_query = STRTRIM(c_query[0])
  IF (STRLEN(c_query) EQ 0) THEN $
    MESSAGE, 'ERR: Query string is empty.'

  Query_query, dbq, c_query, 1

  IF (Query_gotRecordSet(dbq)) THEN $
    nrec = RecordSet_nRows(dbq) $
  ELSE $
    nrec = Query_nAffectedRows(dbq)

;Check for errors (managed by CATCH)
  IF (ifd_got_error()) THEN $
    del_Query, dbq

  RETURN, 0
END


FUNCTION DBExec_TabQry, dbname, user, pass, query, HOST=host, $
         ARR_STRUCT=arr_struct, FFNAMES=ffnames, KEEP_CONN=keep_conn, $
         DEBUG=debug
;+
; NAME:
;       DBEXEC_TABQRY
;
; PURPOSE:
;   Full query execution and result saving into the structure "tab_stru".
;   This is available to the user in the common block TABinfo.
;   If the "query" parameter is missing, then use the query string "c_query"
;   which is passed via the common block DBinfo.
;   The DB connection is CLOSED, unless KEEP_CONN is set!
;
; NOTES:
;   Simply use DBEXEC_QRY, TABSTRUCT, TABFILL (and DBCLOSE) in sequence.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBExec_TabQry('db_name', 'username', 'password', 'SQL query')
;
; OPTIONAL INPUT PARAMETERS:
;   DBNAME: database name to connect to.
;   USER: user name.
;   PASSWORD: password.
;   QUERY: SQL query to be executed. If missing then use "c_query" from DBinfo.
;
; INPUT KEYWORDS:
;   HOST:       If set then use this as DB server.
;   ARR_STRUCT: If set then the returned table will be a structure of arrays
;               (one per field) rather than an array of structures
;               (one per row).
;   FFNAMES:    If set then the structure tag names are NOT the actual field
;               names but simply fN where N is a sequencial integer.
;   KEEP_CONN:  If set then do not close DB connection.
;   DEBUG:      If set then print some debug info.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   - the (INT) returned status from DBQUERY if query execution failed;
;   - the (INT) -1 if DBQUERY does not returns a RecordSet (from a SELECT);
;   - 0 on success.
;   Query output table structure "tab_stru" in common block TABinfo.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  18/07/2006: Added the keyword GET_FNAMES.
;  28/07/2008: LN, v0.3a: changed keywords and input parameters.
;-
COMMON DBptr, dbc, dbq

  status = DBExec_Qry(dbname, user, pass, query, HOST=host)

  IF (status) THEN $
    RETURN, status

  IF (Query_gotRecordSet(dbq)) THEN BEGIN
    TabStruct, ARR_STRUCT=arr_struct, FFNAMES=ffnames, DEBUG=debug
    TabFill
    IF (NOT KEYWORD_SET(keep_conn)) THEN $
      DBClose
    RETURN, 0
  ENDIF ELSE BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, -1
  ENDELSE

;  RETURN, status
END


FUNCTION DBExec_ShowQry, dbname, user, pass, query, HOST=host, $
         ARR_STRUCT=arr_struct, FFNAMES=ffnames, KEEP_CONN=keep_conn, $
         USE_SAVED_INFO=use_saved_info, DEBUG=debug
;+
; NAME:
;       DBEXEC_SHOWQRY
;
; PURPOSE:
;   Full query execution and result display. The returned query tables
;   "tab_stru" is still available to the user in the common block TABinfo.
;   The user can avoid to pass the parameters by using the keyword
;   USE_SAVED_INFO and using the query string "c_query" which is
;   passed via the common block DBinfo.
;   The DB connection is CLOSED, unless the keyword KEEP_CONN is set!
;
; NOTES:
;   Simply use DBEXEC_QRY, TABSTRUCT, TABFILL (and DBCLOSE) in sequence.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBExec_ShowQry('db_name', 'username', 'password', 'SQL query')
;
; OPTIONAL INPUT PARAMETERS:
;   DBNAME:   database name to connect to.
;   USER:     user name.
;   PASSWORD: password.
;   QUERY: SQL query to be executed. If missing then use "c_query" from DBinfo.
;
; INPUT KEYWORDS:
;   HOST:       If set then use this as DB server.
;   ARR_STRUCT: If set then the returned table will be a structure of arrays
;               (one per field) rather than an array of structures
;               (one per row).
;   FFNAMES:    If set then the structure tag names are NOT the actual field
;               names but simply fN where N is a sequencial integer.
;   KEEP_CONN:  If set then do not close DB connection.
;   USE_SAVED_INFO: If set then read any INPUT PARAMETERS missing information
;                   from COMMON block DBinfo.
;                   Input order (dbname, user, pass, query) must be preserved.
;   DEBUG:      If set then print some debug info.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   - the (INT) returned status from DBQUERY if query execution failed;
;   - the (INT) -1 if DBQUERY does not returns a RecordSet (from a SELECT);
;   - 0 on success.
;   Print the resulting table from the given query.
;   Query output table structure "tab_stru" in common block TABinfo.
;
; COMMON BLOCKS:
;   DBptr, DBinfo, TABinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  18/07/2006: Added the keyword GET_FNAMES.
;  11/12/2006: Fixed print of structure.
;  28/07/2008: LN, v0.3a: changed keywords and input parameters.
;              Also check a SELECT query was issued and manage the "_null"
;              fields in the structure.
;  19/09/2008: LN, v0.3b: changed structure with _null fields at the end.
;  07/12/2014: LN, v0.3d: added missing "query = c_query" for use_saved_info
;-
COMMON DBptr, dbc, dbq
COMMON DBinfo, c_host, c_user, c_password, c_dbname, c_query
COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

  IF (KEYWORD_SET(use_saved_info)) THEN BEGIN
    SWITCH N_PARAMS() OF
      0: dbname = c_dbname
      1: user = c_user
      2: pass = c_password
      3: query = c_query
      ELSE:
    ENDSWITCH

  ENDIF

  status = DBExec_Qry(dbname, user, pass, query, HOST=host)

  IF (status) THEN $
    RETURN, status

  IF (Query_gotRecordSet(dbq)) THEN BEGIN
    TabStruct, ARR_STRUCT=arr_struct, FFNAMES=ffnames, DEBUG=debug
    TabFill
    IF (NOT KEYWORD_SET(keep_conn)) THEN DBClose
  ENDIF ELSE BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, -1
  ENDELSE

  maxrows = 1000L  ; could become a keyword
  IF (n_rows GT maxrows) THEN BEGIN
    MESSAGE, 'WRN: There are ' + STRTRIM(n_rows,1) + ' rows to display.', /INFO
    MESSAGE, 'WRN: This is more than the def. limit of ' + STRTRIM(maxrows,1), /INFO
    ans = ''
    READ, 'Show anyway (Y/N) [N]: ', ans
    IF ((ans NE 'Y') AND (ans NE 'y')) THEN RETURN, 0
  ENDIF

  IF is_rowstru THEN BEGIN
    FOR j=0L, n_rows-1 DO BEGIN
      row = ''
      FOR i=0, n_fields-1 DO BEGIN
        row += ' ' + $
            STRTRIM( (tab_stru[j].(n_fields+i) ? ' NULL ' : tab_stru[j].(i)),2 )
      ENDFOR
      PRINT, j+1, row, FORMAT='(I, ":", X, A)'
    ENDFOR
  ENDIF ELSE BEGIN
    FOR j=0L, n_rows-1 DO BEGIN
      row = ''
      FOR i=0, n_fields-1 DO BEGIN
        row += ' ' + $
             STRTRIM( (tab_stru.(n_fields+i)[j] ? ' NULL ' : tab_stru.(i)[j]),2)
      ENDFOR
      PRINT, j+1, row, FORMAT='(I, ":", X, A)'
    ENDFOR
  ENDELSE

  RETURN, 0
END


PRO FieldNames, INFO=info
;+
; NAME:
;       FIELDNAMES
;
; PURPOSE:
;   Get table column names and Columns (integer) IDs "mcs_types".
;   Also sets "n_fields".
;
; NOTES:
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   FieldNames
;
; NOTE:
;   DB connection and query must have been performed before calling this
;   routine.
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   INFO: if set then display info on the query returned table.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   None: all goes into common block TABinfo:
;
;   mcs_types: Static int array with MCS defined types IDs (+ unsigned).
;   f_itype:   Int array with MCS type IDs for the columns.
;   n_fields:  Number of fields in the table.
;
; COMMON BLOCKS:
;   DBptr, TABinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  17/04/2007: Use RecordSet_metarec to get fields info.
;  28/07/2008: LN, v0.3a: manage unsigned.
;-
COMMON DBptr, dbc, dbq
COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

; Only manage some of the MCS types + unsigned int/long
  mcs_types = [Types_TINY(), Types_SMALL(), Types_MEDIUM(), Types_INT(), $
               Types_BIGINT(), Types_FLOAT(), Types_DOUBLE(), Types_STRING(), $
               Types_TIME(), Types_TINY_BLOB(), Types_BLOB(), Types_POINTER(), $
               20, 21]

  n_fields = RecordSet_nFields(dbq)
  IF (n_fields EQ 0) THEN BEGIN
    MESSAGE, 'WRN: No fields in result query!', /INFO
    f_name  = "none"
    f_itype = 0
    RETURN
  ENDIF
  f_name  = STRARR(n_fields)
  f_itype = INTARR(n_fields)

  v = RecordSet_metarec(dbq)
  FOR j=0, n_fields-1 DO BEGIN
    d = Record_field(v, j)
    f_name[j]  = Data_name(d)
    t = Data_type(d)
    is_uns = Data_isUnsigned(d)
    IF (is_uns) THEN BEGIN
      IF ((t EQ mcs_types[0]) OR (t EQ mcs_types[1]) OR $
          (t EQ mcs_types[2]) OR (t EQ mcs_types[3])) THEN $
        f_itype[j] = mcs_types[12] $
      ELSE IF (t EQ mcs_types[8]) THEN $
        f_itype[j] = mcs_types[8] $
      ELSE $
        f_itype[j] = mcs_types[13]
    ENDIF ELSE $
      f_itype[j] = t
  ENDFOR

  IF KEYWORD_SET(info) THEN BEGIN
    MESSAGE, 'Info on the query returned table fields:', /INFO
    PRINT, STRTRIM(n_fields,1), $
      FORMAT='(/,"Nr of fields: ",A,/, "Field names:")'
    PRINT, '  ', f_name
    PRINT, FORMAT='(/,"MCS fields ID types:")'
    PRINT, '  ', f_itype
    PRINT, ''
  ENDIF

END


PRO FieldInfo, ARR_STRUCT=arr_struct, INFO=info
;+
; NAME:
;       FIELDINFO
;
; PURPOSE:
;   Get table column types as "IDL variable type" or as "IDL array functions"
;   string array if ARR_STRUCT is set.
;   Also get columns (fields) names + number of columns and rows in the table.
;   Columns type and ID go into the common block TABinfo.
;
; NOTES:
;   Called by TABSTRUCT.
;   Can call it directly to get just the info on the returned query rather than
;   building the output table.
;   DB connection and query must have been performed before calling this
;   routine.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   FieldInfo
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   ARR_STRUCT: If set then the returned field types are appropriate to build
;               a structure of arrays (one per field) rather than an array of
;               structures (one per row). See TABSTRUCT.
;    INFO:      if set then display info on the query returned table.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   None: all goes into common block TABinfo:
;
;   mcs_types: Static int array with MCS defined types IDs (+ unsigned).
;   f_atype:   String array with column types (INTARR, ...).
;   f_itype:   Int array with MCS type IDs for the columns.
;   n_fields:  Number of fields in the table.
;   n_rows:    Number of rows in the table.
;   f_name:    String array with column names (as from DB table).
;
; COMMON BLOCKS:
;   DBptr, TABinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  17/04/2007: Use RecordSet_metarec to get fields info.
;              Added the default tab_stru set for n_fields=0 and n_rows=0.
;  28/07/2008: LN, v0.3a: changed keywords parameters and manage unsigned.
;  19/09/2008: LN, v0.3b: changed n_rows management.
;  22/03/2009: LN, v0.3c: Set here is_rowstru.
;  01/03/2011: LN, v0.3d: (unsigned) TIMESTAMP management.
;-
COMMON DBptr, dbc, dbq
COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

; Only manage some of the MCS types + unsigned int/long
  mcs_types = [Types_TINY(), Types_SMALL(), Types_MEDIUM(), Types_INT(), $
               Types_BIGINT(), Types_FLOAT(), Types_DOUBLE(), Types_STRING(), $
               Types_TIME(), Types_TINY_BLOB(), Types_BLOB(), Types_POINTER(), $
               20, 21]
  idl_types = [ $
    ['INTARR','LONARR','LON64ARR','UINTARR','ULONARR','ULON64ARR', $
     'FLTARR','DBLARR','STRARR','BYTARR','PTRARR'], $
    ['FIX(0)','LONG(0)','LONG64(0)','UINT(0)','ULONG(0)','ULONG64(0)', $
     'FLOAT(0)','DOUBLE(0)','STRING("")','BYTE(0)','PTR_NEW(0)'] $
  ]

  n_fields = RecordSet_nFields(dbq)
  IF (n_fields EQ 0) THEN BEGIN
    MESSAGE, 'WRN: No fields in result query!', /INFO
    f_name  = "none"
    f_atype = "UNDEF"
    f_itype = 0
    RETURN
  ENDIF
  f_name  = STRARR(n_fields)
  f_atype = STRARR(n_fields)
  f_itype = INTARR(n_fields)

  n_rows = RecordSet_nRows(dbq)
  IF (n_rows EQ 0) THEN RETURN

  IF (n_rows LT 0) THEN $
    MESSAGE, 'ERR: Nr of rows selected by the query is negative.'

  v = RecordSet_metarec(dbq)
  IF KEYWORD_SET(arr_struct) THEN BEGIN
    is_rowstru = 0
    itype = 0
  ENDIF ELSE BEGIN
    is_rowstru = 1
    itype = 1
  ENDELSE

  FOR j=0, n_fields-1 DO BEGIN
    d = Record_field(v, j)
    f_name[j] = Data_name(d)
    t = Data_type(d)
    is_uns = Data_isUnsigned(d)
print, f_name[j], t, is_uns
print, mcs_types

; Manage unsigned
    IF is_uns THEN BEGIN
      CASE t OF
        mcs_types[0]: BEGIN  ; '0U'
          f_atype[j] = idl_types[3,itype]  ; '0U'
          f_itype[j] = mcs_types[12]
          END
        mcs_types[1]: BEGIN  ; '0U'
          f_atype[j] = idl_types[3,itype]  ; '0U'
          f_itype[j] = mcs_types[12]
          END
        mcs_types[2]: BEGIN  ; '0UL'
          f_atype[j] = idl_types[4,itype]  ; '0UL'
          f_itype[j] = mcs_types[12]
          END
        mcs_types[3]: BEGIN  ; '0UL'
          f_atype[j] = idl_types[4,itype]  ; '0UL'
          f_itype[j] = mcs_types[12]
          END
        mcs_types[4]: BEGIN  ; '0ULL'
          f_atype[j] = idl_types[5,itype]  ; '0ULL'
          f_itype[j] = mcs_types[13]
          END
        mcs_types[8]: BEGIN  ; TIME -> STRING
          f_atype[j] = idl_types[8,itype]
          f_itype[j] = mcs_types[8]
          END
; Use of unsigned FLOAT/DOUBLE is not recommended
        mcs_types[5]: BEGIN  ; unsigned FLOAT
          f_atype[j] = idl_types[6,itype]
          f_itype[j] = mcs_types[5]
          END
        mcs_types[6]: BEGIN  ; unsigned DOUBLE
          f_atype[j] = idl_types[7,itype]
          f_itype[j] = mcs_types[6]
          END
        ELSE: BEGIN
          f_atype[j] = 'UNDEF'  ; should manage with an error flag
          f_itype[j] = t
          MESSAGE, 'Unsupported MCS data type: ' + STRTRIM(t,1)
          END
      ENDCASE
    ENDIF ELSE BEGIN

      f_itype[j] = t

      CASE t OF
        mcs_types[0]: $    ; TYNY
          f_atype[j] = idl_types[0,itype]
        mcs_types[1]: $    ; SMALL
          f_atype[j] = idl_types[0,itype]
        mcs_types[2]: $    ; MEDIUM
          f_atype[j] = idl_types[1,itype]
        mcs_types[3]: $    ; INT
          f_atype[j] = idl_types[1,itype]
        mcs_types[4]: $    ; BIGINT
          f_atype[j] = idl_types[2,itype]
        mcs_types[5]: $    ; FLOAT
          f_atype[j] = idl_types[6,itype]
        mcs_types[6]: $    ; DOUBLE
          f_atype[j] = idl_types[7,itype]
        mcs_types[7]: $    ; STRING
          f_atype[j] = idl_types[8,itype]
        mcs_types[8]: $    ; TIME
          f_atype[j] = idl_types[8,itype]
        mcs_types[9]: $    ; TINY_BLOB
          f_atype[j] = idl_types[9,itype]
        mcs_types[10]: $   ; BLOB
          f_atype[j] = idl_types[9,itype]
        mcs_types[11]: $   ; POINTER
          f_atype[j] = idl_types[10,itype]
        ELSE: BEGIN
          f_atype[j] = 'UNDEF'  ; should manage with an error flag
          MESSAGE, 'Unsupported MCS data type: ' + STRTRIM(t,1)
          END
      ENDCASE
    ENDELSE

  ENDFOR

  IF KEYWORD_SET(info) THEN BEGIN
    MESSAGE, 'Info on the query returned table fields:', /INFO
    PRINT, STRTRIM(n_fields,1), STRTRIM(n_rows,1), $
      FORMAT='(/,"Nr of fields: ",A,"  Nr of rows: ",A,/, "Field names:")'
    PRINT, '  ', f_name
    PRINT, FORMAT='(/,"IDL fields types:")'
    PRINT, '  ', f_atype
    PRINT, FORMAT='(/,"MCS fields ID types:")'
    PRINT, '  ', f_itype
    PRINT, ''
  ENDIF

END


PRO TabStruct, ARR_STRUCT=arr_struct, FFNAMES=ffnames, DEBUG=debug
;+
; NAME:
;       TABSTRUCT
;
; PURPOSE:
;   Build the table structure using FIELDINFO.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   TabStruct
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   ARR_STRUCT: If set then the returned table will be a structure of arrays
;               (one per field) rather than an array of structures
;               (one per row).
;   FFNAMES:    If set then the structure tag names are NOT the actual field
;               names but simply fN where N is a sequencial integer.
;   DEBUG:      If set then print some debug info.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   None: in common block TABinfo:
;
;   tab_str: Sructure with the output table. Format depends on ARR_STRUCT.
;
; COMMON BLOCKS:
;   DBptr, TABinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  18/07/2006: Added the keyword GET_FNAMES.
;  16/04/2007: Added the default tab_stru set for n_fields=0 and n_rows=0.
;  28/07/2008: LN, v0.3a: changed keywords parameters.
;  19/09/2008: LN, v0.3b: changed structure with _null fields at the end.
;-
COMMON DBptr, dbc, dbq
COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

; Get the field names and types. Also set is_rowstru.
  FieldInfo, ARR_STRUCT=arr_struct, INFO=debug

  IF (n_fields EQ 0) THEN BEGIN
    tab_stru = {f:'no fields'}
    RETURN
  ENDIF
  IF (n_rows EQ 0) THEN BEGIN
    tab_stru = {f:'no rows'}
    RETURN
  ENDIF

; Tag names are the actual field names?
  IF KEYWORD_SET(ffnames) THEN $
    fn = 0 $
  ELSE $
    fn = 1

  s_nr = STRTRIM(n_rows,1)
  IF KEYWORD_SET(arr_struct) THEN BEGIN
;    is_rowstru = 0
    the_stru = 'tab_stru = {'
    the_stru_null = ''
    FOR i=0, n_fields-1 DO BEGIN
      IF (i GT 0) THEN the_stru_null += ', '
      IF fn THEN BEGIN
        the_stru += f_name[i] + ': ' + f_atype[i] + '(' + s_nr + '), '
        the_stru_null += f_name[i] + '_null: BYTARR(' + s_nr + ')'
      ENDIF ELSE BEGIN
        f_is = 'f' + STRTRIM(i,1)
        the_stru += f_is + ': ' + f_atype[i] + '(' + s_nr + '), '
        the_stru_null += f_is + '_null: BYTARR(' + s_nr + ')'
      ENDELSE
    ENDFOR

  ENDIF ELSE BEGIN
;    is_rowstru = 1
    the_stru = 'row_stru = {'
    the_stru_null = ''
    FOR i=0, n_fields-1 DO BEGIN
      IF (i GT 0) THEN the_stru_null += ', '
      IF fn THEN BEGIN
        the_stru += f_name[i] + ': ' + f_atype[i] + ', '
        the_stru_null += f_name[i] + '_null: 0b'
      ENDIF ELSE BEGIN
        f_is = 'f' + STRTRIM(i,1)
        the_stru += f_is + ': ' + f_atype[i] + ', '
        the_stru_null += f_is + '_null: 0b'
      ENDELSE
    ENDFOR

  ENDELSE

  the_stru += the_stru_null + '}'

  IF KEYWORD_SET(debug) THEN BEGIN
    MESSAGE, 'Info on table structure:', /INFO
    PRINT, FORMAT='(/,"Field names:")'
    PRINT, '  ', f_name
    PRINT, 'Table size (rows X columns): ',s_nr,' X ',STRTRIM(n_fields,1)
    IF is_rowstru THEN $
      PRINT, 'Row structure:' $
    ELSE $
      PRINT, 'Table structure:'
    PRINT, the_stru
  ENDIF

  dummy = EXECUTE(the_stru)
  IF is_rowstru THEN $
    tab_stru = REPLICATE(row_stru, n_rows)
  IF KEYWORD_SET(debug) THEN BEGIN
    PRINT, FORMAT='(/,"Table structure HELP info:")'
    HELP, tab_stru, /STRU
  ENDIF

END


PRO TabFill
;+
; NAME:
;       TABFILL
;
; PURPOSE:
;   Fill the table structure "tab_stru".
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   TabFill
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   None: in common block TABinfo:
;
;   tab_str: Sructure with the output table. Format depends on the keyword
;            ARR_STRUCT of TABSTRUCT.
;
; COMMON BLOCKS:
;   DBptr, TABinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  17/04/2007: Added exception handling for n_rows=0.
;  28/07/2008: LN, v0.3a: manage unsigned.
;  19/09/2008: LN, v0.3b: changed n_rows and _null elements management.
;-
COMMON DBptr, dbc, dbq
COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

  IF (n_rows LE 0) THEN $
    MESSAGE, 'ERR: No rows selected by the query.'

  dummy = RecordSet_setFirst(dbq)

  FOR j=0, n_fields-1 DO BEGIN

    IF is_rowstru THEN BEGIN

    CASE 1 OF
; TINY, SMALL
      (f_itype[j] EQ mcs_types[0]) OR (f_itype[j] EQ mcs_types[1]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_ival(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; MEDIUM, INT
      (f_itype[j] EQ mcs_types[2]) OR (f_itype[j] EQ mcs_types[3]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_ival(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; LONG
      (f_itype[j] EQ mcs_types[4]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_lval(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; UINT
      (f_itype[j] EQ mcs_types[12]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_uival(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; ULONG
      (f_itype[j] EQ mcs_types[13]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_ulval(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; FLOAT
      (f_itype[j] EQ mcs_types[5]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_fval(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; DOUBLE
      (f_itype[j] EQ mcs_types[6]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_dval(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; STRING
      (f_itype[j] EQ mcs_types[7]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_sval(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; TIME
      (f_itype[j] EQ mcs_types[8]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_sval(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; TINY_BLOB, BLOB
      (f_itype[j] EQ mcs_types[9]) OR (f_itype[j] EQ mcs_types[10]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_ival(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; POINTER
      (f_itype[j] EQ mcs_types[11]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru[i].(j) = Data_ival(d)
          tab_stru[i].(n_fields+j) = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
    ENDCASE

    ENDIF ELSE BEGIN

    CASE 1 OF
      (f_itype[j] EQ mcs_types[0]) OR (f_itype[j] EQ mcs_types[1]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_ival(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
      (f_itype[j] EQ mcs_types[2]) OR (f_itype[j] EQ mcs_types[3]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_ival(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
      (f_itype[j] EQ mcs_types[4]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_lval(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; UINT
      (f_itype[j] EQ mcs_types[12]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_uival(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
; ULONG
      (f_itype[j] EQ mcs_types[13]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_ulval(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
      (f_itype[j] EQ mcs_types[5]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_fval(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
      (f_itype[j] EQ mcs_types[6]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_dval(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
      (f_itype[j] EQ mcs_types[7]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_sval(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
      (f_itype[j] EQ mcs_types[8]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_sval(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
      (f_itype[j] EQ mcs_types[9]) OR (f_itype[j] EQ mcs_types[10]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_ival(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
      (f_itype[j] EQ mcs_types[11]): $
        FOR i=0L, n_rows-1 DO BEGIN
          d = Record_field(RecordSet_rec(dbq),j)
          tab_stru.(j)[i] = Data_ival(d)
          tab_stru.(n_fields+j)[i] = Data_isNull(d)
          dummy = RecordSet_setNext(dbq)
        ENDFOR
    ENDCASE

    ENDELSE

    dummy = RecordSet_setFirst(dbq)
  ENDFOR

END


FUNCTION DBQryStruct, query, HOST=host, ARR_STRUCT=arr_struct, $
         FFNAMES=ffnames, DEBUG=debug, NREC=nrec
;+
; NAME:
;       DBQRYSTRUCT
;
; PURPOSE:
;   Full query execution and result returned as a strucure. It is also saved
;   into the structure "tab_stru" in the common block TABinfo.
;   The user needs only to pass the query string or to copy it in the string
;   "c_query" which is passed via the common block DBinfo.
;
; NOTES:
;   - Simply use DBQUERY, TABSTRUCT, TABFILL in sequence.
;   - It can only be used for SELECT queries.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBQryStruct('SQL query')
;
; OPTIONAL INPUT PARAMETERS:
;   QUERY: SQL query to be executed. If missing then use "c_query" from DBinfo.
;
; INPUT KEYWORDS:
;   HOST:       If set then use this as DB server.
;   ARR_STRUCT: If set then the returned table will be a structure of arrays
;               (one per field) rather than an array of structures
;               (one per row).
;   FFNAMES:    If set then the structure tag names are NOT the actual field
;               names but simply fN where N is a sequencial integer.
;   DEBUG:      If set then print some debug info.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   NREC: number of records returned by query execution (if the query is
;         SELECT-like) or number of affected records.
;
; OUTPUTS:
;   - the (INT) returned status from DBQUERY if query execution failed;
;   - the (INT) -1 if DBQUERY does not returns a RecordSet (from a SELECT);
;   - an array (one element for each returned record) of anonymous structures
;     whose field's name/type match the corresponding name/type on DB
;     side if ARR_STRUCT is not set, otherwise a 1-d structure of arrays is
;     returned. Also the structure contains a number of fields with the '_null'
;     suffix in their name, which tells whether a field has a NULL value.
;
; COMMON BLOCKS:
;   DBptr, TABinfo
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2008.
;  19/09/2008: LN, v0.3b: changed n_rows management and manage new DBQuery.
;-
COMMON DBptr, dbc, dbq
COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

  query = STRTRIM(query,2)
  IF (NOT STRCMP(query, 'SELECT', 6, /FOLD_CASE)) THEN $
    MESSAGE, 'Please check you are using a SELECT query.'

  status = DBQuery(query, NREC=nrec, /EXEC_ONLY)
  IF (status) THEN $
    RETURN, status

  IF (Query_gotRecordSet(dbq)) THEN BEGIN
    TabStruct, ARR_STRUCT=arr_struct, FFNAMES=ffnames, DEBUG=debug
    IF (n_rows GT 0) THEN BEGIN
      TabFill
      RETURN, tab_stru
    ENDIF
  ENDIF ELSE $
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.'

;  RETURN, 0
END


FUNCTION TabFieldNames, Tabname, Fname
;+
; NAME:
;       TABFIELDNAMES
;
; PURPOSE:
;   Get ALL the column names as reported in a given DB table.
;
; NOTE:
;   This is a stand-alone function, i.e. the common block TABinfo is not
;   affected!
;   DB connection must have been performed before calling this function.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = TabFieldNames(Tabname, Fname)
;
; INPUT PARAMETERS:
;   Tabname: the table name.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   Fname: a string array with the column names.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   Return the number of fields in the table.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;     Written by L. Nicastro @ IASF-INAF, July 2006.
;  17/04/2007: Added direct query on a separate instance.
;-
COMMON DBptr, dbc, dbq

  IF (N_ELEMENTS(dbc) EQ 0) THEN BEGIN
    dbc = ifd_null()
    dbq = ifd_null()
  ENDIF

  IF (dbc EQ ifd_null()) THEN $
    MESSAGE, 'DB connection is closed.'

  query = 'SELECT * FROM ' + tabname + ' LIMIT 0'
  dbq1 = new_Query(ifd_null(), dbc, 0)
;  dummy = RecordSet_setFirst(dbq1)
  Query_prepare, dbq1, query
  Query_execute, dbq1, 1

  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    del_Query, dbq1
    MESSAGE, msg
  ENDIF

;  dummy = RecordSet_setFirst(dbq1)
  nf = RecordSet_nFields(dbq1)
  IF (nf EQ 0) THEN $
    fname =[''] $
  ELSE BEGIN
    fname = STRARR(nf)

    v = RecordSet_metarec(dbq1)
    FOR j=0, nf-1 DO $
      fname[j] = Data_name(Record_field(v, j))
  ENDELSE

; Free and destroy local query object
  Query_close, dbq1

  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    MESSAGE, msg
  ENDIF
  del_Query, dbq1

  RETURN, nf
END


FUNCTION TabFieldTypes, Tabname, Fatype, ARR_STRUCT=arr_struct
;+
; NAME:
;       TABFIELDTYPES
;
; PURPOSE:
;   Get table column types as "IDL variable type" or as "IDL array functions"
;   string array if ARR_STRUCT is set.
;
; NOTE:
;   This is a stand-alone function, i.e. the common block TABinfo is not
;   affected!
;   DB connection must have been performed before calling this function.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = TabFieldTypes(Tabname, Fatype)
;
; INPUT PARAMETERS:
;   Tabname: the table name.
;
; INPUT KEYWORDS:
;   ARR_STRUCT: If set then the returned field types are appropriate to build
;               a structure of arrays (one per field) rather than an array of
;               structures (one per row). See TABSTRUCT.
;
; OUTPUT PARAMETERS:
;   Fatype: a string array with the column types.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   Return the number of fields in the table.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;     Written by L. Nicastro @ IASF-INAF, July 2006.
;  17/04/2007: Added direct query on a separate query.
;  28/07/2008: LN, v0.3a: changed keywords parameters + unsigned management.
;  01/03/2011: LN, v0.3d: (unsigned) TIMESTAMP management.
;-
COMMON DBptr, dbc, dbq

; Only manage some of the MCS types + unsigned int/long
  mcs_types = [Types_TINY(), Types_SMALL(), Types_MEDIUM(), Types_INT(), $
               Types_BIGINT(), Types_FLOAT(), Types_DOUBLE(), Types_STRING(), $
               Types_TIME(), Types_TINY_BLOB(), Types_BLOB(), Types_POINTER(), $
               20, 21]
  idl_types = [ $
    ['INTARR','LONARR','LON64ARR','UINTARR','ULONARR','ULON64ARR', $
     'FLTARR','DBLARR','STRARR','BYTARR','PTRARR'], $
    ['FIX(0)','LONG(0)','LONG64(0)','UINT(0)','ULONG(0)','ULONG64(0)', $
     'FLOAT(0)','DOUBLE(0)','STRING("")','BYTE(0)','PTR_NEW(0)'] $
  ]

  IF (N_ELEMENTS(dbc) EQ 0) THEN BEGIN
    dbc = ifd_null()
    dbq = ifd_null()
  ENDIF

  IF (dbc EQ ifd_null()) THEN BEGIN
    MESSAGE, 'DB connection is closed.', /INFO
    RETURN, 0
  ENDIF

  query = 'SELECT * FROM ' + tabname + ' LIMIT 0'
  dbq1 = new_Query(ifd_null(), dbc, 0)
  Query_prepare, dbq1, query
  Query_execute, dbq1, 1

  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    del_Query, dbq1
    MESSAGE, msg
  ENDIF

  nf = RecordSet_nFields(dbq1)
  IF (nf EQ 0) THEN BEGIN
    fatype =['']
  ENDIF ELSE BEGIN
    fatype = STRARR(nf)

    v = RecordSet_metarec(dbq1)

    IF KEYWORD_SET(arr_struct) THEN $
      itype = 0 $
    ELSE $
      itype = 1

    FOR j=0, nf-1 DO BEGIN
      d = Record_field(v, j)
;      f_name[j] = Data_name(d)
      t = Data_type(d)
      is_uns = Data_isUnsigned(d)

; Manage unsigned
      IF is_uns THEN BEGIN
        CASE t OF
          mcs_types[0]: fatype[j] = idl_types[3,itype]  ; '0U'
          mcs_types[1]: fatype[j] = idl_types[3,itype]  ; '0U'
          mcs_types[2]: fatype[j] = idl_types[4,itype]  ; '0UL'
          mcs_types[3]: fatype[j] = idl_types[4,itype]  ; '0UL'
          mcs_types[4]: fatype[j] = idl_types[5,itype]  ; '0ULL'
          mcs_types[8]: fatype[j] = idl_types[8,itype]  ; 'TIME' -> STRING
; Use of unsigned FLOAT/DOUBLE is not recommended
          mcs_types[5]: fatype[j] = idl_types[6,itype]  ; FLOAT
          mcs_types[6]: fatype[j] = idl_types[7,itype]  ; DOUBLE
          ELSE: BEGIN
            f_atype[j] = 'UNDEF'  ; should manage with an error flag
            MESSAGE, 'Unsupported MCS data type: ' + STRTRIM(t,1), /INFO
            END
        ENDCASE
      ENDIF ELSE BEGIN

        CASE t OF
          mcs_types[0]: $    ; TYNY
            fatype[j] = idl_types[0,itype]
          mcs_types[1]: $    ; SMALL
            fatype[j] = idl_types[0,itype]
          mcs_types[2]: $    ; MEDIUM
            fatype[j] = idl_types[1,itype]
          mcs_types[3]: $    ; INT
            fatype[j] = idl_types[1,itype]
          mcs_types[4]: $    ; BIGINT
            fatype[j] = idl_types[2,itype]
          mcs_types[5]: $    ; FLOAT
            fatype[j] = idl_types[6,itype]
          mcs_types[6]: $    ; DOUBLE
            fatype[j] = idl_types[7,itype]
          mcs_types[7]: $    ; STRING
            fatype[j] = idl_types[8,itype]
          mcs_types[8]: $    ; TIME
            fatype[j] = idl_types[8,itype]
          mcs_types[9]: $    ; TINY_BLOB
            fatype[j] = idl_types[9,itype]
          mcs_types[10]: $   ; BLOB
            fatype[j] = idl_types[9,itype]
          mcs_types[11]: $   ; POINTER
            fatype[j] = idl_types[10,itype]
          ELSE: BEGIN
            fatype[j] = 'UNDEF'  ; should manage with an error flag
            MESSAGE, 'Unsupported MCS data type: ' + STRTRIM(t,1), /INFO
            END
        ENDCASE
      ENDELSE

    ENDFOR
  ENDELSE

; Free and destroy local query object
  Query_close, dbq1

  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    del_Query, dbq1
    MESSAGE, msg
  ENDIF

; Remove the initialization part
  FOR j=0, nf-1 DO $
   fatype[j] = STRMID(fatype[j],0,STRPOS(fatype[j],'('))

  RETURN, nf
END


FUNCTION DBTabSTR
;+
; NAME:
;       DBTABSTR
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d string array.
;
;  NOTES:
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabSTR()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d string array or 0, if not a SELECT query detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = STRARR(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = Data_sval(d)
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  RETURN, data
END


FUNCTION DBTabString
;+
; NAME:
;       DBTABSTRING
;
; PURPOSE:
;   Alias for DBTABSTR.
;-

  RETURN, DBTabSTR()
END


FUNCTION DBTabINT
;+
; NAME:
;       DBTABINT
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d integer*2 array.
;
;  NOTES:
;   - Assume (be sure!) all read columns can fit into a 2-byte signed integer.
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;   - Trying to encode string columns could couse fatal errors.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabINT()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d integer (short) array or 0, if not a SELECT query detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = INTARR(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = FIX(Data_ival(d))
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  dummy = RecordSet_setFirst(dbq)

  RETURN, data
END


FUNCTION DBTabLON
;+
; NAME:
;       DBTABLON
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d integer*4 array.
;
;  NOTES:
;   - Assume (be sure!) all read columns can fit into a 4-byte signed integer.
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;   - Trying to encode string columns could couse fatal errors.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabLON()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d integer array or 0, if not a SELECT query detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = LONARR(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = Data_ival(d)
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  RETURN, data
END


FUNCTION DBTabLON64
;+
; NAME:
;       DBTABLON64
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d integer*8 array.
;
;  NOTES:
;   - Assume (be sure!) all read columns can fit into a 8-byte signed integer.
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;   - Trying to encode string columns could couse fatal errors.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabLON64()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d long integer (8 bytes) array or 0, if not a SELECT query
;   detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = LONARR64(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = Data_lval(d)
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  RETURN, data
END


FUNCTION DBTabUINT
;+
; NAME:
;       DBTABUINT
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d unsigned integer*2 array.
;
;  NOTES:
;   - Assume (be sure!) all read columns can fit into a 2-byte unsigned integer.
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;   - Trying to encode string columns could couse fatal errors.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabUINT()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d unsigned integer (short) array or 0, if not a SELECT query
;   detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2008.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = UINTARR(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = UINT(Data_uival(d))
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  dummy = RecordSet_setFirst(dbq)

  RETURN, data
END


FUNCTION DBTabULON
;+
; NAME:
;       DBTABULON
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d unsigned integer*4 array.
;
;  NOTES:
;   - Assume (be sure!) all read columns can fit into a 4-byte unsigned integer.
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;   - Trying to encode string columns could couse fatal errors.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabULON()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d integer array or 0, if not a SELECT query detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2008.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = ULONARR(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = Data_uival(d)
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  RETURN, data
END


FUNCTION DBTabULON64
;+
; NAME:
;       DBTABLON64
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d unsigned integer*8 array.
;
;  NOTES:
;   - Assume (be sure!) all read columns can fit into a 8-byte unsigned integer.
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;   - Trying to encode string columns could couse fatal errors.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabULON64()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d unsigned long integer (8 bytes) array or 0, if not a SELECT
;   query detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2008.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = ULONARR64(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = Data_ulval(d)
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  RETURN, data
END


FUNCTION DBTabFLT
;+
; NAME:
;       DBTABFLT
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d float array.
;
;  NOTES:
;   - Assume (be sure!) all read columns can fit into a 4-byte float.
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;   - Trying to encode string columns could couse fatal errors.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabFLT()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d float array or 0, if not a SELECT query detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = FLTARR(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = Data_fval(d)
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  RETURN, data
END


FUNCTION DBTabDBL
;+
; NAME:
;       DBTABDBL
;
; PURPOSE:
;   Encode ALL the data from a query as a 2-d double array.
;
;  NOTES:
;   - Assume (be sure!) all read columns can fit into a 8-byte float.
;   - To be called after DBEXEC_QRY (or DBQUERY).
;   - NULL checker fields (see DBQRYSTRUCT) are not returned, so make sure
;     you do not query on columns which might have NULLs.
;   - Trying to encode string columns could couse fatal errors.
;
; CATEGORY:
;   MCS database communication contributed library.
;
; CALLING SEQUENCE:
;   Result = DBTabDBL()
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUT:
;   Return a 2-d double array or 0, if not a SELECT query detected.
;
; COMMON BLOCKS:
;   DBptr
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;-
COMMON DBptr, dbc, dbq

; Make sure a SELECT query was used
  IF (NOT Query_gotRecordSet(dbq)) THEN BEGIN
    MESSAGE, 'DBQuery did not report a RecordSet from a SELECT query.', /INFO
    MESSAGE, 'Please check you are using a SELECT query.', /INFO
    RETURN, 0
  ENDIF

  dummy = RecordSet_setFirst(dbq)
  v = RecordSet_rec(dbq)
  nf = Record_count(v)
  nr = RecordSet_nRows(dbq)
  data = DBLARR(nf, nr)

  FOR i=0L, nr-1 DO BEGIN
    v = RecordSet_rec(dbq)
    FOR j=0L, nf-1 DO BEGIN
      d = Record_field(v, j)
      data[j, i] = Data_dval(d)
    ENDFOR
    dummy = RecordSet_setNext(dbq)
  ENDFOR

  RETURN, data
END
