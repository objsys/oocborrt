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

#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxMemBuf.h"

EXTRTMETHOD void rtxMemBufInit
(OSCTXT* pCtxt, OSRTMEMBUF* pMemBuf, OSSIZE segsize)
{
   pMemBuf->pctxt = pCtxt;
   pMemBuf->segsize = (0 == segsize) ? OSMBDFLTSEGSIZE : segsize;
   pMemBuf->startidx = 0;
   pMemBuf->usedcnt = 0;
   pMemBuf->bufsize = 0;
   pMemBuf->buffer = 0;
   pMemBuf->bitOffset = 0;
   pMemBuf->userState = 0;
   pMemBuf->isDynamic = TRUE;
   pMemBuf->isExpandable = TRUE;
   pMemBuf->useSysMem = FALSE;
}

EXTRTMETHOD void rtxMemBufFree (OSRTMEMBUF* pMemBuf)
{
   if (pMemBuf->isDynamic) {
      rtxMemFreePtr (pMemBuf->pctxt, pMemBuf->buffer);
      pMemBuf->buffer = 0;
      pMemBuf->bufsize = 0;
   }

   pMemBuf->usedcnt = 0;
   pMemBuf->startidx = 0;
   pMemBuf->bitOffset = 0;
}

static int rtxMemBufPreAllocate (OSRTMEMBUF* pMemBuf, OSSIZE nbytes)
{
   if (0 == nbytes) return 0;

   /* Ensure capacity for data */
   if ((pMemBuf->usedcnt + nbytes) > pMemBuf->bufsize) {
      OSSIZE reqbytes = pMemBuf->usedcnt + nbytes;
      OSSIZE nsegs = ((reqbytes - 1) / pMemBuf->segsize) + 1;

      if (pMemBuf->isDynamic && 0 != pMemBuf->buffer) {
         OSOCTET* newbuf;

         newbuf = rtxMemReallocArray
            (pMemBuf->pctxt, pMemBuf->buffer,
             pMemBuf->segsize * nsegs, OSOCTET);

         if (newbuf == 0)
            return LOG_RTERRNEW (pMemBuf->pctxt, RTERR_NOMEM);

         pMemBuf->buffer = newbuf;
      }
      else { /* Do initial mem alloc */
         OSOCTET *oldbuf = pMemBuf->buffer;

         pMemBuf->buffer = rtxMemAllocArray
            (pMemBuf->pctxt, pMemBuf->segsize * nsegs, OSOCTET);

         if (pMemBuf->buffer == 0)
            return LOG_RTERRNEW (pMemBuf->pctxt, RTERR_NOMEM);

         if (oldbuf && pMemBuf->bufsize)
            OSCRTLSAFEMEMCPY (pMemBuf->buffer, pMemBuf->segsize*nsegs,
                  oldbuf, pMemBuf->bufsize);

         pMemBuf->isDynamic = TRUE;
      }
      if (0 == pMemBuf->buffer)
         return LOG_RTERRNEW (pMemBuf->pctxt, RTERR_NOMEM);

      pMemBuf->bufsize = pMemBuf->segsize * nsegs;
   }
   return 0;
}

EXTRTMETHOD int rtxMemBufAppend
(OSRTMEMBUF* pMemBuf, const OSOCTET* pdata, OSSIZE nbytes)
{
   int stat = 0;

   if (0 == nbytes) return 0;

   /* Ensure capacity for data */
   stat = rtxMemBufPreAllocate (pMemBuf, nbytes);

   /* Copy data */
   if (stat == 0) {
      if (pdata) {
         OSCRTLSAFEMEMCPY (&pMemBuf->buffer[pMemBuf->usedcnt],
               pMemBuf->bufsize-pMemBuf->usedcnt, pdata, nbytes);
         pMemBuf->usedcnt += nbytes;
      }
   }
   else return LOG_RTERR (pMemBuf->pctxt, stat);
   return 0;
}

EXTRTMETHOD OSOCTET* rtxMemBufGetData (const OSRTMEMBUF* pMemBuf, int* length)
{
   if (length != 0) {
      OSSIZE sz = pMemBuf->usedcnt - pMemBuf->startidx;
      *length = (sz <= OSINT32_MAX) ? (int)sz : -1;
   }
   return pMemBuf->buffer + pMemBuf->startidx;
}

EXTRTMETHOD OSSIZE rtxMemBufGetDataLen (const OSRTMEMBUF* pMemBuf)
{
   return (pMemBuf->usedcnt >= pMemBuf->startidx) ?
      (OSSIZE)(pMemBuf->usedcnt - pMemBuf->startidx) : 0;
}

EXTRTMETHOD int rtxMemBufCut
(OSRTMEMBUF* pMemBuf, OSSIZE fromOffset, OSSIZE nbytes)
{
   if (fromOffset + nbytes > pMemBuf->usedcnt)
      return LOG_RTERRNEW (pMemBuf->pctxt, RTERR_INVPARAM);

   OSCRTLSAFEMEMMOVE (pMemBuf->buffer + pMemBuf->startidx + fromOffset,
         pMemBuf->usedcnt-nbytes,
         pMemBuf->buffer + pMemBuf->startidx + fromOffset + nbytes,
         pMemBuf->usedcnt - nbytes);

   pMemBuf->usedcnt -= nbytes;
   return 0;
}
