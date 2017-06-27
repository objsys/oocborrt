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
/**
 * @file rtxMemBuf.h
 */
/**
 * @defgroup buffermanfun Memory Buffer Management Functions
 * @{
 *
 * Memory buffer management functions handle the allocation, expansion, and
 * deallocation of dynamic memory buffers used by some encode/decode functions.
 * Dynamic memory buffers are buffers that can grow or shrink to hold variable
 * sized amounts of data. This group of functions allows data to be appended to
 * buffers, to be set within buffers, and to be retrieved from buffers.
 * Currently, these functions are used within the generated SAX decode routines
 * to collect data as it is parsed by an XML parser.
 */

/* memory buffer */

#ifndef _RTXMEMBUF_H_
#define _RTXMEMBUF_H_

#include "rtxsrc/rtxContext.h"

typedef struct OSRTMEMBUF {
   OSCTXT*      pctxt;
   OSSIZE       segsize;        /* segment size                     */
   OSSIZE       startidx;       /* start index of useful info       */
   OSSIZE       usedcnt;        /* used byte count                  */
   OSSIZE       bufsize;        /* buffer size                      */
   OSSIZE       bitOffset;      /* bit offset                       */
   OSUINT32     userState;      /* user state - any value, 0 is initial */
   OSOCTET*     buffer;         /* memory buffer                    */
   OSBOOL       isDynamic;      /* is buffer allocated dynamically? */
   OSBOOL       isExpandable;   /* is buffer expandable?            */
   OSBOOL       useSysMem;      /* use system memory management     */
} OSRTMEMBUF;

#define OSMBDFLTSEGSIZE 1024

#define OSMEMBUFPTR(pmb)    ((pmb)->buffer + (pmb)->startidx)
#define OSMEMBUFENDPTR(pmb) ((pmb)->buffer + (pmb)->startidx + (pmb)->usedcnt)
#define OSMEMBUFUSEDSIZE(pmb) ((OSSIZE)(pmb)->usedcnt)

#define OSMBAPPENDSTR(pmb,str) if (0 != str) \
rtxMemBufAppend(pmb,(OSOCTET*)str,OSCRTLSTRLEN(str))

#define OSMBAPPENDSTRL(pmb,str) \
rtxMemBufAppend(pmb,(OSOCTET*)str,OSCRTLSTRLEN(str))

#define OSMBAPPENDUTF8(pmb,str) if (0 != str) \
rtxMemBufAppend(pmb,(OSOCTET*)str,rtxUTF8LenBytes(str))

#ifdef __cplusplus
extern "C" {
#endif

/* Memory buffer functions */

/**
 * This function appends the data to the end of a memory buffer. If the buffer
 * was dynamic and full then the buffer will be reallocated. If it is static
 * (the static buffer was assigned by a call to rtxMemBufInitBuffer) or it is
 * empty (no memory previously allocated) then a new buffer will be allocated.
 *
 * @param pMemBuf      A pointer to a memory buffer structure.
 * @param pdata        The pointer to the buffer to be appended. The data will
 *                       be copied at the end of the memory buffer.
 * @param nbytes       The number of bytes to be copied from pData.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxMemBufAppend
(OSRTMEMBUF* pMemBuf, const OSOCTET* pdata, OSSIZE nbytes);

/**
 * This function cuts off the part of memory buffer. The beginning of the
 * cutting area is specified by offset "fromOffset" and the length is specified
 * by "nbytes". All data in this part will be lost. The data from the offset
 * "fromOffset + nbytes" will be moved to "fromOffset" offset.
 *
 * @param pMemBuf      A pointer to a memory buffer structure.
 * @param fromOffset   The offset of the beginning part, being cut off.
 * @param nbytes       The number of bytes to be cut off from the memory
 *                       buffer.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxMemBufCut
(OSRTMEMBUF* pMemBuf, OSSIZE fromOffset, OSSIZE nbytes);

/**
 * This function frees the memory buffer. If memory was allocated then it will
 * be freed. Do not use the memory buffer structure after this function is
 * called.
 *
 * @param pMemBuf      A pointer to a memory buffer structure.
 */
EXTERNRT void rtxMemBufFree (OSRTMEMBUF* pMemBuf);

/**
 * This function returns the pointer to the used part of a memory buffer.
 *
 * @param pMemBuf      A pointer to a memory buffer structure.
 * @param length       The pointer to the length of the used part of the memory
 *                       buffer.
 * @return             The pointer to the used part of the memory buffer.
 */
EXTERNRT OSOCTET* rtxMemBufGetData (const OSRTMEMBUF* pMemBuf, int* length);

/**
 * This function returns the length of the used part of a memory buffer.
 *
 * @param pMemBuf      A pointer to a memory buffer structure.
 * @return             The length of the used part of the buffer.
 */
EXTERNRT OSSIZE rtxMemBufGetDataLen (const OSRTMEMBUF* pMemBuf);

/**
 * This function initializes a memory buffer structure. It does not allocate
 * memory; it sets the fields of the structure to the proper states. This
 * function must be called before any operations with the memory buffer.
 *
 * @param pCtxt        A provides a storage area for the function to store all
 *                       working variables that must be maintained between
 *                       function calls.
 * @param pMemBuf      A pointer to the initialized memory buffer structure.
 * @param segsize      The number of bytes in which the memory buffer will be
 *                       expanded incase it is full.
 */
EXTERNRT void rtxMemBufInit
(OSCTXT* pCtxt, OSRTMEMBUF* pMemBuf, OSSIZE segsize);

/**
 * @} buffermanfun
 */
#ifdef __cplusplus
}
#endif

#endif
