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

#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxDateTime.h"
#include "rtxsrc/rtxError.h"

int rtCborEncDateTimeStr (OSCTXT* pctxt, const OSNumDateTime* pvalue)
{
   int ret;
   OSOCTET tag = 0xC0;
   OSUTF8CHAR strbuf[40];

   ret = rtxDateTimeToString (pvalue, strbuf, sizeof(strbuf));
   if (ret < 0) return LOG_RTERR (pctxt, ret);

   /* Write outer tag byte */
   ret = rtxWriteBytes (pctxt, &tag, 1);
   if (0 == ret) {
      /* Encode date/time string */
      ret = rtCborEncUTF8Str (pctxt, (const char*)strbuf, 0);
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
