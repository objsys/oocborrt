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
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"

int rtCborDecDynUTF8Str (OSCTXT* pctxt, OSOCTET byte1, char** ppUtf8Str)
{
   int ret;
   OSOCTET tag = byte1 >> 5;
   OSOCTET len = byte1 & 0x1F;

   OSRTASSERT (tag == OSRTCBOR_UTF8STR);

   if (0 == ppUtf8Str) {
      ret = rtCborDecSkipField (pctxt, byte1);
   }
   else if (len == OSRTCBOR_INDEF) {
      OSSIZE segsize, totalSize = 0;
      char* pvalue, *pvalue2;
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
                  pvalue = (char*) rtxMemAlloc (pctxt, segsize+1);
                  if (0 == pvalue) {
                     ret = RTERR_NOMEM;
                     break;
                  }

                  /* Decode segment and append to list */
                  ret = rtxReadBytes (pctxt, pvalue, segsize);
                  if (0 == ret) {
                     if (0 == rtxSListAppend (&slist, pvalue)) {
                        ret = RTERR_NOMEM;
                     }
                  }
                  if (0 != ret) break;
                  pvalue[segsize] = '\0'; /* null-terminate string */
                  totalSize += segsize;
               }
            }
         }
         else break;
      }

      if (0 != ret) {
         rtxSListFreeAll (&slist);
         return LOG_RTERR (pctxt, ret);
      }

      /* Allocate memory for char string */
      pvalue2 = *ppUtf8Str = (char*) rtxMemAlloc (pctxt, totalSize+1);

      if (0 == pvalue2) {
         rtxSListFreeAll (&slist);
         return LOG_RTERR (pctxt, RTERR_NOMEM);
      }

      /* Copy segments in list to full string */
      *pvalue2 = '\0';
      pnode = slist.head;
      while (0 != pnode) {
         pvalue = (char*) pnode->data;
         rtxStrcat (pvalue2, totalSize+1, pvalue);
         pnode = pnode->next;
      }

      rtxSListFreeAll (&slist);
   }
   else {
      OSSIZE nbytes;
      ret = rtCborDecSize (pctxt, (byte1 & 0x1F), &nbytes);
      if (0 == ret) {
         if (0 != ppUtf8Str) {
            /* Allocate memory for the target string */
            *ppUtf8Str = (char*) rtxMemAlloc (pctxt, nbytes+1);
            if (0 == *ppUtf8Str) return LOG_RTERR (pctxt, RTERR_NOMEM);

            /* Read bytes into target variable */
            ret = rtxReadBytes (pctxt, *ppUtf8Str, nbytes);
            if (0 == ret) {
               (*ppUtf8Str)[nbytes] = '\0';
            }
         }
         else /* skip contents */
            ret = rtxReadBytes (pctxt, 0, nbytes);
      }
   }

   return (0 != ret) ? LOG_RTERR (pctxt, ret) : 0;
}
