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
 *   http://www.obj-sys.com/open/lgpl2.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/
/**
 * @file rtxBigInt.hh
 */

#ifndef _RTXBIGINT_HH_
#define _RTXBIGINT_HH_

#include <stdlib.h>
#include "rtxsrc/rtxBigInt.h"
#include "rtxsrc/rtxErrCodes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UNIT_MASK 0xFF

#define ADDRESS_BITS       3 /* as far as char has 8 bits (2^3) */

#define BITS_PER_UNIT (1 << ADDRESS_BITS)
#define BIT_INDEX_MASK  (BITS_PER_UNIT - 1)

#define bitsLeftOf(x) \
   (x==0 ? UNIT_MASK : (UNIT_MASK << (BITS_PER_UNIT - x)))

#define LONG_MASK 0xFF

// extern unsigned short bitsPerDigit[];

// extern short digitsPerByte[];

// extern short byteRadix[];

/* BigInt functions */
void rtxBigIntDestructiveMulAdd(OSOCTET* x, int len, int y, int z);
void rtxBigIntRemoveLeadingZeroBytes (OSBigInt* pInt);
int rtxBigIntCopyAndInverse
   (OSCTXT* pCtxt, const OSBigInt* pSrc, OSBigInt* pDst);
int rtxBigIntDivideByInt (OSCTXT* pCtxt, OSBigInt* pDivident,
   int divisor, OSBigInt* pQuotient, int* reminder);
int rtxBigIntNegate (OSBigInt* pInt);

#ifdef __cplusplus
}
#endif

#endif /* RTXBIGINT */

