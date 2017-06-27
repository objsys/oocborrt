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
/* These defs allow this function to be used as a template for other
   JSON integer encode functions (int64, unsigned int, etc.) */
#ifndef RTJSONENCINTVALUEFUNC
#define RTJSONENCINTVALUEFUNC rtJsonEncIntValue
#define RTXINTTOCHARSTR rtxIntToCharStr
#define OSINTTYPE OSINT32
#endif

#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxError.h"
#include "rtjsonsrc/osrtjson.h"

int RTJSONENCINTVALUEFUNC (OSCTXT* pctxt, OSINTTYPE value)
{
   int stat;
   char lbuf[40];

   stat = RTXINTTOCHARSTR (value, lbuf, sizeof(lbuf), 0);
   if (stat < 0) return LOG_RTERR (pctxt, stat);

   stat = rtxCopyAsciiText (pctxt, lbuf);
   if (stat != 0) return LOG_RTERR (pctxt, stat);

   return 0;
}
