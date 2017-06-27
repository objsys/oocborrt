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

#ifndef _NO_INT64_SUPPORT
#ifndef INTFUNCNAME
#define INTFUNCNAME rtCborDecInt64
#define INTTYPE OSINT64
#define INTMAX OSINT64MAX
#define UINTFUNCNAME rtCborDecUInt64
#define UINTTYPE OSUINT64
#endif
#include "rtcborsrc/rtCborDecInt32.c"
#endif
