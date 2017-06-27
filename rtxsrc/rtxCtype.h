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
 * @file rtxCtype.h
 */
#ifndef _RTXCTYPE_H_
#define _RTXCTYPE_H_

/* Ctype substitution macros */

#define OS_ISASCII(c) ((unsigned)(c) < 0x80)

#define OS_ISUPPER(c) (c >= 'A' && c <= 'Z')
#define OS_ISLOWER(c) (c >= 'a' && c <= 'z')
#define OS_ISDIGIT(c) (c >= '0' && c <= '9')
#define OS_ISALPHA(c) (OS_ISUPPER(c) || OS_ISLOWER(c))
#define OS_ISSPACE(c) ((c >= 0x09 && c <= 0x0d) || (c == ' '))
#define OS_ISPUNCT(c) (c >= 0 && c <= 0x20)
#define OS_ISALNUM(c) (OS_ISALPHA(c) || OS_ISDIGIT(c))
#define OS_ISPRINT(c) (c >= ' ' && c <= '~')
#define OS_ISGRAPH(c) (c >= '!' && c <= '~')
#define OS_ISCNTRL(c) ((c >= 0 && c <= 0x1F) || c == 0x7F)
#define OS_ISXDIGIT(c) \
(OS_ISDIGIT(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define OS_ISBASE64(c) (OS_ISALNUM(c) || c == '+' || c == '/' || c == '=')

#define OS_TOLOWER(c) (OS_ISUPPER(c) ? (c) - 'A' + 'a' : (c))
#define OS_TOUPPER(c) (OS_ISLOWER(c) ? (c) - 'a' + 'A' : (c))

#endif /* _RTXCTYPE_H_ */

