@mcs_usrlib  ; MCS-Base user contributed Functions and Pro

;+
; Library allowing simple HEALPix map filling and saving by reading from
; DB tables via MCS and DIF.
; See help for each routine.
;
; v0.4a: Work in progress...
;
; NOTES:
;   This library already include the MCS-Base user contributed lib "mcs_usrlib"
;   which means that a user program only needs to put at the top of its code
;   "@mcs_healplib".
;   The majority of the routines of this library use COMMON blocks to pass
;   information:
;
;  COMMON  HEALPmap, hp_nested, hp_nside, hp_order, hp_npix, $
;                   hp_emptypix, hp_npixok, hp_map_nocc, hp_map_sum
;
;  See routines help for returned output.
;
;  Typically users' main PRO do NOT need to include the mcs_usrlib common
;  blocks but only the HEALPix specific common "HEALPmap" (see demo programs).
;  Still users can include "DBinfo" and "TABinfo" for convenience.
;
;  We use the word "Table" to identify the 2-d data returned by a SELECT query.
;  Otherwise we use DB table (or similar) to identify the database table on
;  which the query is performed.
;
; DEPENDENCIES:
;   The MCS package (http://ross.iasfbo.inaf.it/mcs) must be compiled with the
;   IDL interface enabled (--enable-idl or --with-idl) and installed.
;
;   The DIF package (http://ross.iasfbo.inaf.it/dif) must be compiled,
;   installed and applied to the database table being used with the
;   library.
;
;   The HEALPix IDL library (see healpix.jpl.nasa.gov/).
;
;   MCS version required: 0.3.3-alpha2 or above.
;   DIF version required: 0.4.0 or above.
;
; VERSION HISTORY:
;   0.1a: 12-Sep-2007
;   0.1b: 08-Nov-2007
;   0.1c: 03-Jul-2008
;   0.2a: 01-Aug-2008
;   0.3b: 05-Oct-2008
;   0.4a: 17-Jul-2009
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, September 2007.
;
;  12/09/2007: Added FILENAME kwd in save_map.
;  08/11/2007: Added hp_nested in common HEALPmap.
;  06/03/2008: minor fixes.
;  03/07/2008: changed getHEALPDepth into getHEALPOrder
;  01/08/2008: changed to use the new mcs_usrlib v0.3a
;  03/10/2008: changed to use the new mcs_usrlib v0.3b
;  22/03/2009: changed healp_MapSave to manage UNSIGNED (see FXBADDCOL
;              called in write_fits_map)
;  17/07/2009: LN, v0.4a: chamged healp_MapNside for DIF v. 0.5.0 compatibility.
;  22/06/2010: LN, v0.4a: better management of entries not found in dif table.
;              Use negative hp_nested to mark DIF unmanaged tables.
;  17/09/2010: healp_MapFill: fixed check of 2nd field type for float/double
;              initialization and added FLTARRAY kwd.
;-


FUNCTION healp_Nside2Order, Nside
;+
; NAME:
;       HEALP_NSIDE2ORDER
;
; PURPOSE:
;   Get the map Order (resolution level) from Nside.
;
; NOTES:
;   Assume Max order = 29, i.e max nside=536.870.912 for 64 bits integers.
;   (it was Max order = 13, i.e max nside=8192 for 32 bits integers)
;
; CATEGORY:
;   MCS-DIF-HEALPix contributed library.
;
; CALLING SEQUENCE:
;   Result = Nside2Order(Nside)
;
; INPUT PARAMETERS:
;   Nside: the map Nside
;
; INPUT KEYWORDS:
;   None.
;
; OUTPUT PARAMETERS:
;   Nnone.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   Return the map Order (integer value).
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, September 2007.
;  05/10/2008: LN, v0.3b: order_max = 29
;-

;  order_max = 13  ; max nside=2^13=8192 for 32 bits integers
  order_max = 29  ; max nside=2^29 for 64 bits integers
  FOR order=0, order_max DO BEGIN
    ntest = ISHFT(1L,order)
    IF (nside EQ ntest) THEN RETURN, order
    IF (nside LT ntest) THEN RETURN, -1
  ENDFOR

  RETURN, -1
END


FUNCTION healp_MapNside, TABName
;+
; NAME:
;       HEALP_MAPNSIDE
;
; PURPOSE:
;   Set map Order, Nside and Npix. Also get the scheme (NESTED/RING).
;   Read Order using the DIF function "getHEALPOrder" and scheme using
;   "getHEALPNested".
;
; NOTES:
;   Uses a separate instance of Query object on the "dbc" connection.
;
;   As of DIF v. 0.5.0 multiple orders and scheme can exist for the same
;   table, so here the lowest order is assumed!
;
; CATEGORY:
;   MCS-DIF-HEALPix contributed library.
;
; CALLING SEQUENCE:
;    Result = healp_MapNside(TABName)
;
; INPUT PARAMETERS:
;    TABName: the reference DB table name.
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
;   Set hp_nested, hp_nside, hp_order, hp_npix (in common block "HEALPmap").
;
;   Returns 0 on success.
;
; COMMON BLOCKS:
;   DBptr, DBinfo, HEALPmap
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, September 2007.
;  05/10/2008: LN, v0.3b: vars dimension changes and new error management.
;  17/07/2009: LN, v0.4a: DIF v. 0.5.0 compatibility.
;  22/06/2010: LN, v0.4a: better management of entries not found in dif table.
;              Use negative hp_nested to mark DIF unmanaged tables.
;-

COMMON DBptr, dbc, dbq
COMMON DBinfo, c_host, c_user, c_password, c_dbname, c_query

COMMON HEALPmap, hp_nested, hp_nside, hp_order, hp_npix, $
                 hp_emptypix, hp_npixok, hp_map_nocc, hp_map_sum

  hp_nested = -1 & hp_order = 0  &  hp_nside = 0  &  hp_npix = 0
  IF (N_ELEMENTS(tabname) LE 0) THEN $
    MESSAGE, 'ERR: no reference table name given.'

; Minimum order
  query = 'SELECT DIF.getHEALPOrder("'+ c_dbname +'","'+ tabname +'")'
  dbq1 = new_Query(ifd_null(), dbc, 0)
  Query_prepare, dbq1, query[0]
  Query_execute, dbq1, 1
  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    del_Query, dbq1
    MESSAGE, msg
  ENDIF

  if ( NOT Data_IsNull(Record_field(RecordSet_rec(dbq1), 0)) ) then begin
    hp_order = Data_lval(Record_field(RecordSet_rec(dbq1), 0))
    hp_nside = 2L^hp_order
    hp_npix = 12 * (hp_nside*1LL)^2


; Nested?
    query = 'SELECT DIF.getHEALPNested("'+ c_dbname +'","'+ tabname +'",' + $
            STRTRIM(hp_order,1) + ')'
;  dbq1 = new_Query(ifd_null(), dbc, 0)
    Query_prepare, dbq1, query[0]
    Query_execute, dbq1, 1
    IF (ifd_got_error()) THEN BEGIN
      msg = ifd_last_error()
      ifd_reset_error
      del_Query, dbq1
      MESSAGE, msg
    ENDIF

    hp_nested = Data_lval(Record_field(RecordSet_rec(dbq1), 0))
print, 'hp_nested, hp_order, hp_nside, hp_npix=',hp_nested,hp_order,hp_nside,hp_npix

 endif

; Free and destroy local query object
  Query_close, dbq1

  IF (ifd_got_error()) THEN BEGIN
    msg = ifd_last_error()
    ifd_reset_error
    MESSAGE, msg
  ENDIF
  del_Query, dbq1

  RETURN, 0
END


FUNCTION healp_Map_Nside, TABName
;+
; NAME:
;       HEALP_MAP_NSIDE
;
; PURPOSE:
;   Alias for HEALP_MAPNSIDE.
;-

  RETURN, healp_MapNside(TABName)
END


FUNCTION healp_MapFill, TABName, NORMPIX=normpix, EMPTYPIX=emptypix, $
         NANINIT=naninit, FLTARRAY=fltarray, INFO=info

;+
; NAME:
;       HEALP_MAPFILL
;
; PURPOSE:
;   Fill the map array(s) hp_map_nocc (N_entries/pixel) and, for 2 cols query
;   output, hp_map_sum (Sum_values/pixel) in common block "HEALPmap"
;   reading from the query unit.
;
; NOTE:
;   The arrays data types depend on the type of the query output.
;   It is assumed that 1 (HELAPix ID) or 2 columns (ID Value) are returned
;   by the query. The maps are the sum of all entries (but see kwds).
;   Empty pixels in hp_map_nocc (type integer) are set to 0.
;
; CATEGORY:
;   MCS-DIF-HEALPix contributed library.
;
; CALLING SEQUENCE:
;       Result = healp_MapFill( [TABName] )
;
; OPTIONAL INPUT PARAMETERS:
;   TABName: the reference DB table name (used to get map Nside).
;            Note that the DB connection must be open!
;
; INPUT KEYWORDS:
;   NORMPIX:  if set then normalize the Sum map by the Nocc map
;             (hp_map_sum[i_ok] /= hp_map_nocc[i_ok].
;   EMPTYPIX: if set then use this value for hp_emptypix (hp_map_sum empty
;             pixels marker).
;   NANINIT:  if set then initialize hp_emptypix to NaN (def. -1.6375e30).
;   FLTARRAY: if set then the hp_map_sum map is of type FLOAT (def. auto).
;   INFO:     If set then print some debug info.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   All in the common block "HEALPmap".
;   Set hp_map_nocc and hp_map_sum (for 2 cols query output).
;   If TABName not passed then set hp_nside, hp_npix from hp_order.
;   If emptypix not passed then set hp_emptypix.
;
;   Return 0 on success.
;
; COMMON BLOCKS:
;   DBptr, DBinfo, TABinfo, HEALPmap
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, September 2007.
;  01/08/2008: LN, v0.2a: changed managment of the returned map type.
;  05/10/2008: LN, v0.3b: vars dimension changes and new error management.
;  17/09/2010: fixed check of 2nd field type for float/double initialization
;              and added FLTARRAY kwd.
;-

COMMON DBptr, dbc, dbq
COMMON DBinfo, c_host, c_user, c_password, c_dbname, c_query
COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

COMMON HEALPmap, hp_nested, hp_nside, hp_order, hp_npix, $
                 hp_emptypix, hp_npixok, hp_map_nocc, hp_map_sum

; Get map Nside
  IF (N_ELEMENTS(tabname) GT 0) THEN BEGIN
    err = healp_map_nside(tabname)
    IF (err) THEN $
      MESSAGE, 'ERR: "healp_map_nside" returned an error.'
  ENDIF ELSE BEGIN
    IF (N_ELEMENTS(hp_order) LE 0) THEN $
      MESSAGE, 'ERR: Order is not set and no table name given.'
;hp_order = healp_nside2order(hp_nside)
    hp_nside = 2L^hp_order
    hp_npix = 12 * (hp_nside*1LL)^2
  ENDELSE


; Check for empty pixel marker value. Def. = 0
  IF KEYWORD_SET(emptypix) THEN $
    hp_emptypix = emptypix $
  ELSE BEGIN
    IF KEYWORD_SET(naninit) THEN $
      hp_emptypix = !values.f_nan $  ; initialize to NaN
    ELSE $
      hp_emptypix = -1.6375e30  ; HEALPix default
  ENDELSE

  IF (KEYWORD_SET(info)) THEN $
    MESSAGE, 'Map IsNested, Nside, Order, Npix: '+ strtrim(hp_nested,1) +', '+ $
      STRTRIM(hp_nside,1) +', '+ strtrim(hp_order,1) +', '+ $
      STRTRIM(hp_npix,1), /INFO

; Get query info setting default output data as a structure of arrays.
; This is not used here, but it would be more appropriate, in case...
  FieldInfo, /ARR_STRUCT, INFO=info

; Do not exit here to allow to get an empty map
  IF (n_rows EQ 0) THEN BEGIN
    MESSAGE, 'WRN: No rows selected by the query.', /INFO
    RETURN, 2
  ENDIF

  dummy = RecordSet_setFirst(dbq)

; Distinguish among different number of returned columns.
; Note that the Nocc/pix map array is always LONG whereas the Sum/pix map
; is always FLOAT unless a query DOUBLE output is detected.
  CASE n_fields OF
    1: BEGIN
      IF (KEYWORD_SET(info)) THEN $
        MESSAGE, 'One field in the query. Producing map with Nocc/pix.', /INFO
      j0 = 0
      IF (f_itype[j0] EQ mcs_types[4]) THEN $
        hp_map_nocc = LON64ARR(hp_npix) $ ; I*8 zero initialized
      ELSE IF (f_itype[j0] EQ mcs_types[13]) THEN $
        hp_map_nocc = ULON64ARR(hp_npix) $ ; unsigned I*8 zero initialized
      ELSE IF (f_itype[j0] EQ mcs_types[12]) THEN $
        hp_map_nocc = ULONARR(hp_npix) $ ; unsigned I*4 zero initialized
      ELSE $
        hp_map_nocc = LONARR(hp_npix)     ; I*4 zero initialized
      hp_map_sum = hp_emptypix
      CASE 1 OF
; TINY, SMALL, MEDIUM, INT
        (f_itype[j0] EQ mcs_types[0]) OR (f_itype[j0] EQ mcs_types[1]) OR $
        (f_itype[j0] EQ mcs_types[2]) OR (f_itype[j0] EQ mcs_types[3]): BEGIN
          FOR i=0L, n_rows-1 DO BEGIN
            ind = Data_ival(Record_field(RecordSet_rec(dbq),j0))
            IF (ind LT hp_npix) THEN $
              hp_map_nocc[ind] += 1
            dummy = RecordSet_setNext(dbq)
          ENDFOR
        END
        (f_itype[j0] EQ mcs_types[4]): BEGIN  ; LONG64
          FOR i=0L, n_rows-1 DO BEGIN
            ind = Data_lval(Record_field(RecordSet_rec(dbq),j0))
            IF (ind LT hp_npix) THEN $
              hp_map_nocc[ind] += 1
            dummy = RecordSet_setNext(dbq)
          ENDFOR
        END
        (f_itype[j0] EQ mcs_types[12]): BEGIN  ; ULONG
          FOR i=0L, n_rows-1 DO BEGIN
            ind = Data_uival(Record_field(RecordSet_rec(dbq),j0))
            IF (ind LT hp_npix) THEN $
              hp_map_nocc[ind] += 1
            dummy = RecordSet_setNext(dbq)
          ENDFOR
        END
        (f_itype[j0] EQ mcs_types[13]): BEGIN  ; ULONG64
          FOR i=0L, n_rows-1 DO BEGIN
            ind = Data_ulval(Record_field(RecordSet_rec(dbq),j0))
            IF (ind LT hp_npix) THEN $
              hp_map_nocc[ind] += 1
            dummy = RecordSet_setNext(dbq)
          ENDFOR
        END
        (f_itype[j0] EQ mcs_types[5]): BEGIN  ; FLOAT
          MESSAGE, 'ERR: Data type of first field is '+ f_atype[j0]
;          RETURN, 3
        END
        (f_itype[j0] EQ mcs_types[6]): BEGIN  ; DOUBLE
          MESSAGE, 'ERR: Data type of first field is '+ f_atype[j0]
;          RETURN, 3
        END
      ENDCASE

      END

    2: BEGIN
      IF (KEYWORD_SET(info)) THEN $
        MESSAGE, 'Two fields in the query. Producing map with Nocc/pix and Sum/pix.', /INFO
      j0 = 0
      j1 = 1
      hp_map_nocc = LONARR(hp_npix)   ; all zero initialized
      IF (NOT KEYWORD_SET(fltarrary) AND (f_itype[j1] EQ mcs_types[6])) THEN $
        hp_map_sum = DBLARR(hp_npix) $
      ELSE $
        hp_map_sum = FLTARR(hp_npix)

      CASE 1 OF
        (f_itype[j0] EQ mcs_types[0]) OR (f_itype[j0] EQ mcs_types[1]) OR $
        (f_itype[j0] EQ mcs_types[2]) OR (f_itype[j0] EQ mcs_types[3]): BEGIN
        FOR i=0L, n_rows-1 DO BEGIN
          ind = Data_ival(Record_field(RecordSet_rec(dbq),j0))
          IF (ind LT hp_npix) THEN BEGIN
            hp_map_nocc[ind] += 1
            hp_map_sum[ind] += Data_dval(Record_field(RecordSet_rec(dbq),j1))
          ENDIF
          dummy = RecordSet_setNext(dbq)
        ENDFOR
        END
        (f_itype[j0] EQ mcs_types[4]): BEGIN
          FOR i=0L, n_rows-1 DO BEGIN
            ind = Data_lval(Record_field(RecordSet_rec(dbq),j0))
            IF (ind LT hp_npix) THEN BEGIN
              hp_map_nocc[ind] += 1
              hp_map_sum[ind] += Data_dval(Record_field(RecordSet_rec(dbq),j1))
            ENDIF
            dummy = RecordSet_setNext(dbq)
          ENDFOR
        END
        (f_itype[j0] EQ mcs_types[12]): BEGIN  ; ULONG
          FOR i=0L, n_rows-1 DO BEGIN
            ind = Data_uival(Record_field(RecordSet_rec(dbq),j0))
            IF (ind LT hp_npix) THEN BEGIN
              hp_map_nocc[ind] += 1
              hp_map_sum[ind] += Data_dval(Record_field(RecordSet_rec(dbq),j1))
            ENDIF
            dummy = RecordSet_setNext(dbq)
          ENDFOR
        END
        (f_itype[j0] EQ mcs_types[13]): BEGIN  ; ULONG64
          FOR i=0L, n_rows-1 DO BEGIN
            ind = Data_ulval(Record_field(RecordSet_rec(dbq),j0))
            IF (ind LT hp_npix) THEN BEGIN
              hp_map_nocc[ind] += 1
              hp_map_sum[ind] += Data_dval(Record_field(RecordSet_rec(dbq),j1))
            ENDIF
            dummy = RecordSet_setNext(dbq)
          ENDFOR
        END
        (f_itype[j0] EQ mcs_types[5]): BEGIN
          MESSAGE, 'ERR: Data type of first field is '+ f_atype[j0]
;          RETURN, 3
        END
        (f_itype[j0] EQ mcs_types[6]): BEGIN
          MESSAGE, 'ERR: Data type of first field is '+ f_atype[j0]
;          RETURN, 3
        END
      ENDCASE

      end
  ELSE: BEGIN
      hp_map_nocc = 0
      hp_map_sum = hp_emptypix
      MESSAGE, 'ERR: Nr of fields in the query not managed: '+ $
               STRTRIM(n_fields,1)
    END
  ENDCASE

  dummy = RecordSet_setFirst(dbq)

; Normalization?
  i_ok = WHERE(hp_map_nocc GT 0, hp_npixok)
  IF (KEYWORD_SET(normpix)) THEN BEGIN
    IF (hp_npixok GT 0) THEN hp_map_sum[i_ok] /= hp_map_nocc[i_ok]
  ENDIF ELSE i_ok = 0

; Mark empty pixels.
  i_bad = WHERE(hp_map_nocc EQ 0, n_bad)
  IF (n_bad GT 0) THEN hp_map_sum[i_bad] = hp_emptypix

  IF (KEYWORD_SET(info)) THEN $
    MESSAGE, 'Map has '+ STRTRIM(hp_npixok,1) +' not empty pixels.', /INFO

  RETURN, 0
END


FUNCTION healp_MapSave, OUTFILE=outfile, SUMSAVE=sumsave, UNITS=units, $
         COMMENT=comment
;+
; NAME:
;       HEALP_MAPSAVE
;
; PURPOSE:
;   Save into a FITS file hp_map_nocc (N_entries/pixel) or hp_map_sum
;   (Sum_values/pixel) if kwds SUMSAVE is set.
;
; NOTE:
;   The arrays data types depend on the type of the query output.
;   It is assumed that 1 (HELAPix ID) or 2 columns (ID Value) are returned
;   by the query. The maps are the sum of all entries (but see kwds).
;   Empty pixels in hp_map_nocc (type integer) are set to 0.
;
; CATEGORY:
;   MCS-DIF-HEALPix contributed library.
;
; CALLING SEQUENCE:
;   Result = healp_MapSave( )
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   OUTFILE: if set then save in that file (def. into "mcsdb_map.fits").
;   SUMSAVE: if set then save the Sum map "hp_map_sum" rather than
;            "hp_map_nocc" (def.).
;   UNITS:   units of the data set.
;   COMMENT: comment to add to the file header.
;
; OUTPUT PARAMETERS:
;   None.
;
; OUTPUT KEYWORDS:
;   None.
;
; OUTPUTS:
;   Return 0 on success.
;
; COMMON BLOCKS:
;   HEALPmap
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, September 2007.
;  22/03/2009: LN, v0.3c: convert UNSIGNED to SIGNED for a lack of management
;              in FXBxxx routines of such arrays.
;-

COMMON TABinfo, mcs_types, is_rowstru, n_fields, n_rows, f_name, $
                f_atype, f_itype, tab_stru

COMMON HEALPmap, hp_nested, hp_nside, hp_order, hp_npix, $
                 hp_emptypix, hp_npixok, hp_map_nocc, hp_map_sum

  IF (hp_npixok LE 0) THEN BEGIN
    MESSAGE, 'ERR: no good pixel in the map.', /INFO
    RETURN, 1
  ENDIF

  def_fout = 'mcsdb_map.fits'
  IF (KEYWORD_SET(outfile)) THEN BEGIN
    IF ((SIZE(outfile))[1] EQ 7) THEN $
      fout = outfile $
    ELSE $
      fout = def_fout

  ENDIF ELSE $
    fout = def_fout

; Initial empty header. Just put the file name.
  info_header = STRARR(2)
  tmp_head = STRING(REPLICATE(32b,80))     ; empty line
  STRPUT, tmp_head, "FILENAME= '"+ fout +"' /"
  info_header[0] = tmp_head
  info_header[1] = 'END' + STRING(REPLICATE(32b,77))   ; END line

;add_ordering_fits, info_header, /RING
;add_nside_fits, info_header, nside=hp_nside, obs_npix=hp_npix
;add_coordsys_fits, info_header, COORDSYS='C'

  IF (KEYWORD_SET(comment)) THEN $
    sxaddpar, info_header, 'COMMENT', comment

  IF (hp_nested) THEN $
    ord = 'NESTED' $
  ELSE $
    ord = 'RING'
  IF (KEYWORD_SET(sumsave)) THEN $
    write_fits_map, fout, hp_map_sum, info_header, $
                    COORDSYS='C', ORDERING=ord, UNITS=units $
  ELSE BEGIN
; Manage unsigned (FXBADDCOL missing type!). Note the data type change.
;    IF (f_itype[0] EQ mcs_types[12]) THEN BEGIN    ; unsigned I*4
;      hp_map_nocc = LONG(TEMPORARY(hp_map_nocc))
;      f_itype[0] = mcs_types[3]
;      f_atype[0] = 'LONGARR'
;    ENDIF ELSE BEGIN
;      IF (f_itype[0] EQ mcs_types[13]) THEN BEGIN  ; unsigned I*8
;        hp_map_nocc = LONG64(TEMPORARY(hp_map_nocc))
;        f_itype[0] = mcs_types[4]
;        f_atype[0] = 'LON64ARR'
;      ENDIF
;    ENDELSE

    write_fits_map, fout, hp_map_nocc, info_header, $
                    COORDSYS='C', ORDERING=ord, UNITS=units
  ENDELSE

  MESSAGE, "Saved map info file '"+ fout +"'", /INFO
  RETURN, 0
END
