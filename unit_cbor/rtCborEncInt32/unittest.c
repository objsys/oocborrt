#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, OSINT32 value, const OSOCTET* encoding, OSSIZE encsize)
{
   OSOCTET buffer[100];
   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      ret = rtCborEncInt32 (pctxt, value);
      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode value %d failed\n", value);
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
      const OSOCTET encoding[] = { 0x20 };
      if (!testValue (&ctxt, -1, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x38, 0xfe };
      if (!testValue (&ctxt, -255, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x39, 0xff, 0xfe };
      if (!testValue (&ctxt, -65535, encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
