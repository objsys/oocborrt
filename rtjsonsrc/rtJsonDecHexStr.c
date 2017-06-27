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

#include "rtjsonsrc/osrtjson.h"

int rtJsonDecHexStr
(OSCTXT* pctxt, OSOCTET* pvalue, OSUINT32* pnocts, size_t bufsize)
{
   size_t oldsize = 0;
   int stat = 0;
   OSUTF8CHAR c;
   if (pnocts)
      *pnocts = 0;

   rtJsonDecSkipWhitespace (pctxt);
   stat = rtJsonDecMatchChar (pctxt, '"');
   if (0 != stat) return LOG_RTERR (pctxt, stat);

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
         if ((oldsize & 1) == 0)
            pvalue[oldsize >> 1] = (OSOCTET) (c << 4);
         else
            pvalue[oldsize >> 1] |= c;
      }

      if (stat < 0)
         return LOG_RTERR (pctxt, stat);

	  oldsize++;

   }

   if (stat >= 0) {
      stat = 0;

      if (pvalue) {
         if ((oldsize & 1) != 0) {/* shift hex str (211 -> 0211) */
            OSUTF8CHAR* p = pvalue + oldsize / 2;

            for (;p != pvalue; p--) {
               *p = (OSOCTET) ((p[-1] << 4) | (*p >> 4));
            }

            *p >>= 4;
         }
      }

      if (pnocts)
         *pnocts = (OSUINT32) ((oldsize + 1) / 2);
   }

   return stat;
}


