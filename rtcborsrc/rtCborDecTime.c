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
#include "rtxsrc/rtxError.h"

#include <time.h>

int rtCborDecTime (OSCTXT* pctxt, OSOCTET byte1, time_t* pvalue)
{
   int ret;
   OSOCTET tag = 0xC1, innerTag;

   if (byte1 != tag) {
      rtxErrAddUIntParm (pctxt, tag);
      rtxErrAddUIntParm (pctxt, byte1);
      return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
   }

   /* Read inner tag */
   ret = rtxReadBytes (pctxt, &innerTag, 1);
   if (0 == ret) {
      if ((innerTag >> 5) != OSRTCBOR_UINT) {
         rtxErrAddUIntParm (pctxt, OSRTCBOR_UINT);
         rtxErrAddUIntParm (pctxt, (innerTag >> 5));
         return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
      }

      ret = rtCborDecTimeValue (pctxt, innerTag, pvalue);
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
