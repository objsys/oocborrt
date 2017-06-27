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

#include <math.h>
#include <limits.h>
#include "rtxsrc/rtxCtype.h"
#include "rtjsonsrc/osrtjson.h"

int rtJsonDecDecimal (OSCTXT* pctxt, OSREAL* pvalue,
                     int totalDigits, int fractionDigits)
{
   int stat = 0;
   OSBOOL minus = FALSE;
   OSREAL value = 0.;
   OSREAL fracPos = 0.1;
   OSBOOL end = FALSE;
   OSBOOL significantDigit = FALSE;
   int digits = 0;
   int fracDigits = 0;
   OSOCTET ub;
   int mode = 0;
   /* 0 - sign, 1 - int, 2 - frac */

   if ( totalDigits == 0 ||
       (totalDigits > 0 && fractionDigits > totalDigits))
      return LOG_RTERRNEW (pctxt, RTERR_INVPARAM);

   if (totalDigits < 0)
      totalDigits = INT_MAX;

   if (fractionDigits < 0)
      fractionDigits = INT_MAX;

   if (pvalue)
      *pvalue = 0.;

   rtJsonDecSkipWhitespace (pctxt);

   /* Read and process numeric digit characters.  End of input is assumed
      to be first non-numeric character or '.' */

   for (;;) {
      stat = rtxReadBytes (pctxt, &ub, 1);
      if (stat == RTERR_ENDOFBUF) break;
      else if (0 != stat) return LOG_RTERR (pctxt, stat);

      if (ub == ',') {
         /* Push character back to be reread again */
         OSRTASSERT (pctxt->buffer.byteIndex > 0);
         pctxt->buffer.byteIndex--;
         break;
      }

      switch (mode) {
         case 0: /* sign */
            if (ub == '+') {
               mode = 1;
            }
            else if (ub == '-') {
               minus = TRUE;
               mode = 1;
            }
            else if (OS_ISDIGIT (ub)) {
               OSRTASSERT (pctxt->buffer.byteIndex > 0);
               pctxt->buffer.byteIndex--;
               mode = 1;
            }
            else if (ub == '.') {
               mode = 2;
            }
            else {
               return LOG_RTERR (pctxt, RTERR_INVCHAR);
            }
            break;
         case 1: /* integer */
            if (OS_ISDIGIT (ub)) {
               OSINT32 tm = ub - '0';

               if (tm > 0)
                  significantDigit = TRUE;

               if (significantDigit && (++digits > totalDigits)) {
                  return LOG_RTERRNEW (pctxt, RTERR_INVREAL);
               }

               value = value * 10. + tm;
            }
            else if (ub == '.') {
               mode = 2;
            }
            else {
               /* Push character back to be reread again */
               OSRTASSERT (pctxt->buffer.byteIndex > 0);
               pctxt->buffer.byteIndex--;
               end = TRUE;
            }
            break;
         case 2: /* fractional */
            if (OS_ISDIGIT (ub)) {
               OSINT32 tm = ub - '0';

               digits++;
               fracDigits++;

               if ((tm > 0) && ((digits > totalDigits) ||
                   (fracDigits > fractionDigits))) {
                  return LOG_RTERRNEW (pctxt, RTERR_INVREAL);
               }

               value += tm * fracPos;
               fracPos *= 0.1;
            }
            else {
               /* Push character back to be reread again */
               OSRTASSERT (pctxt->buffer.byteIndex > 0);
               pctxt->buffer.byteIndex--;
               end = TRUE;
            }
            break;
      }
      if (end) break;
   }

   /* If negative, negate number */
   if (minus)
      value = -value;

   if (pvalue)
      *pvalue = value;

   return stat;
}
