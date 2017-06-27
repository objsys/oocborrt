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
#include "rtxsrc/rtxReal.h"

#ifndef RTJSONENCDOUBLEVALUEFUNC
#define RTJSONENCDOUBLEVALUEFUNC rtJsonEncDoubleValue
#define PRECISION DEFAULT_DOUBLE_PRECISION
#endif

#define LG2 0.30102999566398119521373889472449

int RTJSONENCDOUBLEVALUEFUNC (OSCTXT* pctxt, OSREAL value,
                         const OSDoubleFmt* pFmtSpec)
{
   char lbuf[512];
   double mantissa;
   int lexp, sign = 1;
   int ch, stat;
   char *p = lbuf, *firstZeroInFractionPtr = 0;
   int totalDigits = -1, integerMaxDigits = INT_MAX, integerMinDigits = 1;
   int fractionDigitsNow = 0, fractionMinDigits = 0;
   int fractionDigits = INT_MAX;
   int expMinValue = OSINT16_MIN, expMaxValue = OSINT16_MAX;
   int intPartLen = 1, expMod, expDig;
   char expSymbol = 'E';

   int defaultPrecision = PRECISION;

   /* special values */
   /* NaN/INF/-INF is not valid in JSON */
#if 0
   if (rtxIsPlusInfinity (value)) {
      stat = rtxCopyAsciiText (pctxt, "INF");
      if (stat != 0) return LOG_RTERR (pctxt, stat);
      return 0;
   }
   else if (rtxIsMinusInfinity (value)) {
      stat = rtxCopyAsciiText (pctxt, "-INF");
      if (stat != 0) return LOG_RTERR (pctxt, stat);
      return 0;
   }
   else if (rtxIsNaN (value)) {
      stat = rtxCopyAsciiText (pctxt, "NaN");
      if (stat != 0) return LOG_RTERR (pctxt, stat);
      return 0;
   }
#endif

   if (rtxIsPlusInfinity (value) ||
       rtxIsMinusInfinity (value) ||
       rtxIsNaN (value)) {
      return LOG_RTERR (pctxt, RTERR_NOTSUPP);
   }

   if (value < 0) {
      sign = -1;
      value = -value;
   }
   if (rtxIsMinusZero (value)) {
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
      if (pFmtSpec->expMinValue > OSINT16_MIN)
         expMinValue = pFmtSpec->expMinValue;
      if (pFmtSpec->expMaxValue < OSINT16_MAX)
         expMaxValue = pFmtSpec->expMaxValue;
      if (pFmtSpec->expSymbol == 'E' || pFmtSpec->expSymbol == 'e')
         expSymbol = (char)pFmtSpec->expSymbol;
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
      totalDigits = defaultPrecision + 1;

   /* Handle sign */

   if (sign < 0 || (pFmtSpec != 0 && pFmtSpec->signPresent)) {
      *p++ = (char)((sign < 0) ? '-' : '+');
   }

   if (pFmtSpec != 0 && pFmtSpec->integerMinDigits >= 0) {

      /* correct exponent accordingly to integer part len */

      intPartLen = integerMinDigits;
      lexp -= (intPartLen - 1);
   }
   else {

      /* or, correct integer length accordingly to exponent's
         min and max values */

      if (expMinValue > OSINT16_MIN && lexp < expMinValue) {
         intPartLen -= (expMinValue - lexp);
         lexp = expMinValue;
      }
      else if (expMaxValue < OSINT16_MAX && lexp > expMaxValue) {
         intPartLen += (lexp - expMaxValue);
         lexp = expMaxValue;
      }
      /* modify intPartLen to show small floats without exponent  */
      else if (lexp > 0 && lexp < 10)  {
         intPartLen = lexp + 1;
         lexp = 0;
      }

   }
   intPartLen--;

   /* round up the mantissa accordingly to precission */

   if (mantissa != 0.0) {
      double correndum;
      int corrpow;

      if (intPartLen > totalDigits - 1)
         corrpow = -totalDigits + 1;
      else if (fractionDigits == INT_MAX) {
         corrpow = -defaultPrecision;
      }
      else if (intPartLen > 0)
         corrpow = -((fractionDigits + intPartLen < totalDigits) ?
                   fractionDigits + intPartLen : totalDigits - 1);
      else
         corrpow = -((fractionDigits < totalDigits) ? fractionDigits : totalDigits - 1);
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

   /* Handle leading zeroes */

   if (intPartLen < 0) {

      if (integerMaxDigits >= 0) {
         *p++ = '0';
      }
      *p++ = '.';

      while (++intPartLen) {
         fractionDigitsNow++;
         *p++ = '0';
      }
      intPartLen--;
   }
   else {
      int i = integerMinDigits - (intPartLen + 1);
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

      if (intPartLen < 0) {
         fractionDigitsNow++;
         if (ch == 0) {
            if   (firstZeroInFractionPtr == 0)
               firstZeroInFractionPtr = p - 1;
         }
         else firstZeroInFractionPtr = 0;
      }
      if (0 == intPartLen--) {
         *p++ = '.';
      }

   } while (--totalDigits > 0 && fractionDigitsNow < fractionDigits);

   /* handle trailing zeroes */
   while (intPartLen >= 0) {

      *p++ = '0';

      if (0 == intPartLen--) {
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
      intPartLen += (int)(p - firstZeroInFractionPtr);
      p = firstZeroInFractionPtr;
   }

   /* handle trailing zeros in fraction part, if necessary */

   while (fractionDigitsNow < fractionMinDigits) {
      *p++ = '0';
      fractionDigitsNow++;
      intPartLen--;
   }

   /* Handle isolated '.' at end */

   if (-1 == intPartLen) {
      if (pFmtSpec == 0 || !pFmtSpec->pointPresent)
         p--; /* eliminate the decimal point */
   }

   /* exponent's symbol and sign */

   if (lexp != 0 || (pFmtSpec != 0 && pFmtSpec->expPresent)) {

      *p++ = expSymbol;
      if (lexp < 0 || (pFmtSpec != 0 && pFmtSpec->expSignPresent)) {
         *p++ = (char)((lexp < 0) ? '-' : '+');
         lexp = -lexp;
      }

      /* handle exponent */

      if (lexp >= 1000)     { expDig = 4; expMod = 10000; }
      else if (lexp >= 100) { expDig = 3; expMod = 1000; }
      else if (lexp >= 10)  { expDig = 2; expMod = 100; }
      else                 { expDig = 1; expMod = 10; }

      /* handle leading zeros of lexp if necessary */

      if (pFmtSpec != 0) {
         int expDigits = pFmtSpec->expDigits;
         while (expDigits > expDig) {
            *p++ = '0';
            expDigits--;
         }
      }

      while (expMod > 1) {
         int expDiv = expMod/10;
         *p++ = (char)(lexp % expMod/expDiv + '0');
         expMod /= 10;
      }
   }
   *p = '\0';

   stat = rtxCopyAsciiText (pctxt, lbuf);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   return (0);
}

