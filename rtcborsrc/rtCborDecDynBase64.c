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
#include "rtxsrc/rtxSList.h"

static int cborDecDynBase64 (OSCTXT* pctxt, OSDynOctStr64* pOctStr, OSBOOL url)
{
   int ret;
   OSOCTET byte1; /* first byte of text string */
   OSOCTET major; /* the major type found (expected text string) */
   char* pText;   /* the text string (the base64 data) */
   OSOCTET* pdata;   /* the resulting binary data */
   long octets;      /* the number of octets in pdata */
   

   /* decode the first byte of the text string */
   ret = rtxReadBytes (pctxt, &byte1, 1);
   if (0 != ret) return LOG_RTERR (pctxt, ret);
   
   major = byte1 >> 5;
   
   if ( major != OSRTCBOR_UTF8STR ) {
      rtxErrAddUIntParm (pctxt, OSRTCBOR_UTF8STR);
      rtxErrAddUIntParm (pctxt, major);
      return LOG_RTERRNEW (pctxt, RTERR_IDNOTFOU);
   }
   
   /* decode the text string */
   ret = rtCborDecDynUTF8Str(pctxt, byte1, &pText);
   if (0 != ret) return LOG_RTERR (pctxt, ret);
   
   /* convert text from base64 to binary */
   if ( url ) octets = rtxBase64UrlDecodeData( pctxt, pText, strlen(pText),
                                                &pdata);
   else octets = rtxBase64DecodeData( pctxt, pText, strlen(pText), &pdata);
   
   rtxMemFreePtr(pctxt, pText);
   
   if ( octets < 0 ) return LOG_RTERR (pctxt, octets);
#if LONG_MAX > SIZE_MAX
   else if (octets > OSSIZE_MAX) {
      return LOG_RTERRNEW(pctxt, RTERR_TOOBIG);
   }
#endif
   else {
      pOctStr->numocts = (OSSIZE) octets;
      pOctStr->data = pdata;
   }
   
   return 0;
}

int rtCborDecDynBase64 (OSCTXT* pctxt, OSDynOctStr64* pOctStr)
{
   return cborDecDynBase64(pctxt, pOctStr, FALSE);
}

int rtCborDecDynBase64Url (OSCTXT* pctxt, OSDynOctStr64* pOctStr)
{
   return cborDecDynBase64(pctxt, pOctStr, TRUE);
}