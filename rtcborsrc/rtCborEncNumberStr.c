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

#include <string.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBigFloat.h"
#include "rtxsrc/rtxBigInt.h"
#include "rtxsrc/rtxError.h"

int rtCborEncNumberStr (OSCTXT* pctxt, const char* pvalue)
{
   int ret = 0;
   if (0 == pvalue) return LOG_RTERR (pctxt, RTERR_INVPARAM);

   if (0 != strpbrk (pvalue, ".Ee")) {
      /* floating point */
      OSBigFloat bigFloat;
      rtxBigFloatInit (&bigFloat);

      ret = rtxBigFloatSetStr (pctxt, &bigFloat, pvalue);
      if (0 == ret) {
         ret = rtCborEncBigFloat (pctxt, &bigFloat);
         rtxBigFloatFree (pctxt, &bigFloat);
      }
   }
   else {
      /* integer */
      OSBigInt bigInt;
      rtxBigIntInit (&bigInt);

      ret = rtxBigIntSetStr (pctxt, &bigInt, pvalue, 10);
      if (0 == ret) {
         ret = rtCborEncBigInt (pctxt, &bigInt);
         rtxBigIntFree (pctxt, &bigInt);
      }
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
