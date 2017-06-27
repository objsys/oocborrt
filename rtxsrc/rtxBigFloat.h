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
 * @file rtxBigFloat.h
 */

#ifndef _RTXBIGFLOAT_H_
#define _RTXBIGFLOAT_H_

#include "rtxsrc/rtxBigInt.h"

/* Big Float
The value represented is given by:
      mantissa * 10**exponent    if (base10)
   OR mantissa *  2**exponent    otherwise
 */

typedef struct OSBigFloat
{
   OSBigInt    mantissa;
   OSINT64     exponent;
   OSBOOL      base10;
} OSBigFloat;


#ifdef __cplusplus
extern "C" {
#endif

/* BigFloat functions */

/**
 * This function initializes a big float structure.  It must be called
 * prior to working with the structure.
 *
 * @param pInt Pointer to big float data structure.
 */
EXTERNRT void rtxBigFloatInit (OSBigFloat* pFloat);


/**
 * This function frees internal memory within the big float structure.
 *
 * @param pctxt Pointer to a context structure.
 * @param pInt  Pointer to big float structure in which memory is to
 *    be freed.
 */
EXTERNRT void rtxBigFloatFree (OSCTXT* pctxt, OSBigFloat* pFloat);

/**
 * This function sets an OSBigFloat from a null-terminated decimal string.
 *
 * @param pctxt Pointer to a context structure.
 * @param pFloat Pointer to big float structure to receive converted value.
 *                pFloat->base10 will be set TRUE.
 * @param value Numeric string to convert. It must be in the following
 *                format:
 *                [-] integer [.fraction] [E|e [-] integer]
 *                There must be at least one integer digit before the optional
 *                fraction.  Use of an exponent is optional.  The exponent
 *                has an optional sign.
 *              Leading and trailing whitespace are acceptable and ignored;
 *              no other whitespace is allowed.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigFloatSetStr
(OSCTXT* pctxt, OSBigFloat* pFloat, const char* value);

/**
 * This function sets an OSBigFloat from a character string using the given
 * number of characters.
 *
 * @param pctxt Pointer to a context structure.
 * @param pFloat Pointer to big float structure to receive converted value.
 *                pFloat->base10 will be set TRUE.
 * @param value Numeric string to convert. It must be in the following
 *                format:
 *                [-] integer [.fraction] [E|e [-] integer]
 *                There must be at least one integer digit before the optional
 *                fraction.  Use of an exponent is optional.  The exponent
 *                has an optional sign.
 *              Leading and trailing whitespace are acceptable and ignored;
 *              no other whitespace is allowed.
 * @param len   Number of characters from character string to use.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigFloatSetStrn
(OSCTXT* pctxt, OSBigFloat* pFloat, const char* value, OSSIZE len);



/**
 * Return an approximate size for the string returned by rtxBigFloatToString.
 * The value returned will be at least as large as needed to hold the result
 * of using rtxBigFloatToString to convert to a string.
 */
EXTERNRT OSSIZE rtxBigFloatStringSize(OSBigFloat* pFloat);

/**
 * This function converts the given floating point number, which must be base 2,
 * into an OSREAL, if this can be done exactly, without rounding.
 *
 * @return FALSE if the conversion cannot be done exactly.
 */
EXTERNRT OSBOOL rtxBigFloatToReal(const OSBigFloat* pFloat, OSREAL* pvalue);

/**
 * This function is used to convert a floating point number to a string.
 *
 * If pFloat->base10 is true, the format of the string will be:
 *    [-]ddd[E[-]ddd]
 * That is, an integer mantissa, followed by an optional integer exponent,
 * either of which may have a negative sign.
 *
 * If pFloat->base10 is false, the result depends on whether the OSBigFloat can
 * be converted exactly to a double (see rtxBigFloatToReal).  If exact
 * conversion can be done, the result is as for fprint using %G.  Otherwise, the
 * result is inspired by fprint's %A format:
 *    [-]0xHHHHHP{+/-]d,
 *
 * @param pctxt Pointer to a context structure.
 * @param pFloat  Pointer to OSBigFloat structure to convert.
 * @param str   Character string buffer to receive converted value.
 * @param strSize Size, in bytes, of the character string buffer.
 * @return Status of the operation, 0 = success, negative code if error.
 */
EXTERNRT int rtxBigFloatToString
(OSCTXT* pctxt, const OSBigFloat* pFloat, char* str, OSSIZE strSize);



#ifdef __cplusplus
}
#endif

#endif /* _RTXBIGFLOAT_H_ */

