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

#include "rtxsrc/rtxBase64.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxMemBuf.h"

/**
 * Return base64/base64url character for given index.
 * @param url if true, return the base64url character; otherwise, return the
 *          base64 character.
 */
static char getBase64Char (int idx, OSBOOL url)
{
   if (idx >= 0 && idx <= 25) return (char)('A' + (char)idx);
   else if (idx > 25 && idx <= 51) return (char)('a' + (char)idx - 26);
   else if (idx > 51 && idx <= 61) return (char)('0' + (char)idx - 52);
   else if (idx == 62) return url ? '-' : '+';
   else if (idx == 63) return url ? '_' : '/';
   else return -1;
}

/**
 * Convert binary data to base64/base64url.
 * @param urlParam: If true, convert to base4url; otherwise to base64.
 */
static long base64EncodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize,
 OSOCTET** ppDstData, OSBOOL url)
{
   size_t i;
   OSRTMEMBUF membuf;

   OSOCTET* pDstData;
   size_t numFullGroups = srcDataSize/3;
   size_t numBytesInPartialGroup = srcDataSize - 3*numFullGroups;
   size_t resultLen = 4*((srcDataSize + 2)/3);

   rtxMemBufInit (pctxt, &membuf, 64);

   pDstData = *ppDstData = rtxMemAllocArray (pctxt, resultLen + 1, OSOCTET);
   if (pDstData == 0) return LOG_RTERRNEW (pctxt, RTERR_NOMEM);

   /* Translate all full groups from byte array elements to Base64 */
   for (i = 0; i < numFullGroups; i++) {
       int byte0 = *pSrcData++ & 0xff;
       int byte1 = *pSrcData++ & 0xff;
       int byte2 = *pSrcData++ & 0xff;
       *pDstData++ = getBase64Char (byte0 >> 2, url);
       *pDstData++ = getBase64Char (((byte0 << 4) & 0x3f) | (byte1 >> 4), url);
       *pDstData++ = getBase64Char (((byte1 << 2) & 0x3f) | (byte2 >> 6), url);
       *pDstData++ = getBase64Char (byte2 & 0x3f, url);
   }

   /* Translate partial group if present */
   if (numBytesInPartialGroup != 0) {
       int byte0 = *pSrcData++ & 0xff;
       *pDstData++ = getBase64Char (byte0 >> 2, url);
       if (numBytesInPartialGroup == 1) {
          *pDstData++ = getBase64Char ((byte0 << 4) & 0x3f, url);
          *pDstData++ = '=';
          *pDstData++ = '=';
       } else {
          /* assert numBytesInPartialGroup == 2; */
          int byte1 = *pSrcData++ & 0xff;
          *pDstData++ = getBase64Char (((byte0 << 4) & 0x3f) | (byte1 >> 4),
                                       url);
          *pDstData++ = getBase64Char ((byte1 << 2) & 0x3f, url);
          *pDstData++ = '=';
       }
   }
   *pDstData = '\0';

   return (long)resultLen;
}

EXTRTMETHOD long rtxBase64EncodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData)
{
   return base64EncodeData (pctxt, pSrcData, srcDataSize, ppDstData, FALSE);
}

EXTRTMETHOD long rtxBase64UrlEncodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData)
{
   return base64EncodeData (pctxt, pSrcData, srcDataSize, ppDstData, TRUE);
}

EXTRTMETHOD long rtxBase64EncodeURLParam
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData)
{
   OSOCTET* pBase64Str;
   int plusCount = 0;
   long ret = base64EncodeData (pctxt, pSrcData, srcDataSize, ppDstData, FALSE);
   if (ret <= 0) return ret;

   /* Scan returned string for '+' characters.  These need to be escaped
      by converting to %2B sequences. */
   pBase64Str = *ppDstData;
   if (pBase64Str != 0) {
      while (*pBase64Str != '\0') {
         if (*pBase64Str++ == '+') plusCount++;
      }
   }

   if (plusCount > 0) {
      size_t newSize = (plusCount * 2) + ret;
      OSOCTET* pSrcData2;
      OSOCTET* pDstData = rtxMemAllocArray (pctxt, newSize + 1, OSOCTET);
      if (pDstData == 0) return LOG_RTERRNEW (pctxt, RTERR_NOMEM);

      pBase64Str = pSrcData2 = *ppDstData;
      *ppDstData = pDstData;

      while (*pBase64Str != '\0') {
         if (*pBase64Str == '+') {
            *pDstData++ = '%';
            *pDstData++ = '2';
            *pDstData++ = 'B';
         }
         else {
            *pDstData++ = *pBase64Str;
         }
         pBase64Str++;
      }

      *pDstData = '\0';
      rtxMemFreePtr (pctxt, pSrcData2);
      ret = (long) newSize;
   }

   return ret;
}

/* Decode */

/*
 * This array is a lookup table that translates characters
 * drawn from the "Base64 Alphabet" (as specified in Table 1 of RFC 2045)
 * into their 6-bit positive integer equivalents. Characters that
 * are not in the Base64 alphabet but fall within the bounds of the
 * array are translated to -1. Note, first 40 values are omitted, because
 * all of them are -1. Use offset -40 to fetch values from this table.
 */
static const signed char decodeTable[] = {
   /*
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, */
   -1, -1, -1, 62, -1, -1, -1, 63,
   52, 53, 54, 55, 56, 57, 58, 59,
   60, 61, -1, -1, -1, -1, -1, -1,
   -1,  0,  1,  2,  3,  4,  5,  6,
   7,   8,  9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22,
   23, 24, 25, -1, -1, -1, -1, -1,
   -1, 26, 27, 28, 29, 30, 31, 32,
   33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48,
   49, 50, 51, -1, -1, -1, -1, -1
};

/*
 * This array is a lookup table that translates characters
 * drawn from the "URL and Filename safe" Base 64 Alphabet (as specified in
 * Table 2 of RFC 4648)
 * into their 6-bit positive integer equivalents. Characters that
 * are not in the Base64 alphabet but fall within the bounds of the
 * array are translated to -1. Note, first 40 values are omitted, because
 * all of them are -1. Use offset -40 to fetch values from this table.
 */
static const signed char decodeTableUrl[] = {
   /*
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, */
   -1, -1, -1, -1, -1, 62, -1, -1,
   52, 53, 54, 55, 56, 57, 58, 59,
   60, 61, -1, -1, -1, -1, -1, -1,
   -1,  0,  1,  2,  3,  4,  5,  6,
   7,   8,  9, 10, 11, 12, 13, 14,
   15, 16, 17, 18, 19, 20, 21, 22,
   23, 24, 25, -1, -1, -1, -1, 63,
   -1, 26, 27, 28, 29, 30, 31, 32,
   33, 34, 35, 36, 37, 38, 39, 40,
   41, 42, 43, 44, 45, 46, 47, 48,
   49, 50, 51, -1, -1, -1, -1, -1
};

#define BASE64TOINT(c) (((((OSOCTET)c) >= 40) && (((OSOCTET)c) < 128)) ? decodeTable [(c) - 40] : -1)

#define BASE64URLTOINT(c) (((((OSOCTET)c) >= 40) && (((OSOCTET)c) < 128)) ? decodeTableUrl [(c) - 40] : -1)

/*
 * Convert base64 or base64url string to binary.
 * This will treat URL escape sequence for '+' (%2B) as a '+' character if
 * url is FALSE (base64url does not use the '+' character).
 * @param url If true, process input as base64url otherwise as base64.
 */
static long base64ToBin
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET* pvalue,
   OSBOOL url)
{
   size_t i, j = 0;
   int ch[4]; /* group buffer */
   OSOCTET* bufp = pvalue;

   for (i = 0; i < srcDataSize; ) {
      if (OS_ISSPACE (pSrcData[i])) {
         i++;
      }
      else if (pSrcData[i] != '=') {
         /* Check for URL escaped + character */
         if (pSrcData[i] == '%' && pSrcData[i+1] == '2' &&
             pSrcData[i+2] == 'B' && !url) {
            ch[j] = BASE64TOINT ('+');
            i += 2;
         }
         else {
            ch[j] = url ? BASE64URLTOINT (pSrcData[i]) : 
                                                      BASE64TOINT (pSrcData[i]);
         }

         if (ch[j] < 0) {
            return (0 != pctxt) ?
               LOG_RTERRNEW (pctxt, RTERR_INVBASE64) : RTERR_INVBASE64;
         }
         else if (++j == 4) {
            /* Group is complete */
            *pvalue++ = (OSOCTET) ((ch[0] << 2) | (ch[1] >> 4));
            *pvalue++ = (OSOCTET) ((ch[1] << 4) | (ch[2] >> 2));
            *pvalue++ = (OSOCTET) ((ch[2] << 6) |  ch[3]);
            j = 0;
         }
         i++;
      }
      else { /* partial group */
         if (j < 2 || j > 3) {
            return (0 != pctxt) ?
               LOG_RTERRNEW (pctxt, RTERR_INVBASE64) : RTERR_INVBASE64;
         }
         *pvalue++ = (OSOCTET) ((ch[0] << 2) | (ch[1] >> 4));
         if (j == 3) {
            *pvalue++ = (OSOCTET) ((ch[1] << 4) | (ch[2] >> 2));
         }
         while (pSrcData[i] == '=') i++;
         j = 0;
      }
   }

   return (long)(pvalue - bufp);
}

static long getBinDataLen (const char* pSrcData, size_t srcDataSize)
{
   size_t i, j = 0;
   long nbytes = 0;
   for (i = 0; i < srcDataSize; ) {
      if (OS_ISSPACE (pSrcData[i])) {
         i++;
      }
      else if (pSrcData[i] != '=') {
         if (++j == 4) {
            nbytes += 3;
            j = 0;
         }
         /* check for escaped + character */
         if (pSrcData[i] == '%' && pSrcData[i+1] == '2' && pSrcData[i+2] == 'B')
            i += 3;
         else
            i++;
      }
      else { /* partial group */
         if (j == 2) nbytes += 1;
         else if (j == 3) nbytes += 2;
         else return RTERR_INVBASE64;
         while (pSrcData[i] == '=') i++;
         j = 0;
      }
   }
   return nbytes;
}

/**
 * Decode base64/base64url data to binary.
 * @param url If true, decode base64url; otherwise, decode base64.
 */
static long base64DecodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData,
   OSBOOL url)
{
   size_t bufsiz;
   OSOCTET* pvalue;
   long stat = getBinDataLen (pSrcData, srcDataSize);

   if (stat < 0) return LOG_RTERRNEW (pctxt, (int)stat);
   else bufsiz = (size_t) stat;

   pvalue = *ppDstData = rtxMemAllocArray (pctxt, bufsiz, OSOCTET);
   if (pvalue == 0) return LOG_RTERRNEW (pctxt, RTERR_NOMEM);

   stat = base64ToBin (pctxt, pSrcData, srcDataSize, pvalue, url);

   return stat;
}

EXTRTMETHOD long rtxBase64DecodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData)
{
   return base64DecodeData(pctxt, pSrcData, srcDataSize, ppDstData, FALSE);
}

EXTRTMETHOD long rtxBase64UrlDecodeData
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize, OSOCTET** ppDstData)
{
   return base64DecodeData(pctxt, pSrcData, srcDataSize, ppDstData, TRUE);
}

/**
 * Decode base64/base64url data to binary.
 * @param url If true, decode base64url; otherwise, decode base64.
 */
static long base64DecodeDataToFSB
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize,
 OSOCTET* buf, size_t bufsiz, OSBOOL url)
{
   size_t nbytes;
   long stat = getBinDataLen (pSrcData, srcDataSize);

   if (stat < 0)
      return (0 != pctxt) ? LOG_RTERRNEW (pctxt, (int)stat) : (int)stat;
   else
      nbytes = (size_t) stat;

   if (nbytes > bufsiz)
      return (0 != pctxt) ?
         LOG_RTERRNEW (pctxt, RTERR_BUFOVFLW) : RTERR_BUFOVFLW;

   return base64ToBin (pctxt, pSrcData, srcDataSize, buf, url);
}

EXTRTMETHOD long rtxBase64DecodeDataToFSB
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize,
 OSOCTET* buf, size_t bufsiz)
{
   return base64DecodeDataToFSB(pctxt, pSrcData, srcDataSize, buf, bufsiz,
                                 FALSE);
}

EXTRTMETHOD long rtxBase64UrlDecodeDataToFSB
(OSCTXT* pctxt, const char* pSrcData, size_t srcDataSize,
 OSOCTET* buf, size_t bufsiz)
{
   return base64DecodeDataToFSB(pctxt, pSrcData, srcDataSize, buf, bufsiz,
                                 TRUE);
}

EXTRTMETHOD long rtxBase64GetBinDataLen
(const char* pSrcData, size_t srcDataSize)
{
   return getBinDataLen (pSrcData, srcDataSize);
}
