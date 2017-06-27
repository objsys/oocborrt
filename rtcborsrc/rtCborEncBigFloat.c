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
#include "rtxsrc/rtxBigFloat.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"

int rtCborEncBigFloat (OSCTXT* pctxt, OSBigFloat* pvalue)
{
   int ret = 0;

   OSOCTET tag;
   if ( pvalue->base10 ) {
      tag = 0xC4;
   }
   else tag = 0xC5;

   /* encode the decimal fraction or bigfloat tag */
   ret = rtxWriteBytes(pctxt, &tag, 1);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* encode tag for array of length 2 */
   tag = 0x82;
   ret = rtxWriteBytes(pctxt, &tag, 1);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* encode the exponent */
   ret = rtCborEncInt64(pctxt, pvalue->exponent);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* encode the mantissa */
   ret = rtCborEncBigInt(pctxt, &pvalue->mantissa);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   return 0;
}
