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

/* Run-time utility functions */

#include <stdlib.h>
#include <string.h>
#include "rtxsrc/osMacros.h"
#include "rtxsrc/rtxHexDump.h"
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxErrCodes.h"
#include "rtxsrc/rtxMemory.h"

/* Hex dump function */

int rtxByteToHexChar (OSOCTET byte, char* buf, OSSIZE bufsize)
{
   OSUINT32 tmpval = byte;
   if (bufsize < 3) return RTERR_BUFOVFLW;
   tmpval >>= 4;
   buf[0] = (char)((tmpval <= 9) ? tmpval + '0' : tmpval - 10 + 'a');
   tmpval = byte & 0x0F;  /* shave off MS nibble */
   buf[1] = (char)((tmpval <= 9) ? tmpval + '0' : tmpval - 10 + 'a');
   buf[2] = '\0';
   return 0;
}

int rtxByteToHexCharWithPrefix
(OSOCTET byte, char* buf, OSSIZE bufsize, const char* prefix)
{
   OSSIZE prefixLen = (!OS_ISEMPTY(prefix)) ? OSCRTLSTRLEN(prefix) : 0;
   if (bufsize < prefixLen+3) return RTERR_BUFOVFLW;
   if (prefixLen > 0) rtxStrcpy (buf, bufsize, prefix);
   return rtxByteToHexChar (byte, buf+prefixLen, bufsize-prefixLen);
}

int rtxHexDumpToString
(const OSOCTET* data, OSSIZE numocts, char* buffer,
 OSSIZE bufferIndex, OSSIZE bufferSize)
{
   return rtxHexDumpToStringEx
      (data, numocts, buffer, bufferIndex, bufferSize, 1);
}

int rtxHexDumpToStringEx
(const OSOCTET* data, OSSIZE numocts, char* buffer,
 OSSIZE bufferIndex, OSSIZE bufferSize, OSSIZE bytesPerUnit)
{
   OSSIZE      i, k;
   OSUINT32    val;
   OSSIZE      unitsPerLine = 16 / bytesPerUnit;
   OSOCTET     b, dumpstr = 1;
   char        hexstr[49], ascstr[17], buf[20], *pbuf;
   int         ai, shift;

   if (bufferIndex >= bufferSize) {
      return -1;
   }

   /* Check no of octets to be copied */
   if (numocts == 0) {
      return 0;
   }

   if ((bytesPerUnit != 1 && bytesPerUnit != 2 &&
        bytesPerUnit != 4 && bytesPerUnit != 8) ||
       numocts % bytesPerUnit != 0)
      return 0;

   OSCRTLMEMSET (hexstr, ' ', 48); hexstr[48] = '\0';
   OSCRTLMEMSET (ascstr, ' ', 16); ascstr[16] = '\0';
   ai = 0;

   if (bytesPerUnit > 4) bytesPerUnit = 4;

   for (i = 0; i < numocts / bytesPerUnit; i++)
   {
      pbuf = (char*)buf;
      buf [bytesPerUnit * 2] = 0;
      if (bytesPerUnit == 1) {
         val = *data++;
         shift = 0;
      }
      else if (bytesPerUnit == 2) {
         val = *data++;
         val = (val * 256) + *data++;
         shift = 8;
      }
      else { /* greater than 2 */
         OSSIZE ii;
         for (ii = 0, val = 0; ii < sizeof (OSUINT32); ii++) {
            val = (val * 256) + *data++;
         }
         shift = (sizeof (OSUINT32) - 1) * 8;
      }
      for (k = 0; k < bytesPerUnit; k++, pbuf += 2, shift -= 8) {
         b = (OSOCTET)((val >> shift) & 0xFF);
         rtxByteToHexChar (b, pbuf, 20);
         ascstr[ai++] = (char) (OS_ISPRINT(b) ? b : '.');
      }
      *pbuf = 0;

      k = i % unitsPerLine * (bytesPerUnit * 2 + 1);
      OSCRTLSAFEMEMCPY (&hexstr[k], sizeof(hexstr) - k, buf, bytesPerUnit * 2);

      if ((dumpstr = (OSOCTET) ((i + 1) % unitsPerLine == 0)) != 0)
      {
         if (bufferSize < (bufferIndex+48+1+16+1)) {
            return -1;
         }
         rtxStrcat (buffer, bufferSize, hexstr);
         rtxStrcat (buffer, bufferSize, " ");
         rtxStrcat (buffer, bufferSize, ascstr);
         rtxStrcat (buffer, bufferSize, "\n");
         bufferIndex = OSCRTLSTRLEN (buffer);
         if (i < (numocts - 1)) {
            OSCRTLMEMSET (hexstr, ' ', 48);
            OSCRTLMEMSET (ascstr, ' ', 16);
            ai = 0;
         }
      }
   }

   if (!dumpstr) {
      char* tascstr =  rtxStrTrimEnd(ascstr);
      if (bufferSize < (bufferIndex+48+1+OSCRTLSTRLEN(tascstr)+1)) {
         return -1;
      }
      rtxStrcat (buffer, bufferSize, hexstr);
      rtxStrcat (buffer, bufferSize, " ");
      rtxStrcat (buffer, bufferSize, ascstr);
      rtxStrcat (buffer, bufferSize, "\n");
   }
   return (int)OSCRTLSTRLEN (buffer);
}

#define MAXDIFFITEMS 100

char* rtxHexDiffToDynString
(OSCTXT* pctxt, const OSOCTET* pdata1, const OSOCTET* pdata2, OSSIZE numocts)
{
   char lbuf[64], *diffbuf = 0;
   OSSIZE i, count = 0;

   for (i = 0; i < numocts; i++) {
      if (pdata1[i] != pdata2[i]) {
         os_snprintf (lbuf, sizeof(lbuf), "[" OSSIZEFMT "]%02x != %02x",
                      i, pdata1[i], pdata2[i]);

         if (0 == diffbuf) {
            OSSIZE bufsize = OSCRTLSTRLEN(lbuf) + 1;
            diffbuf = (char*) rtxMemAlloc (pctxt, bufsize);
            if (0 == diffbuf) return 0;
            rtxStrcpy (diffbuf, bufsize, lbuf);
         }
         else {
            char* newbuf = rtxStrDynJoin (pctxt, diffbuf, ", ", lbuf, 0, 0);
            rtxMemFreePtr (pctxt, diffbuf);
            diffbuf = newbuf;
         }

         if (++count > MAXDIFFITEMS) {
            char* newbuf = rtxStrDynJoin (pctxt, diffbuf, ", ...", 0, 0, 0);
            rtxMemFreePtr (pctxt, diffbuf);
            diffbuf = newbuf;
            break;
         }
      }
   }

   return diffbuf;
}

void rtxHexDumpToFile
(FILE* fp, const OSOCTET* data, OSSIZE numocts)
{
   rtxHexDumpToFileEx (fp, data, numocts, 1);
}

void rtxHexDumpToFileEx
(FILE* fp, const OSOCTET* data, OSSIZE numocts, OSSIZE bytesPerUnit)
{
   OSSIZE   i, k;
   OSUINT32 val;
   OSOCTET  b, dumpstr = 1;
   char     hexstr[49], ascstr[17], buf[20], *pbuf;
   OSSIZE   unitsPerLine = 16 / bytesPerUnit, ai, shift;

   if (0 == data || numocts == 0) return;

   OSCRTLMEMSET (hexstr, ' ', 48); hexstr[48] = '\0';
   OSCRTLMEMSET (ascstr, ' ', 16); ascstr[16] = '\0';
   ai = 0;

   if (bytesPerUnit > 4) bytesPerUnit = 4;

   for (i = 0; i < numocts / bytesPerUnit; i++)
   {
      pbuf = (char*)buf;
      buf [bytesPerUnit * 2] = 0;
      if (bytesPerUnit == 1) {
         val = *data++;
         shift = 0;
      }
      else if (bytesPerUnit == 2) {
         val = *data++;
         val = (val * 256) + *data++;
         shift = 8;
      }
      else { /* greater than 2 */
         OSSIZE ii;
         for (ii = 0, val = 0; ii < sizeof (OSUINT32); ii++) {
            val = (val * 256) + *data++;
         }
         shift = (sizeof(OSUINT32) - 1) * 8;
      }
      for (k = 0; k < bytesPerUnit; k++, pbuf += 2, shift -= 8) {
         b = (OSOCTET)((val >> shift) & 0xFF);
         rtxByteToHexChar (b, pbuf, 20);
         ascstr[ai++] = (char) (OS_ISPRINT(b) ? b : '.');
      }
      *pbuf = 0;

      k = i % unitsPerLine * (bytesPerUnit * 2 + 1);
      OSCRTLSAFEMEMCPY (&hexstr[k], sizeof(hexstr) - k, buf, bytesPerUnit * 2);

      if ((dumpstr = (OSOCTET) ((i + 1) % unitsPerLine == 0)) != 0)
      {
         fprintf (fp, "%48s %16s\n", hexstr, ascstr);
         if (i < (numocts - 1)) {
            OSCRTLMEMSET (hexstr, ' ', 48);
            OSCRTLMEMSET (ascstr, ' ', 16);
            ai = 0;
         }
      }
   }

   if (!dumpstr) fprintf (fp, "%48s %s\n", hexstr, rtxStrTrimEnd(ascstr));
}

void rtxHexDump (const OSOCTET* data, OSSIZE numocts)
{
   rtxHexDumpToFile (stdout, data, numocts);
}
