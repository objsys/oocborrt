#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, OSUINT32 value, const OSOCTET* encoding, OSSIZE encsize)
{
   OSOCTET buffer[100];
   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      ret = rtCborEncUInt32 (pctxt, value, OSRTCBOR_UINT);
      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode value %u failed\n", value);
            printf ("encoding does not match expected encoding\n");
            printf ("expected:\n");
            rtxHexDump (encoding, encsize);
            printf ("actual:\n");
            rtxHexDump (pctxt->buffer.data, pctxt->buffer.byteIndex);
            return FALSE;
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
