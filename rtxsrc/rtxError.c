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
/*  CHANGE LOG */
/*  Date         Init    Description */
/* ////////////////////////////////////////////////////////////////////////// */
/* Run-time error utility functions */
#include <stdarg.h>
#include <stdio.h>

#include "rtxsrc/osMacros.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxCommonDefs.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxUTF8.h"


/* Error status text */
#if !defined(_ARMTCC) && !defined(_COMPACT) && !defined(__SYMBIAN32__)
static const char* const g_status_text[] = {
    "Encode buffer overflow",
    "Unexpected end of buffer on decode",
    "Unexpected identifier encountered: expected = %s, parsed = %s",
    "Enumerated value %s not in defined set",
    "Duplicate element in container object (SEQUENCE, SET, etc.)",
    "Missing required element in container object (SEQUENCE, SET, etc.)",
    "Element index %s not within bounds of ASN.1 SET or XSD all type",
    "Max elements defined for repeating field exceeded",
    "Element with identifier %s is an invalid option in choice",
    "No dynamic memory available",
    "Invalid hex string",
    "Invalid real value",
    "Max items in sized character or binary string field exceeded",
    "Invalid value specification",
    "Nesting level too deep",
    "Value constraint violation: field %s, value %s",
    "Unexpected end of file detected",
    "Invalid UTF-8 character at index %s",
    "Array index out of bounds",
    "Invalid parameter passed to function or method",
    "Invalid value format",
    "Context is not initialized",
    "Value will not fit in target variable",
    "Character is not within the defined character set",
    "Invalid XML state for attempted operation",
    "Error condition returned from XML parser:\n%s",
    "Sequence elements not in correct order",
    "File not found or can't be opened",
    "Read error",
    "Write error",
    "Invalid Base64 string",
    "Socket error",
    "Attribute \"%s.%s\" not defined",
    "Regexp error: %s: %s",
    "Pattern match violation: field %s, value %s",
    "Missing required attribute \"%s\"",
    "Host name could not be resolved",
    "HTTP error: %s",
    "SOAP error. Code: %s, Reason:\n%s",
#ifdef RTEVAL
    "Evaluation license is expired",
#else
    "", /* placeholder */
#endif
    "Unexpected element \"%s\" found",
    "Invalid number of occurences of element %s (%s)",
    "Invalid message buffer",
    "Decode \"%s\" for element \"%s.%s\" failed",
    "Decode of attribute \"%s.%s\" failed",
    "Current stream must be closed before opening a new stream",
    "Unexpected null pointer value: %s",
    "General failure: %s",
    "Attribute '%s' value '%s' does not match defined fixed value",
    "Multiple errors; see context error list for details on each error",
    "Derived type could not be decoded due to missing type info",
    "Address is already in use",
    "Remote connection was reset",
    "Host was unreachable due to network or host failure",
    "Socket is not connected",
    "Connection refused",
    "Invalid socket option",
    "SOAP fault message",
    "Mark is not supported on this type of stream",
    "Feature not supported: %s",
    "Unbalanced structure detected",
    "Expected name in name/value pair but encountered value instead",
    "Invalid Unicode character encountered",
    "Invalid Boolean value keyword encountered",
    "Invalid null or nil value keyword encountered",
    "Invalid field length detected",
    "Unknown information element received, ID = %d",
    "Element expected to start on byte boundary",
    "Extraneous data exists in buffer after decode complete",
    "Invalid Message Authentication Code; expected = %s; computed = %s",
    "No security parameters were provided to decrypt/validate message",
    "Copy operation failed",
    "Unable to parse message '%s'",
    "Values not equal: element '%s', value1 '%s', value2 '%s'",
    "Buffers not equal: element '%s', diffs = '%s'"
} ;

struct OSRTErrorTable {
   const char* const* ppStatusText;
   OSINT32      minErrCode;
   OSINT32      maxErrCode;
} ;

#define OSRTMAXERRTABENTRIES 5

static struct OSRTErrorTable g_error_table[OSRTMAXERRTABENTRIES];
static OSINT16 g_numErrorTableEntries = 0;
#endif /* COMPACT, ARMTCC, SYMBIAN */

/* Add error table entry to error table array */

OSBOOL rtxErrAddErrorTableEntry
(const char* const* ppStatusText, OSINT32 minErrCode, OSINT32 maxErrCode)
{
#if !defined(_ARMTCC) && !defined(_COMPACT) && !defined(__SYMBIAN32__)
   if (g_numErrorTableEntries < OSRTMAXERRTABENTRIES) {

      /* Make sure table entry has not already been added */
      OSINT16 i;
      for (i = 0; i < g_numErrorTableEntries; i++) {
         if (g_error_table[i].ppStatusText == ppStatusText) return FALSE;
      }

      /* Add entry */

      g_error_table[g_numErrorTableEntries].ppStatusText = ppStatusText;
      g_error_table[g_numErrorTableEntries].minErrCode = minErrCode;
      g_error_table[g_numErrorTableEntries++].maxErrCode = maxErrCode;

      return TRUE;
   }
#endif
   return FALSE;
}

void rtxErrInit ()
{
#if !defined(_ARMTCC) && !defined(_COMPACT) && !defined(__SYMBIAN32__)
   rtxErrAddErrorTableEntry (g_status_text, -1, -99);
#endif
}

OSBOOL rtxErrAddCtxtBufParm (OSCTXT* pctxt)
{
   return rtxErrAddStrnParm
      (pctxt, (const char*)OSRTBUFPTR (pctxt), OSRTBUFSIZE (pctxt));
}

OSBOOL rtxErrAddElemNameParm (OSCTXT* pctxt)
{
   OSBOOL res;

   if (pctxt->elemNameStack.count > 0) {
      OSUTF8CHAR* elemName = 0;
      rtxDListToUTF8Str (pctxt, &pctxt->elemNameStack, &elemName, '.');

      if (0 != elemName) {
         /* Add string parameter */
         res = rtxErrAddStrParm (pctxt, (const char*)elemName);

         /* Free temp memory */
         rtxMemFreePtr (pctxt, elemName);
      }
      else res = FALSE;
   }
   else res = rtxErrAddStrParm (pctxt, "?");

   return res;
}

/* Add an integer parameter to an error message */

OSBOOL rtxErrAddIntParm (OSCTXT* pctxt, int errParm)
{
   char lbuf[16];
   rtxIntToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}

#if !defined(_NO_INT64_SUPPORT)
/* Add an 64-bit integer parameter to an error message */

OSBOOL rtxErrAddInt64Parm (OSCTXT* pctxt, OSINT64 errParm)
{
   char lbuf[32];
   rtxInt64ToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}
#endif /* !defined(_NO_INT64_SUPPORT) */

/* Add a character string parameter to an error message */

OSBOOL rtxErrAddStrParm (OSCTXT* pctxt, const char* pErrParm)
{
   OSRTErrInfo* pErrInfo = &pctxt->errInfo.reserved;
   if (pErrParm == 0) pErrParm = "(null)";
   if (pErrInfo != 0 && pErrInfo->parmcnt < OSRTMAXERRPRM) {
      size_t bufsiz = OSCRTLSTRLEN(pErrParm) + 1;
      OSUTF8CHAR* tmpstr =
         rtxMemAllocArray (pctxt, bufsiz, OSUTF8CHAR);
      if (tmpstr == NULL) return FALSE;
      rtxStrcpy ((char*)tmpstr, bufsiz, pErrParm);
      pErrInfo->parms[pErrInfo->parmcnt] = tmpstr;
      pErrInfo->parmcnt++;
      return TRUE;
   }
   else
      return FALSE;
}

/* Add a character string parameter to an error message */

OSBOOL rtxErrAddStrnParm
(OSCTXT* pctxt, const char* pErrParm, size_t nchars)
{
   OSRTErrInfo* pErrInfo = &pctxt->errInfo.reserved;
   if (pErrParm == 0) pErrParm = "(null)";
   if (pErrInfo != 0 && pErrInfo->parmcnt < OSRTMAXERRPRM) {
      OSUTF8CHAR* tmpstr =
         rtxMemAllocArray (pctxt, nchars + 1, OSUTF8CHAR);
      if (tmpstr == NULL) return FALSE;
      rtxStrncpy ((char*)tmpstr, nchars + 1, pErrParm, nchars);
      pErrInfo->parms[pErrInfo->parmcnt] = tmpstr;
      pErrInfo->parmcnt++;
      return TRUE;
   }
   else
      return FALSE;
}

/* Add an unsigned integer parameter to an error message */

OSBOOL rtxErrAddUIntParm (OSCTXT* pctxt, unsigned int errParm)
{
   char lbuf[16];
   rtxUIntToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}

#if !defined(_NO_INT64_SUPPORT)
/* Add an unsigned 64-bit integer parameter to an error message */

OSBOOL rtxErrAddUInt64Parm (OSCTXT* pctxt, OSUINT64 errParm)
{
   char lbuf[32];
   rtxUInt64ToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}
#endif /* !defined(_NO_INT64_SUPPORT) */

/* Add a size-typed parameter to an error message */

OSBOOL rtxErrAddSizeParm (OSCTXT* pctxt, OSSIZE errParm)
{
   char lbuf[16];
   rtxSizeToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}

/* Add a double parameter to an error message */

OSBOOL rtxErrAddDoubleParm (OSCTXT* pctxt, double errParm)
{
   char lbuf[200];
#if !defined(_ARMTCC) && !defined(_COMPACT) && !defined(__SYMBIAN32__)
   os_snprintf (lbuf, 200, "%E", errParm);
#else
   rtxStrncpy (lbuf, 200, "?", 1);
#endif
   return rtxErrAddStrParm (pctxt, lbuf);
}

void rtxErrFreeNodeParms (OSCTXT* pctxt, OSRTErrInfo* pErrInfo)
{
   OSUINT8 i, parmcnt;

   if (pErrInfo == 0) return;

   parmcnt = OS_MIN (pErrInfo->parmcnt, OSRTMAXERRPRM);

   for (i = 0; i < parmcnt; i++) {
      rtxMemFreeArray (pctxt, (char*)pErrInfo->parms[i]);
   }
   pErrInfo->parmcnt = 0;
   pErrInfo->status = 0;
}

/* Free error parameter memory */

void rtxErrFreeParms (OSCTXT* pctxt)
{
   OSRTDListNode* pNode = pctxt->errInfo.list.head;

   for ( ; pNode != 0; pNode = pNode->next) {
      OSRTErrInfo* pErrInfo = (OSRTErrInfo*)pNode->data;

      /* 13-09-26 MEM:  Allocated on l. 614. */
      rtxMemFreePtr (pctxt, pErrInfo->elemName);
      rtxErrFreeNodeParms (pctxt, pErrInfo);
   }

   /* Free params in reserved node */
   rtxErrFreeNodeParms (pctxt, &pctxt->errInfo.reserved);
}

/* Reset error */

int rtxErrReset (OSCTXT* pctxt)
{
   OSRTDListNode* pNode;
   OSRTErrInfo* pErrInfo;

   /* Clear off the element name stack. It may have had things lingering
      due to an error having occured.  The names should have been literals,
      so they aren't freed. */
   rtxDListFreeNodes( pctxt, &pctxt->elemNameStack );

   rtxErrFreeParms (pctxt);

   for (pNode = pctxt->errInfo.list.head; pNode != 0; pNode = pNode->next) {
      pErrInfo = (OSRTErrInfo*) pNode->data;
      if (pErrInfo != &pctxt->errInfo.reserved) {
         rtxMemFreePtr (pctxt, (void*)pErrInfo);
      }
   }
   rtxDListFreeNodes (pctxt, &pctxt->errInfo.list);

   return 0;
}

/* Format error message */

static char* rtxErrFmtMsgText
(OSRTErrInfo* pErrInfo, const char* errmsg, char* bufp, size_t bufsiz)
{
   const char* tp;
   size_t j = 0;
   OSUINT8 pcnt = 0;

   if (pErrInfo->parmcnt > OSRTMAXERRPRM) {
      pErrInfo->parmcnt = OSRTMAXERRPRM;
   }

   /* Substitute error parameters into error message */

   tp = errmsg;

   while (*tp) {
      if (*tp == '%' && *(tp+1) == 's') {

         /* Plug in error parameter */

         if (pcnt < pErrInfo->parmcnt && pErrInfo->parms[pcnt]) {
            size_t k = j + OSCRTLSTRLEN ((const char*)pErrInfo->parms[pcnt]);
            if (k < (bufsiz - 1)) {
               rtxStrcpy (&bufp[j], bufsiz-j,
                          (const char*)pErrInfo->parms[pcnt++]);
               j = k;
            }
            else break;
         }
         else
            bufp[j++] = '?';

         tp += 2;
      }
      else if (j < (bufsiz - 1)) {
         bufp[j++] = *tp++;
      }
      else break;
   }

   /* append additional params */
   while (pcnt < (size_t)pErrInfo->parmcnt && pErrInfo->parms[pcnt]) {
      size_t k = 1 + j + OSCRTLSTRLEN ((const char*)pErrInfo->parms[pcnt]);
      if (k < (bufsiz - 1)) {
         bufp[j++] = ' ';
         rtxStrcpy (&bufp[j], bufsiz-j,
                    (const char*)pErrInfo->parms[pcnt++]);
         j = k;
      }
      else break;
   }

   bufp[j] = '\0';        /* null terminate string */

   return bufp;
}

static const char* getStatusTextTmpl (OSRTErrInfo* pErrInfo)
{
#if !defined(_ARMTCC) && !defined(_COMPACT) && !defined(__SYMBIAN32__)
   const char* const* ppStatusText = 0;
   OSINT16 i;

   /* Find error table */

   for (i = 0; i < g_numErrorTableEntries; i++) {
      /* Note: test is reversed because status codes are negative */
      if (pErrInfo->status <= g_error_table[i].minErrCode &&
          pErrInfo->status > g_error_table[i].maxErrCode) {
         ppStatusText = g_error_table[i].ppStatusText;
         break;
      }
   }

   /* Return error text template */

   if (0 != ppStatusText) {
      OSINT32 erridx = abs (pErrInfo->status - g_error_table[i].minErrCode);
      return ppStatusText[erridx];
   }
#endif
   return 0;
}

const char* rtxErrFmtMsg
(OSRTErrInfo* pErrInfo, char* bufp, size_t bufsiz)
{
   if (0 == pErrInfo || 0 == bufp || 0 == bufsiz) return 0;

   if (pErrInfo->status < 0)
   {
      const char* pErrMsgText = getStatusTextTmpl (pErrInfo);

      if (0 != pErrMsgText) {
         rtxErrFmtMsgText (pErrInfo, pErrMsgText, bufp, bufsiz);
      }
      else
         rtxStrcpy (bufp, bufsiz, "unrecognized completion status");
   }
   else rtxStrcpy (bufp, bufsiz, "normal completion status");

   return (bufp);
}

static size_t calcErrMsgLen (OSRTErrInfo* pErrInfo)
{
   const char* pErrMsgText = getStatusTextTmpl (pErrInfo);
   size_t i, len = 0;

   if (0 == pErrMsgText) return 0;

   if (pErrInfo->parmcnt > OSRTMAXERRPRM) {
      pErrInfo->parmcnt = OSRTMAXERRPRM;
   }

   /* Buffer size is approximately length of message + length of parameters
      (this is a little over because it does not account for format specifiers
      being replaced with actual text, but for a buffer size calculation, a
      little over is OK).. */

   for (i = 0; i < pErrInfo->parmcnt; i++) {
      if (0 != pErrInfo->parms[i]) {
         len += OSCRTLSTRLEN ((const char*)pErrInfo->parms[i]);
      }
   }

   return (OSCRTLSTRLEN(pErrMsgText) + len + 1);
}

char* rtxErrGetMsgText (OSCTXT* pctxt)
{
   size_t bufsiz = 0, idx = 0;
   OSRTDListNode* pNode;
   char* pstrbuf;

   /* Calculate buffer size */
   for (pNode = pctxt->errInfo.list.head; pNode != 0; pNode = pNode->next) {
      bufsiz += calcErrMsgLen ((OSRTErrInfo*)pNode->data);
      if (0 != pNode->next) bufsiz += 2; /* padding for newline */
   }

   /* Allocate memory */
   pstrbuf = (char*) rtxMemAlloc (pctxt, bufsiz);
   if (0 == pstrbuf) return 0;

   /* Format error message(s).  If multiple, message are separated by
      newline character. */
   *pstrbuf = '\0';
   for (pNode = pctxt->errInfo.list.head; pNode != 0; pNode = pNode->next) {
      rtxErrFmtMsg ((OSRTErrInfo*)pNode->data, &pstrbuf[idx], bufsiz - idx);
      if (0 != pNode->next) {
         idx = OSCRTLSTRLEN(pstrbuf);
         rtxStrcat (pstrbuf, bufsiz - idx, "\n");
         idx += OSCRTLSTRLEN("\n");
      }
   }

   return pstrbuf;
}

/* Get error text in a dynamic or static memory buffer.  If pBuf and    */
/* pBufSize arguments are non-null, it is assumed a static buffer is    */
/* provided and this will be used.  Otherwise, this allocates memory    */
/* using the 'rtxMemAlloc' function.  This memory is automatically      */
/* freed at the time the 'rtxMemFree' function is called.               */
/* The calling function should free the memory.                         */

char* rtxErrGetText (OSCTXT* pctxt, char* pBuf, size_t* pBufSize)
{
   size_t bufsiz;
   OSRTDListNode* pNode;
   if (pBuf == 0 || pBufSize == 0) {
      /* calculate buffer size */
      bufsiz = 200;
      for (pNode = pctxt->errInfo.list.head; pNode != 0; pNode = pNode->next) {
         OSRTErrInfo* pErrInfo = (OSRTErrInfo*)pNode->data;
         bufsiz += 100 * (2 + pErrInfo->stkx) * sizeof(char);
         if (0 != pErrInfo->elemName) {
            bufsiz += rtxUTF8LenBytes (pErrInfo->elemName) + 20;
         }
      }
      pBuf = rtxMemAllocArray (pctxt, bufsiz, char);
      if (pBuf == NULL) return NULL;
      if (pBufSize != 0) *pBufSize = bufsiz;
   }
   else
      bufsiz = *pBufSize;

   rtxErrGetTextBuf (pctxt, pBuf, bufsiz);

   return pBuf;
}

/* Get error text in static buffer */
char* rtxErrGetTextBuf (OSCTXT* pctxt, char* pbuf, size_t bufsiz)
{
   char lbuf[200], numbuf[32];
   OSRTDListNode* pNode;

   if (pbuf == 0 || bufsiz == 0) return 0;

   *pbuf = '\0';

   for (pNode = pctxt->errInfo.list.head; pNode != 0; pNode = pNode->next) {
      OSRTErrInfo* pErrInfo = (OSRTErrInfo*)pNode->data;

      rtxStrcat (pbuf, bufsiz, "ERROR: Status ");
      rtxIntToCharStr (pErrInfo->status, numbuf, sizeof(numbuf), 0);
      rtxStrcat (pbuf, bufsiz, numbuf);
      if (!rtxStrcat (pbuf, bufsiz, "\n")) break;

      rtxStrcat (pbuf, bufsiz, rtxErrFmtMsg (pErrInfo, lbuf, sizeof(lbuf)));
      if (!rtxStrcat (pbuf, bufsiz, "\n")) break;
#ifndef _COMPACT
      {
      OSUINT8 stkx = pErrInfo->stkx;

      if (0 != pErrInfo->elemName) {
         rtxStrcat (pbuf, bufsiz, "Element name: ");
         rtxStrcat (pbuf, bufsiz, (const char*)pErrInfo->elemName);
         if (!rtxStrcat (pbuf, bufsiz, "\n")) break;
      }
      if (stkx > 0) {
         if (!rtxStrcat (pbuf, bufsiz, "Stack trace:\n")) break;
      }

      while (stkx > 0) {
         stkx--;
         if (0 != pErrInfo->stack[stkx].module) {
            rtxStrcat (pbuf, bufsiz, "  Module: ");
            rtxStrcat (pbuf, bufsiz, (const char*)pErrInfo->stack[stkx].module);
            rtxStrcat (pbuf, bufsiz, " Line ");
            rtxIntToCharStr
               (pErrInfo->stack[stkx].lineno, numbuf, sizeof(numbuf), 0);
            rtxStrcat (pbuf, bufsiz, numbuf);
            if (!rtxStrcat (pbuf, bufsiz, "\n")) break;
         }
      }}
#endif
   }

   return pbuf;
}

/* Get error text in static buffer.  This version get the message text only.  */
char* rtxErrGetMsgTextBuf (OSCTXT* pctxt, char* pbuf, size_t bufsiz)
{
   char lbuf[200];
   OSRTDListNode* pNode;

   if (pbuf == 0 || bufsiz == 0) return 0;

   *pbuf = '\0';

   for (pNode = pctxt->errInfo.list.head; pNode != 0; pNode = pNode->next) {
      OSRTErrInfo* pErrInfo = (OSRTErrInfo*)pNode->data;

      rtxStrcat (pbuf, bufsiz, rtxErrFmtMsg (pErrInfo, lbuf, sizeof(lbuf)));
      if (!rtxStrcat (pbuf, bufsiz, "\n")) break;
   }

   return pbuf;
}

/* Log error using given callback function */

void rtxErrLogUsingCB (OSCTXT* pctxt, OSErrCbFunc cb, void* cbArg_p)
{
   const char* pBuf = rtxErrGetText (pctxt, 0, 0);

   if (pBuf == NULL) {
      pBuf = "ERROR : no memory available";
   }
   (*cb) (pBuf, cbArg_p);

   rtxErrFreeParms (pctxt);
}

/* Print error information to the standard output */

void rtxErrPrintElement (OSRTErrInfo* pErrInfo)
{
   char lbuf[400];

   printf ("ERROR: Status %d\n", pErrInfo->status);
   printf ("%s\n", rtxErrFmtMsg (pErrInfo, lbuf, sizeof(lbuf)));
#ifndef _COMPACT
   {
   OSUINT8 stkx = pErrInfo->stkx;
   if (0 != pErrInfo->elemName) {
      printf ("Element name: %s\n", pErrInfo->elemName);
   }
   if (stkx > 0) printf ("Stack trace:");
   while (stkx > 0) {
      stkx--;

      /* we may have some errors pushed onto the stack that are null if debug
       * output is preserved in some parts of the code and not others */
      if (0 != pErrInfo->stack[stkx].module) {
         printf ("  Module: %s, Line %d\n",
                 pErrInfo->stack[stkx].module,
                 pErrInfo->stack[stkx].lineno);
      }
   }}
#endif
}

void rtxErrPrint (OSCTXT* pctxt)
{
   OSRTDListNode* pNode;

   for (pNode = pctxt->errInfo.list.head; pNode != 0; pNode = pNode->next) {
      OSRTErrInfo* pErrInfo = (OSRTErrInfo*)pNode->data;
      rtxErrPrintElement (pErrInfo);
   }
}

/* Set error data */

int rtxErrSetData
(OSCTXT* pctxt, int status, const char* module, int lineno)
{
   OSRTErrInfo* pErrInfo = OSRT_GET_LAST_ERROR_INFO (pctxt);
   if (pErrInfo == 0) {
      return rtxErrSetNewData (pctxt, status, module, lineno);
   }
   if (pErrInfo->status == 0) {
      pErrInfo->status = (OSINT16)status;
   }
   else if (pErrInfo->status != status) {
      /* Assume this is a new error.  Note that this test is not sufficient
         to cover all cases.  A new error with the same status value as the
         last error may be being logged, but this function in its current
         form will think information is being added to the last error. */
      pErrInfo = rtxErrNewNode (pctxt);
      if (0 == pErrInfo) return RTERR_NOMEM;
   }
#ifndef _COMPACT
   /* copy parameters from reserved */
   if ((pErrInfo != &pctxt->errInfo.reserved)
         && (pctxt->errInfo.reserved.parmcnt != 0)) {
      OSUINT8 i, parmcnt =
         OS_MIN (pctxt->errInfo.reserved.parmcnt, OSRTMAXERRPRM);
      for (i = 0; i < parmcnt; i++) {
         pErrInfo->parms[i] = pctxt->errInfo.reserved.parms[i];
         pctxt->errInfo.reserved.parms[i] = 0;
      }
      pErrInfo->parmcnt = parmcnt;
      pctxt->errInfo.reserved.parmcnt = 0;
   }
   if (0 == (pctxt->flags & OSRESERVED1)) {
      if (pErrInfo->stkx < OSRTERRSTKSIZ) {
         if (! OS_ISEMPTY(module)) {
            pErrInfo->stack[pErrInfo->stkx].module = (const OSUTF8CHAR*)module;
            pErrInfo->stack[pErrInfo->stkx++].lineno = lineno;
         }
      }

      /* If element name is present, copy it to the error structure */
      if (OS_ISEMPTY (pErrInfo->elemName)) {
         rtxDListToUTF8Str
            (pctxt, &pctxt->elemNameStack, &pErrInfo->elemName, '.');
      }
   }
#endif
   return rtxErrGetStatus (pctxt);
}

int rtxErrSetNewData
(OSCTXT* pctxt, int status, const char* module, int lineno)
{
   OSRTErrInfo* pErrInfo;

   /* create new error info node */

   pErrInfo = rtxErrNewNode (pctxt);
   if (pErrInfo == 0) {

      /* use reseved OSRTErrInfo element */

      if (pctxt->errInfo.reserved.status == 0 && status == RTERR_NOMEM) {
         pErrInfo = &pctxt->errInfo.reserved;
         pctxt->errInfo.reservedNode.data = pErrInfo;
         rtxDListAppendNode (&pctxt->errInfo.list,
            &pctxt->errInfo.reservedNode);
      }
      else
         return RTERR_NOMEM;
   }
   else {
      pctxt->errInfo.reserved.status = 0; /* clear reserved errInfo */
   }
   return rtxErrSetData (pctxt, status, module, lineno);
}

OSRTErrInfo* rtxErrNewNode (OSCTXT* pctxt)
{
   OSRTErrInfo* pErrInfo = rtxMemAllocTypeZ (pctxt, OSRTErrInfo);
   if (pErrInfo != 0) {
      if (0 == rtxDListAppend (pctxt, &pctxt->errInfo.list, pErrInfo)) {
         rtxMemFreePtr (pctxt, pErrInfo);
         return 0;
      }
   }
   return pErrInfo;
}

void rtxErrAssertionFailed
(const char* conditionText, int lineNo, const char* fileName)
{
   printf ("Run-time assertion '%s' failed.  Line %d, File %s\n",
           conditionText, lineNo, fileName);
#ifndef _ARMTCC
   exit (-1);
#endif
}

int rtxErrGetFirstError (const OSCTXT* pctxt)
{
   OSRTErrInfo* pErrInfo = OSRT_GET_FIRST_ERROR_INFO (pctxt);
   if (pErrInfo != 0)
      return pErrInfo->status;
   return 0;
}

int rtxErrGetLastError (const OSCTXT* pctxt)
{
   OSRTErrInfo* pErrInfo = OSRT_GET_LAST_ERROR_INFO (pctxt);
   return (pErrInfo != 0) ? pErrInfo->status : 0;
}

OSSIZE rtxErrGetErrorCnt (const OSCTXT* pctxt)
{
   return pctxt->errInfo.list.count;
}

int rtxErrGetStatus (const OSCTXT* pctxt)
{
   OSSIZE errcnt = rtxErrGetErrorCnt (pctxt);
   if (errcnt == 0) return 0;
   else if (errcnt == 1) {
      return rtxErrGetFirstError (pctxt);
   }
   else return RTERR_MULTIPLE;
}

int rtxErrResetLastErrors (OSCTXT* pctxt, int errorsToReset)
{
   OSRTDListNode* pNode = pctxt->errInfo.list.tail, *pPrevNode;

   for (;errorsToReset > 0 && pNode != 0; pNode = pPrevNode, errorsToReset--) {
      OSRTErrInfo* pErrInfo = (OSRTErrInfo*)pNode->data;
      pPrevNode = pNode->prev;

      rtxErrFreeNodeParms (pctxt, pErrInfo);

      if (pErrInfo != &pctxt->errInfo.reserved) {
         rtxDListFreeNode (pctxt, &pctxt->errInfo.list, pNode);
      }
      else { /* just remove, but not free */
         rtxDListRemove (&pctxt->errInfo.list, pNode);
      }
   }
   return 0;
}

static void copyErrInfo
(OSCTXT* pctxt, OSRTErrInfo* pDest, const OSRTErrInfo* pSrc)
{
   OSUINT8 i, parmcnt = OS_MIN (pSrc->parmcnt, OSRTMAXERRPRM);
   for (i = 0; i < OSRTERRSTKSIZ; i++) {
      pDest->stack[i] = pSrc->stack[i];
   }
   pDest->status = pSrc->status;
   pDest->stkx = pSrc->stkx;
   pDest->parmcnt = parmcnt;
   for (i = 0; i < parmcnt; i++) {
      pDest->parms[i] = rtxUTF8Strdup (pctxt, pSrc->parms[i]);
   }
   pDest->elemName = (0 != pSrc->elemName) ?
      rtxUTF8Strdup (pctxt, pSrc->elemName) : 0;
}

int rtxErrCopy (OSCTXT* pDestCtxt, const OSCTXT* pSrcCtxt)
{
   int ret = 0;

   /* Reset any errors that may exist in the destination context */
   rtxErrReset (pDestCtxt);

   /* Copy info from reserved */
   copyErrInfo (pDestCtxt,
      &pDestCtxt->errInfo.reserved, &pSrcCtxt->errInfo.reserved);

   /* Copy error info from the nodes in the error list */
   ret = rtxErrAppend (pDestCtxt, pSrcCtxt);
   if (0 != ret) {
      rtxErrReset (pDestCtxt);
   }

   return ret;
}

int rtxErrAppend (OSCTXT* pDestCtxt, const OSCTXT* pSrcCtxt)
{
   OSRTDListNode* pnode;
   OSRTErrInfo*   pErrInfo;

   /* Copy error info from the nodes in the error list */
   pnode = pSrcCtxt->errInfo.list.head;
   while (0 != pnode) {
      pErrInfo = rtxMemAllocType (pDestCtxt, OSRTErrInfo);
      if (0 == pErrInfo) {
         return RTERR_NOMEM;
      }
      copyErrInfo (pDestCtxt, pErrInfo, (OSRTErrInfo*)pnode->data);

      rtxDListAppend (pDestCtxt, &pDestCtxt->errInfo.list, pErrInfo);

      pnode = pnode->next;
   }

   return 0;
}

int rtxErrInvUIntOpt (OSCTXT* pctxt, OSUINT32 ident)
{
   char numbuf[40];
   rtxUIntToCharStr (ident, numbuf, sizeof(numbuf), 0);
   rtxErrAddStrParm (pctxt, numbuf);
   return RTERR_INVOPT;
}
