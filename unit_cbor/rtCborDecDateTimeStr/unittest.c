#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxDateTime.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const OSNumDateTime* expdata, 
 const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         OSNumDateTime decval; /* decode value */
         ret = rtCborDecDateTimeStr (pctxt, ub, &decval);
         if (0 == ret) {
            /* verify decode value == expected value */
            if (0 != rtxCmpDateTime (expdata, &decval)) {
               OSUTF8CHAR tmpStr[100];
               printf ("decoded content does not match expected\n");
               rtxDateTimeToString (expdata, tmpStr, sizeof (tmpStr));
               printf ("expected: %s\n", tmpStr);
               rtxDateTimeToString (&decval, tmpStr, sizeof (tmpStr));
               printf ("actual: %s\n", tmpStr);
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
      if (!testValue (&ctxt, &value, 
                      encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
