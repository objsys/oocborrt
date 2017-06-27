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
 * @file rtxBuffer.h
 * Common runtime functions for reading from or writing to the message
 * buffer defined within the context structure.
 */
#ifndef _RTXBUFFER_H_
#define _RTXBUFFER_H_

#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxSList.h"

/* reserve octets to enable short backoffs */
#define MIN_STREAM_BACKOFF 0

/**
 Buffer location descriptor
*/
typedef struct _OSRTBufLocDescr {
   OSSIZE numocts;
   OSSIZE offset;
} OSRTBufLocDescr;

#ifdef __cplusplus
extern "C" {
#endif


EXTERNRT int rtxCheckBuffer (OSCTXT* pctxt, size_t nbytes);

/**
 * This function checks to ensure that the output buffer has sufficient space
 * to hold the requested number of bytes. Dynamic buffers are resized if the
 * check fails, while static buffers induce a buffer overflow error. This
 * function may return RTERR_NOMEM if reallocating the dynamic buffer fails.
 *
 * @param   pctxt    Pointer to a context structure.
 * @param   nbytes   The requested capacity for the buffer.
 *
 * @return  0 on success, or less than zero on failure.
 */
EXTERNRT int rtxCheckOutputBuffer (OSCTXT* pctxt, size_t nbytes);

/**
 * This function attempts to expand the output buffer by the requested
 * number of bytes. Dynamic buffers are resized if the
 * check fails, while static buffers induce a buffer overflow error. This
 * function may return RTERR_NOMEM if reallocating the dynamic buffer fails.
 *
 * @param   pctxt    Pointer to a context structure.
 * @param   nbytes   The requested capacity for the buffer.
 *
 * @return  0 on success, or less than zero on failure.
 */
EXTERNRT int rtxExpandOutputBuffer (OSCTXT* pctxt, size_t nbytes);

EXTERNRT int rtxCheckInputBuffer (OSCTXT* pctxt, size_t nbytes);

EXTERNRT int rtxCopyAsciiText (OSCTXT* pctxt, const char* text);

EXTERNRT int rtxCopyUTF8Text (OSCTXT* pctxt, const OSUTF8CHAR* text);

EXTERNRT int rtxCopyUnicodeText (OSCTXT* pctxt, const OSUNICHAR* text);

/**
 * This function safely reads bytes from the currently open stream or
 * memory buffer into the given static buffer.  This function is preferred
 * over \c rtxReadBytes because it will detect buffer overflow.
 *
 * @param pctxt         Pointer to a context structure.
 * @param buffer        Static buffer into which bytes are to be read.
 * @param bufsize       Size of the static buffer.
 * @param nocts         Number of bytes (octets) to read.
 * @return              Status of the operation: 0 if success,
 *                        negative value if error.
 */
EXTERNRT int rtxReadBytesSafe
(OSCTXT* pctxt, OSOCTET* buffer, size_t bufsize, size_t nocts);

/**
 * This function reads bytes from the currently open stream or memory buffer.
 *
 * @param pctxt         Pointer to a context structure.
 * @param pdata         Pointer to byte array where bytes are to be copied.
 * @param nocts         Number of bytes (octets) to read.
 * @return              Status of the operation: 0 if success,
 *                        negative value if error.
 */
EXTERNRT int rtxReadBytes (OSCTXT* pctxt, OSOCTET* pdata, size_t nocts);

/**
 * This function reads bytes from the currently open stream or memory buffer.
 * In this case the function MAY allocate memory to hold the read bytes.  It
 * will only do this if the requested number of bytes will not fit in the
 * context buffer; othwerwise, a pointer to a location in the context buffer
 * is returned.  If memory was allocated, it should be freed using
 * rtxMemFreePtr.
 *
 * @param pctxt         Pointer to a context structure.
 * @param ppdata        Pointer to byte buffer pointer.
 * @param nocts         Number of bytes (octets) to read.
 * @param pMemAlloc     Pointer to boolean value which is set to true if
 *                        memory was allocated to hold requested bytes.
 * @return              Status of the operation: 0 if success,
 *                        negative value if error.
 */
EXTERNRT int rtxReadBytesDynamic
(OSCTXT* pctxt, OSOCTET** ppdata, size_t nocts, OSBOOL* pMemAlloc);

/**
 * This function writes bytes to the currently open stream or memory buffer.
 *
 * @param pctxt         Pointer to a context structure.
 * @param pdata         Pointer to location where bytes are to be copied.
 * @param nocts         Number of bytes to read.
 * @return              I/O byte count.
 */
EXTERNRT int rtxWriteBytes
(OSCTXT* pctxt, const OSOCTET* pdata, size_t nocts);


/**
 * Sort a list of buffer locations, referring to component encodings, by
 * comparing the referenced encodings as octet strings.
 *
 * The sorting can be used with canonical-BER (CER), distinguished-BER (DER),
 * and canonical-XER (XER).
 *
 * Encoding into the buffer may be done as a normal encoding (start to end) or
 * as a reverse encoding (end to start).  This affects the parameters as
 * described below.
 *
 * @param refPoint      Reference point in the buffer for the buffer locations.
 *                      For normal encoding, refPoint is the start of the
 *                      buffer; for reverse encoding, refPoint is the end of the
 *                      buffer.
 * @param pList         List of OSRTBufLocDescr, each of which locates the
 *                      start of an encoded component.  The offsets for the
 *                      locations are relative to refPoint.
 *                      If normal is TRUE, this function orders the list from
 *                      least to greatest.  Otherwise, it is ordered from
 *                      greatest to least.
 * @param normal        TRUE for normal encoding; FALSE for reverse encoding.
 *                      This tells the function whether to add or substract
 *                      offsets from refPoint to locate the component encodings
 *                      and also how to order the list.
 * @returns             TRUE if any changes to pList were made; FALSE otherwise
 *                      (meaning the list was already in the desired order).
 */
EXTERNRT OSBOOL rtxCanonicalSort (OSOCTET* refPoint, OSRTSList* pList,
                                 OSBOOL normal);


/**
 * Encode the encodings held in pMemCtxt into pctxt, first sorting them as
 * required for canonical BER (and other encoding rules) by X.690 11.6.
 *
 * @param pctxt         Pointer to context structure into which the sorted
 *                      encodings should be encoded.
 * @param pMemCtxt      Pointer to context structure which holds the unsorted
 *                      encodings.
 * @param pList         List of Asn1BufLocDescr, each of which locates an
 *                      encoding in pMemCtxt's buffer, the whole being the
 *                      encodings that are to be sorted.
 */
EXTERNRT int rtxEncCanonicalSort (OSCTXT* pctxt,
                                   OSCTXT* pMemCtxt,
                                   OSRTSList* pList);

/**
 * Set the buffer location description's offset (pDescr->offset) to the current
 * position in pCtxt's buffer.
 */
EXTERNRT void rtxGetBufLocDescr (OSCTXT *pctxt, OSRTBufLocDescr* pDescr);

/**
 * Create a new Asn1BufLocDescr for an element just encoded and append it to
 * pElemList.
 *
 * @param pctxt      Pointer to context where data has been encoded.
 * @param pElemList  List of Asn1BufLocDescr to which a new entry will be added.
 * @param pDescr     Pointer to Asn1BufLocDescr whose offset indicates the start
 *                   of the element just encoded.  The new Asn1BufLocDescr that
 *                   is added will have the same offset and will have numocts
 *                   determined by this offset and pctxt's current buffer
 *                   position.
 */
EXTERNRT void rtxAddBufLocDescr (OSCTXT *pctxt, OSRTSList* pElemList,
                                  OSRTBufLocDescr* pDescr);

/* Macros */

#define OSRTPUTCHAR(pctxt,ch) rtxWriteBytes (pctxt, (OSOCTET*)&ch, 1)

#define OSRTPUTCHARREV(pctxt,ch) \
(pctxt)->buffer.data[--(pctxt)->buffer.byteIndex]=(OSOCTET)ch;

#define OSRTZTERM(pctxt) \
(pctxt)->buffer.data[(pctxt)->buffer.byteIndex]=(OSOCTET)0;

#define OSRTSAFEZTERM(pctxt) \
do { \
if (rtxCheckOutputBuffer (pctxt, 1) == 0) \
(pctxt)->buffer.data[(pctxt)->buffer.byteIndex]=(OSOCTET)0; \
else return LOG_RTERRNEW (pctxt, RTERR_BUFOVFLW); \
} while (0)

#define OSRTSAFEPUTCHAR(pctxt,ch) \
do { \
if (rtxCheckOutputBuffer (pctxt, 1) == 0) \
(pctxt)->lastChar= \
(pctxt)->buffer.data[(pctxt)->buffer.byteIndex++]=(OSOCTET)ch; \
else return LOG_RTERRNEW (pctxt, RTERR_BUFOVFLW); \
} while (0)

#define OSRTSAFEPUTCHAR1(pctxt,ch,stat) \
do { \
OSOCTET b = (OSOCTET)ch; \
rtxWriteBytes (pctxt, &b, 1); \
} while (0)

#if 0
#define OSRTSAFEPUTCHAR2(pctxt,ch,prealloc) \
do { \
if (rtxCheckOutputBuffer (pctxt, ((prealloc > 1)?prealloc:1)) == 0) \
(pctxt)->lastChar= \
(pctxt)->buffer.data[(pctxt)->buffer.byteIndex++]=(OSOCTET)ch; \
else return LOG_RTERRNEW (pctxt, RTERR_BUFOVFLW); \
} while (0)
#endif

#define OSRTMEMCPY(pctxt,bdata,len) \
do { \
OSCRTLSAFEMEMCPY (&(pctxt)->buffer.data[(pctxt)->buffer.byteIndex], \
      (pctxt)->buffer.size-(pctxt)->buffer.byteIndex, bdata, len); \
(pctxt)->buffer.byteIndex += len; \
(pctxt)->lastChar = (pctxt)->buffer.data[(pctxt)->buffer.byteIndex-1]; \
} while (0)

#define OSRTMEMCPYREV(pctxt,bdata,len) \
do { \
(pctxt)->buffer.byteIndex -= len; \
OSCRTLSAFEMEMCPY (&(pctxt)->buffer.data[(pctxt)->buffer.byteIndex], \
      (pctxt)->buffer.size-(pctxt)->buffer.byteIndex, bdata, len); \
} while (0)

#define OSRTSAFEMEMCPY(pctxt,bdata,len) \
do { \
if (rtxCheckOutputBuffer (pctxt, len) == 0) { \
OSCRTLMEMCPY (&(pctxt)->buffer.data[(pctxt)->buffer.byteIndex], bdata, len); \
(pctxt)->buffer.byteIndex += len; \
(pctxt)->lastChar = (pctxt)->buffer.data[(pctxt)->buffer.byteIndex-1]; } \
else return LOG_RTERRNEW (pctxt, RTERR_BUFOVFLW); \
} while (0)

#define OSRTSAFEMEMCPY1(pctxt,bdata,len,stat) \
do { \
if (rtxCheckOutputBuffer (pctxt, len) == 0) { \
OSCRTLMEMCPY (&(pctxt)->buffer.data[(pctxt)->buffer.byteIndex], bdata, len); \
(pctxt)->buffer.byteIndex += len; \
(pctxt)->lastChar = (pctxt)->buffer.data[(pctxt)->buffer.byteIndex-1]; \
stat = 0; } \
else stat = RTERR_BUFOVFLW; \
} while (0)

#define OSRTGETBUFUTF8LEN(pctxt) \
rtxCalcUTF8Len (OSRTBUFPTR (pctxt), OSRTBUFSIZE (pctxt))

#define OSRTCHKBUFUTF8LEN(pctxt,lower,upper,stat) \
do { size_t nchars = OSRTGETBUFUTF8LEN (pctxt); \
stat = (nchars >= lower && nchars <= upper) ? 0 : RTERR_CONSVIO; } while(0)

#define OSRTENDOFBUF(pctxt) ((pctxt)->buffer.byteIndex >= (pctxt)->buffer.size)

#ifdef __cplusplus
}
#endif

#endif
