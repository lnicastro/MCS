@mcs_usrlib   ; User contributed Functions and Pro

PRO mcsdemo2, Host=host, User=user, Password=password, DBName=dbname, $
              Query=query, Debug=debug
;+
; NAME:
;       MCSDEMO2
;
; PURPOSE:
;   MCS demonstration program using the routines in the "mcs_usrlib" library.
;
;   Show usage of DBExec_Qry, TabStruct, TabFill, FieldInfo,
;   TabFieldNames, TabFieldTypes, FieldNames, DBFree.
;   Here the query returned table is parsed into a structure of arrays
;   (one per field).
;
; CATEGORY:
;   MCS demonstration program using the mcs_usrlib library.
;
; CALLING SEQUENCE:
;   mcsdemo2 [, see keywords section]
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
    query = 'SELECT htmID,GSChtm FROM regions'
    PRINT, 'Using query: ' + query
  ENDIF

; DB section
; Connect and execute the query using DBExec_Qry (return 0 if OK)
  status = DBExec_Qry(dbname, user, password, query, Host=host)

  IF (status EQ 0) THEN BEGIN
    PRINT, "Fields info from FieldInfo:"
    FieldInfo, /INFO
    nf = TabFieldNames('regions', fn)
    PRINT, "Fields info from TabFieldNames:", fn
    nf = TabFieldTypes('regions', ft)
    PRINT, "Fields info from TabFieldTypes:", ft
    FieldNames, /INFO
; Build and fill the table structure 'tab_stru' as a structure of arrays
    TabStruct, /ARR_STRUCT, Debug=debug
    TabFill
  ENDIF
  DBFree                        ; Free resources

  IF (status NE 0) THEN RETURN  ; Error?

  PRINT, 'Info on the returned structure:'
  HELP, tab_stru
  HELP, /STRU, tab_stru
  PRINT, ''
  PRINT, 'First 4 elements of the structure:'
  PRINT, f_name[0], tab_stru.(0)[0:3]
  IF (n_fields GT 1) THEN PRINT, f_name[1], tab_stru.(1)[0:3]

END
