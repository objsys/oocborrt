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
#include "rtjsonsrc/osrtjson.h"

#ifndef XMLP_DECODE_SEGSIZE
   #define XMLP_DECODE_SEGSIZE 1
#endif

int rtJsonDecDynHexStr (OSCTXT* pctxt, OSDynOctStr* pvalue)
{
   OSOCTET data = 0;
   int stat = 0;
   OSUINT32 nchars = 0;
   OSRTMEMBUF membuf;
   OSUTF8CHAR c;

   if (pvalue) {
      pvalue->numocts = 0;
      pvalue->data = 0;
   }

   rtJsonDecSkipWhitespace (pctxt);
   stat = rtJsonDecMatchChar (pctxt, '"');
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   rtxMemBufInit (pctxt, &membuf, 128);

   for (;;) {
      stat = rtxReadBytes (pctxt, &c, 1);
      if (stat == RTERR_ENDOFBUF) break;
      else if (0 != stat) return LOG_RTERR (pctxt, stat);

      if (c == ',' || c == '}') {
         /* Push character back to be reread again */
         OSRTASSERT (pctxt->buffer.byteIndex > 0);
         pctxt->buffer.byteIndex--;
         break;
      }

      /* Consume characters until ending double-quote character (") is found */
      if (c == '"') break;

      if (c >= '0' && c <= '9')
         c -= '0';
      else if (c >= 'a' && c <= 'f')
         c -= 'a' - 10;
      else if (c >= 'A' && c <= 'F')
         c -= 'A' - 10;
      else {
         stat = LOG_RTERRNEW (pctxt, RTERR_INVHEXS);
         break;
      }

      if (pvalue) {
         if ((nchars & 1) == 0)
            data = (OSOCTET) (c << 4);
         else {
            data |= c;
            stat = rtxMemBufAppend (&membuf, &data, 1);
            if (0 != stat) return LOG_RTERR (pctxt, stat);
         }
      }

      if (stat < 0)
         return LOG_RTERR (pctxt, stat);

      nchars++;
   }

   if (stat >= 0) {
      stat = 0;

      if (pvalue) {
         if ((nchars & 1) != 0) {/* shift hex str (211 -> 0211) */
            OSOCTET* pdata = membuf.buffer;
            OSUTF8CHAR* p = pdata + nchars / 2;

            for (;p != pdata; p--) {
               *p = (OSOCTET) ((p[-1] << 4) | (*p >> 4));
            }

            *p >>= 4;
         }

         pvalue->numocts = nchars / 2;
         pvalue->data = membuf.buffer;
      }
      else
         rtxMemBufFree (&membuf);
   }
   else
      rtxMemBufFree (&membuf);

   return stat;
}


