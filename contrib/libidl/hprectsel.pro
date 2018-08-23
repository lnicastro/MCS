@mcs_healplib   ; MCS-Base and MCS-HEALPix user contributed Functions and Pro

pro hprectsel, TABName=tabname, Rect=rect, OutFile=outfile, $
; xxxView passed options
    _EXTRA=e
;+
; ; NAME:
;       HPRECTSEL
;
; PURPOSE:
;   Show rectangular selection of objects on a given table.
;   Show usage of "dbexec_tabqry" and "healp_mapfill".
;
; CALLING SEQUENCE:
;   hprectsel [, TABNAME=tabname, RECT=rect]
;
; INPUT PARAMETERS:
;   None.
;
; INPUT KEYWORDS:
;   TABName: The reference table name (i.e. without the "_htm" or "_healp"
;            extensions). Must be HTM indexed. Def. 'UCAC_2orig'.
;      Rect: a 4 elements array with the corner of the rectangle:
;            [RAmin,DECmin,RAmax,DECmax]. Def. [10,30,240,120] (degrees) 
;   OutFile: If set then save in that file (def. into "mcsdb_map.fits").
;
; Any extra keyword is passed to "gnomview".
;
; COMMON BLOCKS:
;   Note that all the info are passed using COMMON blocks and typically
;   in a main PRO need to include HEALPmap (see below).
;
;    HEALPmap, hp_nested, hp_nside, hp_order, hp_npix, $
;              hp_emptypix, hp_npixok, hp_map_nocc, hp_map_s
;
; MODIFICATION HISTORY:
;   Written by L. Nicastro @ IASF-INAF, September 2007.
;  01/08/2008: LN: changed to use mcs_usrlib v0.3a.
;  03/10/2008: LN: changed to use the new mcs_usrlib/mcs_healplib v0.3b.
;  17/07/2009: LN: DIF v. 0.5.0 compatibility.
;  22/06/2010: LN: DIF v. 0.5.2 compatibility.
;  08/09/2010: LN: does not assume input catalogues were HEALPix indexed.
;-

COMMON HEALPmap, hp_nested, hp_nside, hp_order, hp_npix, $
                 hp_emptypix, hp_npixok, hp_map_nocc, hp_map_s

  IF (NOT KEYWORD_SET(tabname)) THEN $
    tabname = 'UCAC_2orig'
  htmtab = tabname + '_htm'

  IF (NOT KEYWORD_SET(rect)) THEN $
    rect = [10,30,240,120]

; Change DB access info below
  host = 'localhost'
  user = 'generic'
  password = 'password'
  dbname = 'RossCats'
;--

  ra = STRTRIM(rect[0],1)
  de = STRTRIM(rect[1],1)
  sra = STRTRIM(rect[2],1)
  sde = STRTRIM(rect[3],1)

  hp_order = 8
  hp_nested = 0
  query = 'SELECT HEALPLookup('+ $
          STRTRIM(hp_nested,1) +','+ STRTRIM(hp_order,1) + $
          ', RAcs/3.6e5,DECcs/3.6e5) as kpx8 FROM '+ htmtab + $
          ' WHERE DIF_Rect('+ ra +','+ de +','+ sra +','+ sde +')'

  PRINT,'Query: ',query

; Open DB conn. and execute the query: return 0 if OK
  err = DBExec_Qry(dbname, user, password, query, Host=host)

; Get/set Nside
; Note: As of DIF v. 0.5.0 multiple orders can exist for the same
;       table, so here the lowest order is assumed, if not given!
;  err = healp_MapNside(tabname)
  
  IF (err EQ 0) THEN BEGIN
    err = healp_MapFill(/INFO)
    IF (hp_npixok EQ 0) THEN $
      err = 1  ; Error
  ENDIF
  DBClose                         ; Free resources

  IF (err NE 0) THEN RETURN  ; Error?


  hside_degra = rect[2]/120./COS(rect[1]*!PI/180.)
  r1d = rect[0] - hside_degra
  r2d = rect[0] + hside_degra
  d1d = rect[1] - rect[3]/120.
  d2d = rect[1] + rect[3]/120.
  outra = [r1d,r2d,r2d,r1d,r1d]
  outde = [d1d,d1d,d2d,d2d,d1d]
  rotd = [rect[0],rect[1]]

PRINT, 'hside_degra=',hside_degra
PRINT, 'outra=',outra
PRINT, 'outde=',outde

; Units and plot title
  units = 'Nr of entries'
  ptitle = 'MCS - DIF demo plot: '+ tabname

; View
  gnomview, hp_map_nocc, /GRATICULE, COORD=['C','C'], $
            TITLE=ptitle, UNITS=units, NESTED=hp_nested, $
            OUTLINE={coord:'c', ra:outra, dec:outde, line:2}, $
            ROT=rotd, $
            _EXTRA=e

  IF (KEYWORD_SET(outfile)) THEN BEGIN
    PRINT, 'Saving into file.'
    err = healp_mapsave(OUTFILE=outfile, UNITS=units)
  ENDIF

END
