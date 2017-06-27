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

#include <stdarg.h>
#include <stdlib.h>
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxSList.h"

/* Singly-linked list */

void rtxSListInit (OSRTSList* pList)
{
   if (pList) {
      pList->count = 0;
      pList->head = (OSRTSListNode*) 0;
      pList->tail = (OSRTSListNode*) 0;
      pList->pctxt = 0;
   }
}

void rtxSListInitEx (OSCTXT* pctxt, OSRTSList* pList)
{
   if (pList) {
      pList->count = 0;
      pList->head = (OSRTSListNode*) 0;
      pList->tail = (OSRTSListNode*) 0;
      pList->pctxt = pctxt;
   }
}

static void freeSList (OSRTSList* pList, OSBOOL freeData)
{
   if (pList) {
      OSRTSListNode *p1, *p2 = pList->head;
      while (p2) {
         p1 = p2;
         p2 = p1->next;
         if (pList->pctxt) {
            if (freeData) rtxMemFreePtr (pList->pctxt, p1->data);
            rtxMemFreeType (pList->pctxt, p1);
         }
         else {
            if (freeData) free (p1->data);
            free (p1);
         }
      }
      rtxSListInitEx (pList->pctxt, pList);
   }
}

void rtxSListFree (OSRTSList* pList)
{
   freeSList (pList, FALSE);
}

void rtxSListFreeAll (OSRTSList* pList)
{
   freeSList (pList, TRUE);
}

OSRTSList* rtxSListCreate ()
{
   OSRTSList* pList = (OSRTSList*) malloc (sizeof(OSRTSList));
   if (pList == 0) return 0;
   rtxSListInit (pList);
   return pList;
}

OSRTSList* rtxSListCreateEx (OSCTXT* pctxt)
{
   OSRTSList* pList = rtxMemAllocType (pctxt, OSRTSList);
   if (pList == 0) return 0;
   rtxSListInitEx (pctxt, pList);
   return pList;
}

OSRTSListNode* rtxSListAppend (OSRTSList* pList, void* pData)
{
   OSRTSListNode* pListNode =
      (OSRTSListNode*) ((pList->pctxt) ?
      rtxMemAllocType (pList->pctxt, OSRTSListNode) :
      malloc (sizeof(OSRTSListNode)));

   if (pListNode) {
      pListNode->data = pData;
      pListNode->next = (OSRTSListNode*) 0;
      if (pList->tail) {
         pList->tail->next = pListNode;
      }
      if (!pList->head) {
         pList->head = pListNode;
      }
      pList->tail = pListNode;
      pList->count++;
   }
   return pListNode;
}

OSBOOL rtxSListFind (OSRTSList* pList, void* pData)
{
   OSRTSListNode* pNode = pList->head;
   while (0 != pNode) {
      if (pNode->data == pData) return TRUE;
      else pNode = pNode->next;
   }
   return FALSE;
}

void rtxSListRemove (OSRTSList* pList, void* pData)
{
   OSRTSListNode* pNode = pList->head;
   OSRTSListNode* pPrev = 0;

   while (0 != pNode) {
      if (pNode->data == pData) break;
      else {
         pPrev = pNode;
         pNode = pNode->next;
      }
   }

   if (0 != pNode) {
      if (0 == pPrev) {
         /* head node */
         pList->head = pNode->next;
      }
      else {
         pPrev->next = pNode->next;
      }
      if (pList->tail == pNode) {
         pList->tail = pPrev;
      }
      pList->count--;
      if (pList->pctxt)
         rtxMemFreeType (pList->pctxt, pNode);
      else
         free (pNode);
   }
}

