#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, OSDOUBLE expval, const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         OSDOUBLE decval; /* decode value */
         ret = rtCborDecFloat (pctxt, ub, &decval);
         if (0 == ret) {
            /* verify decode value == expected value */
            if (decval != expval) {
               printf ("decode failed, decoded value %g does not match "
                       "expected value %g\n", decval, expval);
               return FALSE;
            }
         }
      }
   }

   if (0 != ret) {
      printf ("run-time function failed:\n");
      rtxErrPrint (pctxt);
      return FALSE;
   }

   return TRUE;
}

int main() 
{
   OSCTXT ctxt;
   int ret, errcnt = 0;

   ret = rtxInitContext (&ctxt);
   if (ret != 0) {
      rtxErrPrint(&ctxt);
      return 1;
   }
   {
      const OSOCTET encoding[] = { 0xf9, 0x00, 0x00 };
      if (!testValue (&ctxt, 0.0, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0xf9, 0x3c, 0x00 };
      if (!testValue (&ctxt, 1.0, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 
         0xfb, 0x3f, 0xb9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9a
      };
      if (!testValue (&ctxt, 0.1, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 
         0xfb, 0xbf, 0xb9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9a
      };
      if (!testValue (&ctxt, -0.1, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 
         0xfa, 0x48, 0x59, 0x03, 0x80
      };
      if (!testValue (&ctxt, 222222.0, encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
