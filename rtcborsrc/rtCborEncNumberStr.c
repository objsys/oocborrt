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

#include <ctype.h>
#include <errno.h>
#include <string.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBigFloat.h"
#include "rtxsrc/rtxBigInt.h"
#include "rtxsrc/rtxError.h"

/**
 * Return the number of significant digits in pvalue, a decimal numeric string
 * with a possible exponent.
 * The result is undefined if the string is not a valid numeric string.
 */ 
static size_t countSigDigits(const char* pvalue)
{
   size_t digits = 0;
   
   /* skip leading sign, zeros, and whitespace */
   while (*pvalue == '0' || *pvalue == '-' || *pvalue == '+' ||
          isspace(*pvalue) ) pvalue++;
          
   if ( *pvalue == '.' ) {
      /* skip decimal point and leading zeros after decimal point */
      pvalue++;
      while ( *pvalue == '0' ) pvalue++;
   }
   
   /* We're beyond leading zeros.  Remaining characters are all significant
      digits, possibly the decimal point, and E/e/null.
   */
   for(;;) {
      if ( *pvalue == 0 || *pvalue == 'E' || *pvalue == 'e') return digits;
      else if (*pvalue != '.') digits++;
      
      pvalue++;      
   }  
}

/*
   rtCborEncNumberStr: see description in header.
   
   The significance of DBL_DIG here is that any decimal floating-point number
   that has at most that many digits can be converted to double and then can
   be converted back to a decimal floating-point number with the exact same
   decimal digits.
*/
int rtCborEncNumberStr (OSCTXT* pctxt, const char* pvalue)
{
   int ret = 0;
   if (0 == pvalue) return LOG_RTERR (pctxt, RTERR_INVPARAM);

   if (0 != strpbrk (pvalue, ".Ee")) {
      double dblVal = 0;
      size_t sigDigits;
      
      sigDigits = countSigDigits(pvalue);
      
      if ( sigDigits < DBL_DIG )
      {
         errno = 0;
         dblVal = strtod(pvalue, 0);
      }
      
      if ( (sigDigits > 0 && dblVal == 0) ||
            sigDigits > DBL_DIG ||
            errno == ERANGE )
      {
         /* value has non-zero digits and strtod returned zero,
            OR value has more than DBL_DIG digits
            OR strtod indicated over/underflow.
            Use decimal fraction */
      OSBigFloat bigFloat;

      rtxBigFloatInit (&bigFloat);

      ret = rtxBigFloatSetStr (pctxt, &bigFloat, pvalue);
      if (0 == ret) {
         ret = rtCborEncBigFloat (pctxt, &bigFloat);
         rtxBigFloatFree (pctxt, &bigFloat);
         }
      }
      else {
         ret = rtCborEncFloat(pctxt, dblVal);
      }
   }
   else {
      /* integer */
      OSBigInt bigInt;
      rtxBigIntInit (&bigInt);

      ret = rtxBigIntSetStr (pctxt, &bigInt, pvalue, 10);
      if (0 == ret) {
         ret = rtCborEncBigInt (pctxt, &bigInt);
         rtxBigIntFree (pctxt, &bigInt);
      }
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
