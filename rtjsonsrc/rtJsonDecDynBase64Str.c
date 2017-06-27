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
#include "rtxsrc/rtxMemBuf.h"
#include "rtxsrc/rtxCtype.h"
#include "rtjsonsrc/osrtjson.h"

#ifndef JSON_DECODE_SEGSIZE
   #define JSON_DECODE_SEGSIZE 1
#endif

int rtJsonDecDynBase64Str (OSCTXT* pctxt, OSDynOctStr* pvalue)
{
   OSOCTET data;
   int stat = 0;
   OSUINT32 nocts = 0;
   OSRTMEMBUF membuf;
   size_t srcPos = 0;
   OSBOOL pad = FALSE;
   int prevVal = 0; /* remove warning C4701 */
   OSOCTET c;

   if (pvalue) {
      pvalue->numocts = 0;
      pvalue->data = 0;
   }

   rtJsonDecSkipWhitespace (pctxt);
   stat = rtJsonDecMatchChar (pctxt, '"');
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   rtxMemBufInit (pctxt, &membuf, 128);

   /* Consume characters until ending double-quote character (") is found */
   for (;;) {
      int val;
      stat = rtxReadBytes (pctxt, &c, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);

      if (c == ' ') continue;
      if (c == ',' || c == '}') {
         pctxt->buffer.byteIndex--;
         break;
      }

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

      switch (srcPos & 3) {
         case 0:
            data = (OSOCTET)(val << 2);
            stat = rtxMemBufAppend (&membuf, &data, 1);
            if (0 != stat) return LOG_RTERR (pctxt, stat);
            break;
         case 1:
            rtxMemBufCut (&membuf, 0, 1);
            data |= (OSOCTET)(val >> 4);
            stat = rtxMemBufAppend (&membuf, &data, 1);
            if (0 != stat) return LOG_RTERR (pctxt, stat);
            nocts++;
            break;
         case 2:
            data = (OSOCTET)((prevVal << 4) | (val >> 2));
            stat = rtxMemBufAppend (&membuf, &data, 1);
            if (0 != stat) return LOG_RTERR (pctxt, stat);
            nocts++;
            break;
         case 3:
            data = (OSOCTET)((prevVal << 6) | val);
            stat = rtxMemBufAppend (&membuf, &data, 1);
            if (0 != stat) return LOG_RTERR (pctxt, stat);
            nocts++;
            break;
      }

      srcPos++;
      prevVal = val;
   }

   if (stat >= 0 && (srcPos % 4) != 0 &&
       c != ',' && c != '}') {
      stat = LOG_RTERRNEW (pctxt, RTERR_INVBASE64);
   }

   if (stat >= 0) {
      stat = 0;

      if (pvalue) {
         pvalue->numocts = nocts;
         pvalue->data = membuf.buffer;
      }
      else
         rtxMemBufFree (&membuf);
   }
   else
      rtxMemBufFree (&membuf);

   return stat;
}


