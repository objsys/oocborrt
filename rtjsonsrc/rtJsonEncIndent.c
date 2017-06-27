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
#include "rtxsrc/rtxErrCodes.h"
#include "osrtjson.h"

/**
 * This function adds the given amount of indentation to the output
 * stream.
 */
int rtJsonEncIndent (OSCTXT* pctxt)
{
   register int i, stat, nspaces;

   if (pctxt->indent > 0) {
      char indentChar = ' ';

      nspaces = pctxt->level * pctxt->indent;

      /* Verify indentation whitespace will fit in encode buffer */
      stat = rtxCheckOutputBuffer (pctxt, nspaces + 1);
      if (stat != 0) return LOG_RTERR (pctxt, stat);

      /* Copy data to buffer */

      if (pctxt->state != OSJSONNOCOMMA)
      {
         // Add comma to preceding line before setting carriage return and
         //  indenting for current line...
         OSJSONPUTCOMMA(pctxt);
      }
      OSRTSAFEPUTCHAR (pctxt, '\n');

      for (i = 0; i < nspaces; i++) {
         OSRTSAFEPUTCHAR (pctxt, indentChar);
      }
   }
   else // No-whitespace case...
   {
      if (pctxt->state != OSJSONNOCOMMA)
      {
         // Add comma to "preceding" line...
         OSJSONPUTCOMMA(pctxt);
      }
   }
   return 0;
}

