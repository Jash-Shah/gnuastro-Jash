/*********************************************************************
Convolve - Convolve input data with a given kernel.
Convolve is part of GNU Astronomy Utilities (gnuastro) package.

Copyright (C) 2013-2015 Mohammad Akhlaghi
Tohoku University Astronomical Institute, Sendai, Japan.
http://astr.tohoku.ac.jp/~akhlaghi/

gnuastro is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

gnuastro is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with gnuastro. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include "timing.h"   	        /* Includes time.h and sys/time.h */

#include "main.h"

#include "convolve.h"
#include "ui.h"                 /* Needs convolveparams in main.h */

int
main(int argc, char *argv[])
{
  struct timeval t1;
  struct convolveparams p={{0}, {0}, 0};

  /* Set the starting time.*/
  time(&p.rawtime);
  gettimeofday(&t1, NULL);

  /* Read the input parameters. */
  setparams(argc, argv, &p);

  /* Run Image Crop */
  convolve(&p);

  /* Free all non-freed allocations. */
  freeandreport(&p, &t1);

  /* Return successfully. */
  return EXIT_SUCCESS;
}