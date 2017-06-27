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

#include "rtxsrc/rtxCharStr.h"
#include "rtxsrc/rtxErrCodes.h"

#ifndef XXFUNC
#define XXFUNC rtxIntToCharStr
#define XXTYPE OSINT32
#endif

int XXFUNC (XXTYPE value, char* dest, size_t bufsiz, char padchar)
{
   OSUINT32 nchars = 0;
   XXTYPE  rem, tmpval = value;
   OSINT32 i;
#ifndef XXUNSIGNED
   OSBOOL  minus = (OSBOOL)(value < 0);
#endif
   if (dest == 0 || bufsiz < 2) return RTERR_INVPARAM;

   /* Compute number of digits */
   if (value == 0) nchars = 1;
   else {
      while (tmpval != 0) {
         tmpval /= 10L;
         nchars++;
      }
#ifndef XXUNSIGNED
      if (value < 0) {
         nchars++; /* for '-' sign */
         /* Note: do not try to make value positive; */
         /* if value == MIN (e.g. INT_MIN), you can't. */
         /* It is better to just handle how % deals with the negative value */
         /* as is done below. */
      }
#endif
   }

   if (nchars >= bufsiz) return RTERR_BUFOVFLW;

   i = (padchar == '\0') ? (OSINT32)nchars : (OSINT32)(bufsiz - 1);
   nchars = (OSUINT32) i;
   dest[i--] = '\0'; /* set null-terminator */

   if (0 == value) {
      dest[i--] = '0';
   }
   else {
      tmpval = value;
      while (tmpval != 0) {
         rem = tmpval % 10L;
#ifndef XXUNSIGNED
         if ( minus ) {
            /*  % operator varies by implementation. */
            /*  Implementations giving a negative remainder give the digit we */
            /*  want, but with the wrong sign. */
            /*  Implementations giving a positive remainder give us x where */
            /*  x + correct digit = 10 */
            if ( rem > 0 ) rem = 10 - rem;
            else if ( rem < 0 ) rem = -rem;
         }
#endif
         dest[i--] = (char)(rem + '0');
         tmpval /= 10L;
      }
   }

#ifndef XXUNSIGNED
   if (minus && padchar != '0') dest[i--] = '-';
#endif
   if (padchar != '\0') {
      while (i >= 0) dest[i--] = padchar;
   }
#ifndef XXUNSIGNED
   if (minus && padchar == '0') dest[0] = '-';
#endif

   return (int)nchars;
}
