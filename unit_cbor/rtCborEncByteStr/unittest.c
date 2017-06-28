#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const OSOCTET* pvalue, OSSIZE nbytes, 
 const OSOCTET* encoding, OSSIZE encsize, OSSIZE segsize)
{
   OSOCTET buffer[1000];
   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      ret = rtCborEncByteStr (pctxt, pvalue, nbytes, segsize);
      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode byte string value failed\n");
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
      const OSOCTET value[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
      const OSOCTET encoding[] = { 0x45, 0x01, 0x02, 0x03, 0x04, 0x05 };
      if (!testValue (&ctxt, value, sizeof(value), encoding, 
                      sizeof(encoding), 0)) errcnt++;
   }
   {
      OSSIZE i, j = 0;
      OSOCTET value[255];
      OSOCTET encoding[sizeof(value)+2];

      encoding[j++] = 0x58; encoding[j++] = 0xff;
      for (i = 0; i < sizeof(value); i++) {
         value[i] = (OSOCTET)i; encoding[j++] = (OSOCTET)i;
      }
      if (!testValue (&ctxt, value, sizeof(value), encoding, 
                      sizeof(encoding), 0)) errcnt++;
   }
   {
      /* indefinite length test */
      const OSOCTET value[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
      const OSOCTET encoding[] = { 
         0x5F, 0x42, 0x01, 0x02, 0x42, 0x03, 0x04, 0x41, 0x05, 0xFF };
      if (!testValue (&ctxt, value, sizeof(value), encoding, 
                      sizeof(encoding), 2)) errcnt++;
   }

   return errcnt;
}
