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

/* Error management functions */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtxsrc/rtxCommon.h"

/* Error status text */
static const char* g_status_text[] = {
    "Encode buffer overflow",
    "Unexpected end of buffer on decode",
    "Unexpected tag encountered: expected = %s, parsed = %s",
    "Invalid object identifier",
    "Invalid field length detected",
    "Enumerated value %s not in defined set",
    "Duplicate element in SET",
    "Missing required element in SET",
    "Element with tag %s not part of SET",
    "Max elements defined for SEQUENCE field exceeded",
    "Element with tag %s is an invalid option in choice",
    "No dynamic memory available",
    "Invalid string type",
    "Invalid hex string",
    "Invalid binary string",
    "Invalid real value",
    "Max items in sized BIT or OCTET STRING field exceeded",
    "Invalid value specification",
    "No definition found for referenced defined value",
    "No definition found for referenced defined type",
    "Invalid tag value",
    "Nesting level too deep",
    "Value constraint violation: field %s, value %s",
    "Value range error: lower bound is greater than upper",
    "Unexpected end of file detected",
    "Invalid UTF-8 character at index %d",
    "List error: concurrent modification attempt while iterating",
    "List error: illegal state for attempted operation",
    "Array index out of bounds",
    "Invalid parameter passed to function or method",
    "Invalid time string format",
    "Context is not initialized",
    "ASN.1 value will not fit in target variable",
    "Character is not within the defined character set",
    "Invalid XML state for attempted operation",
    "Error condition returned from XML parser:\n%s",
    "SEQUENCE elements not in correct order",
    "Invalid index for table constraint identifier",
    "Invalid value for relational table constraint fixed type field",
    "File not found",
    "File read error",
    "File write error",
    "Invalid Base64 string",
    "Socket error",
    "XML interface library not found",
    "Invalid XML interface library"
} ;

#define OSRT_K_MAX_STAT (sizeof(g_status_text)/sizeof(char *))

/* Add an integer parameter to an error message */

int rtxErrAddIntParm (OSCTXT* pctxt, int errParm)
{
   char lbuf[16];
   rtxIntToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}

/* Add a character string parameter to an error message */

int rtxErrAddStrParm (OSCTXT* pctxt, const char* errprm_p)
{
   ASN1ErrInfo* pErrInfo = &pctxt->errInfo;
   if (pErrInfo->parmcnt < ASN_K_MAXERRP) {
      OSSIZE bufsiz = strlen(errprm_p)+1;
      char* tmpstr = (char*) malloc (bufsiz);
      rtxStrcpy (tmpstr, bufsiz, errprm_p);
      pErrInfo->parms[pErrInfo->parmcnt] = tmpstr;
      pErrInfo->parmcnt++;
      return TRUE;
   }
   else return FALSE;
}

/* Add an element name parameter (not supported in OO version) */

OSBOOL rtxErrAddElemNameParm (OSCTXT* pctxt)
{
   return rtxErrAddStrParm (pctxt, "?");
}

/* Add an unsigned integer parameter to an error message */

int rtxErrAddUIntParm (OSCTXT* pctxt, unsigned int errParm)
{
   char lbuf[16];
   rtxUIntToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}

/* Add a 64-bit integer parameter to an error message */

OSBOOL rtxErrAddInt64Parm (OSCTXT* pctxt, OSINT64 errParm)
{
   char lbuf[32];
   rtxInt64ToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}

/* Add an unsigned 64-bit integer parameter to an error message */

OSBOOL rtxErrAddUInt64Parm (OSCTXT* pctxt, OSUINT64 errParm)
{
   char lbuf[32];
   rtxUInt64ToCharStr (errParm, lbuf, sizeof(lbuf), 0);
   return rtxErrAddStrParm (pctxt, lbuf);
}

/* Add a size-typed parameter to an error message */

OSBOOL rtxErrAddSizeParm (OSCTXT* pctxt, OSSIZE errParm)
{
   char lbuf[16];
#if defined(_MSC_VER)
   _snprintf_s (lbuf, sizeof(lbuf), _TRUNCATE, OSSIZEFMT, errParm);
#else
   snprintf (lbuf, sizeof(lbuf), OSSIZEFMT, errParm);
#endif
   return rtxErrAddStrParm (pctxt, lbuf);
}

/* Add a double parameter to an error message */

OSBOOL rtxErrAddDoubleParm (OSCTXT* pctxt, double errParm)
{
   char lbuf[200];
#if defined(_MSC_VER)
   _snprintf_s (lbuf, sizeof(lbuf), _TRUNCATE, "%E", errParm);
#else
   snprintf (lbuf, sizeof(lbuf), "%E", errParm);
#endif
   return rtxErrAddStrParm (pctxt, lbuf);
}

/* Free error parameter memory */

void rtxErrFreeParms (OSCTXT* pctxt)
{
   ASN1ErrInfo* pErrInfo = &pctxt->errInfo;
   int i;

   for (i = 0; i < pErrInfo->parmcnt; i++) {
      free ((char*)pErrInfo->parms[i]);
      pErrInfo->parms[i] = 0;
   }
   pErrInfo->parmcnt = 0;
   pErrInfo->status = 0;
}

/* Reset error */

int rtxErrReset (OSCTXT* pctxt)
{
   rtxErrFreeParms (pctxt);
   pctxt->errInfo.stkx = 0;
   return 0;
}

/* Format error message */

static char* errFmtMsg (ASN1ErrInfo* pErrInfo, char* bufp, size_t bufsiz)
{
   const char* tp;
   size_t i, j, pcnt;

   if (pErrInfo->status < 0)
   {
      i = abs (pErrInfo->status + 1);

      if (i < OSRT_K_MAX_STAT)
      {
         /* Substitute error parameters into error message */

         j  = pcnt = 0;
         tp = g_status_text[i];

         while (*tp)
         {
            if (*tp == '%' && *(tp+1) == 's')
            {
               /* Plug in error parameter */

               if (pcnt < (size_t)pErrInfo->parmcnt && pErrInfo->parms[pcnt]) {
                  size_t k = j +
                     OSCRTLSTRLEN ((const char*)pErrInfo->parms[pcnt]);
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
            else
               bufp[j++] = *tp++;
         }

         bufp[j] = '\0';        /* null terminate string */
      }
      else
         rtxStrcpy (bufp, bufsiz, "unrecognized completion status");
   }
   else rtxStrcpy (bufp, bufsiz, "normal completion status");

   return (bufp);
}

/* Get error text in a dynamic memory buffer.  This allocates memory    */
/* using the 'rtxMemAlloc' function.  This memory is automatically freed */
/* at the time the 'memFree' function is called.                      */

char* rtxErrGetText (OSCTXT* pctxt)
{
   char lbuf[500], numbuf[32];
   ASN1ErrInfo* pErrInfo = &pctxt->errInfo;
   size_t bufsiz = (sizeof(lbuf) + 100 * (2 + pErrInfo->stkx)) * sizeof(char);
   char* pBuf = (char*) rtxMemAlloc (pctxt, bufsiz);

   rtxStrcpy (pBuf, bufsiz, "ERROR: Status ");
   rtxIntToCharStr (pErrInfo->status, numbuf, sizeof(numbuf), 0);
   rtxStrcat (pBuf, bufsiz, numbuf);
   rtxStrcat (pBuf, bufsiz, errFmtMsg (pErrInfo, lbuf, sizeof(lbuf)));
   rtxStrcat (pBuf, bufsiz, "\nStack trace:\n");

   while (pErrInfo->stkx > 0) {
      pErrInfo->stkx--;
      rtxStrcat (pBuf, bufsiz, "  Module: ");
      rtxStrcat (pBuf, bufsiz, pErrInfo->stack[pErrInfo->stkx].module);
      rtxStrcat (pBuf, bufsiz, ", Line ");
      rtxIntToCharStr (pErrInfo->stack[pErrInfo->stkx].lineno,
                       numbuf, sizeof(numbuf), 0);
      rtxStrcat (pBuf, bufsiz, numbuf);
      rtxStrcat (pBuf, bufsiz, "\n");
   }

   rtxErrFreeParms (pctxt);

   return pBuf;
}

/* Print error information to the standard output */

void rtxErrPrint (OSCTXT* pctxt)
{
   ASN1ErrInfo* pErrInfo = &pctxt->errInfo;
   char lbuf[200];
   printf ("ERROR: Status %d\n", pErrInfo->status);
   printf ("%s\n", errFmtMsg (pErrInfo, lbuf, sizeof(lbuf)));
   printf ("Stack trace:");
   while (pErrInfo->stkx > 0) {
      pErrInfo->stkx--;
      printf ("  Module: %s, Line %d\n",
              pErrInfo->stack[pErrInfo->stkx].module,
              pErrInfo->stack[pErrInfo->stkx].lineno);
   }
   rtxErrFreeParms (pctxt);
}

int rtxErrSetData (OSCTXT* pctxt, int status, const char* module, int lno)
{
   ASN1ErrInfo* pErrInfo = &pctxt->errInfo;
   if (pErrInfo->status == 0) {
      pErrInfo->status = status;
   }
   return status;
}

void rtxErrAssertionFailed
(const char* conditionText, int lineNo, const char* fileName)
{
   printf ("Run-time assertion '%s' failed.  Line %d, File %s\n",
           conditionText, lineNo, fileName);

   exit (-1);
}

