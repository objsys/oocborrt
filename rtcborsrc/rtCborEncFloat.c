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

#define OSRTCBOR_FLT16ENC 0xF9
#define OSRTCBOR_FLT32ENC 0xFA
#define OSRTCBOR_FLT64ENC 0xFB

#ifndef _NO_INT64_SUPPORT
#define OSUINTTYPE OSUINT64
#else
#define OSUINTTYPE OSUINT32
#endif

/*
Fill buf in reverse order.
@param encSize the number of byte expected to be written into the buffer.
@return the index closest to zero that was assigned in buf.
*/
static OSSIZE encUIntToBuf
(OSUINTTYPE value, OSOCTET* buf, OSSIZE bufsize, OSSIZE encsize)
{
   OSSIZE idx = bufsize, cnt = 0;
   OSOCTET lb;

   /* Encode value to temp buffer */
   do {
      lb = (OSOCTET) (value % 256);
      value /= 256;
      buf[--idx] = lb;
      cnt++;
   } while (value != 0 && idx > 0);

   /* Add padding */
   for ( ; cnt < encsize; cnt++) {
      buf[--idx] = 0;
   }

   return idx;
}

int rtCborEncFloat (OSCTXT* pctxt, OSDOUBLE value)
{
   int ret = 0;
   OSOCTET lbuf[10];
   OSSIZE  idx;
   OSFLOAT float_val = (OSFLOAT)value;
   OSBOOL  float32 = FALSE;

   if (float_val == value) {   /* 32 bits is enough and we aren't NaN */
      OSUINT16 u16;
      union {
         float f;
         OSUINT32 u;
      } u32;
      u32.f = float_val;
      if ((u32.u & 0x1FFF) == 0) { /* worth trying half */
         int s16 = (u32.u >> 16) & 0x8000;
         int exp = (u32.u >> 23) & 0xff;
         int mant = u32.u & 0x7fffff;
         if (exp == 0 && mant == 0)
            ;              /* 0.0, -0.0 */
         else if (exp >= 113 && exp <= 142) /* normalized */
            s16 += ((exp - 112) << 10) + (mant >> 13);
         else if (exp >= 103 && exp < 113) { /* denorm, exp16 = 0 */
            if (mant & ((1 << (126 - exp)) - 1))
               float32 = TRUE;         /* loss of precision */
            else
               s16 += ((mant + 0x800000) >> (126 - exp));
         }
         else if (exp == 255 && mant == 0) { /* Inf */
            s16 += 0x7c00;
         }
         else
            float32 = TRUE;           /* loss of range */

         if (!float32) {
            u16 = (OSUINT16) s16;
            idx = encUIntToBuf (u16, lbuf, sizeof(lbuf), 2);
            lbuf[--idx] = OSRTCBOR_FLT16ENC;
            ret = rtxWriteBytes (pctxt, &lbuf[idx], 3);
         }
      }
      else float32 = TRUE;

      if (float32) {
         idx = encUIntToBuf (u32.u, lbuf, sizeof(lbuf), 4);
         lbuf[--idx] = OSRTCBOR_FLT32ENC;
         ret = rtxWriteBytes (pctxt, &lbuf[idx], 5);
      }
   }
   else if (value != value) {    /* NaN -- we always write a half NaN*/
      static const OSOCTET nanEnc[] = { OSRTCBOR_FLT32ENC, 0x7e, 0x00 };
      ret = rtxWriteBytes (pctxt, nanEnc, sizeof(nanEnc));
   }
   else {
      union {
         OSDOUBLE d;
         OSUINT64 u;
      } u64;
      u64.d = value;
      idx = encUIntToBuf (u64.u, lbuf, sizeof(lbuf), 8);
      lbuf[--idx] = OSRTCBOR_FLT64ENC;
      ret = rtxWriteBytes (pctxt, &lbuf[idx], 9);
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
