#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxMemLeakCheck.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, OSSIZE expnbytes, const OSOCTET* expdata, 
 const OSOCTET* encoding, OSSIZE encsize)
{
   int ret;
   OSBOOL retb = TRUE;

   DECLARE_MEMLEAK_DETECTOR;

   ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         OSDynOctStr64 decval; /* decode value */
         ret = rtCborDecDynByteStr (pctxt, ub, &decval);
         if (0 == ret) {
            /* verify decode value == expected value */
            if (decval.numocts != expnbytes) {
               printf ("decode failed, decoded count " OSSIZEFMT " does not "
                       "match expected count " OSSIZEFMT "\n", 
                       decval.numocts, expnbytes);
               retb = FALSE;
            }
            if (OSCRTLMEMCMP (decval.data, expdata, decval.numocts)) {
               printf ("decoded content does not match expected\n");
               printf ("expected:\n");
               rtxHexDump (expdata, expnbytes);
               printf ("actual:\n");
               rtxHexDump (decval.data, decval.numocts);
               retb = FALSE;
            }
            rtxMemFreePtr (pctxt, decval.data);
         }
      }
   }

   if (0 != ret) {
      printf ("run-time function failed:\n");
      rtxErrPrint (pctxt);
      retb = FALSE;
   }

   return retb;
}

int main() 
{
   OSCTXT ctxt;
   int ret, errcnt = 0;
   static const OSOCTET value[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };

   ret = rtxInitContext (&ctxt);
   if (ret != 0) {
      rtxErrPrint(&ctxt);
      return 1;
   }
   {
      const OSOCTET encoding[] = { 0x45, 0x01, 0x02, 0x03, 0x04, 0x05 };
      if (!testValue (&ctxt, sizeof(value), value, 
                      encoding, sizeof(encoding))) errcnt++;
   }
   {
      /* indefinite length test */
      const OSOCTET encoding[] = { 
         0x5F, 0x42, 0x01, 0x02, 0x42, 0x03, 0x04, 0x41, 0x05, 0xFF };
      if (!testValue (&ctxt, sizeof(value), value, encoding, 
                      sizeof(encoding))) errcnt++;
   }

   rtxFreeContext (&ctxt);

   return errcnt;
}
