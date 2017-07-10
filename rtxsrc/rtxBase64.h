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
 * @file rtxBase64.h
 *
 * base64 and base64url are defined in RFC 4648.
 */
#ifndef _RTXBASE64_H_
#define _RTXBASE64_H_

#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encode binary data into base64 string form to a dynamic buffer.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to binary data to encode.
 * @param srcDataSize  Length of the binary data in octets.
 * @param ppDstData    Pointer to pointer variable to hold address of
 *                       dynamically allocated buffer the encoded base64
 *                       string.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64EncodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData);

/**
 * Encode binary data into base64 string form to a dynamic buffer, converting
 * '+' characters to the URL escape sequence %2B so that the encoded string
 * may be used in a query string parameter in a URL.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to binary data to encode.
 * @param srcDataSize  Length of the binary data in octets.
 * @param ppDstData    Pointer to pointer variable to hold address of
 *                       dynamically allocated buffer the encoded base64
 *                       string.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64EncodeURLParam
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData);

/**
 * Decode base64 string to binary form into a dynamic buffer.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to base64 string to decode.
 * @param srcDataSize  Length of the base64 string.
 * @param ppDstData    Pointer to pointer variable to hold address of
 *                       dynamically allocated buffer to hold data.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64DecodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData);

/**
 * Decode base64 string to binary form into a fixed-size buffer.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to base64 string to decode.
 * @param srcDataSize  Length of the base64 string.
 * @param buf          Address of buffer to receive decoded binary data.
 * @param bufsiz       Size of output buffer.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64DecodeDataToFSB
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize,
 OSOCTET* buf, size_t bufsiz);

/**
 * Calculate number of byte required to hold a decoded base64/base64url string
 * in binary form.
 *
 * @param pSrcData     Pointer to base64/base64url string.
 * @param srcDataSize  Length of the base64/base64url string.
 * @return             Completion status of operation:
 *                       If success, positive value is number of bytes,
 *                       If failure, negative status code.
 */
EXTERNRT long rtxBase64GetBinDataLen
(const char* pSrcData, size_t srcDataSize);


/**
 * Encode binary data into base64url string form to a dynamic buffer.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to binary data to encode.
 * @param srcDataSize  Length of the binary data in octets.
 * @param ppDstData    Pointer to pointer variable to hold address of
 *                       dynamically allocated buffer the encoded base64
 *                       string.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64UrlEncodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData);

/**
 * Decode base64url string to binary form into a dynamic buffer.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to base64 string to decode.
 * @param srcDataSize  Length of the base64 string.
 * @param ppDstData    Pointer to pointer variable to hold address of
 *                       dynamically allocated buffer to hold data.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64UrlDecodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData);

/**
 * Decode base64url string to binary form into a fixed-size buffer.
 *
 * @param pctxt        Pointer to context structure.
 * @param pSrcData     Pointer to base64 string to decode.
 * @param srcDataSize  Length of the base64 string.
 * @param buf          Address of buffer to receive decoded binary data.
 * @param bufsiz       Size of output buffer.
 * @return             Completion status of operation:
 *                       - number of binary bytes written
 *                       - negative return value is error.
 */
EXTERNRT long rtxBase64UrlDecodeDataToFSB
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize,
 OSOCTET* buf, size_t bufsiz);

#ifdef __cplusplus
}
#endif

#endif /* RTXBASE64 */

