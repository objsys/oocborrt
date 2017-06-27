/**
 * Copyright (c) 1997-2017 by Objective Systems, Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxFile.h"
#include "rtxsrc/rtxMemory.h"

EXTRTMETHOD int rtxFileOpen
(FILE** ppFile, const char* filePath, const char* access)
{
   int ret = 0;

   if (0 != ppFile) {
#if defined(_MSC_VER) && _MSC_VER >= 1400 && !defined(_WIN32_WCE)
      errno_t err = fopen_s (ppFile, filePath, access);
      if (0 != err) {
         printf ("errno returned from fopen_s is %d\n", err);
         *ppFile = 0;
         ret = (err == ENOENT) ? RTERR_FILNOTFOU : RTERR_FAILED;
      }
#else
      *ppFile = fopen (filePath, access);
      ret = (0 == *ppFile) ? RTERR_FAILED : 0;
#endif
   }
   else ret = RTERR_NULLPTR;

   return ret;
}

static int readOpenFile (OSCTXT* pctxt, FILE* fp,
                         OSOCTET** ppMsgBuf, size_t* pLength, char fileType)
{
   OSOCTET* msgbuf;
   long len;
   size_t len2;

   fseek (fp, 0L, SEEK_END);
   if ((len = ftell(fp)) != -1L) {
      /* If text file, allocate one more char than length to hold
         null terminator char */
      size_t memsize = len;
      if (fileType == 'a') memsize++;

      msgbuf = (OSOCTET*) rtxMemAlloc (pctxt, memsize);

      if (msgbuf == NULL) {
         return LOG_RTERR (pctxt, RTERR_NOMEM);
      }
      fseek (fp, 0L, SEEK_SET);

      len2 = fread (msgbuf, 1, (size_t)len, fp);

      if ((long)len2 != len && fileType == 'b' ) {
         rtxMemFreeArray (pctxt, msgbuf);
         return LOG_RTERR (pctxt, RTERR_READERR);
      }

      *ppMsgBuf = msgbuf;
      *pLength = len2;

      /* Add null terminator */
      if (fileType == 'a') msgbuf[len2] = '\0';

      return 0;
   }
   else {
      return LOG_RTERR (pctxt, RTERR_READERR);
   }
}

static int readFile (OSCTXT* pctxt, const char* filePath,
                     OSOCTET** ppMsgBuf, size_t* pLength, char fileType)
{
   const char* access = (fileType == 'a') ? "r" : "rb";
   FILE* fp;
   int ret = rtxFileOpen (&fp, filePath, access);
   if (0 == ret) {
      ret = readOpenFile(pctxt, fp, ppMsgBuf, pLength, fileType);
      fclose(fp);
      return ret;
   }
   else {
      return LOG_RTERR (pctxt, RTERR_FILNOTFOU);
   }
}

EXTRTMETHOD int rtxFileReadBinary
(OSCTXT* pctxt, const char* filePath, OSOCTET** ppMsgBuf, size_t* pLength)
{
   return readFile (pctxt, filePath, ppMsgBuf, pLength, 'b');
}

EXTRTMETHOD int rtxFileReadText
(OSCTXT* pctxt, const char* filePath, OSOCTET** ppMsgBuf, size_t* pLength)
{
   return readFile (pctxt, filePath, ppMsgBuf, pLength, 'a');
}

EXTRTMETHOD int rtxFileWriteBinary
(const char* filePath, const OSOCTET* pMsgBuf, size_t length)
{
   FILE* fp;
   int ret = rtxFileOpen (&fp, filePath, "wb");
   if (0 == ret) {
      fwrite (pMsgBuf, 1, length, fp);
      fclose (fp);
   }
   else ret = RTERR_WRITEERR;

   return ret;
}

EXTRTMETHOD int rtxFileWriteText
(const char* filePath, const char* pMsgBuf)
{
   FILE* fp;
   int ret = rtxFileOpen (&fp, filePath, "w");
   if (0 == ret) {
      fputs ((char*)pMsgBuf, fp);
      fclose (fp);
   }
   else ret = RTERR_WRITEERR;

   return ret;
}
