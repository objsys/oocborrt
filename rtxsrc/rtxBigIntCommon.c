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

#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxBigInt.hh"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"

/* bitsPerDigit in the given radix times 1024
 * Rounded up to avoid underallocation.
 */

unsigned short rtxBigIntBitsPerDigit (int radix) {
   switch (radix) {
      case 0: return 0;
      case 1: return 1024;
      case 2: return 2048;
      case 3: return 2648;
      case 4: return 3072;
      case 5: return 3402;
      case 6: return 3672;
      case 7: return 3899;
      case 8: return 4096;
      default: return 0;
   }
}

/* unsigned short bitsPerDigit[] = { 0,
    1024, 2048, 2648, 3072, 3402, 3672, 3899, 4096 }; */

short rtxBigIntByteRadix (int halfRadix) {
   switch (halfRadix) {
      case 0: case 2: case 3: case 6: case 7: return 0;
      case 1: case 8: return 0x100;
      case 4: return 0100;
      case 5: return 100;

      default: return 0;
   }
}

/* short byteRadix[] = {0, 0x100, 0, 0, 0100, 100, 0, 0, 0x100 }; */

/* Multiply x array times word y in place, and add word z */
void rtxBigIntDestructiveMulAdd(OSOCTET* x, int len, int y, int z) {
   /* Perform the multiplication word by word */
   long ylong = y;
   long zlong = z;

   unsigned long product = 0, sum;
   int i;
   long carry = 0;
   for (i = len-1; i >= 0; i--) {
      product = ylong * (x[i] & LONG_MASK) + carry;
      x[i] = (OSOCTET)product;
      carry = product >> 8;
   }

   /* Perform the addition */
   sum = (x[len-1] & LONG_MASK) + zlong;
   x[len-1] = (OSOCTET)sum;
   carry = sum >> 8;
   for (i = len-2; i >= 0; i--) {
      sum = (x[i] & LONG_MASK) + carry;
      x[i] = (OSOCTET)sum;
      carry = sum >> 8;
   }
}

/* Removes leading zero bytes */
void rtxBigIntRemoveLeadingZeroBytes (OSBigInt* pInt) {
   OSSIZE i = 0;

   while (i < pInt->numocts && pInt->mag[i] == 0)
      i++;

   pInt->numocts -= i;
   OSCRTLSAFEMEMMOVE (pInt->mag, pInt->numocts, pInt->mag + i, pInt->numocts);
}

/*
 * Shifts all bits to the left by 'shift' bits.
 */
int rtxBigIntShiftLeft(OSOCTET* units, int len, OSUINT32 shift) {
   int srcOct = shift >> ADDRESS_BITS;
   int srcIndex = shift & BIT_INDEX_MASK;
   int invIndex = BITS_PER_UNIT - srcIndex;
   int dstOct = 0;
   int targetOcts, unitsUsed = len;

   if(len == 0)
      return 0;
   len <<= ADDRESS_BITS;
   targetOcts = ((len - shift + BITS_PER_UNIT - 1) >> ADDRESS_BITS);
   /* Process all octets but the last octet */
   while(dstOct < targetOcts - 1) {
      units[dstOct++] = (OSOCTET)
         ((units[srcOct] << srcIndex) |
         ((invIndex == BITS_PER_UNIT) ?
            0 : (units[srcOct + 1] >> invIndex)));
      srcOct++;
      }

   len &= BIT_INDEX_MASK;
   /* Process the last octet */
   units[dstOct] = (OSOCTET)((unitsUsed == targetOcts ?
     (units[srcOct] & bitsLeftOf(len)) << srcIndex :
     (units[srcOct] << srcIndex) | ((invIndex == BITS_PER_UNIT) ? 0 :
     (units[srcOct + 1] & bitsLeftOf(len)) >> invIndex)));

   /* Clear out unused bytes */
   if(targetOcts < unitsUsed) {
      OSCRTLMEMSET(&units[targetOcts], 0, (unitsUsed - targetOcts) * sizeof(OSOCTET));
   }

   return 0;
}

/* Divide BigInt by ordinary int, Used by to string conversion function. */
int rtxBigIntDivideByInt (OSCTXT* pCtxt, OSBigInt* pDivident,
   int divisor, OSBigInt* pQuotient, int* reminder)
{
   OSOCTET *p = pDivident->mag, *pq;
   int sh = 4, pdvd = 0;
   OSSIZE dignum = 0, maxdigs;

   if (divisor == 0)
      return LOG_RTERRNEW (pCtxt, RTERR_INVPARAM);
   *reminder = 0;
   if (pDivident->sign == 0) {
      pQuotient->sign = 0;
      pQuotient->numocts = 0;
      return 0;
   }

   if (pQuotient->allocated < pDivident->numocts) {
      pQuotient->allocated = pDivident->numocts;
      if (pQuotient->dynamic) {
         OSOCTET* newmag =
            rtxMemReallocArray (pCtxt, pQuotient->mag, pQuotient->allocated, OSOCTET);
         if (newmag == NULL) {
            rtxMemFreeArray (pCtxt, pQuotient->mag);
            return LOG_RTERRNEW (pCtxt, RTERR_NOMEM);
         }
         pQuotient->mag = newmag;
      }
      else {
         pQuotient->mag = rtxMemAllocArray (pCtxt, pQuotient->allocated, OSOCTET);
         if (pQuotient->mag == NULL) return LOG_RTERRNEW (pCtxt, RTERR_NOMEM);
         pQuotient->dynamic = TRUE;
      }
   }
   OSCRTLMEMSET (pQuotient->mag, 0, pQuotient->allocated);
   pQuotient->numocts = pDivident->numocts;
   pq = pQuotient->mag + pQuotient->numocts - 1;
   pQuotient->sign = (pQuotient->sign * divisor > 0) ? 1: -1;
   maxdigs = pDivident->numocts * 2;
   while (dignum < maxdigs) {
      pdvd <<= 4;
      dignum++;
      pdvd = pdvd | ((*p>>sh) & 0xF);
      if (sh == 0) {
         sh = 4;
         p++;
      }
      else sh = 0;
      rtxBigIntShiftLeft (pQuotient->mag, (int)pQuotient->numocts, 4);
      if (pdvd >= divisor) {
         *pq |= (pdvd / divisor) & 0xF;
         pdvd = pdvd % divisor;
      }
      *reminder = pdvd;
   }
   rtxBigIntRemoveLeadingZeroBytes (pQuotient);
   return 0;
}

/* Copies one BigInt to another. Destination BigInt should not be initialized yet */
int rtxBigIntCopyAndInverse (OSCTXT* pCtxt, const OSBigInt* pSrc, OSBigInt* pDst) {
   pDst->dynamic = TRUE;
   if (pSrc->numocts) {
      pDst->allocated = pSrc->numocts + 1;
      pDst->mag = rtxMemAllocArray (pCtxt, pDst->allocated, OSOCTET);
      if (pDst->mag == NULL) return LOG_RTERRNEW (pCtxt, RTERR_NOMEM);
      if (pSrc->sign < 0) {
         pDst->numocts = rtxBigIntGetData (pCtxt, pSrc, pDst->mag, pDst->allocated);
         pDst->sign = 1;
      }
      else {
         OSCRTLSAFEMEMCPY (pDst->mag, pDst->allocated, pSrc->mag, pSrc->numocts);
         pDst->numocts = pSrc->numocts;
         pDst->sign = pSrc->sign;
      }
   }
   else {
      pDst->mag = NULL;
      pDst->numocts = pDst->allocated = 0;
   }
   return 0;
}

/* This func negates the magnitude and inverse the sign. For internal use only. */
int rtxBigIntNegate (OSBigInt* pInt)
{
   OSSIZE i;
   OSOCTET *p = pInt->mag;

   if (pInt->sign == 0) return 0;
   pInt->sign = (pInt->sign == 1) ? -1 : 1;

   /* Copy one's complement of input into output, leaving extra
    * int (if it exists) == 0x00 */
   for (i = 0; i < pInt->numocts; i++, p++)
      *p = (OSOCTET)~(*p);

   /* Add one to one's complement to generate two's complement */
   for (i = pInt->numocts - 1; ++pInt->mag[i] == 0; i--)
      ;
   return 0;
}

