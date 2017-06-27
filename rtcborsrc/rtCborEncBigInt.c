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
#include "rtxsrc/rtxCommonDefs.h"
#include "rtxsrc/rtxBigInt.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"

int rtCborEncBigInt (OSCTXT* pctxt, OSBigInt* pvalue)
{
   /*
   Encode the value as CBOR int, if possible.  Otherwise, encode as bignum
   using rtCborEncBigNum.

   There are three cases where we can encode using cbor int:
   1) positive value and numocts <= 8
   2) negative value and numocts <= 8
   3) negative value and numocts == 9 and MSB is 1 and all remaining bytes
      are zero. In this case, the value is -(OSUINT64MAX + 1) and the
      value-as-encoded is OSUINT64MAX).
   */
   if ( pvalue->numocts <= 8 || (pvalue->sign < 0 &&
                                 pvalue->numocts == 9 && pvalue->mag[0] == 1))
   {
      OSUINT64 valueToEncode = 0;

      /* convert mag into OSUINT64MAX.  Note that for numocts == 9, we aren't
         doing a full conversion.  This is accounted for further down.
      */
      OSUINT8 i;
      for (i = 0; i < pvalue->numocts && i < 8; i++) {
         valueToEncode = valueToEncode * 256 + pvalue->mag[i];
      }

      if ( pvalue->sign >= 0 ) {
         return rtCborEncUInt64(pctxt, valueToEncode, OSRTCBOR_UINT);
      }
      else {
         if ( pvalue->numocts <= 8 ) {
            /* subtract 1 and encode that */
            valueToEncode--;
            return rtCborEncUInt64(pctxt, valueToEncode, OSRTCBOR_NEGINT);
         }
         else if ( pvalue->numocts == 9 && valueToEncode == 0x0100000000000000
                  && pvalue->mag[8] == 0 )
         {
            /* 3rd case applies: MSB is 1 and all other bytes, incl. 9th byte
               are zero. */
            return rtCborEncUInt64(pctxt, OSUINT64MAX, OSRTCBOR_NEGINT);
         }
         /* else: negative value requires CBOR bignum */
      }
   }

   /* The value cannot be encoded using CBOR int.  Use bignum. */
   return rtCborEncBigNum(pctxt, pvalue);
}

int rtCborEncBigNum (OSCTXT* pctxt, OSBigInt* pvalue)
{
   int ret;
   OSSIZE nbytes;
   OSOCTET tag;
   OSOCTET* pByte = pvalue->mag;         /* for looping over bytes */
   OSOCTET* pFirstByte = pvalue->mag;    /* first byte to encode */
   OSOCTET* pFirstAltered = pByte;  /* the most significant byte that will be
                                       altered when subtracting 1.  This is
                                       also the least significant byte that is
                                       non-zero.*/
   OSOCTET* pLastByte = pvalue->mag + pvalue->numocts - 1;

   if (0 == pvalue)
      return LOG_RTERR (pctxt, RTERR_BADVALUE);

   /* Determine the number of bytes we will encode, taking into account the
      trimming of leading zero bytes.
   */
   if ( pvalue->sign < 0 ) {
      /* Negative value.  Advance pFirstByte to skip leading zeros. Also locate
         the most significant byte that will have 1 subtracted from it
         (this will be pFirstAltered).  This byte could turn into a zero and
         also be trimmed.
      */
      while ( pByte <= pLastByte ) {
         if ( *pByte != 0 ) pFirstAltered = pByte;
         else if ( pFirstAltered == pvalue->mag && pvalue->mag[0] == 0 &&
                   pByte < pLastByte )
         {
            /* This is a zero byte, it is not the last byte, and we have not yet
               found any non-zero bytes.  Thus, this is a trimmable leading zero
               byte*/
            pFirstByte = pByte + 1;
         }
         /* else: zero byte but it is preceeded by one or more non-zero bytes */

         pByte++;
      }

      nbytes = pLastByte - pFirstByte + 1;

      if ( pFirstByte == pFirstAltered && *pFirstAltered == 1
            && pFirstAltered < pLastByte )
      {
         /* The first byte to encode and the first altered byte are the same
         byte, and the first altered byte == 1.  Thus, the first altered byte
         will become a leading zero and there will be one less byte to encode.
         */
         nbytes--;
      }
      else if ( pFirstByte == pLastByte && *pFirstByte == 0 )
      {
         /* The OSBigInt represented negative zero, which should not be allowed.
            Fix the sign to be zero. We'll still have correctly set pFirstByte
            and nbytes;
         */
         pvalue->sign = 0;
      }
   }
   else {
      /* Zero or positive value.  Advance pFirstByte to skip leading zeros.
         Do not let pFirstByte point past pLastByte (can't trim to nothing).
      */
      while ( pFirstByte < pLastByte ) {
         if ( *pFirstByte != 0 ) break;
         else pFirstByte++;
      }
      nbytes = pLastByte - pFirstByte + 1;
   }

   if (pvalue->sign >= 0 ) tag = 0xC2;
   else tag = 0xC3;

   /* Write bignum pos/neg tag */
   ret = rtxWriteBytes (pctxt, &tag, 1);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* Encode byte string tag and byte string length */
   ret = rtCborEncSize (pctxt, nbytes, OSRTCBOR_BYTESTR);
   if (0 != ret) return LOG_RTERR (pctxt, ret);


   if ( pvalue->sign < 0 ) {
      /* encode each byte as-is until the first altered byte.
         For the first altered byte, encode that byte - 1. If that is
         zero and it would be the first byte encoded, then omit it.
         For all subsequent bytes, encode 0xFF.
      */
      nbytes = pFirstAltered - pFirstByte;
      if ( nbytes > 0 ) {
         ret = rtxWriteBytes (pctxt, pFirstByte, nbytes);
         if (0 != ret) return LOG_RTERR (pctxt, ret);
      }

      if ( nbytes > 0 || *pFirstAltered > 1 || pLastByte == pFirstAltered )
      {
         OSOCTET value = *pFirstAltered - 1;
         ret = rtxWriteBytes (pctxt, &value, 1);
         if (0 != ret) return LOG_RTERR (pctxt, ret);
      }
      /* else nbytes == 0 && *pFirstAltered == 1 && pLastByte > pFirstAlered
         The first byte (also the first altered byte) becomes a non-final
         leading zero byte, which we omit. */

      nbytes = pLastByte - pFirstAltered;
      while ( nbytes > 0 ) {
         static const OSOCTET FF = 0xFF;
         ret = rtxWriteBytes (pctxt, &FF, 1);
         if (0 != ret) return LOG_RTERR (pctxt, ret);
         nbytes--;
      }
   }
   else {
      /* encode the bytes as-is */
      ret = rtxWriteBytes (pctxt, pFirstByte, nbytes);
      if (0 != ret) return LOG_RTERR (pctxt, ret);
   }

   return 0;
}
