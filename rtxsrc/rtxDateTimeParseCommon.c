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
#include "rtxsrc/rtxDateTime.hh"

int rtxParseTimeZone
(const OSUTF8CHAR* inpdata, size_t inpdatalen, OSNumDateTime* pvalue)
{
   const OSUTF8CHAR *cur = inpdata;
   OSUINT32 tmpi = 0;

   if (*cur != '+' && *cur != '-' && *cur != 'Z') return RTERR_INVFORMAT;
   pvalue->tz_flag = TRUE;

   /* If the character after HH:MM:SS.SS is Z set timezone value to 0 */
   if (*cur == 'Z') {
      pvalue->tzo = 0;
   }
   else {
      int sign = 1, stat;

      if (*cur == '-') sign = -1;

      /* If the char after HH:MM:SS.SS is +/- parse timezone format HH:MM */
      /* Parse timezone HH from next two charcter after +/- */
      cur++;

      if ((size_t)(cur - inpdata + 5) > inpdatalen) return RTERR_INVFORMAT;

      PARSE_2_DIGITS(tmpi, cur, stat);

      /* check the parse status of time zone hour
                    and presence of ':' after HH*/
      if (stat != 0 || *cur != ':') return RTERR_INVFORMAT;
      if (tmpi > 24) return RTERR_BADVALUE;
      pvalue->tzo = 60 * tmpi;

      /* Parse timezone MM from next two charcter after HH: */
      cur++;
      PARSE_2_DIGITS(tmpi, cur, stat);

      /* check the parse status of time zone min */
      if (stat != 0) return RTERR_INVFORMAT;
      if (tmpi > 59) return RTERR_BADVALUE;
      tmpi = pvalue->tzo + tmpi;
      if (tmpi > 840) return RTERR_BADVALUE;
      pvalue->tzo = tmpi;

      /* Set time zone value -ve if the timezone flag contains '-' */
      pvalue->tzo *= sign;
   }
   return (int)(cur-inpdata);
}

