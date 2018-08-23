;+
; Simple query using the MCS to IDL wrapper via the mcs_usrlib.pro.
;-

@mcs_usrlib   ; User contributed Functions and Pro

PRO dbtest1

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
  DBConnect, ''

; Get the list of available databases: for a SELECT query, DBQuery returns a
; structure matching the returned data format unless the kwd EXEC_ONLY is set
  sql = 'SELECT SCHEMA_NAME FROM information_schema.SCHEMATA'
  rec = DBQuery(sql, /exec_only, NREC=nrec)

  PRINT, 'Available databases:'

; Read as string
  t = DBTabSTR()

  FOR i=0, nrec-1 DO $
    PRINT, ' - ', t[i]
    ;PRINT, ' - ', rec[i].SCHEMA_NAME

; Close DB connection
  DBClose
END
