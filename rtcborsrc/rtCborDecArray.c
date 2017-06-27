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

int rtCborDecArray (OSCTXT* pctxt, OSOCTET byte1, OSRTGenPtrArray* pPtrArray)
{
   int ret;

   if (0 == pPtrArray) {
      ret = rtCborDecSkipField (pctxt, byte1);
   }
   else {
      OSOCTET tag = byte1 >> 5;
      OSOCTET len = byte1 & 0x1F;

      OSRTASSERT (tag == OSRTCBOR_ARRAY);

      if (len == OSRTCBOR_INDEF) {
         OSRTGenValue* pvalue;
         OSRTSListNode* pnode;
         OSRTSList slist;
         rtxSListInitEx (pctxt, &slist);

         /* Decode items to linked list */
         for (;;) {
            if (OSRTCBOR_MATCHEOC (pctxt)) {
               pctxt->buffer.byteIndex++;
               break;
            }

            /* Allocate memory for value */
            pvalue = rtxMemAllocType (pctxt, OSRTGenValue);
            if (0 == pvalue) {
               rtxSListFreeAll (&slist);
               return LOG_RTERR (pctxt, RTERR_NOMEM);
            }
            /* Decode generic value and append to list */
            ret = rtCborDecValue (pctxt, pvalue);
            if (0 == ret) {
               if (0 == rtxSListAppend (&slist, pvalue)) {
                  ret = RTERR_NOMEM;
               }
            }
            if (0 != ret) {
               rtxMemFreePtr (pctxt, pvalue);
               rtxSListFreeAll (&slist);
               return LOG_RTERR (pctxt, ret);
            }
         }

         /* Allocate memory for array */
         pPtrArray->nitems = 0;
         pPtrArray->values =
            rtxMemAllocArray (pctxt, slist.count, OSRTGenValue*);

         if (0 == pPtrArray->values) {
            rtxSListFreeAll (&slist);
            return LOG_RTERR (pctxt, RTERR_NOMEM);
         }

         /* Transfer list to array */
         pnode = slist.head;
         while (0 != pnode) {
            pPtrArray->values[pPtrArray->nitems++] = (OSRTGenValue*)pnode->data;
            pnode = pnode->next;
         }

         rtxSListFree (&slist); /* will free only nodes */
      }
      else {
         OSSIZE nitems;

         /* Decode tag and number of items */
         ret = rtCborDecSize (pctxt, len, &nitems);
         if (0 == ret) {
            OSSIZE i;

            /* Allocate memory for array */
            pPtrArray->nitems = 0;
            pPtrArray->values = rtxMemAllocArray (pctxt, nitems, OSRTGenValue*);
            if (0 == pPtrArray->values) return LOG_RTERR (pctxt, RTERR_NOMEM);

            /* Loop to decode array items */
            for (i = 0; i < nitems; i++) {
               /* Allocate memory for value */
               OSRTGenValue* pvalue = rtxMemAllocType (pctxt, OSRTGenValue);
               if (0 == pvalue) return LOG_RTERR (pctxt, RTERR_NOMEM);

               /* Decode generic value */
               ret = rtCborDecValue (pctxt, pvalue);
               if (0 != ret) {
                  rtxMemFreePtr (pctxt, pvalue);
                  return LOG_RTERR (pctxt, ret);
               }
               else {
                  pPtrArray->values[i] = pvalue;
                  pPtrArray->nitems++;
               }
            }
         }
      }
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
