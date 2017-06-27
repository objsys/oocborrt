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
#include <stdio.h>
#include <string.h>
#include "rtxsrc/osMacros.h"
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxDateTime.hh"

int rtxTimeZoneToString
   (const OSNumDateTime* pvalue, OSUTF8CHAR* buffer, size_t bufsize)
{
   int stat = 0;
   char* const str = (char*)buffer;

   if (bufsize < 7) return RTERR_STROVFLW;

   *str = 0;

   if (pvalue->tz_flag) {
      /* add "-/+??:??" if tzo is out-of-range */
      OSINT32 tzo = pvalue->tzo; /* -840 <= tzo <= 840 */
      if (tzo < 0) {
         rtxStrcat (str, bufsize, "-");
         tzo = -tzo;
      }
      else if (tzo > 0)
         rtxStrcat (str, bufsize, "+");

      if (tzo < -840 || tzo > 840) {
         rtxStrcat (str, bufsize, "??:??");
         stat = RTERR_INVFORMAT;
      }
      else if (tzo == 0) {
         rtxStrcat (str, bufsize, "Z");
      }
      else {
         char tmpbuf[3];
         rtxIntToCharStr (tzo/60, tmpbuf, sizeof(tmpbuf), '0');
         rtxStrcat (str, bufsize, tmpbuf);
         rtxStrcat (str, bufsize, ":");
         rtxIntToCharStr (tzo%60, tmpbuf, sizeof(tmpbuf), '0');
         rtxStrcat (str, bufsize, tmpbuf);
      }
   }

   return (stat == 0) ? (int)OSCRTLSTRLEN ((const char*)buffer) : stat;
}
