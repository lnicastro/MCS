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
int ffopen(fitsfile **fptr,      /* O - FITS file pointer                   */
           const char *name,     /* I - full name of file to open           */
           int mode,             /* I - 0 = open readonly; 1 = read/write   */
           int *status)          /* IO - error status                       */
/*
  Open an existing FITS file with either readonly or read/write access.
*/
{
    fitsfile *newptr;
    int  driver, hdutyp, hdunum, slen, writecopy, isopen;
    LONGLONG filesize;
    long rownum, nrows, goodrows;
    int extnum, extvers, handle, movetotype, tstatus = 0;
    char urltype[MAX_PREFIX_LEN], infile[FLEN_FILENAME], outfile[FLEN_FILENAME];
    char origurltype[MAX_PREFIX_LEN], extspec[FLEN_FILENAME];
    char extname[FLEN_VALUE], rowfilter[FLEN_FILENAME], tblname[FLEN_VALUE];
    char imagecolname[FLEN_VALUE], rowexpress[FLEN_FILENAME];
    char binspec[FLEN_FILENAME], colspec[FLEN_FILENAME], pixfilter[FLEN_FILENAME];
    char histfilename[FLEN_FILENAME];
    char filtfilename[FLEN_FILENAME];
    char wtcol[FLEN_VALUE];
    char minname[4][FLEN_VALUE], maxname[4][FLEN_VALUE];
    char binname[4][FLEN_VALUE];

    char *url;
    double minin[4], maxin[4], binsizein[4], weight;
    int imagetype, naxis = 1, haxis, recip;
    int skip_null = 0, skip_image = 0, skip_table = 0, open_disk_file = 0;
    char colname[4][FLEN_VALUE];
    char errmsg[FLEN_ERRMSG];
    char *hdtype[3] = {"IMAGE", "TABLE", "BINTABLE"};
    char *rowselect = 0;

    if (*status > 0)
        return(*status);

    if (*status == SKIP_NULL_PRIMARY)
    {
      /* this special status value is used as a flag by ffdopn to tell */
      /* ffopen to skip over a null primary array when opening the file. */

       skip_null = 1;
       *status = 0;
    }
    else if (*status == SKIP_IMAGE)
    {
      /* this special status value is used as a flag by fftopn to tell */
      /* ffopen to move to 1st significant table when opening the file. */

       skip_image = 1;
       *status = 0;
    }
    else if (*status == SKIP_TABLE)
    {
      /* this special status value is used as a flag by ffiopn to tell */
      /* ffopen to move to 1st significant image when opening the file. */

       skip_table = 1;
       *status = 0;
    }
    else if (*status == OPEN_DISK_FILE)
    {
      /* this special status value is used as a flag by ffdkopn to tell */
      /* ffopen to not interpret the input filename using CFITSIO's    */
      /* extended filename syntax, and simply open the specified disk file */

       open_disk_file = 1;
       *status = 0;
    }

    *fptr = 0;              /* initialize null file pointer */
    writecopy = 0;  /* have we made a write-able copy of the input file? */

    if (need_to_initialize) {          /* this is called only once */

       if (need_to_initialize != 1) {
	  /* This is bad. looks like memory has been corrupted. */
	  ffpmsg("Vital CFITSIO parameters held in memory have been corrupted!!");
	  ffpmsg("Fatal condition detected in ffopen.");
	  *status = FILE_NOT_OPENED;
	  return(*status);
       }
	
       *status = fits_init_cfitsio();
    }

    if (*status > 0)
        return(*status);

    url = (char *) name;
    while (*url == ' ')  /* ignore leading spaces in the filename */
        url++;

    if (*url == '\0')
    {
        ffpmsg("Name of file to open is blank. (ffopen)");
        return(*status = FILE_NOT_OPENED);
    }

    if (open_disk_file)
    {
      /* treat the input URL literally as the name of the file to open */
      /* and don't try to parse the URL using the extended filename syntax */

        strcpy(infile,url);
        strcpy(urltype, "file://");
        outfile[0] = '\0';
        extspec[0] = '\0';
        binspec[0] = '\0';
        colspec[0] = '\0';
        rowfilter[0] = '\0';
        pixfilter[0] = '\0';
    }
    else
    {
        /* parse the input file specification */
        fits_parse_input_filename(url, urltype, infile, outfile, extspec,
              rowfilter, binspec, colspec, pixfilter, status);
    }

    if (*status > 0)
    {
        ffpmsg("could not parse the input filename: (ffopen)");
        ffpmsg(url);
        return(*status);
    }

    imagecolname[0] = '\0';
    rowexpress[0] = '\0';

    if (*extspec)
    {
       /* parse the extension specifier into individual parameters */
       ffexts(extspec, &extnum,
         extname, &extvers, &movetotype, imagecolname, rowexpress, status);

      if (*status > 0)
          return(*status);
    }

    /*-------------------------------------------------------------------*/
    /* special cases:                                                    */
    /*-------------------------------------------------------------------*/

    histfilename[0] = '\0';
    filtfilename[0] = '\0';
    if (*outfile && (*binspec || *imagecolname || *pixfilter))
    {
        /* if binspec or imagecolumn are specified, then the  */
        /* output file name is intended for the final image,  */
        /* and not a copy of the input file.                  */
        strcpy(histfilename, outfile);
        outfile[0] = '\0';
    }
    else if (*outfile && (*rowfilter || *colspec))
    {
        /* if rowfilter or colspece are specified, then the    */
        /* output file name is intended for the filtered file  */
        /* and not a copy of the input file.                   */
        strcpy(filtfilename, outfile);
        outfile[0] = '\0';
    }

    /*-------------------------------------------------------------------*/
    /* check if this same file is already open, and if so, attach to it  */
    /*-------------------------------------------------------------------*/

    if (fits_already_open(fptr, url, urltype, infile, extspec, rowfilter,
            binspec, colspec, mode, &isopen, status) > 0)
    {
        return(*status);
    }

    if (isopen)
       goto move2hdu;

    /* get the driver number corresponding to this urltype */
    *status = urltype2driver(urltype, &driver);

    if (*status > 0)
    {
        ffpmsg("could not find driver for this file: (ffopen)");
        ffpmsg(urltype);
        ffpmsg(url);
        return(*status);
    }

    /*-------------------------------------------------------------------
        deal with all those messy special cases which may require that
        a different driver be used:
            - is disk file compressed?
            - are ftp:, gsiftp:, or http: files compressed?
            - has user requested that a local copy be made of
              the ftp or http file?
      -------------------------------------------------------------------*/

    if (driverTable[driver].checkfile)
    {
        strcpy(origurltype,urltype);  /* Save the urltype */

        /* 'checkfile' may modify the urltype, infile and outfile strings */
        *status =  (*driverTable[driver].checkfile)(urltype, infile, outfile);

        if (*status)
        {
            ffpmsg("checkfile failed for this file: (ffopen)");
            ffpmsg(url);
            return(*status);
        }

        if (strcmp(origurltype, urltype))  /* did driver changed on us? */
        {
            *status = urltype2driver(urltype, &driver);
            if (*status > 0)
            {
                ffpmsg("could not change driver for this file: (ffopen)");
                ffpmsg(url);
                ffpmsg(urltype);
                return(*status);
            }
        }
    }

    /* call appropriate driver to open the file */
    if (driverTable[driver].open)
    {
        *status =  (*driverTable[driver].open)(infile, mode, &handle);
        if (*status > 0)
        {
            ffpmsg("failed to find or open the following file: (ffopen)");
            ffpmsg(url);
            return(*status);
       }
    }
    else
    {
        ffpmsg("cannot open an existing file of this type: (ffopen)");
        ffpmsg(url);
        return(*status = FILE_NOT_OPENED);
    }

        /* get initial file size */
    *status = (*driverTable[driver].size)(handle, &filesize);
    if (*status > 0)
    {
        (*driverTable[driver].close)(handle);  /* close the file */
        ffpmsg("failed get the size of the following file: (ffopen)");
        ffpmsg(url);
        return(*status);
    }

        /* allocate fitsfile structure and initialize = 0 */
    *fptr = (fitsfile *) calloc(1, sizeof(fitsfile));

    if (!(*fptr))
    {
        (*driverTable[driver].close)(handle);  /* close the file */
        ffpmsg("failed to allocate structure for following file: (ffopen)");
        ffpmsg(url);
        return(*status = MEMORY_ALLOCATION);
    }

        /* allocate FITSfile structure and initialize = 0 */
    (*fptr)->Fptr = (FITSfile *) calloc(1, sizeof(FITSfile));

    if (!((*fptr)->Fptr))
    {
        (*driverTable[driver].close)(handle);  /* close the file */
        ffpmsg("failed to allocate structure for following file: (ffopen)");
        ffpmsg(url);
        free(*fptr);
        *fptr = 0;
        return(*status = MEMORY_ALLOCATION);
    }

    slen = strlen(url) + 1;
    slen = maxvalue(slen, 32); /* reserve at least 32 chars */
    ((*fptr)->Fptr)->filename = (char *) malloc(slen); /* mem for file name */

    if ( !(((*fptr)->Fptr)->filename) )
    {
        (*driverTable[driver].close)(handle);  /* close the file */
        ffpmsg("failed to allocate memory for filename: (ffopen)");
        ffpmsg(url);
        free((*fptr)->Fptr);
        free(*fptr);
        *fptr = 0;              /* return null file pointer */
        return(*status = MEMORY_ALLOCATION);
    }

    /* mem for headstart array */
    ((*fptr)->Fptr)->headstart = (LONGLONG *) calloc(1001, sizeof(LONGLONG));

    if ( !(((*fptr)->Fptr)->headstart) )
    {
        (*driverTable[driver].close)(handle);  /* close the file */
        ffpmsg("failed to allocate memory for headstart array: (ffopen)");
        ffpmsg(url);
        free( ((*fptr)->Fptr)->filename);
        free((*fptr)->Fptr);
        free(*fptr);
        *fptr = 0;              /* return null file pointer */
        return(*status = MEMORY_ALLOCATION);
    }
        /* store the parameters describing the file */
    ((*fptr)->Fptr)->MAXHDU = 1000;              /* initial size of headstart */
    ((*fptr)->Fptr)->filehandle = handle;        /* file handle */
    ((*fptr)->Fptr)->driver = driver;            /* driver number */
    strcpy(((*fptr)->Fptr)->filename, url);      /* full input filename */
    ((*fptr)->Fptr)->filesize = filesize;        /* physical file size */
    ((*fptr)->Fptr)->logfilesize = filesize;     /* logical file size */
    ((*fptr)->Fptr)->writemode = mode;           /* read-write mode    */
    ((*fptr)->Fptr)->datastart = DATA_UNDEFINED; /* unknown start of data */
    ((*fptr)->Fptr)->curbuf = -1;            /* undefined current IO buffer */
    ((*fptr)->Fptr)->open_count = 1;      /* structure is currently used once */
    ((*fptr)->Fptr)->validcode = VALIDSTRUC; /* flag denoting valid structure */

    ffldrc(*fptr, 0, REPORT_EOF, status);     /* load first record */

    fits_store_Fptr( (*fptr)->Fptr, status);  /* store Fptr address */

    if (ffrhdu(*fptr, &hdutyp, status) > 0)  /* determine HDU structure */
    {
        ffpmsg(
          "ffopen could not interpret primary array header of file: ");
        ffpmsg(url);

        if (*status == UNKNOWN_REC)
           ffpmsg("This does not look like a FITS file.");

        ffclos(*fptr, status);
        *fptr = 0;              /* return null file pointer */
        return(*status);
    }

    /* ------------------------------------------------------------- */
    /* At this point, the input file has been opened. If outfile was */
    /* specified, then we have opened a copy of the file, not the    */
    /* original file so it is safe to modify it if necessary         */
    /* ------------------------------------------------------------- */

    if (*outfile)
        writecopy = 1;

move2hdu:

    /* ---------------------------------------------------------- */
    /* move to desired extension, if specified as part of the URL */
    /* ---------------------------------------------------------- */

    if (*extspec)
    {
      if (extnum)  /* extension number was specified */
      {
        ffmahd(*fptr, extnum + 1, &hdutyp, status);
      }
      else if (*extname) /* move to named extension, if specified */
      {
        ffmnhd(*fptr, movetotype, extname, extvers, status);
      }

      if (*status > 0)  /* clean up after error */
      {
        ffpmsg("ffopen could not move to the specified extension:");
        if (extnum > 0)
        {
          sprintf(errmsg,
          " extension number %d doesn't exist or couldn't be opened.",extnum);
          ffpmsg(errmsg);
        }
        else
        {
          sprintf(errmsg,
          " extension with EXTNAME = %s,", extname);
          ffpmsg(errmsg);

          if (extvers)
          {
             sprintf(errmsg,
             "           and with EXTVERS = %d,", extvers);
             ffpmsg(errmsg);
          }

          if (movetotype != ANY_HDU)
          {
             sprintf(errmsg,
             "           and with XTENSION = %s,", hdtype[movetotype]);
             ffpmsg(errmsg);
          }

          ffpmsg(" doesn't exist or couldn't be opened.");
        }

        ffclos(*fptr, status);
        *fptr = 0;              /* return null file pointer */
        return(*status);
      }
    }
    else if (skip_null || skip_image || skip_table ||
            (*imagecolname || *colspec || *rowfilter || *binspec))
    {
      /* ------------------------------------------------------------------

      If no explicit extension specifier is given as part of the file
      name, and, if a) skip_null is true (set if ffopen is called by
      ffdopn) or b) skip_image or skip_table is true (set if ffopen is
      called by fftopn or ffdopn) or c) other file filters are
      specified, then CFITSIO will attempt to move to the first
      'interesting' HDU after opening an existing FITS file (or to
      first interesting table HDU if skip_image is true);

      An 'interesting' HDU is defined to be either an image with NAXIS
      > 0 (i.e., not a null array) or a table which has an EXTNAME
      value which does not contain any of the following strings:
         'GTI'  - Good Time Interval extension
         'OBSTABLE'  - used in Beppo SAX data files

      The main purpose for this is to allow CFITSIO to skip over a null
      primary and other non-interesting HDUs when opening an existing
      file, and move directly to the first extension that contains
      significant data.
      ------------------------------------------------------------------ */

      fits_get_hdu_num(*fptr, &hdunum);
      if (hdunum == 1) {

        fits_get_img_dim(*fptr, &naxis, status);

        if (naxis == 0 || skip_image) /* skip primary array */
        {
          while(1)
          {
            /* see if the next HDU is 'interesting' */
            if (fits_movrel_hdu(*fptr, 1, &hdutyp, status))
            {
               if (*status == END_OF_FILE)
                  *status = 0;  /* reset expected error */

               /* didn't find an interesting HDU so move back to beginning */
               fits_movabs_hdu(*fptr, 1, &hdutyp, status);
               break;
            }

            if (hdutyp == IMAGE_HDU && skip_image) {

                continue;   /* skip images */

            } else if (hdutyp != IMAGE_HDU && skip_table) {

                continue;   /* skip tables */

            } else if (hdutyp == IMAGE_HDU) {

               fits_get_img_dim(*fptr, &naxis, status);
               if (naxis > 0)
                  break;  /* found a non-null image */

            } else {

               tstatus = 0;
               tblname[0] = '\0';
               fits_read_key(*fptr, TSTRING, "EXTNAME", tblname, NULL,&tstatus);

               if ( (!strstr(tblname, "GTI") && !strstr(tblname, "gti")) &&
                    strncasecmp(tblname, "OBSTABLE", 8) )
                  break;  /* found an interesting table */
            }
          }  /* end while */
        }
      } /* end if (hdunum==1) */
    }

    if (*imagecolname)
    {
       /* ----------------------------------------------------------------- */
       /* we need to open an image contained in a single table cell         */
       /* First, determine which row of the table to use.                   */
       /* ----------------------------------------------------------------- */

       if (isdigit((int) *rowexpress))  /* is the row specification a number? */
       {
          sscanf(rowexpress, "%ld", &rownum);
          if (rownum < 1)
          {
             ffpmsg("illegal rownum for image cell:");
             ffpmsg(rowexpress);
             ffpmsg("Could not open the following image in a table cell:");
             ffpmsg(extspec);
             ffclos(*fptr, status);
             *fptr = 0;              /* return null file pointer */
             return(*status = BAD_ROW_NUM);
          }
       }
       else if (fits_find_first_row(*fptr, rowexpress, &rownum, status) > 0)
       {
          ffpmsg("Failed to find row matching this expression:");
          ffpmsg(rowexpress);
          ffpmsg("Could not open the following image in a table cell:");
          ffpmsg(extspec);
          ffclos(*fptr, status);
          *fptr = 0;              /* return null file pointer */
          return(*status);
       }

       if (rownum == 0)
       {
          ffpmsg("row statisfying this expression doesn't exist::");
          ffpmsg(rowexpress);
          ffpmsg("Could not open the following image in a table cell:");
          ffpmsg(extspec);
          ffclos(*fptr, status);
          *fptr = 0;              /* return null file pointer */
          return(*status = BAD_ROW_NUM);
       }

       /* determine the name of the new file to contain copy of the image */
       if (*histfilename && !(*pixfilter) )
           strcpy(outfile, histfilename); /* the original outfile name */
       else
           strcpy(outfile, "mem://_1");  /* create image file in memory */

       /* Copy the image into new primary array and open it as the current */
       /* fptr.  This will close the table that contains the original image. */

       /* create new empty file to hold copy of the image */
       if (ffinit(&newptr, outfile, status) > 0)
       {
          ffpmsg("failed to create file for copy of image in table cell:");
          ffpmsg(outfile);
          return(*status);
       }

       if (fits_copy_cell2image(*fptr, newptr, imagecolname, rownum,
                                status) > 0)
       {
          ffpmsg("Failed to copy table cell to new primary array:");
          ffpmsg(extspec);
          ffclos(*fptr, status);
          *fptr = 0;              /* return null file pointer */
          return(*status);
       }

       /* close the original file and set fptr to the new image */
       ffclos(*fptr, status);

       *fptr = newptr; /* reset the pointer to the new table */

       writecopy = 1;  /* we are now dealing with a copy of the original file */

       /* add some HISTORY; fits_copy_image_cell also wrote HISTORY keywords */

/*  disable this; leave it up to calling routine to write any HISTORY keywords
       if (*extname)
        sprintf(card,"HISTORY  in HDU '%.16s' of file '%.36s'",extname,infile);
       else
        sprintf(card,"HISTORY  in HDU %d of file '%.45s'", extnum, infile);

       ffprec(*fptr, card, status);
*/
    }

    /* --------------------------------------------------------------------- */
    /* edit columns (and/or keywords) in the table, if specified in the URL  */
    /* --------------------------------------------------------------------- */

    if (*colspec)
    {
       /* the column specifier will modify the file, so make sure */
       /* we are already dealing with a copy, or else make a new copy */

       if (!writecopy)  /* Is the current file already a copy? */
           writecopy = fits_is_this_a_copy(urltype);

       if (!writecopy)
       {
           if (*filtfilename && *outfile == '\0')
               strcpy(outfile, filtfilename); /* the original outfile name */
           else
               strcpy(outfile, "mem://_1");   /* will create copy in memory */

           writecopy = 1;
       }
       else
       {
           ((*fptr)->Fptr)->writemode = READWRITE; /* we have write access */
           outfile[0] = '\0';
       }

       if (ffedit_columns(fptr, outfile, colspec, status) > 0)
       {
           ffpmsg("editing columns in input table failed (ffopen)");
           ffpmsg(" while trying to perform the following operation:");
           ffpmsg(colspec);
           ffclos(*fptr, status);
           *fptr = 0;              /* return null file pointer */
           return(*status);
       }
    }

    /* ------------------------------------------------------------------- */
    /* select rows from the table, if specified in the URL                 */
    /* or select a subimage (if this is an image HDU and not a table)      */
    /* ------------------------------------------------------------------- */

    if (*rowfilter)
    {
     fits_get_hdu_type(*fptr, &hdutyp, status);  /* get type of HDU */
     if (hdutyp == IMAGE_HDU)
     {
        /* this is an image so 'rowfilter' is an image section specification */

        if (*filtfilename && *outfile == '\0')
            strcpy(outfile, filtfilename); /* the original outfile name */
        else if (*outfile == '\0') /* output file name not already defined? */
            strcpy(outfile, "mem://_2");  /* will create file in memory */

        /* create new file containing the image section, plus a copy of */
        /* any other HDUs that exist in the input file.  This routine   */
        /* will close the original image file and return a pointer      */
        /* to the new file. */

        if (fits_select_image_section(fptr, outfile, rowfilter, status) > 0)
        {
           ffpmsg("on-the-fly selection of image section failed (ffopen)");
           ffpmsg(" while trying to use the following section filter:");
           ffpmsg(rowfilter);
           ffclos(*fptr, status);
           *fptr = 0;              /* return null file pointer */
           return(*status);
        }
        writecopy = 1;
     }
     else
     {
       /* this is a table HDU, so the rowfilter is really a row filter */

      if (*binspec)
      {
        /*  since we are going to make a histogram of the selected rows,   */
        /*  it would be a waste of time and memory to make a whole copy of */
        /*  the selected rows.  Instead, just construct an array of TRUE   */
        /*  or FALSE values that indicate which rows are to be included    */
        /*  in the histogram and pass that to the histogram generating     */
        /*  routine                                                        */

        fits_get_num_rows(*fptr, &nrows, status);  /* get no. of rows */

        rowselect = (char *) calloc(nrows, 1);
        if (!rowselect)
        {
           ffpmsg(
           "failed to allocate memory for selected columns array (ffopen)");
           ffpmsg(" while trying to select rows with the following filter:");
           ffpmsg(rowfilter);
           ffclos(*fptr, status);
           *fptr = 0;              /* return null file pointer */
           return(*status = MEMORY_ALLOCATION);
        }

        if (fits_find_rows(*fptr, rowfilter, 1L, nrows, &goodrows,
            rowselect, status) > 0)
        {
           ffpmsg("selection of rows in input table failed (ffopen)");
           ffpmsg(" while trying to select rows with the following filter:");
           ffpmsg(rowfilter);
           free(rowselect);
           ffclos(*fptr, status);
           *fptr = 0;              /* return null file pointer */
           return(*status);
        }
      }
      else
      {
        if (!writecopy)  /* Is the current file already a copy? */
           writecopy = fits_is_this_a_copy(urltype);

        if (!writecopy)
        {
           if (*filtfilename && *outfile == '\0')
               strcpy(outfile, filtfilename); /* the original outfile name */
           else if (*outfile == '\0') /* output filename not already defined? */
               strcpy(outfile, "mem://_2");  /* will create copy in memory */
        }
        else
        {
           ((*fptr)->Fptr)->writemode = READWRITE; /* we have write access */
           outfile[0] = '\0';
        }

        /* select rows in the table.  If a copy of the input file has */
        /* not already been made, then this routine will make a copy */
        /* and then close the input file, so that the modifications will */
        /* only be made on the copy, not the original */

        if (ffselect_table(fptr, outfile, rowfilter, status) > 0)
        {
          ffpmsg("on-the-fly selection of rows in input table failed (ffopen)");
           ffpmsg(" while trying to select rows with the following filter:");
           ffpmsg(rowfilter);
           ffclos(*fptr, status);
           *fptr = 0;              /* return null file pointer */
           return(*status);
        }

        /* write history records */
        ffphis(*fptr,
        "CFITSIO used the following filtering expression to create this table:",
        status);
        ffphis(*fptr, name, status);

      }   /* end of no binspec case */
     }   /* end of table HDU case */
    }  /* end of rowfilter exists case */

    /* ------------------------------------------------------------------- */
    /* make an image histogram by binning columns, if specified in the URL */
    /* ------------------------------------------------------------------- */

    if (*binspec)
    {
       if (*histfilename  && !(*pixfilter) )
           strcpy(outfile, histfilename); /* the original outfile name */
       else
           strcpy(outfile, "mem://_3");  /* create histogram in memory */
                                         /* if not already copied the file */

       /* parse the binning specifier into individual parameters */
       ffbins(binspec, &imagetype, &haxis, colname,
                          minin, maxin, binsizein,
                          minname, maxname, binname,
                          &weight, wtcol, &recip, status);

       /* Create the histogram primary array and open it as the current fptr */
       /* This will close the table that was used to create the histogram. */
       ffhist(fptr, outfile, imagetype, haxis, colname, minin, maxin,
              binsizein, minname, maxname, binname,
              weight, wtcol, recip, rowselect, status);

       if (rowselect)
          free(rowselect);

       if (*status > 0)
       {
      ffpmsg("on-the-fly histogramming of input table failed (ffopen)");
      ffpmsg(" while trying to execute the following histogram specification:");
      ffpmsg(binspec);
           ffclos(*fptr, status);
           *fptr = 0;              /* return null file pointer */
           return(*status);
       }

        /* write history records */
        ffphis(*fptr,
        "CFITSIO used the following expression to create this histogram:",
        status);
        ffphis(*fptr, name, status);
    }

    if (*pixfilter)
    {
       if (*histfilename)
           strcpy(outfile, histfilename); /* the original outfile name */
       else
           strcpy(outfile, "mem://_4");  /* create in memory */
                                         /* if not already copied the file */

       /* Ensure type of HDU is consistent with pixel filtering */
       fits_get_hdu_type(*fptr, &hdutyp, status);  /* get type of HDU */
       if (hdutyp == IMAGE_HDU) {

          pixel_filter_helper(fptr, outfile, pixfilter, status);

          if (*status > 0) {
             ffpmsg("pixel filtering of input image failed (ffopen)");
             ffpmsg(" while trying to execute the following:");
             ffpmsg(pixfilter);
             ffclos(*fptr, status);
             *fptr = 0;              /* return null file pointer */
             return(*status);
          }

          /* write history records */
          ffphis(*fptr,
          "CFITSIO used the following expression to create this image:",
          status);
          ffphis(*fptr, name, status);

          return *status;
       }
       else {
          ffpmsg("cannot use pixel filter on non-IMAGE HDU");
          ffpmsg(pixfilter);
          ffclos(*fptr, status);
          *fptr = 0;              /* return null file pointer */
          *status = NOT_IMAGE;
          return(*status);
       }
    }

    return(*status);
}
/*--------------------------------------------------------------------------*/
int ffreopen(fitsfile *openfptr, /* I - FITS file pointer to open file  */
             fitsfile **newfptr,  /* O - pointer to new re opened file   */
             int *status)        /* IO - error status                   */
/*
  Reopen an existing FITS file with either readonly or read/write access.
  The reopened file shares the same FITSfile structure but may point to a
  different HDU within the file.
*/
{
    if (*status > 0)
        return(*status);

    /* check that the open file pointer is valid */
    if (!openfptr)
        return(*status = NULL_INPUT_PTR);
    else if ((openfptr->Fptr)->validcode != VALIDSTRUC) /* check magic value */
        return(*status = BAD_FILEPTR);

        /* allocate fitsfile structure and initialize = 0 */
    *newfptr = (fitsfile *) calloc(1, sizeof(fitsfile));

    (*newfptr)->Fptr = openfptr->Fptr; /* both point to the same structure */
    (*newfptr)->HDUposition = 0;  /* set initial position to primary array */
    (((*newfptr)->Fptr)->open_count)++;   /* increment the file usage counter */

    return(*status);
}
