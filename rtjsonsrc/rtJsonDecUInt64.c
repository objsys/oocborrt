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

#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxError.h"
#include "rtjsonsrc/osrtjson.h"

static const OSUINT64 maxUInt64 = OSUI64CONST(0xFFFFFFFFFFFFFFFF);
static const OSUINT64 maxUInt64_10 = OSUI64CONST(0x1999999999999999);

int rtJsonDecUInt64Value (OSCTXT* pctxt, OSUINT64* pvalue)
{
   OSBOOL minus = FALSE;
   register OSUINT64 value = 0;
   int stat = 0;
   OSOCTET ub;

   if (pvalue)
      *pvalue = 0;

   /* Consume leading white space */

   rtJsonDecSkipWhitespace (pctxt);

   /* First character must be a + sign or digit */

   stat = rtxReadBytes (pctxt, &ub, 1);
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   if (ub == '+') {
      /* Read next char */
      stat = rtxReadBytes (pctxt, &ub, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);
   }

   /* Read and process numeric digit characters.  End of input is assumed
      to be first non-numeric character */

   for (;;) {
      if (OS_ISDIGIT (ub)) {
         OSUINT32 tm = ub - '0';

         if (value > maxUInt64_10)
            return LOG_RTERRNEW (pctxt, RTERR_TOOBIG);

         value *= 10;

         if (value > maxUInt64 - tm)
            return LOG_RTERRNEW (pctxt, RTERR_TOOBIG);

            value += tm;
         }
         else if (minus && ub == '0') ; /* -0000... */
         else {
         /* Push character back to be reread again */
         OSRTASSERT (pctxt->buffer.byteIndex > 0);
         pctxt->buffer.byteIndex--;
         break;
      }

      stat = rtxReadBytes (pctxt, &ub, 1);
      if (stat == RTERR_ENDOFBUF) break;
      else if (0 != stat) return LOG_RTERR (pctxt, stat);
   }

   if (stat >= 0) {
      if (pvalue)
         *pvalue = value;
   }

   return (0);
}
