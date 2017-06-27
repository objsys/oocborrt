/**
 * Copyright (c) 1997-2017 by Objective Systems, Inc.
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

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "rtxsrc/rtxCommon.h"

const signed char gDaysInMonth[12] =
   { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

OSBOOL rtxDateIsValid (const OSNumDateTime* dateTime)
{
   int dim;
   if (dateTime->year < 0 || dateTime->year > 9999 ||
       dateTime->mon < 1 || dateTime->mon > 12 ||
       dateTime->day < 1 || dateTime->day > 31 ||
       (dateTime->tz_flag && (dateTime->tzo < -840 || dateTime->tzo > 840)) )
      return FALSE;

   dim = (int) gDaysInMonth [dateTime->mon - 1];

   /* check for February of leap-year */
   if (dateTime->mon == 2 && dateTime->year % 4 == 0 &&
       (dateTime->year % 100 != 0 || dateTime->year % 400 == 0))
      dim++;

   if (dateTime->day < 1 || dateTime->day > dim)
      return FALSE;

   return TRUE;
}

OSBOOL rtxTimeIsValid (const OSNumDateTime* dateTime)
{
   if (dateTime->hour > 23 ||
       dateTime->min > 59 ||
       dateTime->sec < 0 || dateTime->sec >= 60 ||
       (dateTime->tz_flag && (dateTime->tzo < -840 || dateTime->tzo > 840)))
      return FALSE;

   return TRUE;
}


OSBOOL rtxDateTimeIsValid (const OSNumDateTime* dateTime)
{
   return (OSBOOL)(rtxDateIsValid (dateTime) && rtxTimeIsValid (dateTime));
}

#define SEC_PRECISION 0.0000005

static void normalizeTimeZone(OSNumDateTime*);

 /**
  * rtxCmpDate:
  * This function compares date part of two OSNumDateTime structure and
  * return the result as integer.
  */
int rtxCmpDate (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2)
{

   int stat = 0;
   if (pvalue1->year > pvalue2->year) {
      stat = 1;
   } else if (pvalue1->year < pvalue2->year) {
      stat = -1;
   } else {
      if (pvalue1->mon > pvalue2->mon) {
         stat = 1;
      } else if (pvalue1->mon < pvalue2->mon) {
         stat = -1;
      } else {
         if (pvalue1->day > pvalue2->day) {
            stat = 1;
         } else if (pvalue1->day < pvalue2->day) {
            stat = -1;
         } else {
            stat = 0;
         }
      }
   }
   return stat;
}

/**
 * rtxCmpTime:
 * This function compares time part of two OSNumDateTime structure and
 * return the result as integer.
 */
int rtxCmpTime (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2)
{
   int stat = 0;
   if (pvalue1->hour  > pvalue2->hour) {
      stat = 1;
   } else if (pvalue1->hour < pvalue2->hour) {
      stat = -1;
   } else {
      if (pvalue1->min > pvalue2->min) {
         stat = 1;
      } else if (pvalue1->min < pvalue2->min) {
         stat = -1;
      } else {
         if (pvalue1->sec > pvalue2->sec + SEC_PRECISION) {
            stat = 1;
         } else if (pvalue1->sec < pvalue2->sec  - SEC_PRECISION) {
            stat = -1;
         } else {
            stat = 0;
         }
      }
   }
   return stat;
}

/**
 * rtxCmpDateTime:
 * This function compares date and time  of two OSNumDateTime structure and
 * return the result as integer.
 */
int rtxCmpDateTime (const OSNumDateTime* pvalue1, const OSNumDateTime* pvalue2)
{
   int stat = 0;
   OSNumDateTime tmpvalue1, tmpvalue2;
   if (pvalue1->tz_flag == (OSBOOL)TRUE) {
      memcpy (&tmpvalue1, pvalue1, sizeof(OSNumDateTime));
      normalizeTimeZone (&tmpvalue1);
      pvalue1 = &tmpvalue1;
   }
   if (pvalue2->tz_flag == (OSBOOL)TRUE) {
      memcpy (&tmpvalue2, pvalue2, sizeof(OSNumDateTime));
      normalizeTimeZone (&tmpvalue2);
      pvalue2 = &tmpvalue2;
   }

   stat = rtxCmpDate(pvalue1, pvalue2);
   if (stat == 0)  stat = rtxCmpTime(pvalue1, pvalue2);
   return stat;
}

/**
 * normalizeTimeZone:
 * This function normalizes the Time Zone value of OSNumDateTime structure.
 */
#define IS_LEAP(y) (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))
#define FQUOTIENT(a,b)  (int)(floor((double)a/(double)b))
#define FQUOTIENT_RANGE(a,low,high)  (FQUOTIENT((a-low),(high-low)))
#define MODULO(a,b)     (a - (FQUOTIENT(a,b))*b)
#define MODULO_RANGE(a,low,high) ((MODULO((a-low),(high-low)))+low)
#define MAX_DAY_INMONTH(yr,mon) \
((IS_LEAP(yr)&& mon == 2)? (gDaysInMonth[mon-1] + 1) : gDaysInMonth[mon-1])

static void normalizeTimeZone(OSNumDateTime* pvalue)
{
   OSINT32 tmp, carry, tmpdays;
   tmp = pvalue->min - pvalue->tzo;
   pvalue->min = (OSINT8)(MODULO(tmp,60));
   carry = FQUOTIENT(tmp,60);
   tmp = pvalue->hour + carry;
   pvalue->hour = (OSINT8)(MODULO(tmp,24));
   carry = FQUOTIENT(tmp,24);
   tmpdays = pvalue->day + carry;

   for (;;) {
      if(tmpdays < 1){
         int tmon = MODULO_RANGE(pvalue->mon-1, 1, 13);
         int tyr  = pvalue->year + FQUOTIENT_RANGE(pvalue->mon-1, 1, 13);
         if (tyr == 0)
            tyr--;
         tmpdays += MAX_DAY_INMONTH(tyr, tmon);
         carry = -1;
      } else if (tmpdays > MAX_DAY_INMONTH(pvalue->year, pvalue->mon)){
         tmpdays = tmpdays - MAX_DAY_INMONTH(pvalue->year, pvalue->mon);
         carry = 1;
      } else
         break;
      tmp = pvalue->mon + carry;
      pvalue->mon = (OSINT8)(MODULO_RANGE(tmp,1,13));
      pvalue->year = (OSINT8)(pvalue->year + FQUOTIENT_RANGE(tmp,1,13));
   }
   pvalue->day = (OSINT8)tmpdays;
}
