;+
; Simple query using the MCS to IDL wrapper via the mcs_usrlib.pro.
;-

@mcs_usrlib   ; User contributed Functions and Pro


PRO dbtest2

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
  DBConnect, 'information_schema'

; Get the list of available databases: for a SELECT query, DBQuery returns a
; structure matching the returned data format unless the kwd EXEC_ONLY is set
  sql = 'SELECT SCHEMA_NAME FROM SCHEMATA'

; Retrieve table structure: DBQryStruct returns a structure
  ;rec = DBQryStruct('SELECT * FROM SCHEMATA')

; Check that the returned structure matches the corresponding one on DB side
  ;PRINT, 'Table test_mcs structure: '
  ;HELP, rec, /STRUCT


  ;PRINT, "Using 'DBQuery' with kwd EXEC_ONLY and DBTabSTR."
  ;PRINT, "Note how unset (NULL) 'f' fields are reported with the last valid value"
  ;PRINT,''

  err = DBQuery(sql, /EXEC_ONLY)  ;, /DEBUG)
  PRINT, DBTabSTR()

; Table fields info
  ;PRINT,''
  PRINT, "Use 'TabFieldNames' to get field names:"
  nf = TabFieldNames('SCHEMATA', fnames)
  PRINT, "--> ", fnames
  PRINT, '3. ', fnames[3]
  PRINT, "Use 'TabFieldTypes' to get field (IDL) types:"
  nf = TabFieldTypes('SCHEMATA', ftypes)
  PRINT, "--> ", ftypes

; Close DB connection
  DBClose
END
