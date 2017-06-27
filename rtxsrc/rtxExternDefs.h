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

#ifndef _RTXEXTERNDEFS_H_
#define _RTXEXTERNDEFS_H_

/**
 * @file rtxExternDefs.h
 * Common definitions of external function modifiers used to define the
 * scope of functions used in DLL's (Windows only).
 */

/* Symbian requires different exporting rules, and so we define different
 * constants to turn on or off the appropriate linkage types.  See
 * rtsrc/rtExternDefs.h for more detail. */
#if defined(__SYMBIAN32__)       /* OS: Symbian */
#define EXTRTCLASS               /* turn off class externs */

#ifndef EXTERN
#ifdef BUILDDLL
#define EXTERN EXPORT_C
#elif defined (USEDLL)
#define EXTERN IMPORT_C
#else
#define EXTERN
#endif /* BUILD/USE DLL */
#endif /* EXTERN */

#ifndef EXTERNRT
#if defined(BUILDASN1RTDLL) || defined(BUILDRTXDLL)
#define EXTERNRT     EXPORT_C
#define EXTRTMETHOD  EXPORT_C    /* turn on method externs */
#elif defined (USEASN1RTDLL) || defined(USERTXDLL)
#define EXTERNRT     IMPORT_C
#define EXTRTMETHOD  IMPORT_C
#else
#define EXTERNRT
#define EXTRTMETHOD
#endif /* BUILDASN1RTDLL */
#endif /* EXTERNRT */

#else                            /* OS: Win, Unix, Linux, ... */
#define EXTRTMETHOD              /* turn off method externs */

#if defined(_WIN32) || defined(_WIN64)
#ifndef EXTERNRT
#if defined(BUILDASN1RTDLL) || defined(BUILDRTXDLL)
#define EXTERNRT        __declspec(dllexport)
#define EXTRTCLASS      __declspec(dllexport)
#elif defined (USERTXDLL) || defined(USEASN1RTDLL)
#define EXTERNRT        __declspec(dllimport)
#define EXTRTCLASS      __declspec(dllimport)
#else
#define EXTERNRT
#define EXTRTCLASS
#endif /* BUILDASN1RTDLL */
#endif /* EXTERNRT */

#ifdef BUILDDLL
#define EXTERN __declspec(dllexport)
#elif defined (USEDLL)
#define EXTERN __declspec(dllimport)
#else
#define EXTERN
#endif /* BUILDDLL */

#else /* not _WIN32 */
#define EXTERNRT
#define EXTRTCLASS
#define EXTERN
#endif

#endif
#endif
