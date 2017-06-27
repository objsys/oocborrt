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

int rtCborEncTime (OSCTXT* pctxt, time_t value)
{
   int ret;
   OSOCTET tag = 0xC1;

   /* Write outer tag byte */
   ret = rtxWriteBytes (pctxt, &tag, 1);
   if (0 == ret) {
      /* Encode time value */
      ret = rtCborEncTimeValue (pctxt, value, OSRTCBOR_UINT);
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
