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

/*****************************************************************************\
*
* rtxBigFloat
*
* Routines for manipulating Big Floats.
*
\*****************************************************************************/

#include "rtxsrc/rtxBigFloat.h"
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxCommonDefs.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxReal.h"
#include <math.h>

/* Initializes OSBigFloat structure for further usage */
void rtxBigFloatInit (OSBigFloat* pFloat)
{
   pFloat->base10 = 1;
   pFloat->exponent = 0;
   rtxBigIntInit(&pFloat->mantissa);
}


int rtxBigFloatSetStr
(OSCTXT* pctxt, OSBigFloat* pFloat, const char* value)
{
   return rtxBigFloatSetStrn (pctxt, pFloat, value, OSCRTLSTRLEN(value));
}

int rtxBigFloatSetStrn
(OSCTXT* pctxt, OSBigFloat* pFloat, const char* value, OSSIZE vlen)
{
   int ret = 0;
   OSSIZE idx = 0;
   OSSIZE endInteger;
   OSSIZE endMantissa;

   /* remove leading and trailing whitespace */
   while (OS_ISSPACE (value[idx])) { idx++; }
   while (vlen > 0 && OS_ISSPACE (value[vlen-1])) vlen--;

   /* format: [-] integer [.fraction] [E|e [-] integer]  */

   /* scan for the end of the integer ('.', 'E', 'e', or end of string) */
   endInteger = idx;
   while ( endInteger < vlen && value[endInteger] != '.' &&
            value[endInteger] != 'E' &&
            value[endInteger] != 'e' ) { endInteger++; }

   if ( idx == vlen ) {
      /* end of mantissa is end of integer is end of string */
      endMantissa = idx;
   }
   else if ( value[endInteger] == '.' ) {
      /* find the end of the fraction (also end of mantissa; 'E', 'e'
         or end of string) */
      endMantissa = endInteger;
      while ( endMantissa < vlen &&
            value[endMantissa] != 'E' &&
            value[endMantissa] != 'e' ) { endMantissa++; }
   }
   else {
      /* end of mantissa is end of integer */
      endMantissa = endInteger;
   }

   if ( value[endInteger] == '.' )
   {
      /* there is a fraction part*/
      char* mantissa;                     /* the mantissa w/o decimal point */
      OSSIZE mantissaLen;                 /* length of mantissa */
      OSSIZE fracLen;                     /* # of fraction digits */


      /* Create a string that has the integer and the fraction as one integer.
         The length (without the null terminator) will be
         endMantissa - idx - 1 since we drop the decimal point.
      */
      fracLen = endMantissa - endInteger - 1;
      mantissaLen = endMantissa - idx;    /* '.' omitted; leaves room for null*/

      mantissa = rtxMemAllocArray(pctxt, mantissaLen, char);
      if ( mantissa == 0 ) return LOG_RTERRNEW (pctxt, RTERR_NOMEM);

      /* copy the integer then the fraction */
      OSCRTLSTRNCPY_S(mantissa, mantissaLen, value + idx,
                        endInteger - idx);
      OSCRTLSTRNCPY_S(mantissa + endInteger - idx,
                        mantissaLen - (endInteger - idx),
                        value + endInteger + 1, fracLen);

      if ( fracLen > OSINT64MAX ) {
         ret= LOG_RTERRNEW(pctxt, RTERR_TOOBIG );
      }

      pFloat->exponent = -(OSINT64)fracLen; /* account for fraction*/
      if ( ret == 0 ) ret = rtxBigIntSetStrn(pctxt, &pFloat->mantissa, mantissa,
                              mantissaLen - 1, 10);   /* -1 for null char */

      rtxMemFreeArray(pctxt, mantissa);

      if ( ret != 0 ) return LOG_RTERR(pctxt, ret);
   }
   else {
      /* no fractional part */
      pFloat->exponent = 0;
      ret = rtxBigIntSetStrn(pctxt, &pFloat->mantissa, value + idx,
                                    endInteger - idx, 10);
      if ( ret != 0 ) return LOG_RTERR(pctxt, ret);
   }

   if ( endMantissa < vlen )
   {
      /* End of mantissa is not end of string; there is an exponent.*/
      char* endOfExponent;
      OSINT64 exponent = strtoll(value + endMantissa + 1, &endOfExponent, 10);
      if ( endOfExponent == value + endMantissa )
         return LOG_RTERRNEW(pctxt, RTERR_BADVALUE);
      if ( exponent < 0 && pFloat->exponent < 0 &&
            pFloat->exponent + exponent > 0 )
         return LOG_RTERRNEW(pctxt, RTERR_TOOBIG);    /* overflow */
      /* else: pFloat->exponent == 0 or exponent > 0 and no overflow */

      pFloat->exponent += exponent;
   }

   return 0;
}


OSSIZE rtxBigFloatStringSize(OSBigFloat* pFloat)
{
   if ( pFloat->base10 ) {
      OSSIZE result = 0;
      if ( pFloat->mantissa.sign < 0 ) result++;   /* for sign */

      result += rtxBigIntDigitsNum(&pFloat->mantissa, 10);

      if ( pFloat->exponent != 0 ) {
         double exp = (double)
               (pFloat->exponent > 0 ? pFloat->exponent : -pFloat->exponent);
         OSSIZE expDigits = (OSSIZE) floor(log10(exp)) + 1;
         if ( expDigits == 1 ) expDigits++;        /* use min of 2 exp digits */
         result += expDigits;

         result++;     /* expo sign always included */
      }
      return result;
   }
   else {
      OSBOOL fitsInReal;
      OSREAL realVal;

      fitsInReal = rtxBigFloatToReal(pFloat, &realVal);
      if ( fitsInReal ) {
         /* format will be G which uses:
            1) E format with precision P = 6 (default) and exponent X
            2) F format with precision P - X - 1.
            Since F is used only when X >= -4, it turns out the E format
            always gives the longest result, so use that length.
         */
         if ( realVal == 0 ) return 1;
         else {
            OSREAL approxExp = floor(log10(fabs(realVal))) + 1;
            int approxExpDig = (int) floor(log10(fabs(approxExp))) + 1;

            /* 11 is for sign, leading digit, decimal point, E, exponent sign,
               and 6 digitis of precision)
            */
            return 11 + OS_MAX(approxExpDig,2);
         }
      }
      else {
         /* use %A-inspired format:
               [-]0xHHHHHP{+/-]d,
         */
         int expDigits = pFloat->exponent == 0 ? 1 :
                     (int) floor(log10(fabs((double)pFloat->exponent))) + 1;

         /* 5 is for '0x', 'P', exponent sign, and null terminator, then
            possibly 1 for the sign, and the number of exponent digits
         */
         OSSIZE reqLen = 5 + (pFloat->mantissa.sign > 0 ? 0 : 1) +
                              expDigits;

         if ( pFloat->mantissa.numocts > (OSSIZE_MAX - reqLen) / 2 ) {
            /* ridiculously large value that will require an enormous buffer
               and will lead to overflows computing the length.
               Just return OSSIZE_MAX.  They'll get buffer overflow later since
               even this is not big enough to fit the string.
            */
            return OSSIZE_MAX;
         }

         reqLen = reqLen + pFloat->mantissa.numocts * 2;
         return reqLen;
      }
   }
}

/*
The 2^SMALLEST_SUBNORMAL_EXP gives the smallest subnormal double value.
In IEEE 754, you can find this is emin + 1 - p, which equals
(1-emax) + 1 - p.  DBL_MAX_EXP gives emax + 1 (C says it is emax, but the C
emax and the IEEE emax are slightly different because of a different
presentation of the information.
*/
#define SMALLEST_SUBNORMAL_EXP (-DBL_MAX_EXP + 3 - DBL_MANT_DIG)


/*
The smallest subnormal double
*/
#define SMALLEST_SUBNORMAL (DBL_MIN * DBL_EPSILON)

OSBOOL rtxBigFloatToReal(const OSBigFloat* pFloat, OSREAL* pvalue)
{
   OSREAL result;
   OSBOOL ret;
   OSREAL powerOf2;


   OSRTASSERT(!pFloat->base10);    /* only base 2 is supported. Base 10 is not
                                 supported because generally the conversion is
                                 not exact as required by the function
                                 description. */

   ret = rtxBigIntToReal(&pFloat->mantissa, &result);
   if ( !ret ) return ret;

   if ( result == 0 ) {
      if (pvalue) *pvalue = result;
      return TRUE;
   }

   if ( pFloat->exponent >= DBL_MAX_EXP )
   {
      /* This exponent is too big. */
      return FALSE;
   }

   else if ( pFloat->exponent <= SMALLEST_SUBNORMAL_EXP - DBL_MAX_EXP )
   {
      /* exponent is too small even to be adjusted as in next block.
         Allowing exponent to be smaller would result in mustAdd, below,
         being >= DBL_MAX_EXP, which is larger than the largest double. */
      return FALSE;
   }
   else if ( pFloat->exponent < SMALLEST_SUBNORMAL_EXP ) {
      /* The smallest subnormal double is 2^(-1074).
         We can take any integer
         multiple of that, but we cannot take an integer multiple of any smaller
         power of 2.  If the mantissa is a large enough power of 2, we will
         be able to adjust the exponent up to -1074.  If not, the value
         cannot be represented exactly.
      */
      OSINT64 mustAdd = SMALLEST_SUBNORMAL_EXP - pFloat->exponent;
                                 /* 0 <= mustAdd < DBL_MAX_EXP */
      double divideBy = pow(2, (double) mustAdd);
      if ( fmod(result, divideBy) == 0 )
      {
         /* Dividing the mantissa by the power of 2 given by mustAdd
            is still an integer.  This means:
               mantissa * 2^exponent ==
               (mantissa / 2^mustAdd) * 2^(exponent + mustAdd) ==
               (mantissa / 2^mustAdd) * 2^(SMALLEST_SUBNORMAL_EXP), which is
               exactly representable.
         */
         result = ( result / divideBy ) * SMALLEST_SUBNORMAL;
      }
      else {
         /* Value is not exactly representable in double.*/
         return FALSE;
      }
   }
   else {
      /* -1074 <= exponent <= 1023.  We can represent 2^exponent exactly.*/
      powerOf2 = pow(2, (double) pFloat->exponent);

      result = result * powerOf2;

      /* Multiplying by some power of 2 will not change the precision (so it
         won't cause rounding).  It could result in making the value overflow or
         underflow; otherwise, the result will be exact.
      */
      if ( rtxIsPlusInfinity(result) || rtxIsMinusInfinity(result) ||
            result == 0 )
      {
         /* exponent too large or too small */
         return FALSE;
      }
   }

   if ( pvalue ) *pvalue = result;

   return TRUE;
}


int rtxBigFloatToString
(OSCTXT* pctxt, const OSBigFloat* pFloat, char* str, OSSIZE strSize)
{
   int ret;

   if ( pFloat->base10 ) {
      OSSIZE length; /* actual length of first part of string */

      ret = rtxBigIntToString(pctxt, &pFloat->mantissa, 10, str, strSize);
      if ( ret < 0 ) return LOG_RTERR(pctxt, ret);

      /* shrink buffer size by length of string and point str to the null char
         so we can append */
      length = OSCRTLSTRLEN(str);
      strSize = strSize - length;
      str = str + length;

      if ( pFloat->exponent != 0 ) {
         os_snprintf(str, strSize, "E%+03" OSINT64FMT_TS, pFloat->exponent);
      }
   }
   else {
      /* For values that fit in a double, convert to double and use *printf.
         For all other values, use similar format to %A format from *printf,
         which does not require base 2 to base 10 conversion.
      */
      OSBOOL fitsInReal;
      OSREAL realVal;

      fitsInReal = rtxBigFloatToReal(pFloat, &realVal);
      if ( fitsInReal ) {
         os_snprintf(str, strSize, "%G", realVal);
      }
      else {
         /* use %A-inspired format; it is %A format but not using a
            decimal point.  I don't use %A because putting the decimal point in
            the right place would require adjusting the exponent, and I might
            then end up with an exponent that is outside OSINT64.
            So, we use this format:
               [-]0xHHHHHP{+/-]d,
         */
         OSSIZE idx = 0;      /* index into str to write to */
         OSSIZE octIdx = 0;
         int expDigits = pFloat->exponent == 0 ? 1 :
            (int) floor(log10(fabs((double) pFloat->exponent))) + 1;

         /* 5 is for '0x', 'P', exponent sign, and null terminator, then
            possibly 1 for the sign, and the number of exponent digits
         */
         OSSIZE reqLen = 5 + (pFloat->mantissa.sign > 0 ? 0 : 1) +
                              expDigits;

         if ( pFloat->mantissa.numocts > (OSSIZE_MAX - reqLen) / 2 ) {
            /* ridiculously large value that will require an enormous buffer
               and will lead to overflows computing the length
            */
            return LOG_RTERR(pctxt, RTERR_BUFOVFLW);
         }

         reqLen = reqLen + pFloat->mantissa.numocts * 2;

         if ( strSize < reqLen ) {
            return LOG_RTERR(pctxt, RTERR_BUFOVFLW);
         }

         if ( pFloat->mantissa.sign < 0 ) {
            str[idx++] = '-';
         }

         str[idx++] = '0';
         str[idx++] = 'x';

         /* now add on 2 hex chars per byte for the mantissa. */
         for(octIdx = 0; octIdx < pFloat->mantissa.numocts; octIdx++)
         {
            OSOCTET octet = pFloat->mantissa.mag[octIdx];
            NIBBLETOUCHEXCHAR ((octet >> 4), str[idx]);   /* high nibble */
            idx++;
            NIBBLETOUCHEXCHAR ((octet & 0x0F), str[idx]);   /* low nibble */
            idx++;
         }

         str[idx++] = 'P';

         /* append the exponent and null term.  %A always uses at least one
            digit for the exponent, so that's what I've done.
         */
         os_snprintf(str+idx, strSize - idx, "%+" OSINT64FMT_TS,
                     pFloat->exponent);
      }
   }

   return 0;
}


void rtxBigFloatFree (OSCTXT* pCtxt, OSBigFloat* pFloat)
{
   if (pFloat == 0) return;

   rtxBigIntFree(pCtxt, &pFloat->mantissa);
}
