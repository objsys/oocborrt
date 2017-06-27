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

#include "rtxsrc/rtxCtype.h"
#include "rtjsonsrc/osrtjson.h"

static const int multiplier[4] = {1, 10, 100, 1000};
static const int tzh_multiplier[4] = {60, 600};
static const int tzm_multiplier[4] = {1, 10};

enum DecDateFrom {
   Years, Months, Days, TimeStart, TimeEnd = 8
};

static int rtJsonDecDateTimeInner (OSCTXT* pctxt, OSXSDDateTime* pvalue,
   int decDateFrom, int decDateTo)
{
   OSBOOL minus = FALSE;
   OSBOOL tz_minus = FALSE;
   OSREAL frac = 0.1;
   int minusCnt = 0;
   int stat = 0;
   OSXSDDateTime value;
   OSUTF8CHAR c;

   int mode = decDateFrom;
   /* 0 - yyyy, 1 - mm, 2 - dd, 3 - hh, 4 - mm, 5 - ss, 6 - .ss,
   7 - tzh, 8 - tzm, 9 - end */

   int idx = (decDateFrom == Years) ? 3 : 1;

   if (!pvalue)
      pvalue = &value;

   OSCRTLMEMSET (pvalue, 0, sizeof(OSXSDDateTime));

   rtJsonDecSkipWhitespace (pctxt);

   stat = rtxReadBytes (pctxt, &c, 1);
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   /* Check for '-' first character */
   if ((mode == 0) && (c == '-')) {
      minus = TRUE;
   }

   for (;;) {
      stat = rtxReadBytes (pctxt, &c, 1);
      if (stat == RTERR_ENDOFBUF) break;
      else if (0 != stat) return LOG_RTERR (pctxt, stat);

      if (c == ',' || c == ']' || c == '}') {
         /* Push character back to be reread again */
         OSRTASSERT (pctxt->buffer.byteIndex > 0);
         pctxt->buffer.byteIndex--;
         break;
      }

      if (mode == 9) {
         stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
         break;
      }

      if (OS_ISDIGIT (c)) {
         OSINT32 tm = c - '0';

         if ((mode == 0) && (idx < 0)) { /* year > 4 digits */
            pvalue->year *= 10;
            idx = 0;
         }

         if ((idx < 0) && (mode != 6)) {
            stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
            break;
         }

         switch (mode) {
         case 0:
            pvalue->year += tm * multiplier[idx--];
            break;
         case 1:
            if (decDateFrom == 1 && minusCnt != 2) {
               stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
               break;
            }

            pvalue->mon = (OSUINT8)(pvalue->mon + tm * multiplier[idx--]);
            break;
         case 2:
            if (decDateFrom == 2 && minusCnt != 3) {
               stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
               break;
            }

            pvalue->day = (OSUINT8)(pvalue->day + tm * multiplier[idx--]);
            break;
         case 3:
            pvalue->hour = (OSUINT8)(pvalue->hour + tm * multiplier[idx--]);
            break;
         case 4:
            pvalue->min = (OSUINT8)(pvalue->min + tm * multiplier[idx--]);
            break;
         case 5:
            pvalue->sec += tm * multiplier[idx--];
            break;
         case 6:
            pvalue->sec += tm * frac;
            frac *= 0.1;
            break;
         case 7:
            pvalue->tzo += tm * tzh_multiplier[idx--];
            break;
         case 8:
            pvalue->tzo += tm * tzm_multiplier[idx--];
            break;
         }

         if (0 != stat)
         {
            break;
         }
      }
      else if ((c == '-') && (idx == 1) &&
               (decDateFrom == 1 || decDateFrom == 2) &&
               (mode == decDateFrom)) {

         if (minusCnt <= decDateFrom)
            minusCnt++;
         else {
            stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
            break;
         }
      }
      else if (idx < 0) {
         if (c == '-') {
            if (mode < 2) {
               mode++;
               idx = 1;
            }
            else if (mode == decDateTo || mode == 5 || mode == 6) {
               mode = 7;
               idx = 1;
               pvalue->tz_flag = TRUE;
               tz_minus = TRUE;
            }
            else {
               stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
               break;
            }
         }
         else if (c == 'T') {
            if ((decDateFrom == 0) && (mode == 2)) {
               mode++;
               idx = 1;
            }
            else {
               stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
               break;
            }
         }
         else if (c == ':') {
            if((mode > 2) && ((mode < 5) || (mode == 7))) {
               mode++;
               idx = 1;
            }
            else {
               stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
               break;
            }
         }
         else if (c == '.') {
            if(mode == 5)
               mode++;
            else {
               stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
               break;
            }
         }
         else if (c == '+') {
            if(mode == decDateTo || mode == 5 || mode == 6) {
               mode = 7;
               idx = 1;
               pvalue->tz_flag = TRUE;
            }
            else {
               stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
               break;
            }
         }
         else if (c == 'Z') {
            pvalue->tz_flag = TRUE;
            mode = 9;
         }
         else {
            stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
            break;
         }
      }
      else {
         stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
         break;
      }
   }

   if (stat >= 0) {
      stat = 0;
      if ((idx >= 0) || ((decDateTo < TimeEnd) && (mode < decDateTo))) {
         stat = LOG_RTERRNEW (pctxt, RTERR_INVFORMAT);
      }
   }

   if (minus) pvalue->year = -pvalue->year;
   if (tz_minus) pvalue->tzo = -pvalue->tzo;

   return stat;
}

EXTJSONMETHOD int rtJsonDecDate (OSCTXT* pctxt, OSXSDDateTime* pvalue)
{
   return rtJsonDecDateTimeInner (pctxt, pvalue, Years, Days);
}

EXTJSONMETHOD int rtJsonDecTime (OSCTXT* pctxt, OSXSDDateTime* pvalue)
{
   return rtJsonDecDateTimeInner (pctxt, pvalue, TimeStart, TimeEnd);
}

EXTJSONMETHOD int rtJsonDecDateTime (OSCTXT* pctxt, OSXSDDateTime* pvalue)
{
   return rtJsonDecDateTimeInner (pctxt, pvalue, Years, TimeEnd);
}

EXTJSONMETHOD int rtJsonDecGYear (OSCTXT* pctxt, OSXSDDateTime* pvalue)
{
   return rtJsonDecDateTimeInner (pctxt, pvalue, Years, Years);
}

EXTJSONMETHOD int rtJsonDecGYearMonth (OSCTXT* pctxt, OSXSDDateTime* pvalue)
{
   return rtJsonDecDateTimeInner (pctxt, pvalue, Years, Months);
}

EXTJSONMETHOD int rtJsonDecGMonth (OSCTXT* pctxt, OSXSDDateTime* pvalue)
{
   return rtJsonDecDateTimeInner (pctxt, pvalue, Months, Months);
}

EXTJSONMETHOD int rtJsonDecGMonthDay (OSCTXT* pctxt, OSXSDDateTime* pvalue)
{
   return rtJsonDecDateTimeInner (pctxt, pvalue, Months, Days);
}

EXTJSONMETHOD int rtJsonDecGDay (OSCTXT* pctxt, OSXSDDateTime* pvalue)
{
   return rtJsonDecDateTimeInner (pctxt, pvalue, Days, Days);
}

