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
#include "rtxsrc/rtxError.h"
#include "rtjsonsrc/osrtjson.h"

int rtJsonEncStringValue2
(OSCTXT* pctxt, const OSUTF8CHAR* value, size_t valueLen)
{
   OSRTSAFEPUTCHAR (pctxt, '"');
   if (0 != value && 0 != *value) {
      size_t i;

      /* If encoding JSON attribute and first character of name if not
         '@', add it now.. */
      if (rtxCtxtTestFlag (pctxt, OSJSONATTR)) {
         if (value[0] != '@') {
            OSRTSAFEPUTCHAR (pctxt, '@');
         }
         rtxCtxtClearFlag (pctxt, OSJSONATTR);
      }

      /* Encode string content */

      for (i = 0; i < valueLen; i++) {
         switch (value[i]) {
         case '"':
         case '\\':
            OSRTSAFEPUTCHAR (pctxt, '\\');
            OSRTSAFEPUTCHAR (pctxt, value[i]);
            break;
         case '\b':
            OSRTSAFEPUTCHAR (pctxt, '\\');
            OSRTSAFEPUTCHAR (pctxt, 'b');
            break;
         case '\f':
            OSRTSAFEPUTCHAR (pctxt, '\\');
            OSRTSAFEPUTCHAR (pctxt, 'f');
            break;
         case '\n':
            OSRTSAFEPUTCHAR (pctxt, '\\');
            OSRTSAFEPUTCHAR (pctxt, 'n');
            break;
         case '\r':
            OSRTSAFEPUTCHAR (pctxt, '\\');
            OSRTSAFEPUTCHAR (pctxt, 'r');
            break;
         case '\t':
            OSRTSAFEPUTCHAR (pctxt, '\\');
            OSRTSAFEPUTCHAR (pctxt, 't');
            break;
         default:
            OSRTSAFEPUTCHAR (pctxt, value[i]);
         }
      }
   }
   OSRTSAFEPUTCHAR (pctxt, '"');

   return 0;
}

int rtJsonEncStringValue (OSCTXT* pctxt, const OSUTF8CHAR* value)
{
   if (0 == value) { value = OSUTF8(""); }
   return rtJsonEncStringValue2 (pctxt, value, OSUTF8LEN(value));
}

int rtJsonEncStringPair2
(OSCTXT* pctxt, const OSUTF8CHAR* name, size_t nameLen,
 const OSUTF8CHAR* value, size_t valueLen)
{
   int stat;
   OSBOOL whitespace = (pctxt->indent > 0);

   stat = rtJsonEncStringValue2 (pctxt, name, nameLen);

   if (0 == stat)
   {
      if (whitespace)
      {
         OSRTSAFEPUTCHAR(pctxt, ' ');
      }
      OSRTSAFEPUTCHAR (pctxt, ':');
      if (whitespace)
      {
         OSRTSAFEPUTCHAR(pctxt, ' ');
      }
   }

   if (0 == stat)
      stat = rtJsonEncStringValue2 (pctxt, value, valueLen);

   return stat;
}

int rtJsonEncStringPair
(OSCTXT* pctxt, const OSUTF8CHAR* name, const OSUTF8CHAR* value)
{
   if (OS_ISEMPTY (name)) return LOG_RTERR (pctxt, RTERR_BADVALUE);
   if (0 == value) { value = OSUTF8(""); }
   return rtJsonEncStringPair2
      (pctxt, name, OSUTF8LEN(name), value, OSUTF8LEN(value));
}

int rtJsonEncStringObject2
(OSCTXT* pctxt, const OSUTF8CHAR* name, size_t nameLen,
 const OSUTF8CHAR* value, size_t valueLen)
{
   int stat;
   OSRTSAFEPUTCHAR (pctxt, '{');

   stat = rtJsonEncStringPair2 (pctxt, name, nameLen, value, valueLen);

   if (0 == stat)
      OSRTSAFEPUTCHAR (pctxt, '}');

   return stat;
}

int rtJsonEncStringObject
(OSCTXT* pctxt, const OSUTF8CHAR* name, const OSUTF8CHAR* value)
{
   if (OS_ISEMPTY (name)) return LOG_RTERR (pctxt, RTERR_BADVALUE);
   if (0 == value) { value = OSUTF8(""); }
   return rtJsonEncStringObject2
      (pctxt, name, OSUTF8LEN(name), value, OSUTF8LEN(value));
}

int rtJsonEncStringNull(OSCTXT* pctxt)
{
   return rtxCopyAsciiText(pctxt, "null");
}

int rtJsonEncStringRaw(OSCTXT* pctxt, const OSUTF8CHAR* value)
{
   if (OS_ISEMPTY(value)) {
      return LOG_RTERR (pctxt, RTERR_BADVALUE);
   }

   return rtxCopyUTF8Text(pctxt, value);
}
