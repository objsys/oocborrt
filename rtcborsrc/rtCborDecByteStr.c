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

int rtCborDecByteStr
(OSCTXT* pctxt, OSOCTET byte1, OSSIZE* pnumocts, OSOCTET* buffer, OSSIZE bufsiz)
{
   int ret;
   OSSIZE nbytes;
   OSOCTET tag = byte1 >> 5;
   OSOCTET len = byte1 & 0x1F;

   OSRTASSERT (tag == OSRTCBOR_BYTESTR);

   if (len == OSRTCBOR_INDEF) {
      OSSIZE idx = 0, segsize;
      for (;;) {
         ret = rtxReadBytes (pctxt, &byte1, 1);
         if (0 == ret) {
            if (byte1 == OSRTCBOR_EOC) break;
            else {
               ret = rtCborDecSize (pctxt, (byte1 & 0x1F), &segsize);
               if (0 == ret) {
                  if (idx + segsize <= bufsiz) {
                     ret = rtxReadBytes (pctxt, &buffer[idx], segsize);
                     if (0 == ret) idx += segsize;
                  }
                  else ret = RTERR_TOOBIG;
               }
            }
         }
         if (0 != ret) break;
      }

      if (0 == ret && 0 != pnumocts) *pnumocts = idx;
   }
   else {
      ret = rtCborDecSize (pctxt, (byte1 & 0x1F), &nbytes);
      if (0 == ret) {
         if (nbytes <= bufsiz) {
            ret = rtxReadBytes (pctxt, buffer, nbytes);
            if (0 == ret && 0 != pnumocts) *pnumocts = nbytes;
         }
         else ret = RTERR_TOOBIG;
      }
   }

   return (0 != ret) ? LOG_RTERR (pctxt, ret) : 0;
}
