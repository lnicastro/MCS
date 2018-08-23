@mcs_usrlib   ; User contributed Functions and Pro

PRO DBExample1
;+
; NAME:
;       DBExample1
;
; PURPOSE:
;   Show how to use the routines in the "mcs_usrlib" library.
;   The library uses the IDL2MCS database interface.
;   See ross.iasfbo.inaf.it/MCS/
;
;   PLEASE READ THE CODE AND THE COMMENTS IN THIS AND OTHER DEMO FILE.
;
; CALLING SEQUENCE:
;   DBExample1
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   None.
;
; NOTES:
;   Must run from a directory where the command "idl2mcs" has been issued.
;   It create logical links to idl2mcs.dlm and idl2mcs.so.
;
;   A MySQL database server must be running on localhost and a MySQL account
;   must be available. Also a database named "mpe2018db" must exist.
;
;   See the help on the various routines with "doc_library" or editing
;   "mcs_usrlib.pro".
;
;
; MODIFICATION HISTORY:
;   Written by G. Calderone @ IFC-INAF, May 2008.
;  28/07/2008: LN: changed to use mcs_usrlib v0.3a.
;  19/09/2008: LN: changed to use mcs_usrlib v0.3b.
;  03/12/2014: LN: changed for the IMPRS DB course.
;-

; Error handler
  CATCH, Error_status
  IF Error_status NE 0 THEN BEGIN
    PRINT, !ERROR_STATE.MSG
    DBClose
    RETURN
  ENDIF

; Open a database connection on localhost, a user name and password will be
; interactively requested, though they can be passed as paramaters
; (see DBConnect help)
  DBConnect, 'mpe2018db'

; Get the list of available databases: for a SELECT query, DBQuery returns a
; structure matching the returned data format unless the kwd EXEC_ONLY is set
  sql = 'SELECT * FROM INFORMATION_SCHEMA.SCHEMATA'
  rec = DBQuery(sql, NREC=nrec)

  PRINT, 'Available databases:'
  FOR i=0, nrec-1 DO $
    PRINT, ' - ', rec[i].SCHEMA_NAME

; Create a test table (first drop it if exists)
  dummy = DBQuery('DROP TABLE IF EXISTS test_mcs')
  dummy = DBQuery('CREATE TABLE test_mcs (c CHAR(10), i INT, f FLOAT, UNIQUE(i))')

; Insert some records using simple INSERT queries
  dummy = DBQuery('INSERT INTO test_mcs VALUES("hello", 100, 2.3),("goodbye", 200, 4.6),("finish", 300, 9.2)')

; Retrieve table structure: DBQryStruct returns a structure
  rec = DBQryStruct('SELECT * FROM test_mcs LIMIT 1')

; Check that the returned structure matches the corresponding one on DB side
  PRINT, 'Table test_mcs structure: '
  HELP, rec, /STRUCT

; Insert some records using a prepared statement:
; DBPrepare returns par as a structure matching the table record
  par = DBPrepare('c i', 'test_mcs', /INSERT)

; Use an array of structures to perform multiple entries insertion
  p10 = REPLICATE(par,10)  ; a 10 elements array of structures
  FOR j=0, 9 DO BEGIN
    p10[j].c = STRTRIM(j,1) + '_chr'
    p10[j].i = j
  ENDFOR
  HELP, /STR, p10
  dummy = DBExecPar(p10)

; As an alternative can use a "structure of arrays" (of the appropriate type!)
; See DBPrepare help
  p10 = {c: STRARR(10), i: LONARR(10), c_null: BYTARR(10), i_null: BYTARR(10)}
  FOR j=0, 9 DO BEGIN
    p10.c[j] = STRTRIM(j+10,1) + '_chr'
    p10.i[j] = j + 10
  ENDFOR
  HELP, /STR, p10
  dummy = DBExecPar(p10)

  PRINT,''
  PRINT, 'Using "DBQuery" with kwd EXEC_ONLY and "DBTabSTR".'
  PRINT, 'Note how unset (NULL) "f" fields are reported with the last valid value'
  PRINT,''
  err = DBQuery('SELECT * FROM test_mcs', /EXEC_ONLY)  ;, /DEBUG)
  PRINT, DBTabSTR()

; Modify all "f" records with i > 5 using an UPDATE query
  par = DBPrepare('i f', 'test_mcs', /UPDATE, WHERE='i > 5')
  par.i_null = 1  ; Set i field to NULL
  par.f = 3.14
  dummy = DBExecPar(par)

; Replace record with i=2, note that for REPLACE to work properly the fields on
; the WHERE clause must be a unique key
  par = DBPrepare('c i', 'test_mcs', /REPLACE, WHERE='i = 2')
  par.c = 'two'
  par.i = 2
  dummy = DBExecPar(par)


; Table fields info
  PRINT,''
  PRINT, 'Use "TabFieldNames" to get field names:'
  nf = TabFieldNames('test_mcs', fnames)
  PRINT, '--> ', fnames
  PRINT, 'Use "TabFieldTypes" to get field (IDL) types:'
  nf = TabFieldTypes('test_mcs', ftypes)
  PRINT, '--> ', ftypes

; Retrieve records
  PRINT, ''
  PRINT, 'Using "DBQryStruct"'
  rec = DBQryStruct('SELECT * FROM test_mcs', NREC=nrec)

; Print values
  PRINT, ''
  PRINT, 'Table test_mcs content (managing NULL values): '
  FOR i=0, nrec-1 DO $
    PRINT $
    , (rec[i].c_null ?  '   NULL  ' : rec[i].c) $
    , (rec[i].i_null ?  '   NULL  ' : rec[i].i) $
    , (rec[i].f_null ?  '   NULL  ' : rec[i].f)


  PRINT, ''
  PRINT, 'Using "DBQuery" with kwd EXEC_ONLY and "DBTabSTR"'
  err = DBQuery('SELECT * FROM test_mcs', /EXEC_ONLY)  ;, /DEBUG)
  PRINT, DBTabSTR()

  PRINT, ''
  PRINT, 'Using "DBExec_ShowQry" with kwds USE_SAVED and KEEP'
  err = DBExec_ShowQry(/USE_SAVED, /KEEP)  ;, /DEBUG)

; Drop the test_mcs table
  dummy = DBQuery('DROP TABLE test_mcs')
  PRINT, ''
  PRINT, 'TABLE test_mcs dropped.'

; Close DB connection
  DBClose
END
