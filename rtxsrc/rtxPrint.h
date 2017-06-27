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
 * @file rtxPrint.h
 */
#ifndef _RTXPRINT_H_
#define _RTXPRINT_H_

#include <stdio.h>
#include "rtxsrc/rtxContext.h"

#define OSRTINDENTSPACES        3       /* number of spaces for indent  */

#ifdef __cplusplus
extern "C" {
#endif

/* Run-time print utility functions */

/**
 * @defgroup valsToStdout Print Functions
 * @{
 *
 * These functions simply print the output in a "name=value" format. The value
 * format is obtained by calling one of the ToString functions with the given
 * value.
 */
/**
 * Prints a boolean value to stdout.
 *
 * @param name         The name of the variable to print.
 * @param value        Boolean value to print.
 */
EXTERNRT void rtxPrintBoolean (const char* name, OSBOOL value);

/**
 * Prints a date value to stdout.
 *
 * @param name         Name of the variable to print.
 * @param pvalue       Pointer to a structure that holds numeric
 *                        DateTime value to print.
 */
EXTERNRT void rtxPrintDate
   (const char* name, const OSNumDateTime *pvalue);

/**
 * Prints a time value to stdout.
 *
 * @param name         Name of the variable to print.
 * @param pvalue       Pointer to a structure that holds numeric
 *                        DateTime value to print.
 */
EXTERNRT void rtxPrintTime
   (const char* name, const OSNumDateTime *pvalue);

/**
 * Prints a dateTime value to stdout.
 *
 * @param name         Name of the variable to print.
 * @param pvalue       Pointer to a structure that holds numeric
 *                        DateTime value to print.
 */
EXTERNRT void rtxPrintDateTime
   (const char* name, const OSNumDateTime *pvalue);

EXTERNRT void rtxPrintGYear
   (const char* name, const OSNumDateTime* pvalue);

EXTERNRT void rtxPrintGYearMonth
   (const char* name, const OSNumDateTime* pvalue);

EXTERNRT void rtxPrintGMonth
   (const char* name, const OSNumDateTime* pvalue);

EXTERNRT void rtxPrintGMonthDay
   (const char* name, const OSNumDateTime* pvalue);

EXTERNRT void rtxPrintGDay
   (const char* name, const OSNumDateTime* pvalue);

/**
 * Prints an integer value to stdout.
 *
 * @param name         The name of the variable to print.
 * @param value        Integer value to print.
 */
EXTERNRT void rtxPrintInteger (const char* name, OSINT32 value);

/**
 * Prints a 64-bit integer value to stdout.
 *
 * @param name         The name of the variable to print.
 * @param value        64-bit integer value to print.
 */
EXTERNRT void rtxPrintInt64 (const char* name, OSINT64 value);

/**
 * Prints an unsigned integer value to stdout.
 *
 * @param name         The name of the variable to print.
 * @param value        Unsigned integer value to print.
 */
EXTERNRT void rtxPrintUnsigned (const char* name, OSUINT32 value);

/**
 * Prints an unsigned 64-bit integer value to stdout.
 *
 * @param name         The name of the variable to print.
 * @param value        Unsigned 64-bit integer value to print.
 */
EXTERNRT void rtxPrintUInt64 (const char* name, OSUINT64 value);

/**
 * This function prints the value of a binary string in hex format
 * to standard output.  If the string is 32 bytes or less, it is printed
 * on a single line with a '0x' prefix.  If longer, a formatted hex dump
 * showing both hex and ascii codes is done.
 *
 * @param name         The name of the variable to print.
 * @param numocts      The number of octets to be printed.
 * @param data         A pointer to the data to be printed.
 */
EXTERNRT void rtxPrintHexStr
(const char* name, OSSIZE numocts, const OSOCTET* data);

/**
 * This function prints the value of a binary string in hex format
 * to standard output.  In contrast to rtxPrintHexStr, it is always printed
 * on a single line with a '0x' prefix.
 *
 * @param name         The name of the variable to print.
 * @param numocts      The number of octets to be printed.
 * @param data         A pointer to the data to be printed.
 */
EXTERNRT void rtxPrintHexStrPlain
(const char* name, OSSIZE numocts, const OSOCTET* data);

/**
 * This function prints the value of a binary string in hex format
 * to standard output.  In contrast to rtxPrintHexStr, it never contains an
 * ASCII dump.
 *
 * @param name         The name of the variable to print.
 * @param numocts      The number of octets to be printed.
 * @param data         A pointer to the data to be printed.
 */
EXTERNRT void rtxPrintHexStrNoAscii
(const char* name, OSSIZE numocts, const OSOCTET* data);

/**
 * Prints an octet string value in hex binary format to stdout.
 *
 * @param name         The name of the variable to print.
 * @param numocts      The number of octets to be printed.
 * @param data         A pointer to the data to be printed.
 */
EXTERNRT void rtxPrintHexBinary
(const char* name, OSSIZE numocts, const OSOCTET* data);

/**
 * Prints an ASCII character string value to stdout.
 *
 * @param name         The name of the variable to print.
 * @param cstring      A pointer to the character string to be printed.
 */
EXTERNRT void rtxPrintCharStr (const char* name, const char* cstring);

/**
 * Prints a UTF-8 encoded character string value to stdout.
 *
 * @param name         The name of the variable to print.
 * @param cstring      A pointer to the character string to be printed.
 */
EXTERNRT void rtxPrintUTF8CharStr
(const char* name, const OSUTF8CHAR* cstring);

/**
 * This function prints a Unicode string to standard output.  Characters
 * in the string that are within the normal Ascii range are printed as
 * single characters.  Characters outside the Ascii range are printed
 * as 4-byte hex codes (0xnnnn).
 *
 * @param name         The name of the variable to print.
 * @param str          Pointer to unicode sring to be printed.  String is
 *                       an array of C unsigned short data variables.
 * @param nchars       Number of characters in the string.  If value is
 *                       negative, string is assumed to be null-terminated
 *                       (i.e. ends with a 0x0000 character).
 */
EXTERNRT void rtxPrintUnicodeCharStr
(const char* name, const OSUNICHAR* str, int nchars);

EXTERNRT void rtxPrintUnicodeCharStr64
(const char* name, const OSUNICHAR* str, OSSIZE nchars);

/**
 * Prints a REAL (float, double, decimal) value to stdout.
 *
 * @param name         The name of the variable to print.
 * @param value        REAL value to print.
 */
EXTERNRT void rtxPrintReal (const char* name, OSREAL value);

/**
 * Prints a NULL value to stdout.
 *
 * @param name         The name of the variable to print.
 */
EXTERNRT void rtxPrintNull (const char* name);

/**
 * Prints a name-value pair to stdout.
 *
 * @param name         The name of the variable to print.
 * @param value        A pointer to name-value pair structure to print.
 */
EXTERNRT void rtxPrintNVP (const char* name, const OSUTF8NVP* value);

/**
 * This function prints the contents of a text file to stdout.
 *
 * @param filename     The name of the text file to print.
 * @return             Status of operation, 0 if success.
 */
EXTERNRT int rtxPrintFile (const char* filename);

#ifndef __SYMBIAN32__

/**
 * This function prints indentation spaces to stdout.
 */
EXTERNRT void rtxPrintIndent (OSVOIDARG);

/**
 * This function increments the current indentation level.
 */
EXTERNRT void rtxPrintIncrIndent (OSVOIDARG);

/**
 * This function decrements the current indentation level.
 */
EXTERNRT void rtxPrintDecrIndent (OSVOIDARG);

/**
 * This function closes a braced region by decreasing the indent level,
 * printing indent spaces, and printing the closing brace.
 */
EXTERNRT void rtxPrintCloseBrace (OSVOIDARG);

/**
 * This function opens a braced region by printing indent spaces,
 * printing the name and opening brace, and increasing the indent level.
 */
EXTERNRT void rtxPrintOpenBrace (const char*);

#endif

/**
 * @} valsToStdout
 */
#ifdef __cplusplus
}
#endif

#include "rtxsrc/rtxHexDump.h"

#endif
