/**
 * This file contains code based on CBOR implementation at
 * https://github.com/cabo/cn-cbor.
 *
 * Copyright (c) 2015 Carsten Bormann <cabo@tzi.org>
 *
 * See LICENSE for license info.
 */
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxReal.h"

#include <math.h>

int rtCborDecFloat (OSCTXT* pctxt, OSOCTET byte1, OSDOUBLE* pvalue)
{
   int ret;
   if (byte1 == OSRTCBOR_FLT16ENC) {
      OSUINT16 half;
      ret = rtCborDecUInt16 (pctxt, OSRTCBOR_UINT16_ID, &half);
      if (0 == ret && 0 != pvalue) {
         int exp = (half >> 10) & 0x1f;
         int mant = half & 0x3ff;
         if (exp == 0) *pvalue = ldexp (mant, -24);
         else if (exp != 31) *pvalue = ldexp (mant + 1024, exp - 25);
         else *pvalue = (mant == 0) ? rtxGetPlusInfinity() : rtxGetNaN();
         if (half & 0x8000) *pvalue =  -*pvalue;
      }
   }
   else if (byte1 == OSRTCBOR_FLT32ENC) {
      union {
         OSFLOAT  f;
         OSUINT32 u;
      } u32;
      ret = rtCborDecUInt32 (pctxt, OSRTCBOR_UINT32_ID, &u32.u);
      if (0 == ret && 0 != pvalue) {
         *pvalue = u32.f;
      }
   }
#ifndef _NO_INT64_SUPPORT
   else if (byte1 == OSRTCBOR_FLT64ENC) {
      union {
         OSDOUBLE d;
         OSUINT64 u;
      } u64;
      ret = rtCborDecUInt64 (pctxt, OSRTCBOR_UINT64_ID, &u64.u);
      if (0 == ret && 0 != pvalue) {
         *pvalue = u64.d;
      }
   }
#endif
   else ret = RTERR_BADVALUE;

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
