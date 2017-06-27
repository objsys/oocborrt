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
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxSList.h"

static void freeList (OSRTSList* pList)
{
   OSDynOctStr64* pvalue;
   OSRTSListNode* pnode = pList->head;
   while (0 != pnode) {
      pvalue = (OSDynOctStr64*) pnode->data;
      rtxMemFreePtr (pList->pctxt, pvalue->data);
      pnode = pnode->next;
   }
   rtxSListFreeAll (pList);
}

int rtCborDecDynByteStr
(OSCTXT* pctxt, OSOCTET byte1, OSDynOctStr64* pOctStr)
{
   int ret;
   OSOCTET tag = byte1 >> 5;
   OSOCTET len = byte1 & 0x1F;

   OSRTASSERT (tag == OSRTCBOR_BYTESTR);

   if (0 == pOctStr) {
      ret = rtCborDecSkipField (pctxt, byte1);
   }
   else if (len == OSRTCBOR_INDEF) {
      OSSIZE idx = 0, segsize, totalSize = 0;
      OSDynOctStr64* pvalue;
      OSRTSListNode* pnode;
      OSRTSList slist;
      rtxSListInitEx (pctxt, &slist);

      /* Decode segments to linked list */
      for (;;) {
         ret = rtxReadBytes (pctxt, &byte1, 1);
         if (0 == ret) {
            if (byte1 == OSRTCBOR_EOC) break;
            else {
               ret = rtCborDecSize (pctxt, (byte1 & 0x1F), &segsize);
               if (0 == ret) {
                  /* Allocate memory for segment */
                  pvalue = rtxMemAllocType (pctxt, OSDynOctStr64);
                  if (0 == pvalue) {
                     freeList (&slist);
                     return LOG_RTERR (pctxt, RTERR_NOMEM);
                  }

                  /* Allocate memory for segment */
                  pvalue->numocts = segsize;
                  pvalue->data = (OSOCTET*) rtxMemAlloc (pctxt, segsize);
                  if (0 == pvalue->data) {
                     freeList (&slist);
                     return LOG_RTERR (pctxt, RTERR_NOMEM);
                  }

                  /* Decode segment and append to list */
                  ret = rtxReadBytes (pctxt, pvalue->data, segsize);
                  if (0 == ret) {
                     if (0 == rtxSListAppend (&slist, pvalue)) {
                        ret = RTERR_NOMEM;
                     }
                  }
                  if (0 != ret) break;
                  totalSize += segsize;
               }
            }
         }
         else break;
      }

      if (0 != ret) {
         freeList (&slist);
         return LOG_RTERR (pctxt, ret);
      }

      /* Allocate memory for byte atring */
      pOctStr->numocts = totalSize;
      pOctStr->data = (OSOCTET*) rtxMemAlloc (pctxt, totalSize);

      if (0 == pOctStr->data) {
         freeList (&slist);
         return LOG_RTERR (pctxt, RTERR_NOMEM);
      }

      /* Transfer list to array */
      pnode = slist.head;
      while (0 != pnode) {
         pvalue = (OSDynOctStr64*) pnode->data;
         OSCRTLMEMCPY (&pOctStr->data[idx], pvalue->data, pvalue->numocts);
         idx += pvalue->numocts;
         pnode = pnode->next;
      }

      freeList (&slist);
   }
   else {
      OSSIZE nbytes;
      ret = rtCborDecSize (pctxt, (byte1 & 0x1F), &nbytes);
      if (0 == ret) {
         if (0 != pOctStr) {
            /* Allocate memory for the target string */
            OSOCTET* ptmp = (OSOCTET*) rtxMemAlloc (pctxt, nbytes);
            if (0 == ptmp) return LOG_RTERR (pctxt, RTERR_NOMEM);

            /* Read bytes into target variable */
            ret = rtxReadBytes (pctxt, ptmp, nbytes);
            if (0 == ret) {
               pOctStr->numocts = nbytes;
               pOctStr->data = ptmp;
            }
         }
         else /* skip contents */
            ret = rtxReadBytes (pctxt, 0, nbytes);
      }
   }

   return (0 != ret) ? LOG_RTERR (pctxt, ret) : 0;
}
