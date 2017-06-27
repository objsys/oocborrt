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
#include "rtxsrc/rtxError.h"

#ifndef INTFUNCNAME
#define INTFUNCNAME rtCborDecInt32
#define INTTYPE OSINT32
#define INTMAX OSINT32_MAX
#define UINTFUNCNAME rtCborDecUInt32
#define UINTTYPE OSUINT32
#endif

int INTFUNCNAME (OSCTXT* pctxt, OSOCTET byte1, INTTYPE* pvalue)
{
   UINTTYPE ui;
   int ret;

   /* Decode unsigned integer */
   ret = UINTFUNCNAME (pctxt, (byte1 & 0x1F), &ui);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* If the value is negative and ui > INTMAX, then the value is less than
      the most negative value representable.
      If the value is positive and ui > INTMAX, then obviously it also too big.
   */
   if (ui > INTMAX) return LOG_RTERR (pctxt, RTERR_TOOBIG);

   /* Determine if negative integer */
   if ((byte1 >> 5) == OSRTCBOR_NEGINT) {
      if (0 != pvalue) *pvalue = 0 - (ui + 1);
   }
   else { /* Positive */
      if (0 != pvalue) *pvalue = (INTTYPE)ui;
   }

   return 0;
}
