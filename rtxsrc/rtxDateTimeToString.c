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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxDateTime.hh"
#include "rtxsrc/rtxUTF8.h"

int rtxDatePartToString
(const OSNumDateTime* pvalue, OSUTF8CHAR* buffer, size_t bufsize)
{
   char tmpbuf[10];
   int daysInMonth = 0, stat = 0;

   rtxIntToCharStr (pvalue->year, (char*)buffer, bufsize, 0);
   rtxStrcat ((char*)buffer, bufsize, "-");

   if (pvalue->mon < 1 || pvalue->mon > 12) {
      /*add "??" to buffer if mon is is out-of-range*/
      rtxStrcat ((char*)buffer, bufsize, "??");
      stat = RTERR_INVFORMAT;
      daysInMonth = -1;
   }
   else {
      rtxIntToCharStr (pvalue->mon, tmpbuf, 3, '0');
      rtxStrcat ((char*)buffer, bufsize, tmpbuf);
   }
   rtxStrcat ((char*)buffer, bufsize, "-");
   /* Test to make sure month was not invalid, otherwise this
      would index outside the array, possibly causing a crash. */
   if (0 == daysInMonth) {
      daysInMonth = gDaysInMonth[pvalue->mon - 1];
   }

   if (IS_LEAP(pvalue->year) && pvalue->mon == 2) {
      daysInMonth++;
   }
   if (pvalue->day < 1 || pvalue->day > daysInMonth) {
      /* add "??" to buffer if day is out-of-range */
      rtxStrcat ((char*)buffer, bufsize, "??");
      stat = RTERR_INVFORMAT;
   }
   else {
      rtxIntToCharStr (pvalue->day, tmpbuf, 3, '0');
      rtxStrcat ((char*)buffer, bufsize, tmpbuf);
   }

   return (stat == 0) ? (int)rtxUTF8LenBytes (buffer) : stat;
}

int rtxDateToString
(const OSNumDateTime* pvalue, OSUTF8CHAR* buffer, size_t bufsize)
{
   int stat = 0, stat2, len;

   stat = rtxDatePartToString (pvalue, buffer, bufsize);
   if (stat < 0) return stat;
   len = stat;

   if (pvalue->tz_flag) {
      stat2 = rtxTimeZoneToString
         (pvalue, buffer + stat, bufsize - stat);
      len += stat2;
      if (stat >= 0) stat = stat2;
   }

   return (stat == 0) ? len : stat;
}

int rtxTimeToString
(const OSNumDateTime* pvalue, OSUTF8CHAR* buffer, size_t bufsize)
{
   char tmpbuf[18];
   int  stat = 0, stat2;
   size_t len;

   if (bufsize < 9) { /* HH:MM:SS */
      return RTERR_STROVFLW;
   }

   if (pvalue->hour > 23) {
      /* add "??" to buffer if hour is is out-of-range */
      rtxStrcpy ((char*)buffer, bufsize, "??");
      stat = RTERR_INVFORMAT;
   }
   else {
      rtxIntToCharStr (pvalue->hour, tmpbuf, 3, '0');
      rtxStrcpy ((char*)buffer, bufsize, tmpbuf);
   }

   rtxStrcat ((char*)buffer, bufsize, ":");

   if (pvalue->min > 59) {
      /* add "??" to buffer if min is is out-of-range */
      rtxStrcat ((char*)buffer, bufsize, "??");
      stat = RTERR_INVFORMAT;
   }
   else {
      rtxIntToCharStr (pvalue->min, tmpbuf, 3, '0');
      rtxStrcat ((char*)buffer, bufsize, tmpbuf);
   }

   rtxStrcat ((char*)buffer, bufsize, ":");

   if (pvalue->sec < 0.0 || pvalue->sec >= 60.0) {
      /* add "??.??" if sec is out-of-range */
      rtxStrcat ((char*)buffer, bufsize, "??:??");
      stat = RTERR_INVFORMAT;
   }
   else {
      double frac = pvalue->sec - (int)pvalue->sec;
      rtxIntToCharStr ((int)pvalue->sec, tmpbuf, 3, '0');
      rtxStrcat ((char*)buffer, bufsize, tmpbuf);
      if (frac > 0.00000001 && frac < 1) {
         size_t i;
         /* 0.123456 + NUL = 9 bytes; integer part must be zero;
            will never overflow. */
         os_snprintf (tmpbuf, sizeof(tmpbuf), "%.6f", frac);
         len = OSCRTLSTRLEN (tmpbuf);

         /* cut trailing zeros off frac part */
         for (i = len - 1; i > 0; i--) {
            if (tmpbuf[i] == '0')
               tmpbuf[i] = 0;
            else
               break;
         }
         if (i >= 2) {
            if (bufsize > i+1) {
               rtxStrcat ((char*)buffer, bufsize, ".");
               rtxStrcat ((char*)buffer, bufsize, tmpbuf + 2);
            }
            else return RTERR_STROVFLW;
         }
      }
   }

   if (pvalue->tz_flag) {
      len = rtxUTF8LenBytes (buffer);
      stat2 = rtxTimeZoneToString (pvalue, buffer + len, bufsize - len);
      if (stat >= 0) stat = stat2;
   }

   return (stat >= 0) ? (int) rtxUTF8LenBytes (buffer) : stat;
}

int rtxDateTimeToString
(const OSNumDateTime* pvalue, OSUTF8CHAR* buffer, size_t bufsize)
{
   size_t len;
   int stat;

   stat = rtxDatePartToString (pvalue, buffer, bufsize);
   if (stat < 0) return stat;

   len = (size_t)stat;
   if (len + 3 >= bufsize) return stat; /* '\0', 'T',... */

   rtxStrcat ((char*)buffer, bufsize, "T");
   len++;

   stat = rtxTimeToString (pvalue, buffer+len, bufsize-len);
   if (stat < 0) return stat;

   return (int) rtxUTF8LenBytes (buffer);
}
