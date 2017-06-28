#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxDateTime.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue (OSCTXT* pctxt, const OSNumDateTime* pvalue, 
                         const OSOCTET* encoding, OSSIZE encsize)
{
   OSOCTET buffer[100];
   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      ret = rtCborEncDateTimeStr (pctxt, pvalue);
      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode date/time string value failed\n");
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
   const char* dateTimeStr = "2013-03-21T20:04:00Z";
   OSNumDateTime value;

   ret = rtxParseDateTimeString ((const OSUTF8CHAR*)dateTimeStr, 
                                 OSCRTLSTRLEN(dateTimeStr), &value);
   if (0 != ret) {
      printf ("rtxParseDateTimeString failed, status = %d\n", ret);
      return ret;
   }

   ret = rtxInitContext (&ctxt);
   if (ret != 0) {
      rtxErrPrint(&ctxt);
      return 1;
   }
   {
      const OSOCTET encoding[] = { 
         0xc0, 0x74, 0x32, 0x30, 0x31, 0x33, 0x2d, 0x30, 
         0x33, 0x2d, 0x32, 0x31, 0x54, 0x32, 0x30, 0x3a,
         0x30, 0x34, 0x3a, 0x30, 0x30, 0x5a
      };
      if (!testValue (&ctxt, &value, encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
