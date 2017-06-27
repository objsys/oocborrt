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
#define UINTFUNCNAME rtCborEncUInt32
#define UINTTYPE OSUINT32
#endif

int UINTFUNCNAME (OSCTXT* pctxt, UINTTYPE value, OSRTCBOR_MajorType majorType)
{
   UINTTYPE temp = value;
   OSOCTET  lb, tmpbuf[sizeof(UINTTYPE)+1];
   OSSIZE   cnt = 0, idx = sizeof(tmpbuf);
   int ret;

   /* Encode value to temp buffer */
   do {
      lb = (OSOCTET) (temp % 256);
      temp /= 256;
      tmpbuf[--idx] = lb; cnt++;
   } while (temp != 0);

   if (value <= 23) {} /* no action */
   else if (value <= OSUINT8_MAX) {
      tmpbuf[--idx] = OSRTCBOR_UINT8_ID; cnt++;
   }
   else if (value <= OSUINT16_MAX) {
      while (cnt < 2) {
         tmpbuf[--idx] = 0; cnt++;
      }
      tmpbuf[--idx] = OSRTCBOR_UINT16_ID; cnt++;
   }
   else if (value <= OSUINT32_MAX) {
      while (cnt < 4) {
         tmpbuf[--idx] = 0; cnt++;
      }
      tmpbuf[--idx] = OSRTCBOR_UINT32_ID; cnt++;
   }
   else {
      while (cnt < 8) {
         tmpbuf[--idx] = 0; cnt++;
      }
      tmpbuf[--idx] = OSRTCBOR_UINT64_ID; cnt++;
   }

   OSRTASSERT (idx < sizeof(tmpbuf) && cnt <= sizeof(tmpbuf));

   if (0 != majorType) tmpbuf[idx] |= (majorType << 5);

   ret = rtxWriteBytes (pctxt, &tmpbuf[idx], cnt);

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
