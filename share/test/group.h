// ----------------------------------------------------------------------^
// Copyright (C) 2004, 2005, 2006, 2007, 2008 Giorgio Calderone
// (mailto: <gcalderone@ifc.inaf.it>)
// 
// This file is part of MCS.
// 
// MCS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// MCS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with MCS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 
// ----------------------------------------------------------------------$
#define MAX_HDU_TRACKER 1000

typedef struct _HDUtracker HDUtracker;

struct _HDUtracker
{
  int nHDU;

  char *filename[MAX_HDU_TRACKER];
  int  position[MAX_HDU_TRACKER];

  char *newFilename[MAX_HDU_TRACKER];
  int  newPosition[MAX_HDU_TRACKER];
};

/* functions used internally in the grouping convention module */

int ffgtdc(int grouptype, int xtensioncol, int extnamecol, int extvercol,
	   int positioncol, int locationcol, int uricol, char *ttype[],
	   char *tform[], int *ncols, int  *status);

int ffgtgc(fitsfile *gfptr, int *xtensionCol, int *extnameCol, int *extverCol,
	   int *positionCol, int *locationCol, int *uriCol, int *grptype,
	   int *status);

int ffgmul(fitsfile *mfptr, int rmopt, int *status);

int ffgmf(fitsfile *gfptr, char *xtension, char *extname, int extver,	
	  int position,	char *location,	long *member, int *status);

int ffgtrmr(fitsfile *gfptr, HDUtracker *HDU, int *status);

int ffgtcpr(fitsfile *infptr, fitsfile *outfptr, int cpopt, HDUtracker *HDU,
	    int *status);

int fftsad(fitsfile *mfptr, HDUtracker *HDU, int *newPosition,
	   char *newFileName);

int fftsud(fitsfile *mfptr, HDUtracker *HDU, int newPosition,
	   char *newFileName);

void prepare_keyvalue(char *keyvalue);

int fits_path2url(char *inpath, char *outpath, int  *status);

int fits_url2path(char *inpath, char *outpath, int  *status);

int fits_get_cwd(char *cwd, int *status);

int fits_get_url(fitsfile *fptr, char *realURL, char *startURL,
		 char *realAccess, char *startAccess, int *iostate,
		 int *status);

int fits_clean_url(char *inURL, char *outURL, int *status);

int fits_relurl2url(char *refURL, char *relURL, char *absURL, int *status);

int fits_url2relurl(char *refURL, char *absURL, char *relURL, int *status);

int fits_encode_url(char *inpath, char *outpath, int *status);

int fits_unencode_url(char *inpath, char *outpath, int *status);

int fits_is_url_absolute(char *url);

