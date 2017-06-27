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

#include "rtxsrc/osMacros.h"
#include "rtxsrc/rtxErrCodes.h"
#include "osrtjson.h"

/* This file depends on predefined constants defined in corresponding
 * source files:
 *
 * OSJSONUNIENCFUNC (e.g., rtJsonEncUnicodeData)
 * OSJSONUNICHAR    (e.g., OSUNICHAR or OS32BITCHAR) */

static const OSUINT32 encoding_mask[] = {
   0xfffff800, 0xffff0000, 0xffe00000, 0xfc000000
};

static const unsigned char encoding_byte[] = {
   0xc0, 0xe0, 0xf0, 0xf8, 0xfc
};

int OSJSONUNIENCFUNC
(OSCTXT* pctxt, const OSJSONUNICHAR* value, OSSIZE nchars)
{
   int step;
   OSUINT32 inbufx;
   OSJSONUNICHAR wc;

   OSRTSAFEPUTCHAR (pctxt, '"');

   for (inbufx = 0; inbufx < nchars; inbufx++) {
      wc = value[inbufx];

      if (wc < 0x80) {
         char c = (char) wc;
         /* One byte sequence */
         OSRTSAFEPUTCHAR (pctxt, c);
      }
      else {
         char *outbuf;
         int sz;

         for (step = 2; step < 6; ++step)
            if ((wc & encoding_mask[step - 2]) == 0)
               break;
         sz = step;

         outbuf = (char *)rtxMemAllocZ (pctxt, sz+1);

         /* Copy multi-byte sequence to output buffer */
         outbuf[0] = encoding_byte[step - 2];
         --step;
         do {
            outbuf[step] = (OSOCTET) (0x80 | (wc & 0x3f));
            wc >>= 6;
         }
         while (--step > 0);
         outbuf[0] |= wc;

         rtxCopyAsciiText (pctxt, (void*)outbuf);

         rtxMemFreePtr (pctxt, (void *)outbuf);
      }
   }

   OSRTSAFEPUTCHAR (pctxt, '"');

   return 0;
}


