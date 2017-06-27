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

#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxUTF8.h"

/* UTF-8 validation and conversion functions */
#if !defined(_ARMTCC) && !defined(__SYMBIAN32__)
static const OSUINT32 encoding_mask[] = {
   0xfffff800, 0xffff0000, 0xffe00000, 0xfc000000
};

static const unsigned char encoding_byte[] = {
   0xc0, 0xe0, 0xf0, 0xf8, 0xfc
};
#endif

/***********************************************************************
 *
 *  Routine name: rtxUTF8ToUnicode
 *
 *  Description:  This routine converts a UTF-8 encoded byte stream
 *                into a Unicode string.
 *
 *  Inputs:
 *
 *  Name        Type            Description
 *  ----        ------------    -----------
 *  pctxt       struct	        Pointer to context block structure
 *  inbuf       OSUTF8CHAR*     String of UTF-8 encoded characters
 *  outbufsiz   int             Number of Unicode characters the output
 *                                buffer can hold.
 *
 *  Outputs:
 *
 *  Name        Type            Description
 *  ----        ------------    -----------
 *  stat        int             Character count or error status.
 *  outbuf      OSUNICHAR*      Output buffer
 *
 ***********************************************************************/

long rtxUTF8ToUnicode
(OSCTXT* pctxt, const OSUTF8CHAR* inbuf, OSUNICHAR* outbuf, size_t outbufsiz)
{
   size_t inlen = rtxUTF8LenBytes (inbuf);
   size_t inbufx = 0, outbufx = 0;
   OSUNICHAR value;
   size_t startx;
   int nbytes, result = 0;
   OSOCTET byte;

   while (inbufx < inlen) {
      startx = inbufx;

      /* Make sure there is enough space in the output buffer   */
      /* available to hold the parsed character..               */

      if (0 != outbuf) {
         if (outbufx >= outbufsiz) {
            return LOG_RTERRNEW (pctxt, RTERR_BUFOVFLW);
         }
      }

      /* Process the first byte of the UTF-8 sequence */

      byte = inbuf[inbufx++];
      if (byte < 0x80) {
         nbytes = 0;
         value = byte;
      }
      else if ((byte & 0xe0) == 0xc0) {
         nbytes = 1;
         value = (OSUNICHAR) (byte & 0x1f);
      }
      else if ((byte & 0xf0) == 0xe0) {
         nbytes = 2;
         value = (OSUNICHAR) (byte & 0x0f);
      }
      else if ((byte & 0xf8) == 0xf0) {
         nbytes = 3;
         value = (OSUNICHAR) (byte & 0x07);
      }
      else if ((byte & 0xfc) == 0xf8) {
         nbytes = 4;
         value = (OSUNICHAR) (byte & 0x03);
      }
      else if ((byte & 0xfe) == 0xfc) {
         nbytes = 5;
         value = (OSUNICHAR) (byte & 0x01);
      }
      else {
         rtxErrNewNode (pctxt);
         rtxErrAddUIntParm (pctxt, (OSUINT32)startx);
         return LOG_RTERRNEW (pctxt, RTERR_INVUTF8);
      }

      /* Read the possible remaining bytes.  */

      while (inbufx < inlen && nbytes > 0) {
         byte = inbuf[inbufx++];
         --nbytes;
         if ((byte & 0xc0) != 0x80) {
            /* This is an illegal encoding.  */
            result = RTERR_INVUTF8;
            break;
         }
         value <<= 6;
         value |= byte & 0x3f;
      }

      if (result != 0) {
         rtxErrNewNode (pctxt);
         rtxErrAddUIntParm (pctxt, (OSUINT32)startx);
         return LOG_RTERRNEW (pctxt, result);
      }

      if (0 != outbuf) {
         outbuf[outbufx++] = value;
      }
   }

   return (long)(outbufx);
}

/***********************************************************************
 *
 *  Routine name: rtxValidateUTF8
 *
 *  Description:  This routine validates a UTF-8 encoded byte stream.
 *
 *  Inputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *  pctxt 	struct	Pointer to context block structure
 *  inbuf       char*   String of UTF-8 encoded characters
 *
 *  Outputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *  stat        int     Status of validation.  Will be 0 (zero)
 *                      if validation successful or a negative status
 *                      value if an error is detected.
 *
 ***********************************************************************/

int rtxValidateUTF8 (OSCTXT* pctxt, const OSUTF8CHAR* inbuf)
{
   return rtxUTF8ToUnicode (pctxt, inbuf, 0, 0);
}

/***********************************************************************
 *
 *  Routine name: rtxUTF8Len
 *
 *  Description:  This routine calculates a length of UTF-8 string
 *  (in characters).
 *
 *  Inputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *  pctxt       struct	Pointer to context block structure
 *  inbuf       char*   String of UTF-8 encoded characters
 *
 *  Outputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *  len         int     If positive, indicates number of chars. Will be
 *                      negative if function fails.
 *
 *
 ***********************************************************************/

size_t rtxCalcUTF8Len (const OSUTF8CHAR* inbuf, size_t inbufBytes)
{
   size_t inbufx = 0;
   size_t /* startx, */ nbytes, nchars = 0;
   OSOCTET byte;

   while (inbufx < inbufBytes) {
      /*  startx = inbufx; */

      /* Process the first byte of the UTF-8 sequence */

      byte = inbuf[inbufx++];
      if (byte < 0x80) {
         nbytes = 0;
      }
      else if ((byte & 0xe0) == 0xc0) {
         nbytes = 1;
      }
      else if ((byte & 0xf0) == 0xe0) {
         nbytes = 2;
      }
      else if ((byte & 0xf8) == 0xf0) {
         nbytes = 3;
      }
      else if ((byte & 0xfc) == 0xf8) {
         nbytes = 4;
      }
      else if ((byte & 0xfe) == 0xfc) {
         nbytes = 5;
      }
      else {
         return 0; /*RTERR_INVUTF8;*/  /* invalid encoding error */
      }

      inbufx += nbytes;
      nchars ++;
   }

   return (nchars);
}

size_t rtxUTF8Len (const OSUTF8CHAR* inbuf)
{
   return rtxCalcUTF8Len (inbuf, rtxUTF8LenBytes (inbuf));
}

/***********************************************************************
 *
 *  Routine name: rtxUTF8LenBytes
 *
 *  Description:  This routine calculates a length of UTF-8 string
 *  (in bytes).
 *
 *  Inputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *  pctxt       struct	Pointer to context block structure
 *  inbuf       char*   String of UTF-8 encoded characters
 *
 *  Outputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *  len         int     If positive, indicates number of chars. Will be
 *                      negative if function fails.
 *
 *
 ***********************************************************************/

size_t rtxUTF8LenBytes (const OSUTF8CHAR* inbuf)
{
   size_t len = 0;

   if (0 != inbuf) {
      while (0 != inbuf[len]) len++;
   }

   return len;
}

/***********************************************************************
 *
 *  Routine name: rtxUTF8CharSize
 *
 *  Description:  This routine calculates a size of character being
 *  encoded to UTF-8.
 *
 *  Inputs:
 *
 *  Name        Type           Description
 *  ----        ----           -----------
 *  wc          OS32BITCHAR    Wide-character to length's calculate.
 *
 *  Outputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *  len         int     If positive, indicates the size of char in
 *                      bytes. Will be negative if function fails.
 *
 *
 ***********************************************************************/

int rtxUTF8CharSize (OS32BITCHAR wc)
{
   if (wc >= 0x80) {
#if defined(_ARMTCC) || defined(__SYMBIAN32__)
      OSUINT32 encoding_mask[] = {
         0xfffff800, 0xffff0000, 0xffe00000, 0xfc000000
      };
#endif
      int sz;

      for (sz = 2; sz < 6; ++sz)
         if ((wc & encoding_mask[sz - 2]) == 0)
            break;
      return sz;
   }
   /* One byte sequence */

   return 1;
}

/***********************************************************************
 *
 *  Routine name: rtxUTF8EncodeChar
 *
 *  Description:  This routine encodes a wide character into UTF-8
 *  format.
 *
 *  Inputs:
 *
 *  Name        Type           Description
 *  ----        ----           -----------
 *  wc          OS32BITCHAR    Wide-character to length's calculate.
 *
 *  Outputs:
 *
 *  Name        Type    Description
 *  ----        ----    -----------
 *  buf         byte*   Buffer into which encoded char will be written.
 *  len         int     If positive, indicates the size of char in
 *                      bytes. Will be negative if function fails.
 *
 *
 ***********************************************************************/

int rtxUTF8EncodeChar (OS32BITCHAR wc, OSOCTET* buf, size_t bufsiz)
{
   size_t nbytes = 0;

   if (wc < 0x80) {
      char c = (char) wc;
      /* One byte sequence */
      buf[0] = c;
      return 1;
   }
   else {
#if defined(_ARMTCC) || defined(__SYMBIAN32__)
      OSUINT32 encoding_mask[] = {
         0xfffff800, 0xffff0000, 0xffe00000, 0xfc000000
      };
      unsigned char encoding_byte[] = {
         0xc0, 0xe0, 0xf0, 0xf8, 0xfc
      };
#endif
      size_t step;

      for (step = 2; step < 6; ++step)
         if ((wc & encoding_mask[step - 2]) == 0)
            break;

      nbytes = step;

      if (step > bufsiz)
         return RTERR_BUFOVFLW;

      /* Copy multi-byte sequence to output buffer */
      buf[0] = encoding_byte[step - 2];
      --step;
      do {
         buf[step] = (OSOCTET) (0x80 | (wc & 0x3f));
         wc >>= 6;
      }
      while (--step > 0);
      buf[0] |= wc;
   }

   return (int)nbytes;
}

int rtxUTF8DecodeChar
(OSCTXT* pctxt, const OSUTF8CHAR* pinbuf, int* pInsize)
{
   int inbufx = 0;
   int value, insize;
   int nbytes, result = 0;
   OSUTF8CHAR byte;

   if (pinbuf == 0 || pInsize == 0)
      return (pctxt != 0) ? LOG_RTERRNEW (pctxt, RTERR_INVUTF8) :
         RTERR_INVUTF8;

   insize = *pInsize;

   /* Process the first byte of the UTF-8 sequence */

   byte = pinbuf[inbufx++];
   if (byte < 0x80) {
      nbytes = 0;
      value = byte;
   }
   else if ((byte & 0xe0) == 0xc0) {
      nbytes = 1;
      value = (byte & 0x1f);
   }
   else if ((byte & 0xf0) == 0xe0) {
      nbytes = 2;
      value = (byte & 0x0f);
   }
   else if ((byte & 0xf8) == 0xf0) {
      nbytes = 3;
      value = (byte & 0x07);
   }
   else if ((byte & 0xfc) == 0xf8) {
      nbytes = 4;
      value = (byte & 0x03);
   }
   else if ((byte & 0xfe) == 0xfc) {
      nbytes = 5;
      value = (byte & 0x01);
   }
   else {
      if (pctxt != 0)  {
         rtxErrNewNode (pctxt);
         rtxErrAddUIntParm (pctxt, 0);
         return LOG_RTERRNEW (pctxt, RTERR_INVUTF8);
      }
      return RTERR_INVUTF8;  /* invalid encoding error */
   }

   /* Read the possible remaining bytes.  */

   while (inbufx < insize && nbytes > 0) {
      byte = pinbuf[inbufx++];
      --nbytes;
      if ((byte & 0xc0) != 0x80) {
         /* This is an illegal encoding.  */
         result = RTERR_INVUTF8;
         break;
      }
      value <<= 6;
      value |= byte & 0x3f;
   }

   if (nbytes != 0)
      result = RTERR_INVUTF8;

   if (result != 0) {
#ifndef _COMPACT
      if (pctxt != 0) {
         rtxErrNewNode (pctxt);
         rtxErrAddUIntParm (pctxt, 0);
         return LOG_RTERRNEW (pctxt, result);
      }
#endif
      return (result);
   }

   *pInsize = inbufx;

   return (value);
}
/***********************************************************************
 *
 *  Routine name: rtxUTF8CharToWC
 *
 *  Description:  This routine converts an UTF-8 character sequence
 *  to a wide character.
 *
 *  Inputs:
 *
 *  Name        Type           Description
 *  ----        ----           -----------
 *  buf         OSUTF8CHAR*    Start of UTF8 char sequence.
 *
 *  Outputs:
 *
 *  Name        Type           Description
 *  ----        ----           -----------
 *  wc          OS32BITCHAR    Converted wide-character returned as
 *                             function result or -1 (0xffffffff) if
 *                             invalid UTF-8.
 *  len         OSUINT32*      Number of bytes in UTF8 character.
 *
 ***********************************************************************/

OS32BITCHAR rtxUTF8CharToWC (const OSUTF8CHAR* buf, OSUINT32* len)
{
   OS32BITCHAR value;
   OSOCTET  byte;
   OSUINT32 nbytes;
   OSUINT32 inbufx = *len = 0;

   /* Process the first byte of the UTF-8 sequence */

   byte = buf[inbufx++];
   if (byte < 0x80) {
      nbytes = 0;
      value = byte;
   }
   else if ((byte & 0xe0) == 0xc0) {
      nbytes = 1;
      value = (OS32BITCHAR) (byte & 0x1f);
   }
   else if ((byte & 0xf0) == 0xe0) {
      nbytes = 2;
      value = (OS32BITCHAR) (byte & 0x0f);
   }
   else if ((byte & 0xf8) == 0xf0) {
      nbytes = 3;
      value = (OS32BITCHAR) (byte & 0x07);
   }
   else if ((byte & 0xfc) == 0xf8) {
      nbytes = 4;
      value = (OS32BITCHAR) (byte & 0x03);
   }
   else if ((byte & 0xfe) == 0xfc) {
      nbytes = 5;
      value = (OS32BITCHAR) (byte & 0x01);
   }
   else
      return ((OS32BITCHAR)-1);

   /* Read the possible remaining bytes */

   while (nbytes > 0) {
      byte = buf[inbufx++];
      --nbytes;
      if ((byte & 0xc0) != 0x80) {
         /* This is an illegal encoding.  */
         return ((OS32BITCHAR)-1);
      }
      value <<= 6;
      value |= byte & 0x3f;
   }

   *len = inbufx;
   return (value);
}

/***********************************************************************
 *
 *  Routine name: rtxUTF8Strdup
 *
 *  Description:  This routine create a duplicate copy of the given
 *  UTF-8 string.  It is similar in functionality to the strdup C
 *  run-time function except that it uses OSys memory management.
 *
 *  Inputs:
 *
 *  Name        Type           Description
 *  ----        ----           -----------
 *  pctxt       OSCTXT*        Pointer to context block
 *  utf8str     OSUTF8CHAR*    UTF-8 string to duplicate
 *
 *  Outputs:
 *
 *  Name        Type           Description
 *  ----        ----           -----------
 *  outstr      OSUTF8CHAR*    The duplicate copy of the string returned
 *                             as function result.  Note that memory
 *                             allocated is managed within the OSys
 *                             context and should be managed by calling
 *                             rtxMem functions (not C free).
 *
 *
 ***********************************************************************/

OSUTF8CHAR* rtxUTF8Strdup
(OSCTXT* pctxt, const OSUTF8CHAR* utf8str)
{
   if (0 != utf8str) {
      size_t nbytes = rtxUTF8LenBytes(utf8str) + 1;
      OSUTF8CHAR* pbuf = rtxMemAllocArray (pctxt, nbytes, OSUTF8CHAR);
      if (0 != pbuf) {
         OSCRTLSAFEMEMCPY (pbuf, nbytes, utf8str, nbytes);
      }
      return pbuf;
   }
   else return 0;
}

OSUTF8CHAR* rtxUTF8Strndup
(OSCTXT* pctxt, const OSUTF8CHAR* utf8str, size_t nbytes)
{
   if (utf8str != 0) {
      OSUTF8CHAR* pbuf = rtxMemAllocArray (pctxt, nbytes+1, OSUTF8CHAR);
      if (0 != pbuf) {
         OSCRTLSAFEMEMCPY (pbuf, nbytes, utf8str, nbytes);
         pbuf[nbytes] = 0;
      }
      return pbuf;
   }
   else return 0;
}

/***********************************************************************
 *
 *  Routine name: rtxUTF8StrEqual
 *
 *  Description:  This routine compares two UTF8 strings for equality.
 *  Returns true if the strings are equal or false otherwise.
 *
 *  Inputs:
 *
 *  Name        Type           Description
 *  ----        ----           -----------
 *  utf8str1    OSUTF8CHAR*    UTF-8 string 1 to compare
 *  utf8str2    OSUTF8CHAR*    UTF-8 string 2 to compare
 *
 *  Outputs:
 *
 *  Name        Type           Description
 *  ----        ----           -----------
 *  result      OSBOOL         Boolean comparison result.
 *
 ***********************************************************************/

OSBOOL rtxUTF8StrEqual
(const OSUTF8CHAR* utf8str1, const OSUTF8CHAR* utf8str2)
{
   if (0 != utf8str1 && 0 != utf8str2) {
      OSUINT32 i = 0;
      while (utf8str1[i] != 0) {
         if (utf8str1[i] != utf8str2[i]) return FALSE;
         else i++;
      }
      return (OSBOOL)(0 == utf8str2[i]);
   }
   return FALSE;
}

OSBOOL rtxUTF8StrnEqual
(const OSUTF8CHAR* utf8str1, const OSUTF8CHAR* utf8str2, size_t count)
{
   if (!count)
      return(0);
   if (0 != utf8str1 && 0 != utf8str2) {
      OSUINT32 i = 0;
      while (--count && utf8str1[i] != 0 && utf8str1[i] == utf8str2[i]) {
         i++;
      }
      return (OSBOOL)(utf8str1[i] == utf8str2[i]);
   }
   return FALSE;
}

int rtxUTF8Strcmp
(const OSUTF8CHAR* utf8str1, const OSUTF8CHAR* utf8str2)
{
   int ret = 0 ;

   if (utf8str1 == 0 && utf8str2 == 0)
      return(0);
   if (0 == utf8str1)
      return -1;
   else if (0 == utf8str2)
      return 1;

   while ((ret = *utf8str1 - *utf8str2) == 0 && *utf8str2) {
      ++utf8str1;
      ++utf8str2;
   }

   if (ret < 0)
      ret = -1 ;
   else if (ret > 0)
      ret = 1 ;

   return ret;
}

int rtxUTF8Strncmp
(const OSUTF8CHAR* utf8str1, const OSUTF8CHAR* utf8str2, size_t count)
{
   OSUINT32 i = 0;

   if (!count || (utf8str1 == 0 && utf8str2 == 0))
      return(0);
   if (0 == utf8str1)
      return -1;
   else if (0 == utf8str2)
      return 1;

   while (--count && utf8str1[i] != 0 && utf8str1[i] == utf8str2[i]) {
      i++;
   }
   return (utf8str1[i] - utf8str2[i]);
}

OSUTF8CHAR* rtxUTF8StrChr
(OSUTF8CHAR* utf8str, OS32BITCHAR utf8char)
{
   if (0 != utf8str) {
      size_t i, nchars = rtxUTF8Len (utf8str);
      int ch;

      for (i = 0; i < nchars; i++) {
         int sz = INT_MAX;

         ch = rtxUTF8DecodeChar (0, utf8str, &sz);

         if (ch < 0)
            return 0;
         else if (ch == (int)utf8char)
            return utf8str;

         utf8str += sz;
      }
   }
   return 0;
}

/*
 * Remove leading and trailing whitespace from a string.
 */
int rtxUTF8RemoveWhiteSpace
(const OSUTF8CHAR* utf8instr, size_t nbytes, const OSUTF8CHAR** putf8outstr)
{
   size_t i;

   if (0 == utf8instr) return RTERR_INVPARAM;

   /* Consume leading white space */

   for (i = 0; i < nbytes; i++) {
      if (!OS_ISSPACE (utf8instr[i])) break;
   }

   /* Consume trailing white space */

   if (i < nbytes) {
      for (; nbytes > 0; nbytes--) {
         if (!OS_ISSPACE (utf8instr[nbytes - 1])) break;
      }
   }

   *putf8outstr = utf8instr + i;

   OSRTASSERT (nbytes >= i);

   return (int)(nbytes - i);
}

/*
 * Parse next token from string
 */
const OSUTF8CHAR* rtxUTF8StrNextTok
(OSUTF8CHAR* utf8str, OSUTF8CHAR** ppNext)
{
   OSUTF8CHAR* sp = utf8str;
   const OSUTF8CHAR* ptoken;

   /* Check for NULL input string argument */
   if (0 == sp) return 0;

   /* Consume leading white space */
   while (OS_ISSPACE(*sp)) sp++;

   /* Set token pointer */
   ptoken = (*sp == '\0') ? 0 : sp;

   /* Look for end of token and insert null terminator */
   while (!OS_ISSPACE(*sp)) sp++;
   *sp++ = '\0';
   *ppNext = sp;

   return ptoken;
}

OSUTF8CHAR* rtxUTF8Strcpy
(OSUTF8CHAR* dest, size_t bufsiz, const OSUTF8CHAR* src)
{
   if (0 == dest || 0 == bufsiz || 0 == src)
      return (OSUTF8CHAR*)0;
   else
      return rtxUTF8Strncpy (dest, bufsiz, src, rtxUTF8LenBytes(src));
}

OSUTF8CHAR* rtxUTF8Strncpy
(OSUTF8CHAR* dest, size_t bufsiz, const OSUTF8CHAR* src, size_t nchars)
{
   size_t i;

   if (0 == dest || bufsiz <= nchars || 0 == src)
      return (OSUTF8CHAR*)0;

   for (i = 0; i < nchars; i++) {
      dest[i] = src[i];
   }
   dest[i] = '\0';

   return dest;
}

OSUINT32 rtxUTF8StrHash (const OSUTF8CHAR* str)
{
   OSUINT32 hash = 5381;
   int c;

   while ((c = (int)*str++) != 0)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

   return hash;
}

const OSUTF8CHAR* rtxUTF8StrJoin
(OSCTXT* pctxt, const OSUTF8CHAR* str1, const OSUTF8CHAR* str2,
 const OSUTF8CHAR* str3, const OSUTF8CHAR* str4, const OSUTF8CHAR* str5)
{
   OSUTF8CHAR* targetStr;
   OSUINT32 i;
   size_t totalLen = 0, idx = 0;
   const OSUTF8CHAR* parts[5];

   parts[0] = str1; parts[1] = str2; parts[2] = str3;
   parts[3] = str4, parts[4] = str5;

   for (i = 0; i < 5; i++) {
      if (0 != parts[i]) totalLen += rtxUTF8LenBytes (parts[i]);
   }
   if (totalLen == 0) return 0; else totalLen++;

   targetStr = (OSUTF8CHAR*) rtxMemAlloc (pctxt, totalLen);
   if (0 == targetStr) return 0;

   for (i = 0; i < 5; i++) {
      if (0 != parts[i]) {
         rtxUTF8Strcpy (&targetStr[idx], totalLen - idx, parts[i]);
         idx += rtxUTF8LenBytes (parts[i]);
      }
   }

   targetStr[idx] = '\0';

   return targetStr;
}

