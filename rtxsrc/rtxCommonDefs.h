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
#ifndef _RTXCOMMONDEFS_H_
#define _RTXCOMMONDEFS_H_

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include "rtxsrc/osMacros.h"

/* Constants */

#define OSRTMEMBUFSEG   1024    /* memory buffer extent size            */

/*
Visual Studio 2005: _MSC_VER == 1400
Visual Studio 2013: _MSC_VER == 1800
*/

#if defined(_MSC_VER) && (_MSC_VER < 1800)
   /* strtoll is not present until VS 2013.
      _strtoi64 goes back at least as far as VS 2005
   */
   #define strtoll _strtoi64
#endif // defined

/*
 * Definitions for common run-time functions that may not be availabe on
 * certain embedded platforms.  These definitions can be changed to
 * use custom functions for these operations.
 */
#if (!defined(BREW_MODULE) && !defined(FLAT_BREW))
   #define OSCRTLMEMSET    memset
   #define OSCRTLMEMCMP    memcmp
   #define OSCRTLMEMCPY    memcpy
   #define OSCRTLMEMMOVE   memmove
   #define OSCRTLSTRCHR    strchr
   #define OSCRTLSTRCPY    strcpy
   #define OSCRTLSTRNCPY   strncpy
   #define OSCRTLSTRCAT    strcat
   #define OSCRTLSTRNCAT   strncat
   #define OSCRTLSTRCMP    strcmp
   #define OSCRTLSTRNCMP   strncmp
   #define OSCRTLSTRLEN    strlen
   #define OSCRTLMALLOC    malloc
   #define OSCRTLREALLOC   realloc
   #define OSCRTLFREE      free

   #if defined(_MSC_VER) && (_MSC_VER >= 1400)
      #define OSCRTLSTRNCPY_S(buf,bufsiz,src,nchars) \
      strncpy_s(buf,bufsiz,src,nchars)
   #else
      #define OSCRTLSTRNCPY_S(buf,bufsiz,src,nchars) strncpy(buf,src,nchars)
   #endif
#else
   #define OSCRTLMEMSET    MEMSET
   #define OSCRTLMEMCMP    MEMCMP
   #define OSCRTLMEMCPY    MEMCPY
   #define OSCRTLMEMMOVE   MEMMOVE
   #define OSCRTLSTRCHR    STRCHR
   #define OSCRTLSTRCPY    STRCPY
   #define OSCRTLSTRNCPY   STRNCPY
   #define OSCRTLSTRNCPY_S STRNCPY
   #define OSCRTLSTRCAT    STRCAT
   #define OSCRTLSTRNCAT   STRNCAT
   #define OSCRTLSTRCMP    STRCMP
   #define OSCRTLSTRNCMP   STRNCMP
   #define OSCRTLSTRLEN    STRLEN
   #define OSCRTLMALLOC    MALLOC
   #define OSCRTLREALLOC   REALLOC
   #define OSCRTLFREE      FREE
#endif

/* Macros */

#define OSRTMAX(a,b)        (((a)>(b))?(a):(b))
#define OSRTMIN(a,b)        (((a)<(b))?(a):(b))

#define OSCRTLSAFEMEMCPY(dest,dlen,src,nbytes) \
   do { (dlen >= nbytes) ? OSCRTLMEMCPY(dest,src,nbytes) : 0; } while(0);

#define OSCRTLSAFEMEMMOVE(dest,dlen,src,nbytes) \
   do { (dlen >= nbytes) ? OSCRTLMEMMOVE(dest,src,nbytes) : 0; } while(0);

#define HEXCHARTONIBBLE(ch,b) do { \
if (ch >= '0' && ch <= '9') b = (unsigned char)(ch - '0'); \
else if (ch >= 'a' && ch <= 'f') b = (unsigned char)((ch - 'a') + 10); \
else if (ch >= 'A' && ch <= 'F') b = (unsigned char)((ch - 'A') + 10); \
else b = 0xFF; } while(0)

#define NIBBLETOHEXCHAR(b,ch) do { \
if (b >= 0 && b <= 9) ch = (char)(b + '0'); \
else if (b >= 0x0a && b <= 0x0f) ch = (char)((b - 10)+ 'a'); \
else ch = '?'; } while(0)

#define NIBBLETOUCHEXCHAR(b,ch) do { \
if (b >= 0 && b <= 9) ch = (char)(b + '0'); \
else if (b >= 0x0a && b <= 0x0f) ch = (char)((b - 10)+ 'A'); \
else ch = '?'; } while(0)

#define OSRTSAFECONSTCAST(towhere,ptr) ((towhere)((size_t)ptr))

#define OS_WHITESPACE_COLLAPSE 0
#define OS_WHITESPACE_PRESERVE 1
#define OS_WHITESPACE_REPLACE  2

#define OS_ISNULLPTR(ptr) (((const void*)0) == ptr)

#if defined(_MSC_VER)
/*  this disables 'conditional expression is constant' warning */
#pragma warning(disable: 4127)
/*  this disables 'unreferenced formal parameter' warning */
#pragma warning(disable: 4100)
/*  this disables 'automatic inline expansion' warning */
#pragma warning(disable: 4711)
#endif

#endif /* _RTXCOMMONDEFS_H_ */
