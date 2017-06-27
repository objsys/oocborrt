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

#ifndef _OSMACROS_H_
#define _OSMACROS_H_

#if defined(_MSC_VER)
/* this disables 'conditional expression is constant' warnings
 * caused by using do { ... } while(0) in defines. */
#pragma warning(disable: 4127)
#endif

/* Check if object null before invoke get method */
#define OS_CHECKED_GET(objptr,getmethod) \
(0 == objptr) ? 0 : (objptr)->getmethod()

/* Min/max tests */
#define OS_MAX(a,b) (((a)>(b))?(a):(b))
#define OS_MIN(a,b) (((a)<(b))?(a):(b))

/* Test if character is a hex character */
#define OS_ISHEXCHAR(c) \
((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))

/* Test if Character is printable */
#define OS_ISPRINTABLE(c) \
(c >= 0x20 && c <= 0x7e)

/* Test if string variable (const char*) is empty */
#define OS_ISEMPTY(s) (s == 0 || *s == '\0')

/* Nibble-to-hex char conversion macros */
#define OS_HEXCHARTONIBBLE(ch,b) do { \
if (ch >= '0' && ch <= '9') b = (unsigned char)(ch - '0'); \
else if (ch >= 'a' && ch <= 'f') b = (unsigned char)((ch - 'a') + 10); \
else if (ch >= 'A' && ch <= 'F') b = (unsigned char)((ch - 'A') + 10); \
else b = 0xFF; } while(0)

#define OS_NIBBLETOHEXCHAR(b,ch) do { \
if (b >= 0 && b <= 9) ch = (char)(b + '0'); \
else if (b >= 0x0a && b <= 0x0f) ch = (char)((b - 10)+ 'a'); \
else ch = '?'; } while(0)

/**
 * This macro returns the number of elements in an array.
 *
 * @param x            - Array varaible
 */
#define OSRTARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

/**
 * This macro casts a string literal as a UTF-8 character string value.
 */
#define OSUTF8(str) ((const OSUTF8CHAR*)str)

/**
 * This macro determines the length of a UTF-8 character string value.
 */
#define OSUTF8LEN(str) strlen((const char*)str)

/**
 * This macro can be used to suppress gcc warnings about unused arguments.
 *
 * @param arg Argument name.
 */
#define OS_UNUSED_ARG(arg) (void)arg

/* Generic list iteration macros */

#define ForAllIter(p,listIter,type) \
for(listIter.setFirst(),p=(type)listIter.getCurrentItem();!listIter.isDone();\
listIter.setNext(),p=(type)listIter.getCurrentItem())

#define ForAllIterRev(p,listIter,type) \
for(listIter.setLast(),p=(type)listIter.getCurrentItem();!listIter.isDone();\
listIter.setPrev(),p=(type)listIter.getCurrentItem())

/* Generic list delete macro; useful for OSVoidList.
   It is safe to invoke on a pointer to a null list (does nothing)
   This does not delete the list itself because we want to be able to use
   it on the address of a static storage list.
*/
#define DeleteListContents(pList,objectType) \
   if (pList) { while((pList)->getHead()) delete (objectType*) (pList)->deleteHead(); }


/* UTF-8 character size for -static option */

#define OSUTF8CHAR_SIZE 1

/* snprintf macros */

#include <stdio.h>
#include <stdarg.h>

#if defined(_MSC_VER) && !defined(_NO_SNPRINTF_)
#  if _MSC_VER >= 1400
#    define os_snprintf(str, size, ...) \
     _snprintf_s(str, size, _TRUNCATE, __VA_ARGS__)
#    define os_vsnprintf(str, size, fmt, ...) \
     _vsnprintf_s(str, size, _TRUNCATE, fmt, __VA_ARGS__)
#  elif defined(_SHX_) && !defined(CPP) /* Hitachi/Renesas SH4 */
#    define os_snprintf StringCbPrintfA
#    define os_vsnprintf StringCbVPrintf
#    define strcat(s,d) StringCbCatA(s,strlen(s),d)
#    define strcpy(s,d) StringCbCopyA(s,strlen(s),d)
#  else
#    define os_snprintf _snprintf
#    define os_vsnprintf _vsnprintf
#  endif
#elif defined(__HP_aCC)
/* at least aCC on HP-UX 11 expects format as non-const! */
#  define os_snprintf(str, size, ...) snprintf(str, size, __VA_ARGS__)
#  define os_vsnprintf(str, size, fmt, ...) \
      vsnprintf(str, size, (char *)fmt, __VA_ARGS__)
#elif defined(_NO_SNPRINTF_) && !defined(__vxworks) && !defined (__SYMBIAN32__)
#define os_snprintf(str, size, ...) sprintf(str, __VA_ARGS__)
#define os_vsnprintf(str, size, fmt, ...) vsprintf(str, fmt, __VA_ARGS__)
/* We use gcc 2.95, which requires ##ARGS */
#elif defined(__vxworks) || defined(__SYMBIAN32__)
#define os_snprintf(str,size,ARGS...) sprintf(str,##ARGS)
#define os_vsnprintf(str,size,fmt,ARGS...) vsprintf(str,fmt,##ARGS)
#else
#define os_snprintf snprintf
#define os_vsnprintf vsnprintf
#endif

#endif
