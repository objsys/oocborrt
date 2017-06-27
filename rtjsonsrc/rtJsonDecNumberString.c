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
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxCtype.h"

static int parseDigits
(OSCTXT* pctxt, char* outbuf, OSSIZE* pbufidx, OSSIZE bufsize)
{
   /* Parse digits */
   for (;;) {
      char ch = rtJsonDecPeekChar2 (pctxt);

      if (OS_ISDIGIT(ch)) {
         if (*pbufidx < bufsize) {
            outbuf[(*pbufidx)++] = ch;
            pctxt->buffer.byteIndex++;
         }
         else return LOG_RTERR (pctxt, RTERR_TOOBIG);
      }
      else break;
   }

   return 0;
}

int rtJsonDecNumberString (OSCTXT* pctxt, char** ppCharStr)
{
   char ch, tmpbuf[1025];
   OSSIZE tmpidx = 0, bufsize = sizeof(tmpbuf) - 1;
   OSBOOL needDigit = FALSE;
   int ret = 0;

   /* TODO: current implementation has size limit = sizeof(tmpbuf)-1.
      Use MemBuf to allow arbitray sized numbers to be parsed */

   ch = rtJsonDecPeekChar2 (pctxt);
   if (0 == ch) return LOG_RTERR (pctxt, RTERR_ENDOFBUF);

   /* Parse optional minus sign */
   if (ch == '-') {
      tmpbuf[tmpidx++] = ch;
      pctxt->buffer.byteIndex++;

      ch = rtJsonDecPeekChar2 (pctxt);
      if (0 == ch) return LOG_RTERR (pctxt, RTERR_ENDOFBUF);

      needDigit = TRUE;
   }

   /* Parse leading digit */
   if (ch == '0') {
      needDigit = FALSE;
      tmpbuf[tmpidx++] = ch;
      pctxt->buffer.byteIndex++;

      ch = rtJsonDecPeekChar2 (pctxt);
   }
   else if (ch >= '1' && ch <= '9') {
      needDigit = FALSE;
      tmpbuf[tmpidx++] = ch;
      pctxt->buffer.byteIndex++;

      /* Parse other digits */
      ret = parseDigits (pctxt, tmpbuf, &tmpidx, bufsize);
      if (0 != ret) return LOG_RTERR (pctxt, ret);
   }

   /* Check for floating point number */
   if (ch == '.') {
      if (tmpidx < bufsize) {
         needDigit = TRUE;
         tmpbuf[tmpidx++] = ch;
         pctxt->buffer.byteIndex++;
      }
      else return LOG_RTERR (pctxt, RTERR_TOOBIG);

      /* Parse digits */
      ret = parseDigits (pctxt, tmpbuf, &tmpidx, bufsize);
      if (0 != ret) return LOG_RTERR (pctxt, ret);

      if (OS_ISDIGIT(tmpbuf[tmpidx])) needDigit = FALSE;
   }

   /* Check for exponent */
   if ((ch == 'E' || ch == 'e') && !needDigit) {
      if (tmpidx < bufsize) {
         needDigit = TRUE;
         tmpbuf[tmpidx++] = ch;
         pctxt->buffer.byteIndex++;
      }
      else return LOG_RTERR (pctxt, RTERR_TOOBIG);

      ch = rtJsonDecPeekChar2 (pctxt);
      if (ch == '+' || ch == '-') {
         if (tmpidx < bufsize) {
            tmpbuf[tmpidx++] = ch;
            pctxt->buffer.byteIndex++;
         }
         else return LOG_RTERR (pctxt, RTERR_TOOBIG);
      }

      /* Parse digits */
      ret = parseDigits (pctxt, tmpbuf, &tmpidx, bufsize);
      if (0 != ret) return LOG_RTERR (pctxt, ret);

      if (OS_ISDIGIT(tmpbuf[tmpidx])) needDigit = FALSE;
   }

   tmpbuf[tmpidx++] = '\0';

   /* Allocate memory for number string and copy number */
   if (0 != ppCharStr) {
      *ppCharStr = rtxStrdup (pctxt, tmpbuf);
      if (0 == *ppCharStr) return LOG_RTERR (pctxt, RTERR_NOMEM);
   }

   return needDigit ? LOG_RTERR (pctxt, RTERR_INVFORMAT) : 0;
}
