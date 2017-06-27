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
#include <string.h>
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxStack.h"
#include "rtxsrc/rtxErrCodes.h"

/* Stack */

void rtxStackInit (OSCTXT* pctxt, OSRTStack* pStack)
{
   pStack->pctxt = pctxt;
   rtxDListInit (&pStack->dlist);
}

OSRTStack* rtxStackCreate (OSCTXT* pctxt)
{
   OSRTStack* pStack = rtxMemAllocType (pctxt, OSRTStack);
   if (pStack == 0) return 0;
   rtxStackInit (pctxt, pStack);
   return pStack;
}

int rtxStackPush (OSRTStack* pStack, void* pData)
{
   if (0 != pStack && 0 != pStack->pctxt) {
      rtxDListInsert (pStack->pctxt, &pStack->dlist, 0, pData);
      return 0;
   }
   return RTERR_NULLPTR;
}

void* rtxStackPop (OSRTStack* pStack)
{
   if (0 != pStack && 0 != pStack->pctxt && pStack->dlist.count > 0) {
      void* pdata = pStack->dlist.head->data;
      rtxDListFreeNode (pStack->pctxt, &pStack->dlist, pStack->dlist.head);
      return pdata;
   }
   return NULL;
}

void* rtxStackPeek (OSRTStack* pStack)
{
   return (pStack->dlist.count > 0) ? pStack->dlist.head->data : 0;
}

