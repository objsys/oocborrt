/*
 * Copyright (c) 2003-2022 Objective Systems, Inc.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by Objective Systems, Inc.
 *
 * PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 *
 *****************************************************************************/

//#include "rtxsrc/rtxBitString.h"
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxCommonDefs.h"
#include "rtxsrc/rtxDiag.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxPrint.h"
//#include "rtxsrc/rtxPrintStream.h"
//#include "rtxsrc/rtxPrintToStream.h"
#include "rtxsrc/rtxError.h"

#ifndef __SYMBIAN32__
static OSBOOL g_DiagEnabled = FALSE;
#endif

EXTRTMETHOD OSBOOL rtxDiagEnabled (OSCTXT* pctxt)
{
#ifndef __SYMBIAN32__
   if (g_DiagEnabled) return TRUE;
   else
#endif
      if (0 != pctxt && pctxt->flags & OSDIAG) return TRUE;
      else return FALSE;
}

EXTRTMETHOD OSBOOL rtxSetDiag (OSCTXT* pctxt, OSBOOL value)
{
   OSBOOL prevState = rtxDiagEnabled (pctxt);
   if (value)
      pctxt->flags |= OSDIAG;
   else {
      /* Need to do it this way because some UNIX compilers will set    */
      /* bits in full 32-bit word..                                     */
      OSRTFLAGS lflags = (pctxt->flags & ~OSDIAG);
      pctxt->flags = lflags;
   }
   return prevState;
}

#ifndef __SYMBIAN32__
EXTRTMETHOD OSBOOL rtxSetGlobalDiag (OSBOOL value)
{
   OSBOOL prevState = g_DiagEnabled;
   g_DiagEnabled = value;
   return prevState;
}
#endif

static OSBOOL checkDiagLevel (OSCTXT* pctxt, const char* str)
{
   if (*str == '~') {
      OSRTDiagTraceLevel level = (0 == pctxt) ?
         OSRTDiagDebug : (OSRTDiagTraceLevel) pctxt->diagLevel;
      const char* testStr;
      switch (level) {
      case OSRTDiagError: testStr = "Ee"; break;
      case OSRTDiagWarning: testStr = "EeWw"; break;
      case OSRTDiagInfo:  testStr = "EeWwIi"; break;
      case OSRTDiagDebug: testStr = "EeWwIiDd"; break;
      default: testStr = 0;
      }
      if (0 != testStr) {
         return (OSBOOL) (0 != OSCRTLSTRCHR (testStr, *(str+1)));
      }
   }
   return TRUE;
}

EXTRTMETHOD void rtxDiagPrint (OSCTXT* pctxt, const char* fmtspec, ...)
{
   if (rtxDiagEnabled (pctxt) && checkDiagLevel (pctxt, fmtspec)) {
      va_list  arglist;
      va_start (arglist, fmtspec);
      vprintf  (fmtspec, arglist);
      va_end   (arglist);
   }
}

EXTRTMETHOD void rtxDiagHexDump
(OSCTXT* pctxt, const OSOCTET* data, size_t numocts)
{
   if (rtxDiagEnabled (pctxt)) {
      rtxHexDumpToFile (stdout, data, numocts);
   }
}

EXTRTMETHOD void rtxDiagPrintChars
(OSCTXT* pctxt, const char* data, size_t nchars)
{
   size_t i;
   if (rtxDiagEnabled (pctxt)) {
      for (i = 0; i < nchars; i++) putchar (data[i]);
      printf ("\n");
   }
}

#ifndef _NO_STREAM
EXTRTMETHOD void rtxDiagStream (OSCTXT* pctxt, const char* fmtspec, ...)
{
   if (rtxDiagEnabled (pctxt) && checkDiagLevel (pctxt, fmtspec)) {
      va_list  arglist;
      va_start (arglist, fmtspec);
      rtxDiagToStream  (pctxt, fmtspec, arglist);
      va_end (arglist);
   }
}

EXTRTMETHOD void rtxDiagStreamHexDump
(OSCTXT* pctxt, const OSOCTET* data, size_t numocts)
{
   if (rtxDiagEnabled (pctxt)) {
      rtxHexDumpToStream (pctxt, data, numocts);
   }
}

EXTRTMETHOD void rtxDiagStreamPrintChars
(OSCTXT* pctxt, const char* data, size_t nchars)
{
   if (rtxDiagEnabled (pctxt)) {
      char* buf = (char*) rtxMemAlloc (pctxt, nchars+1);
      if (0 != buf) {
         rtxStrncpy (buf, nchars+1, data, nchars);
         buf[nchars] = '\0';
         rtxPrintToStream (pctxt, "%s", buf);
      }
      else {
         size_t i;
         for (i = 0; i < nchars; i++)
            rtxPrintToStream (pctxt, "%c", data[i]);
      }
   }
}

EXTRTMETHOD void rtxDiagStreamPrintBits
(OSCTXT* pctxt, const char* descr, const OSOCTET* data,
 size_t bitIndex, size_t nbits)
{
   if (rtxDiagEnabled (pctxt)) {
      size_t i, bufidx = 0, bufsiz = nbits + 1;
      char bitchar;
      char* buf;

      if (0 != descr) {
         bufidx = OSCRTLSTRLEN (descr);
         bufsiz += bufidx;
      }
      buf = (char*) rtxMemAlloc (pctxt, bufsiz);

      if (0 == buf) {
         LOG_RTERR (pctxt, RTERR_NOMEM);
         return;
      }

      if (0 != descr) {
         rtxStrcpy (buf, bufsiz, descr);
      }
      for (i = 0; i < nbits; i++) {
         bitchar = rtxTestBit (data, OSUINT32_MAX, bitIndex + i) ? '1' : '0';
         buf[bufidx++] = bitchar;
      }
      buf[bufidx] = '\0';

      rtxPrintToStream (pctxt, "%s", buf);

      rtxMemFreePtr (pctxt, buf);
   }
}
#endif /* _NO_STREAM */

EXTRTMETHOD void rtxDiagSetTraceLevel (OSCTXT* pctxt, OSRTDiagTraceLevel level)
{
   rtxSetDiag (pctxt, TRUE);
   pctxt->diagLevel = (OSOCTET) level;
}

EXTRTMETHOD OSBOOL rtxDiagTraceLevelEnabled
(OSCTXT* pctxt, OSRTDiagTraceLevel level)
{
   if (rtxDiagEnabled(pctxt)) {
      switch (pctxt->diagLevel) {
      case OSRTDiagError: return (OSBOOL)(level == OSRTDiagError);
      case OSRTDiagWarning: return (OSBOOL)(level <= OSRTDiagWarning);
      case OSRTDiagInfo: return (OSBOOL)(level <= OSRTDiagInfo);
      case OSRTDiagDebug: return TRUE;
      }
   }
   return FALSE;
}

