#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, OSUINT32 expval, const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         OSUINT32 decval; /* decode value */
         ret = rtCborDecUInt32 (pctxt, ub, &decval);
         if (0 == ret) {
            /* verify decode value == expected value */
            if (decval != expval) {
               printf ("decode failed, decoded value %u does not match "
                       "expected value %u\n", decval, expval);
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
      const OSOCTET encoding[] = { 0x00 };
      if (!testValue (&ctxt, 0, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x0a };
      if (!testValue (&ctxt, 10, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x18, 0xff };
      if (!testValue (&ctxt, 255, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x19, 0x01, 0x00 };
      if (!testValue (&ctxt, 256, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x19, 0x56, 0xce };
      if (!testValue (&ctxt, 22222, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x19, 0xff, 0xff };
      if (!testValue (&ctxt, 65535, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x1a, 0x00, 0x01, 0x00, 0x00 };
      if (!testValue (&ctxt, 65536, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x1a, 0x00, 0x03, 0x64, 0x0e };
      if (!testValue (&ctxt, 222222, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x1a, 0xff, 0xff, 0xff, 0xff };
      if (!testValue (&ctxt, OSUINT32_MAX, encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
