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

#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"

#ifndef INTFUNCNAME
#define INTFUNCNAME rtCborEncInt32
#define UINTFUNCNAME rtCborEncUInt32
#define INTTYPE OSINT32
#define UINTTYPE OSUINT32
#endif

int INTFUNCNAME (OSCTXT* pctxt, INTTYPE value)
{
   OSRTCBOR_MajorType majorType = OSRTCBOR_UINT;
   if (value < 0) {
      value = (0 - value) - 1;
      majorType = OSRTCBOR_NEGINT;
   }
   return UINTFUNCNAME (pctxt, (UINTTYPE)value, majorType);
}
