/**
 * Copyright (c) 2016-2017 by Objective Systems, Inc.
 * http://www.obj-sys.com
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/lgpl3.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/

#include <string.h>

#include "rtxsrc/rtxCommonDefs.h"
#include "rtxsrc/rtxReal.h"

static const OSUINT32 endian = 0x01020304;
static const OSOCTET *pEndian = (OSOCTET*)&endian;
static int initialized_module = 0;
static OSREAL value_module;

/***********************************************************************
 *
 *  Routine name: REAL data type functions
 *
 *  Description:  These functions support the encoding and decoding
 *                of floating point data types.
 *
 *  Inputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *
 *  Outputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *
 **********************************************************************/

/* Utility function to fetch IEEE plus infinity value
 * The bits for PLUS_INFINITY are  0x7ff0000000000000 (note: assumes
 * IEEE double type). */

OSREAL rtxGetPlusInfinity ()
{
   static int initialized = 0;
   static OSREAL value;
   unsigned char* c;

   if (!initialized) {
      c = (unsigned char*) &value;
      if (pEndian [0] == 4) { /* LITTLE ENDIAN */
         c[sizeof (OSREAL) - 1] = 0x7f;
         c[sizeof (OSREAL) - 2] = 0xf0;
         OSCRTLMEMSET (c, 0, sizeof (OSREAL) - 2);
      }
      else {                  /* BIG ENDIAN */
         c[0] = 0x7f;
         c[1] = 0xf0;
         OSCRTLMEMSET (c + 2, 0, sizeof (OSREAL) - 2);
      }
      initialized = 1; /* ED, 3/1/02 */
   }

   return (value);
}

/* Utility function to fetch IEEE minus infinity value
 * The bits for MINUS_INFINITY are  0xfff0000000000000 (note: assumes
 * IEEE double type). */

OSREAL rtxGetMinusInfinity ()
{
   static int initialized = 0;
   static OSREAL value;
   unsigned char* c;

   if (!initialized) {
      c = (unsigned char*) &value;
      if (pEndian [0] == 4) { /* LITTLE ENDIAN */
         c[sizeof (OSREAL) - 1] = 0xff;
         c[sizeof (OSREAL) - 2] = 0xf0;
         OSCRTLMEMSET (c, 0, sizeof (OSREAL) - 2);
      }
      else {                  /* BIG ENDIAN */
         c[0] = 0xff;
         c[1] = 0xf0;
         OSCRTLMEMSET (c + 2, 0, sizeof (OSREAL) - 2);
      }
      initialized = 1; /* ED, 3/1/02 */
   }

   return (value);
}

/* Utility function to fetch IEEE not-a-number value
 * The bits for NaN are  0x7ff8000000000000 (note: assumes
 * IEEE double type). */

OSREAL rtxGetNaN ()
{
   static int initialized = 0;
   static OSREAL value;
   unsigned char* c;

   if (!initialized) {
      c = (unsigned char*) &value;
      if (pEndian [0] == 4) { /* LITTLE ENDIAN */
         c[sizeof (OSREAL) - 1] = 0x7f;
         c[sizeof (OSREAL) - 2] = 0xf8;
         OSCRTLMEMSET (c, 0, sizeof (OSREAL) - 2);
      }
      else {                  /* BIG ENDIAN */
         c[0] = 0x7f;
         c[1] = 0xf8;
         OSCRTLMEMSET (c + 2, 0, sizeof (OSREAL) - 2);
      }
      initialized = 1; /* ED, 3/1/02 */
   }

   return (value);
}

/* Utility function to fetch IEEE minus zero value
 * The bits for minus zero are  0x8000000000000000 (note: assumes
 * IEEE double type). */

OSREAL rtxGetMinusZero ()
{
   unsigned char* c;

   if (!initialized_module) {
      c = (unsigned char*) &value_module;
      if (pEndian [0] == 4) { /* LITTLE ENDIAN */
         c[sizeof (OSREAL) - 1] = 0x80;
         OSCRTLMEMSET (c, 0, sizeof (OSREAL) - 1);
      }
      else {                  /* BIG ENDIAN */
         c[0] = 0x80;
         OSCRTLMEMSET (c + 1, 0, sizeof (OSREAL) - 1);
      }
      initialized_module = 1; /* ED, 3/1/02 */
   }

   return (value_module);
}

/* Utility function to compare value with NaN */

OSBOOL rtxIsNaN (OSREAL value)
{
   unsigned char* c = (unsigned char*) &value;
   int i;

   if (pEndian [0] == 4) { /* LITTLE ENDIAN */
      if((c[sizeof (OSREAL) - 1] & 0x7F) != 0x7F ||
            (c[sizeof (OSREAL) - 2] & 0xF0) != 0xF0)
         return FALSE;

      c[sizeof (OSREAL) - 2] &= 0xF;

      for (i = 0; i < (int) sizeof (OSREAL) - 1 ; i++)
         if (c[i])
            return TRUE;
   }
   else {                  /* BIG ENDIAN */
      if((c[0] & 0x7F) != 0x7F ||
            (c[1] & 0xF0) != 0xF0)
         return FALSE;

      c[1] &= 0xF;

      for (i = 1; i < (int) sizeof (OSREAL) ; i++)
         if (c[i])
            return TRUE;
   }

   return FALSE;
}

/* Utility function to compare value with -0.0 */

OSBOOL rtxIsMinusZero (OSREAL value)
{
   OSREAL mz = rtxGetMinusZero();

   return (OSBOOL)(memcmp (&mz, &value, sizeof (OSREAL)) == 0);
}

/* Utility function to compare value with +INF */

OSBOOL rtxIsPlusInfinity (OSREAL value)
{
   OSREAL inf = rtxGetPlusInfinity();

   return (OSBOOL)(memcmp (&inf, &value, sizeof (OSREAL)) == 0);
}

/* Utility function to compare value with +INF */

OSBOOL rtxIsMinusInfinity (OSREAL value)
{
   OSREAL inf = rtxGetMinusInfinity();

   return (OSBOOL)(memcmp (&inf, &value, sizeof (OSREAL)) == 0);
}

OSBOOL rtxIsApproximate (OSREAL a, OSREAL b, OSREAL delta)
{
   if (a != b) {
      if (a < b * (1. - delta) || a > b * (1. + delta))
         return FALSE;
   }

   return TRUE;
}

OSBOOL rtxIsApproximateAbs (OSREAL a, OSREAL b, OSREAL delta)
{
   if (a != b) {
      if (a < b - delta || a > b + delta)
         return FALSE;
   }

   return TRUE;
}
