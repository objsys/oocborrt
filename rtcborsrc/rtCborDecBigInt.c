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

#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBigInt.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxDateTime.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"

int rtCborDecBigInt (OSCTXT* pctxt, OSOCTET byte1, OSBigInt* pvalue)
{
   int ret;
   OSOCTET innerTag;
   OSDynOctStr64 octetString;  /* byte string holding the big num */
   OSOCTET majorType = byte1 >> 5;

   if ( majorType == OSRTCBOR_UINT || majorType == OSRTCBOR_NEGINT)
   {
      OSUINT64 value;
      ret = rtCborDecUInt64(pctxt, byte1 & 0x1F, &value);
      if ( ret < 0 ) return LOG_RTERR(pctxt, ret);

      if ( majorType == OSRTCBOR_UINT ) {
         ret = rtxBigIntSetUInt64(pctxt, pvalue, value);
         if ( ret < 0 ) return LOG_RTERR(pctxt, ret);
      }
      else {
         if ( value < OSUINT64MAX ) {
            /* The value is -(value=1). We can just add 1, set the value, and
               then set the sign to negative */
            value++;
            ret = rtxBigIntSetUInt64(pctxt, pvalue, value);
            if ( ret < 0 ) return LOG_RTERR(pctxt, ret);

            pvalue->sign = -1;
         }
         else {
            /* value is -(OSUINT64MAX + 1).  In unsigned form that is:
               0x01 0x00 .. 0x00 (8 zero bytes). We can just copy that data in.
            */
            static const OSOCTET vplus1[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0 };
            rtxBigIntEnsureCapacity(pctxt, pvalue, 9);
            pvalue->numocts = 9;
            pvalue->sign = -1;
            memcpy(pvalue->mag, vplus1, 9);
         }
      }
      return 0;
   }
   /* else: expecting major type 6 with tag of 2 or 3 */

   if (byte1 != 0xC2 && byte1 != 0xC3) {
      rtxErrAddUIntParm (pctxt, 0xC2);
      rtxErrAddUIntParm (pctxt, byte1);
      return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
   }

   /* Read inner tag */

   ret = rtxReadBytes (pctxt, &innerTag, 1);
   if (0 == ret) {
      if ((innerTag >> 5) != OSRTCBOR_BYTESTR) {
         rtxErrAddUIntParm (pctxt, OSRTCBOR_BYTESTR);
         rtxErrAddUIntParm (pctxt, (innerTag >> 5));
         return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
      }
   }
   else return LOG_RTERR (pctxt, ret);

   /* Decode big integer content.  It is a byte string, which could be
      indeterminate length, possibly even with > 1 segment.
   */
   ret = rtCborDecDynByteStr (pctxt, innerTag, &octetString);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   if ( octetString.numocts == 0 )
      return LOG_RTERR(pctxt, RTERR_BADVALUE);  /* must have 1 or more bytes */


   if ( byte1 == 0xC3 ) {
      /* Negative bignum.  Let the value-as-decoded be X.  Then the represented
         value is -(X+1).  We'll need to add 1 to X and set the sign.
      */

      /* add 1 to value, starting with LSB */
      OSSIZE idx = octetString.numocts;  /* > 0 */
      do {
         idx--;
         octetString.data[idx]++;
         if ( octetString.data[idx] != 0 ) {
            /* there was no carry; we're done */
            break;
         }
         /* else: there was carry; keep going */
      } while ( idx > 0 );

      if ( idx == 0 && octetString.data[0] == 0 )
      {
         /* there was a carry in the highest position.
            We cannot use octetString.data; we need 1 extra byte.
            Ensure we have the needed capacity, then set the first octet to 1
            for the carry; the remaining octets will all be zero.*/
         ret = rtxBigIntEnsureCapacity(pctxt, pvalue, octetString.numocts + 1);
         if ( ret < 0 ) return LOG_RTERR(pctxt, ret);

         pvalue->numocts = octetString.numocts + 1;
         pvalue->mag[0] = 1;
         OSCRTLMEMSET(pvalue->mag + 1, 0, octetString.numocts);
      }
      else {
         /* no carry in highest position; we are done modifying
            octetString.data and we'll use it directly. */
         rtxBigIntFree(pctxt, pvalue);
         pvalue->mag = octetString.data;  /* remaining fields set below,
                                             and leading zero bytes trimmed */
      }

      pvalue->sign = -1;
   }
   else {
      /* positive big int*/

      rtxBigIntFree(pctxt, pvalue);

      pvalue->sign = 1; /* possibly should be zero; we'll fix it below if so */

      pvalue->mag = octetString.data;  /* remaining fields set below,
                                             and leading zero bytes trimmed */
   }

   if ( pvalue->mag == octetString.data )
   {
      OSSIZE idx;

      /* we used the octet string data directly */
      pvalue->allocated = octetString.numocts;
      pvalue->numocts = octetString.numocts;
      pvalue->dynamic = TRUE;

      /* now get rid of leading zero bytes.*/
      idx = 0;
      while ( idx < pvalue->allocated - 1 && pvalue->mag[idx] == 0 )
      { idx++; }

      /* idx points to the first nonzero byte or, if all bytes are zero, the
         last byte.  After trimming the leading zero bytes, there should be
         allocated - idx bytes (>= 1) in mag */
      if ( pvalue->numocts > pvalue->allocated - idx )
      {
         pvalue->numocts = pvalue->allocated - idx;
         memmove(pvalue->mag, pvalue->mag + idx, pvalue->numocts);
      }

      /* Correct the sign for zero if the value is zero. */
      if ( pvalue->numocts == 1 && pvalue->mag[0] == 0 ) pvalue->sign = 0;
   }
   else {
      rtxMemFreePtr(pctxt, octetString.data);
   }

   return ret;
}
