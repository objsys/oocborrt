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

#include "rtxsrc/osMacros.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxUTF8.h"
#include "rtjsonsrc/osrtjson.h"

size_t rtJsonGetElemIdx
(OSCTXT* pctxt, const OSUTF8NameAndLen nameArray[], size_t nrows)
{
   OSUTF8CHAR* pToken = 0;
   OSSIZE i, parsedStrLen, retidx = OSNULLINDEX;
   int stat = 0;

   /* Parse token from input buffer or stream */
   stat = rtJsonDecStringValue (pctxt, &pToken);
   if (0 != stat) {
      LOG_RTERR (pctxt, stat);
      return OSNULLINDEX;
   }
   parsedStrLen = rtxUTF8LenBytes (pToken);

   rtxCtxtPushElemName (pctxt, pToken);

   /* Match : */
   stat = rtJsonDecMatchChar (pctxt, ':');
   if (0 != stat) { LOG_RTERR (pctxt, stat); }

   /* Loop through table rows looking for match */
   for (i = 0; i < nrows; i++) {
      if (parsedStrLen == nameArray[i].len &&
          rtxUTF8StrnEqual (pToken, nameArray[i].name, nameArray[i].len)) {
         retidx = i;
         break;
      }
   }

   return retidx;
}

int rtJsonDecMatchChar (OSCTXT* pctxt, OSUTF8CHAR ch)
{
   OSOCTET ub;
   int stat;

   /* Consume whitespace until character is found or EOF */
   for (;;) {
      stat = rtxReadBytes (pctxt, &ub, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);
      else if (ch == ub || !OS_ISSPACE (ub)) break;
   }

   if (ch != ub) {
      /* Push character back for reparse */
      pctxt->buffer.byteIndex--;

      /* Note: error should not be logged in this case because trial
         parse may be being done.  Caller can decide if error should
         be logged.. */
      return RTERR_INVCHAR;
   }
   else return 0;
}

int rtJsonDecPeekChar (OSCTXT* pctxt, OSUTF8CHAR* pch)
{
   OSOCTET ub;
   int stat;

   /* Consume whitespace until character is found or EOF */
   for (;;) {
      stat = rtxReadBytes (pctxt, &ub, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);
      else if (!OS_ISSPACE (ub)) break;
   }

   *pch = ub;

   /* Push character back for reparse */
   pctxt->buffer.byteIndex--;

   return 0;
}

char rtJsonDecPeekChar2 (OSCTXT* pctxt)
{
   OSOCTET ub;
   int stat;

   /* Consume whitespace until character is found or EOF */
   for (;;) {
      stat = rtxReadBytes (pctxt, &ub, 1);
      if (0 != stat) {
         LOG_RTERR (pctxt, stat);
         return 0;
      }
      else if (!OS_ISSPACE (ub)) break;
   }

   /* Push character back for reparse */
   pctxt->buffer.byteIndex--;

   return ub;
}

int rtJsonDecMatchToken2
(OSCTXT* pctxt, const OSUTF8CHAR* token, size_t tokenLen)
{
   OSUTF8CHAR* pToken = 0;
   OSSIZE savedByteIndex = OSNULLINDEX;
   int stat;

   if (0 == token || tokenLen == 0)
      return LOG_RTERR (pctxt, RTERR_INVPARAM);

   if (tokenLen > pctxt->buffer.size) {
      rtxErrAddStrParm
         (pctxt, "Match on token larger than context buffer size");
      return LOG_RTERR (pctxt, RTERR_NOTSUPP);
   }

   /* Mark current position */
   savedByteIndex = pctxt->buffer.byteIndex;

   /* Parse token from input buffer or stream */
   stat = rtJsonDecStringValue (pctxt, &pToken);
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   /* Compare token to parsed string value */
   stat = (tokenLen == rtxUTF8LenBytes (pToken) &&
           rtxUTF8StrnEqual (pToken, token, tokenLen)) ? 0 : RTERR_IDNOTFOU;

   /* If not matched, reset buffer or stream to starting position */
   if (stat == RTERR_IDNOTFOU) {
      pctxt->buffer.byteIndex = savedByteIndex;
   }

   rtxMemFreePtr (pctxt, pToken);

   return stat;
}

int rtJsonDecMatchToken (OSCTXT* pctxt, const OSUTF8CHAR* token)
{
   return rtJsonDecMatchToken2 (pctxt, token, OSUTF8LEN(token));
}

int rtJsonDecMatchObjectStart
(OSCTXT* pctxt, const OSUTF8NameAndLen* nameArray, size_t numNames)
{
   int stat;

   /* Match starting { char */
   stat = rtJsonDecMatchChar (pctxt, '{');

   /* Attempt to match name with name in nameArray */
   if (0 == stat) {
      size_t i = rtJsonGetElemIdx (pctxt, nameArray, numNames);
      if (i == OSNULLINDEX) stat = RTERR_IDNOTFOU;
   }

   return stat;
}

int rtJsonDecSkipWhitespace (OSCTXT* pctxt)
{
   OSOCTET ub;
   int stat;

   /* Consume whitespace until non-space character is found or EOF */
   for (;;) {
      stat = rtxReadBytes (pctxt, &ub, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);
      else if (!OS_ISSPACE (ub)) {
         pctxt->buffer.byteIndex--;
         break;
      }
   }

   return 0;
}

int rtJsonDecSkipWhitespaceAndComma (OSCTXT* pctxt)
{
   int stat;
   OSUTF8CHAR c;

   stat = rtJsonDecSkipWhitespace (pctxt);
   if (stat < 0) return LOG_RTERR (pctxt, stat);

   c = pctxt->buffer.data[pctxt->buffer.byteIndex];

   if (c == ',') {
      pctxt->buffer.byteIndex++;

      stat = rtJsonDecSkipWhitespace (pctxt);
      if (stat < 0) return LOG_RTERR (pctxt, stat);
   }

   return 0;
}
