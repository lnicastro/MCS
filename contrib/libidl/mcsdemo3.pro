@mcs_usrlib   ; User contributed Functions and Pro

PRO mcsdemo3, Host=host, User=user, Password=password, DBName=dbname, $
    Query=query, Debug=debug
;+
; NAME:
;       MCSDEMO3
;
; PURPOSE:
;   MCS demonstration program using the routines in the "mcs_usrlib" library.
;
;   Show usage of DBExec_TabQry and DBExec_ShowQry.
;
; CATEGORY:
;   MCS demonstration program using the mcs_usrlib library.
;
; CALLING SEQUENCE:
;   mcsdemo3 [, see keywords section]
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;      Host: The DB server host name (def. localhost).
;      User: The user name to use for DB access.
;  Password: The password to use for DB access.
;    DBName: The database name to access.
;     Query: The complete query string.
;     Debug: If set then print some debug info.
;
; NOTES:
;   To use the default query, the table "regions" must be present in the
;   selected database (can get it from ross.iasfbo.inaf.it/MCS/).
;
; COMMON BLOCKS:
;   Note that all the info are passed using COMMON blocks and it could be
;   useful to include "DBinfo" or/and "TABinfo" (see below).
;
;    DBinfo, c_host, c_user, c_password, c_dbname, c_query
;    TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
;             f_atype, f_itype, the_stru
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  19/09/2008: LN, changed to use mcs_usrlib v0.3b.
;-

COMMON DBinfo
COMMON TABinfo

; Keywords section (just check there is a query)
  IF (NOT KEYWORD_SET(query)) THEN BEGIN
    query = 'SELECT * FROM regions limit 10'
    PRINT, 'Using query: ' + query
  ENDIF

; DB section
; Connect, execute and show query result using DBExec_TabQry (return 0 if OK)
; Get an array of structures. Note that the DB connection is closed!
  status = DBExec_TabQry(dbname, user, password, query, Host=host, Debug=debug)

  IF (status NE 0) THEN RETURN  ; Error?

  PRINT, 'Info on the returned structure:'
  HELP, /STRU, tab_stru

  PRINT, 'First rows:'
  FOR i=0, MIN([10,n_rows-1]) DO BEGIN
    PRINT, i+1, " ", tab_stru[i]
  ENDFOR

; Call DBExec_ShowQry: this time get a structure of arrays
; Use common block saved (c_) info in case they where not given as input kwds
  PRINT, '... as an array of structures:'
  status = DBExec_ShowQry(c_dbname, c_user, c_password, query, Host=host, /ARR_STRU)

;  FOR i=0, MIN([10,n_rows-1]) DO BEGIN
;    row = ''
;    FOR j=0, n_fields-1 DO BEGIN
;      row += STRTRIM(tab_stru.(j)[i],2) + ' '
;    ENDFOR
;    PRINT, i+1, row, FORMAT='(I10, 2X, A)'
;  ENDFOR

END
