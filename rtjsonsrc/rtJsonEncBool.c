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

#include "osrtjson.h"

int rtJsonEncBoolValue (OSCTXT* pctxt, OSBOOL value)
{
   const char* valueText = value ? "true" : "false";
   int stat = rtxCopyAsciiText (pctxt, valueText);
   return (stat != 0) ? LOG_RTERR (pctxt, stat) : 0;
}

