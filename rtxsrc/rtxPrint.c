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

/* Run-time print utility functions */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxErrCodes.h"
#include "rtxsrc/rtxPrint.h"
#include "rtxsrc/osMacros.h"

void rtxPrintBoolean (const char* name, OSBOOL value)
{
   printf ("%s = %s\n", name, value ? "true" : "false");
}

void rtxPrintInteger (const char* name, OSINT32 value)
{
   printf ("%s = %d\n", name, value);
}

void rtxPrintInt64 (const char* name, OSINT64 value)
{
   printf ("%s = "OSINT64FMT"\n", name, value);
}

void rtxPrintUnsigned (const char* name, OSUINT32 value)
{
   printf ("%s = %u\n", name, value);
}

void rtxPrintUInt64 (const char* name, OSUINT64 value)
{
   printf ("%s = "OSUINT64FMT"\n", name, value);
}

void rtxPrintCharStr (const char* name, const char* cstring)
{
   printf ("%s = '%s'\n", name,
      (cstring == 0) ? "(null)" : cstring);
}

void rtxPrintUTF8CharStr
(const char* name, const OSUTF8CHAR* cstring)
{
   printf ("%s = '%s'\n", name,
      (cstring == 0) ? "(null)" : (const char*)cstring);
}

void rtxPrintUnicodeCharStr64
(const char* name, const OSUNICHAR* str, OSSIZE nchars)
{
   OSSIZE i;
   if (0 == str) return;
   printf ("%s = '", name);
   for (i = 0; i < nchars; i++) {
      if (OS_ISPRINT (str[i]))
         putc (str[i], stdout);
      else
         printf ("0x%04x", str[i]);
   }
   printf ("'\n");
}

void rtxPrintUnicodeCharStr
(const char* name, const OSUNICHAR* str, int nchars)
{
   if (0 == str) return;
   if (nchars < 0) {
      nchars = 0;
      while (str[nchars] != 0) nchars++;
   }
   rtxPrintUnicodeCharStr64 (name, str, nchars);
}

void rtxPrintHexStr
(const char* name, size_t numocts, const OSOCTET* data)
{
   if (numocts <= 32) {
      OSUINT32 i;
      printf ("%s = 0x", name);
      for (i = 0; i < numocts; i++) {
         printf ("%02x", data[i]);
      }
      printf ("\n");
   }
   else {
      printf ("%s =\n", name);
      rtxHexDump (data, numocts);
   }
}

void rtxPrintHexStrPlain
(const char* name, size_t numocts, const OSOCTET* data)
{
   OSUINT32 i;
   printf ("%s = 0x", name);
   for (i = 0; i < numocts; i++) {
      printf ("%02x", data[i]);
   }
   printf ("\n");
}

void rtxPrintNVP (const char* name, const OSUTF8NVP* pnvp)
{
   printf ("%s.name  = '%s'\n", name,
      (pnvp->name == 0) ? "(null)" : (const char*)pnvp->name);
   printf ("%s.value = '%s'\n", name,
      (pnvp->value == 0) ? "(null)" : (const char*)pnvp->value);
}

void rtxPrintHexBinary
(const char* name, size_t numocts, const OSOCTET* data)
{
   printf ("%s.numocts = " OSSIZEFMT "\n", name, numocts);
   printf ("%s.data = \n", name);
   rtxHexDump (data, numocts);
}

void rtxPrintReal (const char* name, OSREAL value)
{
   printf ("%s = %.12g\n", name, value);
}

void rtxPrintNull (const char* name)
{
   printf ("%s = <null>\n", name);
}

/* Indentation for brace text printing */

static OSUINT32 g_indent = 0;

void rtxPrintIndent ()
{
   OSUINT32 i;
   for (i = 0; i < g_indent; i++) printf (" ");
}

void rtxPrintIncrIndent ()
{
   g_indent += OSRTINDENTSPACES;
}

void rtxPrintDecrIndent ()
{
   if (g_indent > 0)
      g_indent -= OSRTINDENTSPACES;
}

void rtxPrintCloseBrace ()
{
   rtxPrintDecrIndent ();
   rtxPrintIndent ();
   printf ("}\n");
}

void rtxPrintOpenBrace (const char* name)
{
   rtxPrintIndent ();
   printf ("%s {\n", name);
   rtxPrintIncrIndent ();
}
