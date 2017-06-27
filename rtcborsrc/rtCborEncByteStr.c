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

int rtCborEncByteStr
(OSCTXT* pctxt, const OSOCTET* pvalue, OSSIZE nbytes, OSSIZE segsize)
{
   int ret;

   if (segsize > 0) {
      /* Do indefinite length encoding */
      OSOCTET ub = (OSOCTET)((OSRTCBOR_BYTESTR << 5)|OSRTCBOR_INDEF);

      ret = rtxWriteBytes (pctxt, &ub, 1);

      if (0 == ret) {
         OSSIZE idx;
         OSOCTET eoc = OSRTCBOR_EOC;

         for (idx = 0; idx < nbytes; ) {
            OSSIZE chunkSize = OSRTMIN (segsize, nbytes-idx);
            ret = rtCborEncSize (pctxt, chunkSize, OSRTCBOR_BYTESTR);
            if (0 == ret) {
               /* Copy bytes to output buffer or stream */
               ret = rtxWriteBytes (pctxt, &pvalue[idx], chunkSize);
               if (0 == ret) idx += chunkSize;
            }
            if (0 != ret) break;
         }

         if (0 == ret) ret = rtxWriteBytes (pctxt, &eoc, 1);
      }
   }
   else {
      /* Encode tag and size */
      ret = rtCborEncSize (pctxt, nbytes, OSRTCBOR_BYTESTR);
      if (0 == ret) {
         /* Copy bytes to output buffer or stream */
         ret = rtxWriteBytes (pctxt, pvalue, nbytes);
      }
   }

   return (0 != ret) ? LOG_RTERR (pctxt, ret) : 0;
}
