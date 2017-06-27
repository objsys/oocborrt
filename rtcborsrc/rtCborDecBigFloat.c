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



int rtCborDecBigFloat (OSCTXT* pctxt, OSOCTET byte1, OSBigFloat* pvalue)
{
   int ret;
   OSOCTET byte;

   if ( byte1 == 0xC4 ) pvalue->base10 = TRUE;
   else if ( byte1 == 0xC5 ) pvalue->base10 = FALSE;
   else {
      rtxErrAddUIntParm (pctxt, 0xC4);
      rtxErrAddUIntParm (pctxt, byte1);
      return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
   }

   /* read array of length 2 tag */
   ret = rtxReadBytes(pctxt, &byte, 1);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   if ( byte != 0x82 ) {
      rtxErrAddUIntParm (pctxt, 0x82);
      rtxErrAddUIntParm (pctxt, byte);
      return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
   }

   /* read exponent tag */
   ret = rtxReadBytes(pctxt, &byte, 1);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* read exponent */
   ret = rtCborDecInt64(pctxt, byte, &pvalue->exponent);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* read mantissa tag */
   ret = rtxReadBytes(pctxt, &byte, 1);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   /* read mantissa */
   ret = rtCborDecBigInt(pctxt, byte, &pvalue->mantissa);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   return 0;
}
