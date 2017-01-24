/*********************************************************************
MakeProfiles - Create mock astronomical profiles.
MakeProfiles is part of GNU Astronomy Utilities (Gnuastro) package.

Original author:
     Mohammad Akhlaghi <akhlaghi@gnu.org>
Contributing author(s):
Copyright (C) 2015, Free Software Foundation, Inc.

Gnuastro is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

Gnuastro is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gnuastro. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include <pthread.h>
#include <gsl/gsl_rng.h>

#include <gnuastro/fits.h>

#include <options.h>


/* Progarm name macros: */
#define PROGRAM_NAME "Arithmetic"      /* Program full name.       */
#define PROGRAM_EXEC "astarithmetic"   /* Program executable name. */
#define PROGRAM_STRING PROGRAM_NAME" (" PACKAGE_NAME ") " PACKAGE_VERSION



/* Some constants */
#define EPSREL_FOR_INTEG   2
#define DEGREESTORADIANS   M_PI/180.0f



/* Types of profiles. */
enum profile_types
{
  PROFILE_SERSIC,               /* Sersic profile.           */
  PROFILE_MOFFAT,               /* Moffat Profile.           */
  PROFILE_GAUSSIAN,             /* Gaussian Profile.         */
  PROFILE_POINT,                /* Point profile.            */
  PROFILE_FLAT,                 /* Flat profile.             */
  PROFILE_CIRCUMFERENCE,        /* Circumference profile.    */

  MAXIMUM_PROFILE_CODE,         /* Just for a sanity checck. */
};
#define MINCIRCUMWIDTH       0.5f



/* Log file:

   0: ID.
   1: Overlap magnitude.
   2: Number of accurate pixels.
   3: Fraction of accurate values.
   4: Is individual file created?   */
#define LOGNUMCOLS      5
#define LOGFILENAME     PROGRAM_EXEC".log"





struct builtqueue
{
  size_t               id;    /* ID of this object.                  */
  int               ispsf;    /* This is a PSF profile.              */
  int            overlaps;    /* ==1: Overlaps with the image.       */
  float              *img;    /* Array of this profile's image.      */
  size_t         imgwidth;    /* Width of *img.                      */
  long        fpixel_i[2];    /* First pixel in output image.        */
  long        lpixel_i[2];    /* Last pixel in output image.         */
  long        fpixel_o[2];    /* First pixel in this array.          */
  int                func;    /* Profile's radial function.          */

  int        indivcreated;    /* ==1: an individual file is created. */
  size_t          numaccu;    /* Number of accurate pixels.          */
  double         accufrac;    /* Difference of accurate values.      */

  struct builtqueue *next;    /* Pointer to next element.            */
};





struct mkprofparams
{
  /* Common parameters structure. */
  struct gal_options_common_params cp; /* Common parameters.           */

  /* Operating modes */
  int           psfinimg;  /* ==1: Build PSF profiles in image.        */
  int         individual;  /* ==1: Build all catalog separately.       */


  /* Profiles */
  int            replace;  /* Replace overlaping profile pixel values. */
  size_t       numrandom;  /* Number of radom points for integration.  */
  float        tolerance;  /* Accuracy to stop integration.            */
  float        zeropoint;  /* Magnitude of zero point flux.            */
  double     circumwidth;  /* Width of circumference (inward).         */
  int          magatpeak;  /* Mag only for peak pixel, not all profile.*/
  int            envseed;  /* Use GSL_RNG_SEED for random seed.        */
  int        mforflatpix;  /* mcol is flat pixel value (f is 4 or 5).  */
  int           tunitinp;  /* ==1: Truncation unit is in pixels.       */
                           /* ==0: It is in radial parameter.          */

  /* Random number generator */
  gsl_rng           *rng;  /* Main instance of random number generator.*/


  /* Output */
  char         *basename;  /* Merged image name with no directory.     */
  char           *outdir;  /* Output directory.                        */
  int           anyblank;  /* ==1: there are blanks in back.           */
  int           nomerged;  /* ==1: Don't make a merged image of all.   */
  long          naxes[2];  /* Size of the output image.                */
  long          shift[2];  /* Shift along axeses position of profiles. */
  size_t      oversample;  /* Oversampling scale.                      */
  int               nwcs;  /* Number of WCS.                           */
  struct wcsprm     *wcs;  /* WCSparam structure.                      */


  /* WCS: */
  double        crpix[2];  /* CRPIX FITS header keywords.              */
  double        crval[2];  /* CRVAL FITS header keywords.              */
  float       resolution;  /* PC1_1 and PC2_2 FITS header keywords.    */


  /* User interface variables. */
  struct gal_linkedlist_stll *allargs; /* Keep all input arguments.    */
  int               type;  /* User's desired output bitpix.            */
  char         *backname;  /* Name of background image file name.      */
  char          *backhdu;  /* HDU of background image.                 */
  char          *catname;  /* Name of catalog of parameters.           */
  int        prepforconv;  /* Shift and expand by size of first psf.   */
  int      inputascanvas;  /* Input image's header for size and WCS.   */
  size_t            fcol;  /* Column specifying profile function.      */
  size_t            xcol;  /* X column of profile center.              */
  size_t            ycol;  /* Y column of profile center.              */
  size_t           racol;  /* RA column of profile center.             */
  size_t          deccol;  /* Dec column of profile center.            */
  size_t            rcol;  /* Effective radius of profile.             */
  size_t            ncol;  /* Sersic index column of profile.          */
  size_t            pcol;  /* Position angle column of profile.        */
  size_t            qcol;  /* Axis ratio column of profile.            */
  size_t            mcol;  /* Magnitude column.                        */
  size_t            tcol;  /* Truncation of the profiles.              */


  /* Processing parameters: */
  time_t         rawtime;  /* Starting time of the program.            */
  gal_data_t        *out;  /* Output image.                            */
  double            *cat;  /* Input catalog.                           */
  size_t             cs0;  /* Number of rows in input catalog.         */
  size_t             cs1;  /* Number of columns in input catalog.      */
  double            *log;  /* Log data to be printed.                  */
  struct builtqueue  *bq;  /* Top (last) elem of build queue.          */
  pthread_cond_t  qready;  /* bq is ready to be written.               */
  pthread_mutex_t  qlock;  /* Mutex lock to change builtq.             */
  double       halfpixel;  /* Half pixel in oversampled image.         */
  int            outtype;  /* Type of finally merged output image.     */
  char        *wcsheader;  /* The WCS header information for main img. */
  int         wcsnkeyrec;  /* The number of keywords in the WCS header.*/
  char    *mergedimgname;  /* Name of merged image.                    */
};

#endif
