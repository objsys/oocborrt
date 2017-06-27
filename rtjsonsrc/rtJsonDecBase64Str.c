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

#ifndef BASE64TOINT
#define BASE64TOINT(c) (((c) < 128) ? decodeTable [(c) - 40] : -1)
#endif

/*
 * This array is a lookup table that translates characters
 * drawn from the "Base64 Alphabet" (as specified in Table 1 of RFC 2045)
 * into their 6-bit positive integer equivalents. Characters that
 * are not in the Base64 alphabet but fall within the bounds of the
 * array are translated to -1. Note, first 40 values are omitted, because
 * all of them are -1. Use offset -40 to fetch values from this table.
 */
const signed char decodeTable[] = {
   /*
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, */
   -1, -1, -1, 62, -1, -1, -1, 63,
   52, 53, 54, 55, 56, 57, 58, 59,
   60, 61, -1, -1, -1, -1, -1, -1,
   -1,  0,  1,  2,  3,  4,  5,  6,
   7,   8,  9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22,
   23, 24, 25, -1, -1, -1, -1, -1,
   -1, 26, 27, 28, 29, 30, 31, 32,
   33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48,
   49, 50, 51, -1, -1, -1, -1, -1
};

int rtJsonDecBase64Str (OSCTXT* pctxt,
                       OSOCTET* pvalue, OSUINT32* pnocts,
                       size_t bufsize)
{
   size_t dstPos = 0;
   size_t srcPos = 0;
   OSBOOL pad = FALSE;
   int prevVal = 0; /* remove warning C4701 */
   int stat = 0;
   OSOCTET c;

   if (pnocts)
      *pnocts = 0;

   rtJsonDecSkipWhitespace (pctxt);
   stat = rtJsonDecMatchChar (pctxt, '"');
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   for (;;) {
      int val;
      stat = rtxReadBytes (pctxt, &c, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);

      if (c == ' ') continue;
      if (c == ',' || c == '}') {
         pctxt->buffer.byteIndex--;
         break;
      }

      /* Consume characters until ending double-quote character (") is found */
      if (c == '"') break;

      if (c == '=') {
         if ((srcPos & 3) >= 2) {
            pad = TRUE;
            srcPos++;
            continue;
         }
         else {
            stat = LOG_RTERRNEW (pctxt, RTERR_INVBASE64);
            break;
         }
      }

      if (pad) {
         stat = LOG_RTERRNEW (pctxt, RTERR_INVBASE64);
         break;
      }

      val = BASE64TOINT(c);

      if (val < 0) {
         stat = LOG_RTERRNEW (pctxt, RTERR_INVBASE64);
         break;
      }

      if (dstPos >= bufsize) {
         stat = LOG_RTERRNEW (pctxt, RTERR_STROVFLW);
         break;
      }

      if (pvalue) {
         switch (srcPos & 3) {
         case 0:
            pvalue[dstPos] = (OSOCTET)(val << 2);
            break;
         case 1:
            pvalue[dstPos] |= (OSOCTET)(val >> 4);
            dstPos++;
            break;
         case 2:
            pvalue[dstPos] = (OSOCTET)((prevVal << 4) | (val >> 2));
            dstPos++;
            break;
         case 3:
            pvalue[dstPos] = (OSOCTET)((prevVal << 6) | val);
            dstPos++;
            break;
         }
      }
      else if ((srcPos & 3) > 0)
         dstPos++;

      srcPos++;
      prevVal = val;
   }

   if (stat >= 0 && (srcPos % 4) != 0 &&
       c != ',' && c != '}') {
      stat = LOG_RTERRNEW (pctxt, RTERR_INVBASE64);
   }

   if (stat == 0) {
      if (pnocts)
         *pnocts = (OSUINT32)dstPos;
   } else
      LOG_RTERR (pctxt, RTERR_INVBASE64);

   return stat;
}


