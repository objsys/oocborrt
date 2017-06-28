#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue (OSCTXT* pctxt, const char* pvalue, 
                         const OSOCTET* encoding, OSSIZE encsize, 
                         OSSIZE segsize)
{
   OSOCTET buffer[1000];
   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      ret = rtCborEncUTF8Str (pctxt, pvalue, segsize);
      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode UTF8 string value failed\n");
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
   static const char* value = "this is a test";

   ret = rtxInitContext (&ctxt);
   if (ret != 0) {
      rtxErrPrint(&ctxt);
      return 1;
   }
   {
      const OSOCTET encoding[] = { 0x6e, 0x74, 0x68, 0x69, 0x73, 0x20, 
                                   0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 
                                   0x65, 0x73, 0x74 };

      if (!testValue (&ctxt, value, encoding, sizeof(encoding), 0)) errcnt++;
   }
   {
      /* indefinite length */
      const OSOCTET encoding[] = { 
         0x7F, 0x64, 0x74, 0x68, 0x69, 0x73, 0x64, 0x20, 
         0x69, 0x73, 0x20, 0x64, 0x61, 0x20, 0x74, 0x65, 
         0x62, 0x73, 0x74, 0xFF 
      };

      if (!testValue (&ctxt, value, encoding, sizeof(encoding), 4)) errcnt++;
   }

   return errcnt;
}
