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
#include "rtxsrc/rtxHexDump.h"

static const OSOCTET OSRTCBOR_FalseEnc[] = { 0xf4 };
static const OSOCTET OSRTCBOR_TrueEnc[] = { 0xf5 };
static const OSOCTET OSRTCBOR_NullEnc[] = { 0xf6 };
static const OSOCTET OSRTCBOR_UnknownEnc[] = { 0xf7 };

int rtCborEncBool (OSCTXT* pctxt, OSBOOL value)
{
   return (value) ?
      rtxWriteBytes (pctxt, OSRTCBOR_TrueEnc, 1) :
      rtxWriteBytes (pctxt, OSRTCBOR_FalseEnc, 1);
}

int rtCborDecBool (OSCTXT* pctxt, OSOCTET byte1, OSBOOL* pvalue)
{
   if (byte1 == OSRTCBOR_FalseEnc[0]) {
      if (0 != pvalue) *pvalue = FALSE;
   }
   else if (byte1 == OSRTCBOR_TrueEnc[0]) {
      if (0 != pvalue) *pvalue = TRUE;
   }
   else {
#ifndef _COMPACT
      char lbuf[8];
      rtxByteToHexChar (byte1, lbuf, sizeof(lbuf));
      rtxErrAddStrParm (pctxt, "0xf4|0xf5");
      rtxErrAddStrParm (pctxt, lbuf);
      return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
#else
      return RTERR_IDNOTFOU;
#endif
   }

   return 0;
}

int rtCborEncNull (OSCTXT* pctxt)
{
   return rtxWriteBytes (pctxt, OSRTCBOR_NullEnc, 1);
}

int rtCborDecNull (OSCTXT* pctxt, OSOCTET byte1)
{
   if (byte1 != OSRTCBOR_NullEnc[0]) {
#ifndef _COMPACT
      char lbuf[8];
      rtxByteToHexChar (byte1, lbuf, sizeof(lbuf));
      rtxErrAddStrParm (pctxt, "0xf6");
      rtxErrAddStrParm (pctxt, lbuf);
      return LOG_RTERR (pctxt, RTERR_IDNOTFOU);
#else
      return RTERR_IDNOTFOU;
#endif
   }
   else return 0;
}
