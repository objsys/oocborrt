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

#ifndef UINTFUNCNAME
#define UINTFUNCNAME rtCborDecUInt32
#define UINTTYPE OSUINT32
#endif

int UINTFUNCNAME (OSCTXT* pctxt, OSOCTET byte1, UINTTYPE* pvalue)
{
   if (byte1 < OSRTCBOR_UINT8_ID) {
      if (0 != pvalue) *pvalue = byte1;
   }
   else {
      int ret;
      OSSIZE nbytes, bufidx = 0;
      OSOCTET tmpbuf[sizeof(UINTTYPE)], ub;

      switch (byte1) {
      case OSRTCBOR_UINT8_ID: nbytes = 1; break;
      case OSRTCBOR_UINT16_ID: nbytes = 2; break;
      case OSRTCBOR_UINT32_ID: nbytes = 4; break;
#ifndef _NO_INT64_SUPPORT
      case OSRTCBOR_UINT64_ID: nbytes = 8; break;
#endif
      default:
#ifndef _COMPACT
         rtxErrAddStrParm (pctxt, "27 or less");
         rtxErrAddUIntParm (pctxt, byte1);
#endif
         return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
      }

      /* Read value into temp buffer */
      if (nbytes <= sizeof(tmpbuf)) {
         ret = rtxReadBytes (pctxt, tmpbuf, nbytes);
         if (ret < 0) return LOG_RTERR (pctxt, ret);
      }
      else {
         /* The assumption is made that the decoded data is the right fit
            for the target variable.  IOW, the case for selecting a decoder
            using a smaller sized variable if the user has out-of-band
            knowledge that the value will be smaller than the selected
            encoded size is not considered.  For example, if the user
            selects an unsigned 16-bit integer target and the encoding is
            32-bit, it will result in this error, even if the decoded
            value is less than OSUINT16_MAX.  */
         return LOG_RTERR (pctxt, RTERR_TOOBIG);
      }

      /* Decode into target variable */
      if (0 != pvalue) {
         *pvalue = 0;
         for ( ; bufidx < nbytes; bufidx++) {
            ub = tmpbuf[bufidx];
            *pvalue = (*pvalue * 256) + ub;
         }
      }
   }

   return 0;
}
