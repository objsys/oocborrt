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
 * @file rtxBigInt.h
 */

#ifndef _RTXBIGINT_H_
#define _RTXBIGINT_H_

#include "rtxsrc/rtxContext.h"

/* Big Integer */

typedef struct OSBigInt {
   OSSIZE    numocts;           /* number of used octets in mag         */
   OSOCTET   *mag;              /* magnitude. Unsigned integer, big-endian
                                   format. Must not have leading zero
                                   bytes.                               */
   int       sign;              /* sign, can be -1, 0, 1                */
   OSSIZE    allocated;         /* number of allocated octets for mag   */
   OSBOOL    dynamic;           /* is buffer dynamic?                   */
} OSBigInt;


#ifdef __cplusplus
extern "C" {
#endif

/* BigInt functions */

/**
 * This function initializes a big integer structure.  It must be called
 * prior to working with the structure.
 *
 * @param pInt Pointer to big integer data structure.
 */
EXTERNRT void rtxBigIntInit (OSBigInt* pInt);

/**
 * This function ensures that the big integer has capacity to hold at least
 * the given number of bytes.  This can be used prior to directly copying bytes
 * into mag.
 * POST: On a successful return, pInt->allocated <= capacity; pInt->dynamic
 * might now be TRUE.  The value of the big integer is unchanged.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big integer structure.
 * @param capacity   Required capacity.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntEnsureCapacity(OSCTXT* pctxt, OSBigInt* pInt,
                                       OSSIZE capacity);

/**
 * This function sets a big integer binary value from a null-terminated string.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big integer structure to receive converted value.
 * @param value Numeric string to convert.
 * @param radix Radix of the string value,  Valid values are 0, 2, 8, 10,
 *                or 16.  Zero must be used if string contains a prefix that
 *                identifies the radix (for example, 0x).
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntSetStr
(OSCTXT* pctxt, OSBigInt* pInt, const char* value, int radix);

/**
 * This function sets a big integer binary value from a character string
 * using the given number of characters.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big integer structure to receive converted value.
 * @param value Numeric string to convert.
 * @param len   Number of bytes from character string to use.
 * @param radix Radix of the string value,  Valid values are 0, 2, 8, 10,
 *                or 16.  Zero must be used if string contains a prefix that
 *                identifies the radix (for example, 0x).
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntSetStrn
(OSCTXT* pctxt, OSBigInt* pInt, const char* value, OSSIZE len, int radix);

/**
 * This function sets a big integer binary value from a signed 64-bit
 * integer value.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big integer structure to receive converted value.
 * @param value 64-bit integer value to convert.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntSetInt64 (OSCTXT* pctxt, OSBigInt* pInt, OSINT64 value);

/**
 * This function sets a big integer binary value from an unsigned 64-bit
 * integer value.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big integer structure to receive converted value.
 * @param value 64-bit integer value to convert.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntSetUInt64
(OSCTXT* pctxt, OSBigInt* pInt, OSUINT64 value);

/**
 * This function sets a big integer binary value from a byte array.  The
 * array is assumed to hold the value in 2's complement form.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big integer structure to receive converted value.
 * @param value Buffer containing binary integer value in 2's complement form.
 * @param vallen Number of byte in the value buffer.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntSetBytes
(OSCTXT* pctxt, OSBigInt* pInt, OSOCTET* value, OSSIZE vallen);

/**
 * This function is used to get the size in bytes of the binary big integer
 * data value.
 *
 * @param pInt Pointer to big integer structure.
 * @return Length in bytes of the binary data value.
 */
EXTERNRT OSSIZE rtxBigIntGetDataLen (const OSBigInt* pInt);

/**
 * This function is used to get the binary big integer data value in a
 * byte array.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt Pointer to big integer structure.
 * @param buffer Buffer into which binary big integer value is to be copied.
 * @param bufSize Size of the data buffer.
 * @return If success, number of bytes in byte array; if error, negative
 *    error code.
 */
EXTERNRT int rtxBigIntGetData
(OSCTXT* pctxt, const OSBigInt* pInt, OSOCTET* buffer, OSSIZE bufSize);

/**
 * This function is used to get the number of digits in the binary big integer
 * data value based on radix.
 *
 * @param pInt Pointer to big integer structure.
 * @param radix Radix of the string value,  Valid values are 2, 8, 10, or 16.
 * @return Number of digits in the binary data value.
 */
EXTERNRT OSSIZE rtxBigIntDigitsNum (const OSBigInt* pInt, int radix);

/**
 * This function is used to copy a big integer data value from one structure
 * to another.
 *
 * @param pctxt Pointer to a context structure.
 * @param pSrc  Pointer to source big integer structure.
 * @param pDst  Pointer to destination big integer structure.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntCopy
(OSCTXT* pctxt, const OSBigInt* pSrc, OSBigInt* pDst);

/**
 * This function is used to copy one BigInt to another. This function will
 * not allocate memory for the byte buffer if the destination BigInt already
 * has a large enough allocated array to hold the data.  The destination
 * BigInt must have been initialized using the rtxBigIntInit function.
 *
 * @param pctxt Pointer to a context structure.
 * @param pSrc  Pointer to source big integer structure.
 * @param pDst  Pointer to destination big integer structure.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntFastCopy
(OSCTXT* pctxt, const OSBigInt* pSrc, OSBigInt* pDst);

/**
 * This function converts the given big integer to a real, if it will fit
 * without requiring rounding (i.e. loss of precision).
 * @param pvalue Receives the OSREAL value.  If null, this function only
 *             tests whether the conversion can be done.
 * @return TRUE/FALSE indicating whether the value can fit in a OSREAL without
 * rounding.
 */
EXTERNRT OSBOOL rtxBigIntToReal(const OSBigInt* pSrc, OSREAL* pvalue);


/**
 * This function is used to convert a binary big integer value to a string.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big integer structure to convert.
 * @param radix Radix of the string value,  Valid values are 2, 8, 10, or 16.
 * @param str   Character string buffer to receive converted value.
 * @param strSize Size, in bytes, of the character string buffer.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntToString
(OSCTXT* pctxt, const OSBigInt* pInt, int radix, char* str, OSSIZE strSize);

/**
 * This function is used to print a big integer value to standard output.
 *
 * @param name Name to print in "name=value" format.
 * @param bigint Pointer to big integer value to be printed.
 * @param radix Radix of the string value,  Valid values are 2, 8, 10, or 16.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigIntPrint
(const OSUTF8CHAR* name, const OSBigInt* bigint, int radix);

/**
 * This function is used to compare two big integer values.
 *
 * @param arg1 First big integer to compare.
 * @param arg2 Second big integer to compare.
 * @return Result of comparison:
 *    -1 = arg1 < arg2,
 *     0 = arg1 == arg2,
 *    +1 = arg1 > arg2
 */
EXTERNRT int rtxBigIntCompare (const OSBigInt* arg1, const OSBigInt* arg2);

/**
 * This function is used to compare two big integer numeric strings.
 *
 * @param pctxt Pointer to a context structure.
 * @param arg1 First big integer string to compare.
 * @param arg2 Second big integer string to compare.
 * @return Result of comparison:
 *    -1 = arg1 < arg2,
 *     0 = arg1 == arg2,
 *    +1 = arg1 > arg2
 */
EXTERNRT int rtxBigIntStrCompare
(OSCTXT* pctxt, const char* arg1, const char* arg2);

/**
 * This function frees internal memory within the big integer structure.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big integer structure in which memory is to
 *    be freed.
 */
EXTERNRT void rtxBigIntFree (OSCTXT* pctxt, OSBigInt* pInt);

/* Arithmetic */
/**
 * This function is used to add two big integer values.
 *
 * @param pctxt  Pointer to a context structure.
 * @param result Pointer to big integer structure to receive result.
 * @param arg1   First big integer to add.
 * @param arg2   Second big integer to add.
 * @return Result of operation: 0 = success, negative error code if error.
 */
EXTERNRT int rtxBigIntAdd
   (OSCTXT* pctxt,
    OSBigInt* result, const OSBigInt* arg1, const OSBigInt* arg2);

/**
 * This function is used to subtract one big integer value from another.
 *
 * @param pctxt  Pointer to a context structure.
 * @param result Pointer to big integer structure to receive result.
 * @param arg1   Big integer value that arg2 is subtracted from (minuend).
 * @param arg2   Big integer to be subtracted from arg1 (subtrahend).
 * @return Result of operation: 0 = success, negative error code if error.
 */
EXTERNRT int rtxBigIntSubtract
   (OSCTXT* pctxt,
    OSBigInt* result, const OSBigInt* arg1, const OSBigInt* arg2);

/**
 * This function is used to multiply two big integer values.
 *
 * @param pctxt  Pointer to a context structure.
 * @param result Pointer to big integer structure to receive result.
 * @param arg1   First big integer to be multiplied.
 * @param arg2   Second big integer to be multiplied.
 * @return Result of operation: 0 = success, negative error code if error.
 */
EXTERNRT int rtxBigIntMultiply
   (OSCTXT* pctxt,
    OSBigInt* result, const OSBigInt* arg1, const OSBigInt* arg2);

/* Common */
EXTERNRT unsigned short rtxBigIntBitsPerDigit (int radix);
EXTERNRT short rtxBigIntByteRadix (int halfRadix);

#ifdef __cplusplus
}
#endif

#endif /* RTXBIGINT */

