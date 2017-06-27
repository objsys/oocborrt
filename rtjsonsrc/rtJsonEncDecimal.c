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

#include <math.h>
#include "osrtjson.h"
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxErrCodes.h"
#include "rtxsrc/rtxReal.h"

#define LG2 0.30102999566398119521373889472449
#define PRECISION 15 /* the default number of fraction
                        digits for normalized double */
#define FRACTIONDIGITS 10

int rtJsonEncDecimalValue (OSCTXT* pctxt, OSREAL value,
                          const OSDecimalFmt* pFmtSpec)
{
   char lbuf[512];
   double mantissa;
   int lexp, sign = 1;
   int ch, stat;
   char *p = lbuf, *firstZeroInFractionPtr = 0;
   int totalDigits = PRECISION + 1;
   int integerMaxDigits = INT_MAX, integerMinDigits = 0;
   int fractionDigits = FRACTIONDIGITS;
   int fractionDigitsNow = 0, fractionMinDigits = 0;

   /* handle special values - for decimals just return error */

   if (rtxIsPlusInfinity (value)) {
      return LOG_RTERRNEW (pctxt, RTERR_INVREAL);
   }
   else if (rtxIsMinusInfinity (value)) {
      return LOG_RTERRNEW (pctxt, RTERR_INVREAL);
   }
   else if (rtxIsNaN (value)) {
      return LOG_RTERRNEW (pctxt, RTERR_INVREAL);
   }

   if (value < 0) {
      sign = -1;
      value = -value;
   }

   if (pFmtSpec != 0) {
      if (pFmtSpec->totalDigits > 0)
         totalDigits = pFmtSpec->totalDigits;
      if (pFmtSpec->fractionDigits >= 0)
         fractionDigits = pFmtSpec->fractionDigits;
      if (pFmtSpec->integerMaxDigits >= 0)
         integerMaxDigits = pFmtSpec->integerMaxDigits;
      if (pFmtSpec->integerMinDigits >= 0)
         integerMinDigits = pFmtSpec->integerMinDigits;
      if (pFmtSpec->fractionMinDigits >= 0)
         fractionMinDigits = pFmtSpec->fractionMinDigits;
   }

   frexp (value, &lexp);

   /* value = mantissa * 2^lexp */
   /* We need convert it to mantissa2 * 10^lexp2 */

   /* calculate new exponent first.
      Assume, 2^lexp = 10^lexp2. In this case lexp2 = n*lg 2 */

   lexp = (int)(LG2 * (double)lexp);

   /* calculate new mantissa. mantissa2 = value/(10^lexp2) */

   if (lexp == 0)
      mantissa = value; /* 10^0 = 1 */
   else
      mantissa = value/pow (10.0, lexp);

   /* now, normalize the mantissa, if necessary*/

   if (mantissa != 0.0) {

      while (mantissa >= 10.0) {
         mantissa = mantissa / 10.0;
         lexp ++;
      }
      while (mantissa < 1.0) {
         mantissa = 10.0 * mantissa;
         lexp --;
      }
   }

   if (totalDigits < 0)
      totalDigits = PRECISION + 1;

   /* round up the mantissa accordingly to precission */

   if (mantissa != 0.0) {
      double correndum;
      int corrpow;

      if (lexp > totalDigits - 1)
         corrpow = -totalDigits + 1;
      else if (fractionDigits == INT_MAX) {
         corrpow = -PRECISION;
      }
      else if (lexp > 0)
         corrpow = -((fractionDigits + lexp < totalDigits) ?
            fractionDigits + lexp : totalDigits - 1);
      else
         corrpow = -((fractionDigits < totalDigits) ?
                     fractionDigits : totalDigits);

      correndum = 0.5 * pow (10.0, corrpow);
      mantissa += correndum;

      /* here we need to perform one-step normalization of the mantissa.
         It might be necessary in some cases, when mantissa becomes
         greater than 1.0 after addition of correndum. For example, it
         happens, if value == 0.01 */

      if (mantissa >= 10.0) {
         mantissa = mantissa / 10.0;
         lexp ++;
      }
      if (mantissa < 1.0) {
         mantissa = 10.0 * mantissa;
         lexp --;
      }
   }

   /* Handle sign */

   if (sign < 0 || (pFmtSpec != 0 && pFmtSpec->signPresent)) {
      *p++ = (char)((sign < 0) ? '-' : '+');
   }

   /* Handle leading zeroes */

   if (lexp < 0) {

      if (integerMaxDigits > 0) {
         int i = (integerMinDigits > 0) ? integerMinDigits : 1;
         for (; i > 0; i--) {
            *p++ = '0';
         }
      }
      *p++ = '.';

      while (++lexp) {
         *p++ = '0';
         fractionDigitsNow++;
      }
      lexp--;
   }
   else {
      int i = integerMinDigits - (lexp + 1);
      for (; i > 0; i--) {
         *p++ = '0';
      }
   }

   /* The real work */
   do {

      ch = (int)mantissa;
      mantissa = mantissa - ch;

      *p++ = (char)(ch + '0');

      mantissa = 10.0 * mantissa;

      if (lexp < 0) {
         fractionDigitsNow++;
         if (ch == 0) {
            if (firstZeroInFractionPtr == 0)
               firstZeroInFractionPtr = p - 1;
         }
         else firstZeroInFractionPtr = 0;
      }
      if (0 == lexp--) {
         *p++ = '.';
      }

   } while (--totalDigits > 0 && fractionDigitsNow < fractionDigits);

   /* handle trailing zeroes */
   while (lexp >= 0) {

      *p++ = '0';

      if (0 == lexp--) {
         *p++ = '.';
      }
   }

   /* remove extra zeros in fraction part */

   if (firstZeroInFractionPtr != 0 && fractionDigitsNow > fractionMinDigits) {
      int z = fractionDigitsNow - (int)(p - firstZeroInFractionPtr);
      if (fractionMinDigits > z) {
         firstZeroInFractionPtr += (fractionMinDigits - z);
         fractionDigitsNow -= (int)(p - firstZeroInFractionPtr);
      }
      lexp += (int)(p - firstZeroInFractionPtr);
      p = firstZeroInFractionPtr;
   }

   /* handle trailing zeros in fraction part, if necessary */

   while (fractionDigitsNow < fractionMinDigits) {
      *p++ = '0';
      fractionDigitsNow++;
      lexp--;
   }

   /* Handle isolated '.' at end */

   if (-1 == lexp) {
      if (pFmtSpec == 0 || !pFmtSpec->pointPresent)
         p--; /* eliminate the decimal point */
   }

   *p = '\0';

   stat = rtxCopyAsciiText (pctxt, lbuf);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   return (0);
}


