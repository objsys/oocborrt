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

int rtJsonDecBool (OSCTXT* pctxt, OSBOOL* pvalue)
{
   int stat = 0;
   OSOCTET ub, us[5];
   OSBOOL value = FALSE;
   size_t falseLen = 5;
   size_t trueLen = 4;

   if (pvalue)
      *pvalue = FALSE;

   rtJsonDecSkipWhitespace (pctxt);

   /* First character must be a 0, 1, t, or f */
   stat = rtxReadBytes (pctxt, &ub, 1);
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   if (ub == '0') {
      value = FALSE;
   }
   else if (ub == '1') {
      value = TRUE;
   }
   else if (ub == 'f') {
      us[0] = ub;
      stat = rtxReadBytes (pctxt, us+1, falseLen-1);
      if (stat == 0 && OSCRTLSTRNCMP ((char*)us, "false", 5) == 0)
         value = FALSE;
      else
         stat = LOG_RTERRNEW (pctxt, RTERR_BADVALUE);
   }
   else if (ub == 't') {
      us[0] = ub;
      stat = rtxReadBytes (pctxt, us+1, trueLen-1);
      if (stat == 0 && OSCRTLSTRNCMP ((char*)us, "true", 4) == 0)
         value = TRUE;
      else
         stat = LOG_RTERRNEW (pctxt, RTERR_BADVALUE);
   }

   if ((stat == 0) && (pvalue))
   {
      *pvalue = value;
   }

   return stat;
}


