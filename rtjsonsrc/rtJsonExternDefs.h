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
 * @file rtJsonExternDefs.h
 * JSON external definitions macro.  This is used for Windows to properly
 * declare function scope within DLL's.
 */
#ifndef _RTJSONEXTERNDEFS_H_
#define _RTJSONEXTERNDEFS_H_

#define EXTJSONMETHOD

#ifdef BUILDJSONDLL
#define EXTERNJSON     __declspec(dllexport)
#define EXTJSONCLASS   __declspec(dllexport)
#elif defined (USEJSONDLL)
#define EXTERNJSON     __declspec(dllimport)
#define EXTJSONCLASS   __declspec(dllimport)
#else
#define EXTERNJSON
#define EXTJSONCLASS
#endif /* BUILDJSONDLL */

#endif
