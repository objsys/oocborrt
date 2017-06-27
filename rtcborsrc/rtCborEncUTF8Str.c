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

int rtCborEncUTF8Str (OSCTXT* pctxt, const char* pvalue, OSSIZE segsize)
{
   int ret;
   OSSIZE slen = OSCRTLSTRLEN(pvalue);

   if (segsize > 0) {
      /* Do indefinite length encoding */
      OSOCTET ub = (OSOCTET)((OSRTCBOR_UTF8STR << 5)|OSRTCBOR_INDEF);

      ret = rtxWriteBytes (pctxt, &ub, 1);

      if (0 == ret) {
         OSSIZE idx;
         OSOCTET eoc = OSRTCBOR_EOC;

         for (idx = 0; idx < slen; ) {
            OSSIZE chunkSize = OSRTMIN (segsize, slen-idx);
            ret = rtCborEncSize (pctxt, chunkSize, OSRTCBOR_UTF8STR);
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
      ret = rtCborEncSize (pctxt, slen, OSRTCBOR_UTF8STR);
      if (0 == ret) {
         /* Copy bytes to output buffer or stream */
         ret = rtxWriteBytes (pctxt, pvalue, slen);
      }
   }

   return (0 != ret) ? LOG_RTERR (pctxt, ret) : 0;
}
