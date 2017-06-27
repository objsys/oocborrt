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

#include "osrtjson.h"

/*
 * This array is a lookup table that translates 6-bit positive integer
 * index values into their "Base64 Alphabet" equivalents as specified
 * in Table 1 of RFC 2045.
 */
static const char encodeTable[] = {
   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
   'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
   'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

int rtJsonEncBase64StrValue (OSCTXT* pctxt, OSUINT32 nocts,
                            const OSOCTET* value)
{
   int i, stat;
   char lbuf[128], *pDstData = (char*)lbuf;
   int numFullGroups = nocts/3;
   int numBytesInPartialGroup = nocts - 3*numFullGroups;
   const OSOCTET* pSrcData = value;

   OSRTSAFEPUTCHAR (pctxt, '"');
   /* Translate all full groups from byte array elements to Base64 */
   for (i = 0; i < numFullGroups; i++) {
      int byte0 = *pSrcData++ & 0xff;
      int byte1 = *pSrcData++ & 0xff;
      int byte2 = *pSrcData++ & 0xff;
      *pDstData++ = encodeTable[byte0 >> 2];
      *pDstData++ = encodeTable[((byte0 << 4) & 0x3f) | (byte1 >> 4)];
      *pDstData++ = encodeTable[((byte1 << 2) & 0x3f) | (byte2 >> 6)];
      *pDstData++ = (OSOCTET)encodeTable[byte2 & 0x3f];

      if (pDstData >= (((char*)lbuf) + sizeof(lbuf)) - 4) {
         /* flush buffer */
         *pDstData = '\0';
         stat = rtxCopyAsciiText (pctxt, lbuf);
         if (stat != 0) return LOG_RTERR (pctxt, stat);
         pDstData = lbuf;
      }
   }

   /* Translate partial group if present */
   if (numBytesInPartialGroup != 0) {
      int byte0 = *pSrcData++ & 0xff;
      *pDstData++ = encodeTable[byte0 >> 2];
      if (numBytesInPartialGroup == 1) {
         *pDstData++ = encodeTable[(byte0 << 4) & 0x3f];
         *pDstData++ = '=';
         *pDstData++ = '=';
      } else {
         /* assert numBytesInPartialGroup == 2; */
         int byte1 = *pSrcData++ & 0xff;
         *pDstData++ = encodeTable[((byte0 << 4) & 0x3f) | (byte1 >> 4)];
         *pDstData++ = encodeTable[(byte1 << 2) & 0x3f];
         *pDstData++ = '=';
      }
   }

   /* flush buffer */
   if (pDstData != lbuf) {
      *pDstData = '\0';
      stat = rtxCopyAsciiText (pctxt, lbuf);
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }
   OSRTSAFEPUTCHAR (pctxt, '"');

   return (0);
}
