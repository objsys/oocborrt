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
/**
 * @file rtxGenValueType.h - Implementation of a generic value type for
 * encoding and decoding values without schema.
 */
#ifndef _RTXGENVALUETYPE_H_
#define _RTXGENVALUETYPE_H_

#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Generic data value type */
typedef enum {
   OSRTDataType_UnsignedInt,
   OSRTDataType_SignedInt,
   OSRTDataType_ByteArray,
   OSRTDataType_CharStr,
   OSRTDataType_Array,
   OSRTDataType_Map,
   OSRTDataType_Bool,
   OSRTDataType_Float
} OSRTDataType;

typedef struct _OSRTGenPtrArray {
   OSSIZE nitems;
   struct _OSRTGenValue** values;
} OSRTGenPtrArray;

typedef struct _OSRTGenKeyValuePair {
   struct _OSRTGenValue* key;
   struct _OSRTGenValue* value;
} OSRTGenKeyValuePair;

typedef struct _OSRTGenValueMap {
   OSSIZE nitems;
   OSRTGenKeyValuePair* values;
} OSRTGenValueMap;

typedef struct _OSRTGenValue {
   OSRTDataType t;
   union {
      /* Unsigned and signed integer values */
#ifndef _NO_INT64_SUPPORT
      OSUINT64 uintval;
      OSINT64  intval;
#else
      OSUINT32 uintval;
      OSINT32  intval;
#endif
      /* Byte string value */
      OSDynOctStr64 bytestr;

      /* UTF-8 string value */
      const char* utf8str;

      /* Array */
      OSRTGenPtrArray array;

      /* Map */
      OSRTGenValueMap map;

      /* Boolean */
      OSBOOL boolval;

      /* Floating point */
      OSDOUBLE fltval;
   } u;
} OSRTGenValue;

/**
 * This function compares two generic values for equality.  Information
 * on values in the structure that are not equal are returned on the
 * error list within the context.
 *
 * @param pctxt    Pointer to a context structure.
 * @param pvalue1  Pointer to first value to compare.
 * @param pvalue2  Pointer to second value to compare.
 * @return Status of the comparison operation.  A negative value indicates
 *   the values are not equal.  Printing the error results using rtxErrPrint
 *   or equivalent will show the specific items that don't match.
 */
EXTERNRT int rtxGenValueCompare
(OSCTXT* pctxt, const OSRTGenValue* pvalue1, const OSRTGenValue* pvalue2);

/**
 * This function returns a textual identifier for the given enumerated
 * value type.
 *
 * @param id            Enumerated data type identifier (OSRTDataType)
 * @return              Textual identifier for id or '<unknown>'
 */
EXTERNRT const char* rtxGenValueGetIdent (OSRTDataType id);

#ifdef __cplusplus
}
#endif

#endif
