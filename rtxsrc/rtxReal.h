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
 * @file rtxReal.h
 * Common runtime functions for working with floating-point numbers.
 */
#ifndef _RTXREAL_H_
#define _RTXREAL_H_

#include "rtxsrc/osSysTypes.h"
#include "rtxsrc/rtxExternDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup rtxReal Floating-point number utility functions
 * @{
 * Floating-point utility function provide run-time functions for handling
 * floating-point number types defined within a schema.
 */

/**
 * Returns the IEEE negative infinity value.  This is defined as
 * 0xfff0000000000000 in IEEE standard 754.  We assume the presence of the
 * IEEE double type, that is, 64-bits of precision. */
EXTERNRT OSREAL rtxGetMinusInfinity (OSVOIDARG);

/**
 * Returns the IEEE minus zero value.  This is defined as
 * 0x8000000000000000 in IEEE standard 754.  We assume the presence of the
 * IEEE double type, that is, 64-bits of precision. */
EXTERNRT OSREAL rtxGetMinusZero (OSVOIDARG);

/**
 * Returns the IEEE Not-A-Number (NaN) value.  This is defined as
 * 0x7ff8000000000000 in IEEE standard 754.  We assume the presence of the
 * IEEE double type, that is, 64-bits of precision. */
EXTERNRT OSREAL rtxGetNaN (OSVOIDARG);

/**
 * Returns the IEEE posative infinity value.  This is defined as
 * 0x7ff0000000000000 in IEEE standard 754.  We assume the presence of the
 * IEEE double type, that is, 64-bits of precision. */
EXTERNRT OSREAL rtxGetPlusInfinity (OSVOIDARG);

/**
 * A utility function that compares the given input value to the IEEE 754
 * value for negative infinity.
 *
 * @param   value    The input real value. */
EXTERNRT OSBOOL rtxIsMinusInfinity (OSREAL value);

/**
 * A utility function that compares the given input value to the IEEE 754
 * value for minus zero.
 *
 * @param   value    The input real value. */
EXTERNRT OSBOOL rtxIsMinusZero (OSREAL value);

/**
 * A utility function that compares the given input value to the IEEE 754
 * value for Not-A-Number (NaN).
 *
 * @param   value    The input real value. */
EXTERNRT OSBOOL rtxIsNaN (OSREAL value);

/**
 * A utility function that compares the given input value to the IEEE 754
 * value for positive infinity.
 *
 * @param   value    The input real value. */
EXTERNRT OSBOOL rtxIsPlusInfinity (OSREAL value);

/**
 * A utility function that return TRUE when first number are approximate
 * to second number with given precision.
 *
 * @param   a        The input real value.
 * @param   b        The input real value.
 * @param   delta    difference must be low than delta * a
                      1E-7  - set best precision for float;
                      1E-15 - set best precision for double.
 */
EXTERNRT OSBOOL rtxIsApproximate (OSREAL a, OSREAL b, OSREAL delta);

/**
 * A utility function that return TRUE when first number are approximate
 * to second number with given absolute precision.
 *
 * @param   a        The input real value.
 * @param   b        The input real value.
 * @param   delta    difference must be low than delta
 */
EXTERNRT OSBOOL rtxIsApproximateAbs (OSREAL a, OSREAL b, OSREAL delta);

/**
 * @} rtxReal
 */

#ifdef __cplusplus
}
#endif

#endif
