@mcs_healplib   ; MCS-Base and MCS-HEALPix user contributed Functions and Pro

PRO mcshpdemo3, Host=host, User=user, Password=password, DBName=dbname, $
    Query=query, $
    TABName=tabname, Order=order, $
    OutFile=outfile, Units=units, Gnomic=gnomic, Debug=debug, $
; xxxView passed options
    PNG=png, _EXTRA=e
;+
; NAME:
;       MCSHPDEMO3
;
; PURPOSE:
;   MCS-DIF-HEALPix demo program which shows how to obtain a map with the
;   number of occurences per pixel of a WHERE filtered query. Here using a
;   table which ONLY has HTM index(es).
;
;   Show usage of DBExec_Qry and healp_MapFill, healp_MapSave.
;
; CATEGORY:
;   MCS-DIF demonstration program using the mcs_usrlib/mcs_healplib libraries.
;
; CALLING SEQUENCE:
;   mcshpdemo3 [, see keywords section]
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;      Host: The DB server host name (def. localhost).
;      User: The user name to use for DB access.
;  Password: The password to use for DB access.
;    DBName: The database name to access.
;     Query: The complete query string (see code for def.).
;            Note that it has to produce a one column output!
;   TABName: The reference table name (i.e. without the "_htm" or "_healp"
;            extensions). Here only used for the plot title!
;     Order: The map Order (resolution level). Here only used for the output
;            map.
;   OutFile: If set then save in that file (def. into "mcsdb_map.fits").
;     Units: Units of output data (used for the plot).
;    Gnomic: If set then plot Gnomic view (see gnomview help).
;     Debug: If set then print some debug info.
;       PNG: If set then save map into PNG files (see mollview help).
;
; Any extra keyword is passed to mollview / orthview / gnomview.
;
; NOTES:
;   To use the default query, the HEALPix indexed table "UCAC_2orig" must be
;   present in the selected database (can get it from ross.iasfbo.inaf.it/MCS/).
;   Here we assume tables are DIF indexed with HTM depth=6 and HEALPix order=8,
;   schema=NESTED.
;
;   Requires the HEALPix IDL library (see healpix.jpl.nasa.gov/).
;
;   It is assumed that the query output just 1 column, i.e. the pixel IDs.
;   In case constant factors are used in the query, then it must be in
;   exponential notation (this is for MySQL - MCS communication reasons)!
;   For example: SELECT healpID, Mag1/1E2 FROM ....
;
; COMMON BLOCKS:
;   Note that all the info are passed using COMMON blocks and typically
;   in a main PRO need to include HEALPmap (see below).
;
;    HEALPmap, hp_nested, hp_nside, hp_order, hp_npix, $
;              hp_emptypix, hp_npixok, hp_map_nocc, hp_map_sum
;
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, September 2007.
;  01/08/2008: LN: changed to use mcs_usrlib v0.3a.
;  03/10/2008: LN: changed to use the new mcs_usrlib/mcs_healplib v0.3b.
;  22/03/2009: LN: added NESTED kwd to mollview, orthview, gnomview call.
;  22/06/2010: LN: DIF v. 0.5.2 compatibility.
;  08/09/2010: LN: assume test catalogues were DIF indexed with HTM depth=6.
;-

COMMON HEALPmap, hp_nested, hp_nside, hp_order, hp_npix, $
                 hp_emptypix, hp_npixok, hp_map_nocc, hp_map_sum

;Error handler
  CATCH, Error_status
  IF Error_status NE 0 THEN BEGIN
    PRINT, !ERROR_STATE.MSG
    DBClose
    RETURN
  ENDIF

; Keywords section
  IF (NOT KEYWORD_SET(query)) THEN BEGIN
    tabname = 'UCAC_2orig'
    query = 'SELECT HEALPLookup(0,8,RAcs/3.6e5,DECcs/3.6e5) as kpx8 FROM UCAC_2orig_htm_6 WHERE DIF_Rect(10, 10, 120)'
    PRINT, 'Query: ', query
  ENDIF

; Here we "assume" the table is ONLY indexed using HTM !
; Order is then only used to define the "output map" pixel size.
  IF (NOT KEYWORD_SET(order)) THEN $
    hp_order = 8 $
  ELSE $
    hp_order = order

; No need to define
  hp_nested = 0

; Open DB conn. and execute the query: return 0 if OK
  err = DBExec_Qry(dbname, user, password, query, Host=host)

  IF (err EQ 0) THEN BEGIN
    err = healp_MapFill(/INFO)
    IF (hp_npixok EQ 0) THEN $
      err = 1  ; Error
  ENDIF
  DBClose                         ; Free resources

  IF (err NE 0) THEN RETURN  ; Error?

; Units and plot title
  IF (NOT KEYWORD_SET(units)) THEN $
     units = 'Nr of entries'
  IF (NOT KEYWORD_SET(tabname)) THEN $
    tabname = 'UCAC_2orig'
  ptitle = 'MCS - DIF demo plot: '+ tabname

; Some views: see mollview help (IDL> doc_library,'mollview')
;  mollview, hp_map_nocc, /online, graticule=[30,30], coord=['C','C'], colt=6, $
;    GIF=gif, PNG=png
;  orthview, hp_map_nocc, /online, graticule=[30,30], coord=['C','C'], colt=6, $
;    GIF=gif, PNG=png

  IF (KEYWORD_SET(gnomic)) THEN BEGIN
    gnomview, hp_map_nocc, /GRATICULE, COORD=['C','C'], $
              TITLE=ptitle, UNITS=units, NESTED=hp_nested, $
              PNG=png, _EXTRA=e
  ENDIF ELSE BEGIN
    mollview, hp_map_nocc, /GRATICULE, COORD=['C','C'], $
              TITLE=ptitle, UNITS=units, NESTED=hp_nested, $
              PNG=png, _EXTRA=e
    orthview, hp_map_nocc, /GRATICULE, COORD=['C','C'], ROT=[30,30], $
              TITLE=ptitle, UNITS=units, NESTED=hp_nested, $
              PNG=png, _EXTRA=e
  ENDELSE

PRINT, 'There are '+ STRTRIM(hp_npixok, 1) +' not empty pixels.'

; List of non empty pixels
;  idxs = where(hp_map_nocc gt 0,nfill)
;print, 'There are '+ strtrim(nfill,1) +' not empty pixels.'
;print, idxs
;print, hp_map_nocc[idxs]

  IF (KEYWORD_SET(outfile)) THEN BEGIN
    PRINT, 'Saving into file.'
    err = healp_MapSave(OUTFILE=outfile, UNITS=units)
  ENDIF

END
