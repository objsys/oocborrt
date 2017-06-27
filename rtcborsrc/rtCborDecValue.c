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
#include "rtxsrc/rtxMemory.h"

#ifndef _NO_INT64_SUPPORT
#define rtCborDecUInt rtCborDecUInt64
#define rtCborDecInt  rtCborDecInt64
#else
#define rtCborDecUInt rtCborDecUInt32
#define rtCborDecInt  rtCborDecInt32
#endif

int rtCborDecValue (OSCTXT* pctxt, OSRTGenValue* pvalue)
{
   int ret;
   OSOCTET ub;

   /* Read byte from stream */
   ret = rtxReadBytes (pctxt, &ub, 1);
   if (0 != ret) return LOG_RTERR (pctxt, ret);

   if (0 == pvalue) {
      ret = rtCborDecSkipField (pctxt, ub);
   }
   else {
      /* Decode item */
      switch (ub >> 5) {
      case OSRTCBOR_UINT:
         pvalue->t = OSRTDataType_UnsignedInt;
         ret = rtCborDecUInt (pctxt, ub, &pvalue->u.uintval);
         break;

      case OSRTCBOR_NEGINT:
         pvalue->t = OSRTDataType_SignedInt;
         ret = rtCborDecInt (pctxt, ub, &pvalue->u.intval);
         break;

      case OSRTCBOR_BYTESTR:
         pvalue->t = OSRTDataType_ByteArray;
         ret = rtCborDecDynByteStr (pctxt, ub, &pvalue->u.bytestr);
         break;

      case OSRTCBOR_UTF8STR:
         pvalue->t = OSRTDataType_CharStr;
         ret = rtCborDecDynUTF8Str (pctxt, ub, (char**)&pvalue->u.utf8str);
         break;

      case OSRTCBOR_ARRAY:
         pvalue->t = OSRTDataType_Array;
         ret = rtCborDecArray (pctxt, ub, &pvalue->u.array);
         break;

      case OSRTCBOR_FLOAT:
         if (ub == OSRTCBOR_FALSEENC || ub == OSRTCBOR_TRUEENC) {
            pvalue->t = OSRTDataType_Bool;
            pvalue->u.boolval = (ub == OSRTCBOR_TRUEENC) ? TRUE : FALSE;
         }
         else if (ub == OSRTCBOR_FLT16ENC ||
                  ub == OSRTCBOR_FLT32ENC ||
                  ub == OSRTCBOR_FLT64ENC) {
            pvalue->t = OSRTDataType_Float;
            ret = rtCborDecFloat (pctxt, ub, &pvalue->u.fltval);
         }
         break;

      default:
         rtxErrAddIntParm (pctxt, (ub >> 5));
         ret = RTERR_NOTSUPP;
      }
   }

   return (0 != ret) ? LOG_RTERR (pctxt, ret) : 0;
}

