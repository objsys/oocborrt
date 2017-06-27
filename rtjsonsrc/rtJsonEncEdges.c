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


int rtJsonEncStartObject(OSCTXT* pctxt, const OSUTF8CHAR* name, OSBOOL noComma)
{
   int stat = 0;
   OSOCTET state = pctxt->state;

   if (noComma)
   {
      // Set state to not write comma for prior line...
      pctxt->state = OSJSONNOCOMMA;
   }
   rtJsonEncIndent(pctxt);
   pctxt->state = state;

   // Don't add name if top-level or name is empty...
   if ((pctxt->level > 0) && (strlen((const char *)name) > 0) && !noComma)
   {
      stat = rtJsonEncStringValue(pctxt, name);
      rtJsonEncBetweenObject(pctxt);
   }

   OSRTSAFEPUTCHAR(pctxt, (OSJSONSEQOF == pctxt->state) ? '[' : '{');

   return stat;
}

int rtJsonEncEndObject(OSCTXT* pctxt)
{
   OSOCTET state = pctxt->state;
   OSBOOL whitespace = (pctxt->indent > 0);

   if (whitespace)
   {
      // Set state to not write comma for prior line, then restore to
      //  previous state...
      pctxt->state = OSJSONNOCOMMA;
      rtJsonEncIndent(pctxt);
      pctxt->state = state;
   }
   OSRTSAFEPUTCHAR(pctxt, (OSJSONSEQOF == pctxt->state) ? ']' : '}');

   return 0;
}

int rtJsonEncBetweenObject(OSCTXT* pctxt)
{
   OSBOOL whitespace = (pctxt->indent > 0);

   if (whitespace)
   {
      OSRTSAFEPUTCHAR(pctxt, ' ');
   }
   OSRTSAFEPUTCHAR(pctxt, ':');
   if (whitespace)
   {
      OSRTSAFEPUTCHAR(pctxt, ' ');
   }

   return 0;
}
