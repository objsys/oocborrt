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

int rtCborDecSkipField (OSCTXT* pctxt, OSOCTET byte1)
{
   OSSIZE nitems;
   OSOCTET tag = byte1 >> 5;
   OSOCTET len = byte1 & 0x1F;
   OSOCTET ub;
   int ret = 0;

   if (len == OSRTCBOR_INDEF) {
      for (;;) {
         ret = rtxReadBytes (pctxt, &ub, 1);
         if (0 == ret) {
            if (ub == OSRTCBOR_EOC) break;
            else {
               if (tag == OSRTCBOR_ARRAY || tag == OSRTCBOR_MAP) {
                  /* Make recursive call */
                  ret = rtCborDecSkipField (pctxt, ub);
               }
               else { /* byte or text string, inner length must be definite */
                  OSSIZE nbytes;
                  ret = rtCborDecSize (pctxt, (ub & 0x1F), &nbytes);
                  if (0 == ret) {
                     ret = rtxReadBytes (pctxt, 0, nbytes);
                  }
               }
            }
            if (0 != ret) return LOG_RTERR (pctxt, ret);
         }
      }
   }
   else {
      /* Decode tag and number of items */
      ret = rtCborDecSize (pctxt, len, &nitems);
      if (0 == ret) {
         if (tag == OSRTCBOR_ARRAY || tag == OSRTCBOR_MAP) {
            OSSIZE i;

            /* Loop to skip array items */
            for (i = 0; i < nitems; i++) {
               /* Read byte from stream */
               ret = rtxReadBytes (pctxt, &ub, 1);
               if (0 == ret) {
                  /* Make recursive call */
                  ret = rtCborDecSkipField (pctxt, ub);
               }
               if (0 != ret) return LOG_RTERR (pctxt, ret);
            }
         }
         else {
            /* Skip primitive field */
            ret = rtxReadBytes (pctxt, 0, nitems);
         }
      }
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
