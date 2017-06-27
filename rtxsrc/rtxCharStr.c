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
#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxMemory.h"

int rtxStricmp (const char* str1, const char* str2)
{
   if (0 != str1 && 0 != str2) {
      size_t len = OSCRTLSTRLEN(str1);
      if (len == OSCRTLSTRLEN(str2)) {
         size_t i;
         for (i = 0; i < len; i++) {
            if (OS_TOLOWER(str1[i]) != OS_TOLOWER(str2[i])) return 1;
         }
         return 0;
      }
      else return 1;
   }
   return -1;
}

char* rtxStrcat (char* dest, size_t bufsiz, const char* src)
{
   if (0 == dest || 0 == bufsiz || 0 == src)
      return (char*)0;
   else
      return rtxStrncat (dest, bufsiz, src, OSCRTLSTRLEN(src));
}

char* rtxStrncat
(char* dest, size_t bufsiz, const char* src, size_t nchars)
{
   size_t i;

   if (0 == dest || bufsiz <= nchars || 0 == src || 0 == nchars)
      return (char*)0;

   i = OSCRTLSTRLEN (dest);

   if ((bufsiz - i) <= nchars || i >= bufsiz) /*  check for underflow */
      return (char*)0;

   return rtxStrncpy (&dest[i], bufsiz - i, src, nchars);
}

char* rtxStrcpy (char* dest, size_t bufsiz, const char* src)
{
   if (0 == dest || 0 == bufsiz || 0 == src)
      return (char*)0;
   else
      return rtxStrncpy (dest, bufsiz, src, OSCRTLSTRLEN(src));
}

char* rtxStrncpy
(char* dest, size_t bufsiz, const char* src, size_t nchars)
{
   size_t i;

   if (0 == dest || bufsiz < nchars || 0 == src || bufsiz == 0) {
      return (char *)0;
   }

   for (i = 0; i < nchars; i++) {
      if (src[i] == '\0') break;
      else dest[i] = src[i];
   }

   if (i < bufsiz) {
      dest[i] = '\0';
      return dest;
   }
   else
   {
      dest[i-1] = '\0'; /* always terminate */
      return (char *)0;
   }
}

char* rtxStrdup (OSCTXT* pctxt, const char* src)
{
   size_t bufsize = OSCRTLSTRLEN(src) + 1;
   char* tmpbuf = (char*) rtxMemAlloc (pctxt, bufsize);
   if (tmpbuf != 0) {
      rtxStrncpy (tmpbuf, bufsize, src, bufsize - 1);
   }
   return tmpbuf;
}

const char* rtxStrJoin
(char* dest, size_t bufsiz, const char* str1, const char* str2,
 const char* str3, const char* str4, const char* str5)
{
   size_t partLen, idx = 0;
   const char* parts[5];
   OSUINT32 i;

   parts[0] = str1; parts[1] = str2; parts[2] = str3;
   parts[3] = str4, parts[4] = str5;

   for (i = 0; i < 5; i++) {
      if (0 != parts[i]) {
         partLen = OSCRTLSTRLEN (parts[i]);
         if ((idx + partLen) < bufsiz) {
            rtxStrcpy (&dest[idx], bufsiz - idx, parts[i]);
            idx += partLen;
         }
         else break;
      }
      else break;
   }

   dest[idx] = '\0';

   return dest;
}

char* rtxStrDynJoin
(OSCTXT* pctxt, const char* str1, const char* str2,
 const char* str3, const char* str4, const char* str5)
{
   char* destbuf = 0;
   size_t bufsize = 0;
   if (0 == str1) return 0;
   else bufsize = OSCRTLSTRLEN (str1);

   if (0 != str2) bufsize += OSCRTLSTRLEN (str2);
   if (0 != str3) bufsize += OSCRTLSTRLEN (str3);
   if (0 != str4) bufsize += OSCRTLSTRLEN (str4);
   if (0 != str5) bufsize += OSCRTLSTRLEN (str5);
   bufsize++; /* for null-terminator */

   destbuf = (char*) rtxMemAlloc (pctxt, bufsize);
   if (0 == destbuf) return 0;

   return (0 != rtxStrJoin (destbuf, bufsize, str1, str2, str3, str4, str5)) ?
      destbuf : 0;
}

char* rtxStrTrimEnd (char *s)
{
   OSSIZE i = OSCRTLSTRLEN(s);
   if (i > 0 && OS_ISSPACE(s[i-1])) {
      while (i > 0) {
         i--;
         if (!OS_ISSPACE(s[i])) break;
      }
      s[i+1] = '\0';
   }
   return (s);
}

