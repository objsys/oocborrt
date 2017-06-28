#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const char* expvalue, const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         char decval[15]; /* decoded value */
         ret = rtCborDecUTF8Str (pctxt, ub, decval, sizeof(decval));
         if (0 == ret) {
            /* verify decode value == expected value */
            if (OSCRTLSTRCMP (decval, expvalue)) {
               printf ("decoded content does not match expected\n");
               printf ("expected: %s\n", expvalue);
               printf ("actual: %s\n", decval);
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
   const char* value = "this is a test";

   ret = rtxInitContext (&ctxt);
   if (ret != 0) {
      rtxErrPrint(&ctxt);
      return 1;
   }
   {
      const OSOCTET encoding[] = { 
         0x6e, 0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 
         0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74 };

      if (!testValue (&ctxt, value, 
                      encoding, sizeof(encoding))) errcnt++;
   }
   {
      /* indefinite length */
      const OSOCTET encoding[] = { 
         0x7F, 0x64, 0x74, 0x68, 0x69, 0x73, 0x64, 0x20, 
         0x69, 0x73, 0x20, 0x64, 0x61, 0x20, 0x74, 0x65, 
         0x62, 0x73, 0x74, 0xFF 
      };

      if (!testValue (&ctxt, value, 
                      encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
