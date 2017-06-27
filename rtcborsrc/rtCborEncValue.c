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
#define rtCborEncUInt rtCborEncUInt64
#define rtCborEncInt  rtCborEncInt64
#else
#define rtCborEncUInt rtCborEncUInt32
#define rtCborEncInt  rtCborEncInt32
#endif

int rtCborEncValue (OSCTXT* pctxt, const OSRTGenValue* pvalue, OSUINT32 flags)
{
   int ret;

   switch (pvalue->t) {
   case OSRTDataType_UnsignedInt:
      ret = rtCborEncUInt (pctxt, pvalue->u.uintval, OSRTCBOR_UINT);
      break;

   case OSRTDataType_SignedInt:
      ret = rtCborEncInt (pctxt, pvalue->u.intval);
      break;

   case OSRTDataType_ByteArray:
      ret = rtCborEncByteStr
         (pctxt, pvalue->u.bytestr.data, pvalue->u.bytestr.numocts, 0);
      break;

   case OSRTDataType_CharStr:
      ret = rtCborEncUTF8Str (pctxt, pvalue->u.utf8str, 0);
      break;

   case OSRTDataType_Array:
      ret = rtCborEncArray
         (pctxt, pvalue->u.array.values, pvalue->u.array.nitems, flags);
      break;

   case OSRTDataType_Map:
      ret = rtCborEncMap (pctxt, &pvalue->u.map, flags);
      break;

   case OSRTDataType_Bool:
      ret = rtCborEncBool (pctxt, pvalue->u.boolval);
      break;

   default:
      rtxErrAddIntParm (pctxt, pvalue->t);
      ret = RTERR_NOTSUPP;
   }

   return (0 != ret) ? LOG_RTERR (pctxt, ret) : 0;
}
