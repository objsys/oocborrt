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
#include "rtxsrc/rtxMemBuf.h"
#include "rtjsonsrc/osrtjson.h"

int rtJsonDecStringValue (OSCTXT* pctxt, OSUTF8CHAR** ppvalue)
{
   OSRTMEMBUF membuf;
   int stat;
   OSOCTET ub;
   OSBOOL escapedChar = FALSE;

   /* Consume whitespace until starting double-quote character (") is found */
   stat = rtJsonDecMatchChar (pctxt, '"');
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   /* If JSON attribute flag is set, match '@' as next character.
      Do not include this character in decoded content. */
   if (rtxCtxtTestFlag (pctxt, OSJSONATTR)) {
      rtxCtxtClearFlag (pctxt, OSJSONATTR);

      stat = rtJsonDecMatchChar (pctxt, '@');
      if (0 != stat) return LOG_RTERR (pctxt, stat);
   }

   rtxMemBufInit (pctxt, &membuf, 128);

   /* Consume characters until ending double-quote character (") is found */
   for (;;) {
      stat = rtxReadBytes (pctxt, &ub, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);

      if (escapedChar) {
         escapedChar = FALSE;
         /* Handle escaped character */
         switch (ub) {
         case '"':
         case '\\':
         case '/':
            /* no action, ub already contains the correct character */
            break;
         case '\b':
            ub = '\b'; break;
         case '\f':
            ub = '\f'; break;
         case '\n':
            ub = '\n'; break;
         case '\r':
            ub = '\r'; break;
         case '\t':
            ub = '\t'; break;
         default:
            return LOG_RTERR (pctxt, RTERR_INVCHAR);
         }
         stat = rtxMemBufAppend (&membuf, &ub, 1);
         if (0 != stat) return LOG_RTERR (pctxt, stat);
      }
      else if ('"' == ub) break;
      else if (ub == '\\') {
         escapedChar = TRUE;
      }
      else {
         stat = rtxMemBufAppend (&membuf, &ub, 1);
         if (0 != stat) return LOG_RTERR (pctxt, stat);
      }
   }

   /* Null-terminate string */
   ub = '\0';
   stat = rtxMemBufAppend (&membuf, &ub, 1);
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   *ppvalue = membuf.buffer;

   return 0;
}

int rtJsonDecXmlStringValue (OSCTXT* pctxt, OSXMLSTRING* pvalue)
{
   if (0 == pvalue) return LOG_RTERR (pctxt, RTERR_NULLPTR);
   else {
      OSUTF8CHAR* strvalue = NULL;
      int stat = rtJsonDecStringValue (pctxt, &strvalue);
      if (stat != 0) return LOG_RTERR (pctxt, stat);

      pvalue->cdata = FALSE;
      pvalue->value = strvalue;
   }
   return 0;
}

int rtJsonDecStringObject
(OSCTXT* pctxt, const OSUTF8CHAR* name, OSUTF8CHAR** ppvalue)
{
   int stat;
   OSUTF8NameAndLen nameArray[1];

   /* Match expected name token */
   nameArray[0].name = name;
   nameArray[0].len = OSUTF8LEN (name);

   stat = rtJsonDecMatchObjectStart (pctxt, nameArray, 1);
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   /* Decode value */
   stat = rtJsonDecStringValue (pctxt, ppvalue);
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   /* Match ending } char */
   stat = rtJsonDecMatchChar (pctxt, '}');
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   return 0;
}

int rtJsonDecNameValuePair (OSCTXT* pctxt, OSUTF8NVP* pvalue)
{
   int stat;
   OSUTF8CHAR* strval = NULL;

   /* Decode name */
   stat = rtJsonDecStringValue (pctxt, &strval);
   if (0 != stat) return LOG_RTERR (pctxt, stat);
   else pvalue->name = strval;

   /* Match colon character */
   stat = rtJsonDecMatchChar (pctxt, ':');
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   /* Decode string value */
   stat = rtJsonDecStringValue (pctxt, &strval);
   if (0 != stat) return LOG_RTERR (pctxt, stat);
   else pvalue->value = strval;

   return 0;
}
