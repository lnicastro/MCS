@mcs_usrlib   ; User contributed Functions and Pro

PRO mcsdemo1, Host=host, User=user, Password=password, DBName=dbname, $
              Query=query, Debug=debug
;+
; NAME:
;       MCSDEMO1
;
; PURPOSE:
;   MCS demonstration program using the routines in the "mcs_usrlib" library.
;
;   Show usage of DBExec_Qry, TabStruct, TabFill, FieldInfo, DBFree and
;   transfer of a column of the output table into an array.
;   Here the output table is parsed into an array of structures (one
;   structure per row).
;
; CATEGORY:
;   MCS demonstration program using the mcs_usrlib library.
;
; CALLING SEQUENCE:
;   mcsdemo1 [, see keywords section]
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
;    TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
;             f_atype, f_itype, the_stru
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, July 2006.
;  19/09/2008: LN, changed to use mcs_usrlib v0.3b.
;-

COMMON TABinfo

; Keywords section (just check there is a query)
  IF (NOT KEYWORD_SET(query)) THEN BEGIN
    query = 'SELECT htmID,GSChtm FROM regions LIMIT 100'
    PRINT, 'Using query: ' + query
  ENDIF

; DB section
; Connect and execute the query using DBExec_Qry (return 0 if OK)
PRINT, SYSTIME()
  status = DBExec_Qry(dbname, user, password, query, Host=host)
  IF (status EQ 0) THEN BEGIN
    TabStruct, Debug=debug ; Build the table structure 'tab_stru'
    TabFill                ; Fill in 'tab_stru'
  ENDIF
PRINT, SYSTIME()

  PRINT, ""
  PRINT, "Fields info from FieldInfo:"

  FieldInfo, /INFO

  DBFree                   ; Free resources
; End DB section

  IF (status NE 0) THEN RETURN  ; Error?

  PRINT, 'Info on the returned structure:'
  HELP, /STRU, tab_stru
  PRINT, ''
  PRINT, 'Transfer first column into a vector "c1":'
  icol = 0
  c1 = tab_stru.(icol)

  HELP, c1
  PRINT, c1[0:3]

END
