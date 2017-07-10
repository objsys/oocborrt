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
#include "rtxsrc/rtxBase64.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"

/**
 * Encode given binary data as base64/base64url with tag 34/33, depending on
 * value of url parameter.
 */
static int cborEncBase64
(OSCTXT* pctxt, const OSOCTET* pvalue, OSSIZE nbytes, OSSIZE segsize,
   OSBOOL url )
{
   int ret;
   long numocts;     /* # chars in pText */
   OSOCTET* pText;    /* the base64 text */
   OSOCTET tag[2];    /* bytes for base64 tag */
   
   /* convert bytes to base64 */
   if ( url ) numocts = rtxBase64UrlEncodeData( pctxt, pvalue, nbytes, &pText);
   else numocts = rtxBase64EncodeData( pctxt, pvalue, nbytes, &pText);
   
   if ( numocts < 0 ) return LOG_RTERR(pctxt, numocts);
   
   /* encode base64 tag */
   tag[0] = 0xD8;
   tag[1] = url ? 33 : 34;
   ret = rtxWriteBytes(pctxt, tag, 2);
   
   if ( ret == 0 ) {
      /* encode base64 text */
      ret = rtCborEncUTF8Str(pctxt, pText, segsize);
   }
   
   rtxMemFreePtr(pctxt, pText);
   
   if ( ret != 0 ) return LOG_RTERR(pctxt, ret);
   else return 0;
}

int rtCborEncBase64
(OSCTXT* pctxt, const OSOCTET* pvalue, OSSIZE nbytes, OSSIZE segsize)
{
   return cborEncBase64(pctxt, pvalue, nbytes, segsize, FALSE);
}

int rtCborEncBase64Url
(OSCTXT* pctxt, const OSOCTET* pvalue, OSSIZE nbytes, OSSIZE segsize)
{
   return cborEncBase64(pctxt, pvalue, nbytes, segsize, TRUE);
}