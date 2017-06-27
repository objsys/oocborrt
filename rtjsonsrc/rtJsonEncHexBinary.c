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

int rtJsonEncHexStr (OSCTXT* pctxt, OSSIZE nocts, const OSOCTET* data)
{
   OSSIZE i, lbufx = 0;
   int  stat, ub;
   char lbuf[80];

   /* Verify indentation whitespace will fit in encode buffer */

   if (nocts > OSSIZE_MAX/2) return LOG_RTERR (pctxt, RTERR_TOOBIG);

   stat = rtxCheckOutputBuffer (pctxt, nocts*2);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   OSRTSAFEPUTCHAR (pctxt, '"');
   for (i = 0; i < nocts; i++) {
      if (lbufx >= sizeof(lbuf) - 2) {
         /* flush buffer */
         lbuf[lbufx] = '\0';
         stat = rtxCopyAsciiText (pctxt, lbuf);
         if (stat != 0) return LOG_RTERR (pctxt, stat);
         lbufx = 0;
      }

      ub = (data[i] >> 4) & 0x0f;
      if ((pctxt->flags & OSUPCASE) != 0)
         NIBBLETOUCHEXCHAR (ub, lbuf[lbufx++]);
      else
         NIBBLETOHEXCHAR (ub, lbuf[lbufx++]);

      ub = (data[i] & 0x0f);
      if ((pctxt->flags & OSUPCASE) != 0)
         NIBBLETOUCHEXCHAR (ub, lbuf[lbufx++]);
      else
         NIBBLETOHEXCHAR (ub, lbuf[lbufx++]);
   }
   lbuf[lbufx] = '\0';

   /* flush remaining characters from local buffer */

   if (lbufx > 0) {
      stat = rtxCopyAsciiText (pctxt, lbuf);
      if (stat != 0) return LOG_RTERR (pctxt, stat);
   }
   OSRTSAFEPUTCHAR (pctxt, '"');

   return 0;
}
