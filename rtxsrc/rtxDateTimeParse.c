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

/**
 * rtxParseDateString: This function parses a date value from a supplied
 * string and sets the pointed OSNumDateTime to the decoded date value.
 */
static int rtxParseDatePartString
(const OSUTF8CHAR* inpdata, size_t inpdatalen, OSNumDateTime* pvalue)
{
   int stat = 0;
   OSUINT32 i = 0;
   OSUINT32 tmpi = 0;
   OSUINT32 daysInMonth;
   const OSUTF8CHAR *cur = inpdata;
   int sign = 1;

   if (inpdata == NULL) return RTERR_NOTINIT;

   if (inpdatalen < 10) return RTERR_INVFORMAT;

   /* parse year taking generally first 4 character
      AB: but it could be more characters, and even with sign '-'! */
   if (*cur == '-') {
      sign = -1;
      cur++;
   }
   for (i = 0; OS_ISDIGIT(*cur); i++) {
      tmpi = tmpi * 10 + (*cur - '0');
      cur++;
   }
   pvalue->year = tmpi * sign;

   /* check the presence of '-' after CCYY*/
   if (*cur != '-') return RTERR_INVFORMAT;

   /* parse month taking next 2 character after '-' */
   cur++;
   PARSE_2_DIGITS(tmpi, cur, stat);

   /* check the parse status of month presence of '-' after CCYY-MM*/
   if (stat != 0 || *cur != '-') return RTERR_INVFORMAT;
   if(tmpi < 1 || tmpi > 12) return RTERR_BADVALUE;
   pvalue->mon = (OSINT8)tmpi;

   /* parse day taking next 2 character after '-' */
   cur++;
   PARSE_2_DIGITS(tmpi, cur, stat);

   /* check the parse status of day and presence of '-' after CCYY-MM-DD*/
   if (stat != 0) return RTERR_INVFORMAT;

   daysInMonth = gDaysInMonth[pvalue->mon - 1];
   if (IS_LEAP(pvalue->year) && pvalue->mon == 2) {
      daysInMonth++;
   }
   if (tmpi < 1 || tmpi > daysInMonth) return RTERR_BADVALUE;
   pvalue->day = (OSINT8)tmpi;

   return (int)(cur-inpdata);
}

int rtxParseDateString
(const OSUTF8CHAR* inpdata, size_t inpdatalen, OSNumDateTime* pvalue)
{
   int stat = 0;
   const OSUTF8CHAR *cur = inpdata;

   if (inpdata == NULL) return RTERR_NOTINIT;

   stat = rtxParseDatePartString (inpdata, inpdatalen, pvalue);
   if (stat < 0) return stat;
   cur += stat;

   /* check the presence of character after CCYY-MM-DD */
   if ((size_t)(cur - inpdata) < inpdatalen) {

      stat = rtxParseTimeZone
         (cur, inpdatalen - (size_t)(cur - inpdata), pvalue);
      if (stat < 0) return stat;
      cur += stat;
   }
   else pvalue->tz_flag = FALSE;

   return 0;
}

/**
 * rtxParseTimeString:
 * This function decodes a time value from a supplied string and set the
 * pointed OSNumDateTime structure to the decoded time value.
 */
int rtxParseTimeString
(const OSUTF8CHAR* inpdata, size_t inpdatalen, OSNumDateTime* pvalue)
{
   int stat = 0;
   OSUINT32 tmpi = 0;
   OSREAL tmpd = 0.0;
   const OSUTF8CHAR *cur = inpdata;

   if (inpdata == NULL) return RTERR_NOTINIT;

   if (inpdatalen < 8)
      return RTERR_INVFORMAT;

   /* parse Hour taking first 2 character  */
   PARSE_2_DIGITS(tmpi, cur, stat);

   /* check the parse status of hour presence of ':' after HH*/
   if (stat != 0 || *cur != ':') return RTERR_INVFORMAT;
   if (tmpi > 23) return RTERR_BADVALUE;
   pvalue->hour = (OSINT8)tmpi;

   /* parse Minute taking next 2 character after ':'  */
   cur++;
   PARSE_2_DIGITS(tmpi, cur, stat);

   /* check the parse status of min presence of ':' after HH:MM*/
   if (stat != 0 || *cur != ':') return RTERR_INVFORMAT;
   if (tmpi > 59) return RTERR_BADVALUE;
   pvalue->min = (OSINT8)tmpi;

   /* parse Sec taking next 2 character after ':'  */
   cur++;
   PARSE_2_DIGITS(tmpi, cur, stat);

   /* check the parse status of sec presence of '.' after HH:MM:SS*/
   if (stat != 0) return RTERR_INVFORMAT;
   if ((size_t)(cur - inpdata) >= inpdatalen || *cur != '.') {
      pvalue->sec = tmpi;
   }
   else {
      double mult = 0.1;

      tmpd = tmpi;

      /* parse fractional Sec taking next n character after '.'  */
      cur++;

      if ((size_t)(cur - inpdata) >= inpdatalen) return RTERR_INVFORMAT;

      while ((size_t)(cur - inpdata) < inpdatalen && OS_ISDIGIT(*cur)) {
         tmpd += (*cur - '0') * mult;
         cur++;
         mult *= 0.1;
      }

      if (tmpd >= 60.0) return RTERR_BADVALUE;
      pvalue->sec = tmpd;
   }
   /* check the presence of character after HH:MM:SS.SS*/
   if ((size_t)(cur - inpdata) < inpdatalen) {

      stat = rtxParseTimeZone
         (cur, inpdatalen - (size_t)(cur - inpdata), pvalue);
      if (stat < 0) return stat;
      cur += stat;
   }
   else pvalue->tz_flag = FALSE;

   return 0;
}

/**
 * rtxParseDateTimeString:
 * This function decodes a date/time value from a supplied string and sets
 * the pointed OSNumDateTime to the decoded date and time value.
 */
int rtxParseDateTimeString
(const OSUTF8CHAR* inpdata, size_t inpdatalen, OSNumDateTime* pvalue)
{
   int stat;
   const OSUTF8CHAR *cur = inpdata;

   /* Decode the Date string from first 10 character */
   stat = rtxParseDatePartString (cur, inpdatalen, pvalue);
   if (stat < 0) return stat;

   cur = cur + stat;

   /* Check the presence of T after <Date> string */
   if (*cur != 'T') return RTERR_INVFORMAT;
   cur++;

   /* Decode the Time string after first 11 ie after <DATE>T  */
   stat = rtxParseTimeString (cur, inpdatalen - stat - 1, pvalue);

   return stat;
}
