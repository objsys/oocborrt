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
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxSList.h"

static int decodeKeyOrValue (OSCTXT* pctxt, OSRTGenValue** ppvalue)
{
   int ret;
   OSRTGenValue* pvalue = rtxMemAllocType (pctxt, OSRTGenValue);
   if (0 == pvalue) return LOG_RTERR (pctxt, RTERR_NOMEM);

   /* Decode generic value */
   ret = rtCborDecValue (pctxt, pvalue);
   if (0 != ret) {
      rtxMemFreePtr (pctxt, pvalue);
      return LOG_RTERR (pctxt, ret);
   }

   *ppvalue = pvalue;

   return ret;
}

int rtCborDecMap (OSCTXT* pctxt, OSOCTET byte1, OSRTGenValueMap* pmap)
{
   int ret;

   if (0 == pmap) {
      ret = rtCborDecSkipField (pctxt, byte1);
   }
   else {
      OSOCTET tag = byte1 >> 5;
      OSOCTET len = byte1 & 0x1F;

      OSRTASSERT (tag == OSRTCBOR_MAP);

      if (len == OSRTCBOR_INDEF) {
         OSRTGenKeyValuePair* pvalue;
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
            pvalue = rtxMemAllocType (pctxt, OSRTGenKeyValuePair);
            if (0 == pvalue) {
               rtxSListFreeAll (&slist);
               return LOG_RTERR (pctxt, RTERR_NOMEM);
            }
            pvalue->key = pvalue->value = 0;

            /* Decode key */
            ret = decodeKeyOrValue (pctxt, &pvalue->key);
            if (0 == ret) {
               /* Decode value */
               ret = decodeKeyOrValue (pctxt, &pvalue->value);
               if (0 != ret) {
                  rtxMemFreePtr (pctxt, pvalue->key);
                  pvalue->key = 0;
               }
            }
            if (0 == ret) {
               if (0 == rtxSListAppend (&slist, pvalue)) {
                  ret = RTERR_NOMEM;
               }
            }
            if (0 != ret) {
               if (0 != pvalue->key) rtxMemFreePtr (pctxt, pvalue->key);
               if (0 != pvalue->value) rtxMemFreePtr (pctxt, pvalue->value);
               rtxMemFreePtr (pctxt, pvalue);

               /* Break loop - items that were decoded successfully will be
                  transferred to map structure */
               break;
            }
         }

         /* Allocate memory for map */
         pmap->nitems = 0;
         pmap->values =
            rtxMemAllocArray (pctxt, slist.count, OSRTGenKeyValuePair);

         if (0 == pmap->values) {
            /* Free existing items on list */
            pnode = slist.head;
            while (0 != pnode) {
               pvalue = (OSRTGenKeyValuePair*)pnode->data;
               rtxMemFreePtr (pctxt, pvalue->key);
               rtxMemFreePtr (pctxt, pvalue->value);
               pnode = pnode->next;
            }
            rtxSListFreeAll (&slist);
            return LOG_RTERR (pctxt, RTERR_NOMEM);
         }

         /* Transfer list to map */
         pnode = slist.head;
         while (0 != pnode) {
            pvalue = (OSRTGenKeyValuePair*)pnode->data;
            pmap->values[pmap->nitems].key = pvalue->key;
            pmap->values[pmap->nitems++].value = pvalue->value;
            rtxMemFreePtr (pctxt, pvalue); /* free value container */
            pnode = pnode->next;
         }

         rtxSListFree (&slist); /* will free only nodes */
      }
      else {
         OSSIZE nitems;

         /* Decode tag and number of items */
         ret = rtCborDecSize (pctxt, (byte1 & 0x1f), &nitems);
         if (0 == ret) {
            OSSIZE i;

            /* Allocate memory for array */
            pmap->nitems = 0;
            pmap->values =
               rtxMemAllocArray (pctxt, nitems, OSRTGenKeyValuePair);
            if (0 == pmap->values) return LOG_RTERR (pctxt, RTERR_NOMEM);

            /* Loop to decode map items */
            for (i = 0; i < nitems; i++) {
               /* Decode key */
               ret = decodeKeyOrValue (pctxt, &pmap->values[i].key);
               if (0 == ret) {
                  /* Decode value */
                  ret = decodeKeyOrValue (pctxt, &pmap->values[i].value);
                  if (0 != ret) rtxMemFreePtr (pctxt, pmap->values[i].key);
               }
               if (0 == ret) {
                  pmap->nitems++;
               }
               else break;
            }
         }
      }
   }

   return (0 == ret) ? 0 : LOG_RTERR (pctxt, ret);
}
