/**
 * Copyright (c) 1997-2017 by Objective Systems, Inc.
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

#include "rtxsrc/rtxCommon.h"
#include "rtxsrc/rtxCharStr.h"
#include <stdlib.h>
#include <string.h>

int rtxInitContext (OSCTXT* pctxt)
{
   memset (pctxt, 0, sizeof(OSCTXT));
   return 0;
}

int rtxInitContextBuffer (OSCTXT* pctxt, OSOCTET* bufaddr, OSSIZE bufsiz)
{
   if (bufaddr == 0) {
      /* dynamic buffer */
      if (bufsiz == 0) bufsiz = OSRTENCBUFSIZ;
      pctxt->buffer.data = (OSOCTET*) rtxMemAlloc (pctxt, bufsiz);
      if (!pctxt->buffer.data) return RTERR_NOMEM;
      pctxt->buffer.size = bufsiz;
      pctxt->buffer.dynamic = TRUE;
   }
   else {
      /* static buffer */
      pctxt->buffer.data = (OSOCTET*) bufaddr;
      pctxt->buffer.size = bufsiz;
      pctxt->buffer.dynamic = FALSE;
   }

   pctxt->buffer.byteIndex = 0;
   pctxt->buffer.bitOffset = 8;

   return 0;
}

void rtxFreeContext (OSCTXT* pctxt)
{
   if (pctxt->buffer.dynamic && pctxt->buffer.data) {
      rtxMemFreePtr (pctxt, pctxt->buffer.data);
   }

   rtxErrFreeParms (pctxt);

   if (pctxt->pMemHeap != 0) {
      rtxMemFree (pctxt);
      free (pctxt->pMemHeap);
      pctxt->pMemHeap = 0;
   }
}

void rtxCopyContext (OSCTXT* pdest, OSCTXT* psrc)
{
   memcpy (&pdest->buffer, &psrc->buffer, sizeof(OSRTBuffer));
   pdest->flags = psrc->flags;
}

void rtxCtxtSetFlag (OSCTXT* pctxt, OSUINT32 mask)
{
   pctxt->flags |= mask;
}

void rtxCtxtClearFlag (OSCTXT* pctxt, OSUINT32 mask)
{
   pctxt->flags &= ~mask;
}

int rtxCtxtPushArrayElemName
(OSCTXT* pctxt, const OSUTF8CHAR* elemName, OSSIZE idx)
{
   const char* pstr;
   char numbuf[32];
   int ret = rtxSizeToCharStr (idx, numbuf, sizeof(numbuf), 0);
   if (ret < 0) return LOG_RTERR (pctxt, ret);

   pstr = rtxStrDynJoin (pctxt, (const char*)elemName, "[", numbuf, "]", 0);
   if (0 == pstr) return LOG_RTERR (pctxt, RTERR_NOMEM);

   return rtxCtxtPushElemName (pctxt, OSUTF8(pstr));
}

int rtxCtxtPushElemName (OSCTXT* pctxt, const OSUTF8CHAR* elemName)
{
   /* Add name to element name stack in context */
   return rtxDListAppend (pctxt, &pctxt->elemNameStack, (void*)elemName) ?
      0 : RTERR_NOMEM;
}

OSBOOL rtxCtxtPopArrayElemName (OSCTXT* pctxt)
{
   const OSUTF8CHAR* elemName = rtxCtxtPopElemName (pctxt);
   if (0 != elemName) {
      rtxMemFreePtr (pctxt, (void*)elemName);
      return TRUE;
   }
   else return FALSE;
}

const OSUTF8CHAR* rtxCtxtPopElemName (OSCTXT* pctxt)
{
   const OSUTF8CHAR* elemName = 0;
   if (pctxt->elemNameStack.count > 0) {
      elemName = (const OSUTF8CHAR*) pctxt->elemNameStack.tail->data;
      rtxDListFreeNode
         (pctxt, &pctxt->elemNameStack, pctxt->elemNameStack.tail);
   }
   return elemName;
}

int rtxExpandOutputBuffer (OSCTXT* pctxt, size_t nbytes)
{
   if (pctxt->buffer.dynamic) {
      OSOCTET* newbuf;
      size_t extent = OSRTMAX (pctxt->buffer.size, nbytes);

      /* If dynamic encoding is enabled, expand the current buffer to   */
      /* allow encoding to continue.                                    */
      pctxt->buffer.size += extent;

      newbuf = (OSOCTET *)rtxMemRealloc
         (pctxt, pctxt->buffer.data, pctxt->buffer.size);

      if (newbuf == NULL) return LOG_RTERR (pctxt, RTERR_NOMEM);

      pctxt->buffer.data = newbuf;
   }
   else {
      return LOG_RTERR (pctxt, RTERR_BUFOVFLW);
   }

   return 0;
}

int rtxCheckOutputBuffer (OSCTXT* pctxt, size_t nbytes)
{
   /* Account for the bit offset. */
   size_t idx = (pctxt->buffer.bitOffset != 8) ?
      pctxt->buffer.byteIndex + 1 : pctxt->buffer.byteIndex;

   if ( ( idx + nbytes ) > pctxt->buffer.size ) {
      int stat = rtxExpandOutputBuffer (pctxt, nbytes);
      if (stat != 0) return LOG_RTERR(pctxt, stat);
   }

   return 0;
}

int rtxReadBytes (OSCTXT* pctxt, OSOCTET* pdata, size_t nocts)
{
   if (nocts == 0) return 0;

   /* Check if buffer contains number of bytes requested */
   if (pctxt->buffer.byteIndex + nocts > pctxt->buffer.size) {
      return LOG_RTERR (pctxt, RTERR_ENDOFBUF);
   }
   else {
      if (0 != pdata) {
         memcpy (pdata, OSRTBUFPTR(pctxt), nocts);
      }
      pctxt->buffer.byteIndex += nocts;
   }

   /* Set last character in context */
   if (pctxt->buffer.byteIndex > 0) {
      pctxt->lastChar = pctxt->buffer.data[pctxt->buffer.byteIndex-1];
   }

   return 0;
}

int rtxWriteBytes (OSCTXT* pctxt, const OSOCTET* pdata, size_t nocts)
{
   if (nocts == 0 || 0 == pdata) return 0;

   /* Check if buffer large enough to hold number of bytes to write
      (note: only static output buffer is supported) */
   if (pctxt->buffer.byteIndex + nocts > pctxt->buffer.size) {
      return LOG_RTERR (pctxt, RTERR_ENDOFBUF);
   }
   else {
      memcpy (OSRTBUFPTR(pctxt), pdata, nocts);
      pctxt->buffer.byteIndex += nocts;
      pctxt->lastChar = pctxt->buffer.data[pctxt->buffer.byteIndex-1];
   }

   return 0;
}

int rtxCopyAsciiText (OSCTXT* pctxt, const char* text)
{
   if (0 != text) {
      size_t nchars = strlen (text);
      if (nchars > 0) {
         return rtxWriteBytes (pctxt, (const OSOCTET*)text, nchars);
      }
   }
   return 0;
}

int rtxCopyUTF8Text (OSCTXT* pctxt, const OSUTF8CHAR* text)
{
   return rtxCopyAsciiText (pctxt, (const char*)text);
}
