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
#include "rtxsrc/rtxIntStack.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxErrCodes.h"

/* Stack */

int rtxIntStackInit
(OSCTXT* pctxt, OSRTIntStack* pstack, size_t capacity)
{
   pstack->pctxt = pctxt;
   pstack->index = 0;
   pstack->size = (0 == capacity) ? OSRTISTK_DEFAULT_CAPACITY : capacity;
   pstack->pdata = rtxMemAllocArray (pctxt, pstack->size, OSINT32);
   return (0 == pstack->pdata) ? RTERR_NOMEM : 0;
}

int rtxIntStackPush (OSRTIntStack* pstack, OSINT32 value)
{
   if (pstack->index >= pstack->size) {
      pstack->size *= 2; /* double stack size */
      pstack->pdata = rtxMemReallocArray
         (pstack->pctxt, pstack->pdata, pstack->size, OSINT32);
      if (0 == pstack->pdata) return RTERR_NOMEM;
   }

   pstack->pdata[pstack->index++] = value;

   return 0;
}

int rtxIntStackPop (OSRTIntStack* pstack, OSINT32* pvalue)
{
   if (pstack->index > 0) {
      pstack->index--;
      if (0 != pvalue) { *pvalue = pstack->pdata[pstack->index]; }
      return 0;
   }
   else return RTERR_ENDOFBUF;
}

int rtxIntStackPeek (OSRTIntStack* pstack, OSINT32* pvalue)
{
   if (pstack->index > 0) {
      if (0 != pvalue) { *pvalue = pstack->pdata[pstack->index - 1]; }
      return 0;
   }
   else return RTERR_ENDOFBUF;
}
