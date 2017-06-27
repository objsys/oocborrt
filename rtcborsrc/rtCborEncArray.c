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

int rtCborEncArray
(OSCTXT* pctxt, const OSRTGenValue** values, OSSIZE nitems, OSUINT32 flags)
{
   int ret;

   /* Encode tag and number of items */
   if (0 != (flags & OSRTCBORM_INDEF)) {
      OSOCTET ub = (OSOCTET)((OSRTCBOR_ARRAY << 5)|OSRTCBOR_INDEF);
      ret = rtxWriteBytes (pctxt, &ub, 1);
   }
   else {
      ret = rtCborEncSize (pctxt, nitems, OSRTCBOR_ARRAY);
   }

   if (0 == ret) {
      OSSIZE i;

      /* Loop to encode array items */
      for (i = 0; i < nitems; i++) {
         ret = rtCborEncValue (pctxt, values[i], flags);
         if (0 != ret) return LOG_RTERR (pctxt, ret);
      }

      if (0 != (flags & OSRTCBORM_INDEF)) {
         OSOCTET eoc = OSRTCBOR_EOC;
         ret = rtxWriteBytes (pctxt, &eoc, 1);
         if (0 != ret) return LOG_RTERR (pctxt, ret);
      }

      return 0;
   }
   else return LOG_RTERR (pctxt, ret);
}
