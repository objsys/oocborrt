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

#include "osrtjson.h"
#include "rtxsrc/osMacros.h"
#include "rtxsrc/rtxErrCodes.h"
#include "rtxsrc/rtxDateTime.hh"

/*
 * rtJsonEncDate:
 * This function encodes a variable of the XSD DateTime type as an
 * string. This version of the function is used
 * to encode OSXSDDateTime value into CCYY-MM-DD format
 */
int rtJsonEncDatePart (OSCTXT* pctxt, const OSXSDDateTime* pvalue)
{
   int stat;
   char lbuf[80];

   if (pvalue->day < 1 || pvalue->day > 31 ||
       pvalue->mon < 1 || pvalue->mon > 12)
       return LOG_RTERRNEW (pctxt, RTERR_BADVALUE);

   stat = rtxDatePartToString (pvalue, (OSUTF8CHAR*)lbuf, sizeof (lbuf));
   if (stat < 0) return LOG_RTERRNEW (pctxt, stat);

   stat = rtxCopyAsciiText (pctxt, lbuf);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   return 0;
}

int rtJsonEncDate (OSCTXT* pctxt, const OSXSDDateTime* pvalue)
{
   int stat;
   char lbuf[80];

   if (pvalue->day < 1 || pvalue->day > 31 ||
       pvalue->mon < 1 || pvalue->mon > 12)
       return LOG_RTERRNEW (pctxt, RTERR_BADVALUE);

   stat = rtxDateToString (pvalue, (OSUTF8CHAR*)lbuf, sizeof (lbuf));
   if (stat < 0) return LOG_RTERRNEW (pctxt, stat);

   stat = rtxCopyAsciiText (pctxt, lbuf);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   return 0;
}

/*
 * rtJsonEncTime:
 * This function encodes a variable of the XSD DateTime type as an
 * string. This version of the function is used
 * to encode OSXSDDateTime value into different format.
 */
int rtJsonEncTime (OSCTXT* pctxt, const OSXSDDateTime* pvalue)
{
   int stat;
   char lbuf[80] = "";

   if (pvalue->hour > 23 || pvalue->min  > 59 ||
       pvalue->sec < 0.0 || pvalue->sec >= 60.0)
      return LOG_RTERRNEW (pctxt, RTERR_BADVALUE);

   stat = rtxTimeToString (pvalue, (OSUTF8CHAR*)lbuf, sizeof (lbuf));
   if (stat < 0) return LOG_RTERRNEW (pctxt, stat);

   stat = rtxCopyAsciiText (pctxt, lbuf);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   return 0;
}

/*
 * rtJsonEncDateTime:
 * This function encodes a variable of the XSD DateTime type as an
 * string. This version of the function is used to encode OSXSDDateTime
 * value into <date>T<time> where <date> would be replaced by a
 * formatted date and <time> by a formatted time
 */
int rtJsonEncDateTime (OSCTXT* pctxt, const OSXSDDateTime* pvalue)
{
   int stat;

   stat = rtJsonEncDatePart (pctxt, pvalue);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   OSRTSAFEPUTCHAR (pctxt, 'T');
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   stat = rtJsonEncTime (pctxt, pvalue);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   return 0;
}



