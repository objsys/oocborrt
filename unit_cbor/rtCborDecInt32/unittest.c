#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, OSINT32 expval, const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         OSINT32 decval; /* decode value */
         ret = rtCborDecInt32 (pctxt, ub, &decval);
         if (0 == ret) {
            /* verify decode value == expected value */
            if (decval != expval) {
               printf ("decode failed, decoded value %d does not match "
                       "expected value %d\n", decval, expval);
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

static OSBOOL testTooBigValue
(OSCTXT* pctxt, const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         OSINT32 decval; /* decode value */
         ret = rtCborDecInt32 (pctxt, ub, &decval);
         if (0 == ret) {
            printf("Failed to detect too big condition.  Decoded %d.\n",
                     decval);
         }
         else if ( ret == RTERR_TOOBIG ) 
         {
            rtxErrReset(pctxt);
            ret = 0;
         }
         /* else: drop down to report error */
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
      const OSOCTET encoding[] = { 0x21 };
      if (!testValue (&ctxt, -2, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x29 };
      if (!testValue (&ctxt, -10, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x38, 0xfe };
      if (!testValue (&ctxt, -255, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x38, 0xff };
      if (!testValue (&ctxt, -256, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x39, 0x56, 0xcd };
      if (!testValue (&ctxt, -22222, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x39, 0xff, 0xfe };
      if (!testValue (&ctxt, -65535, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x39, 0xff, 0xff };
      if (!testValue (&ctxt, -65536, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x3a, 0x00, 0x03, 0x64, 0x0d };
      if (!testValue (&ctxt, -222222, encoding, sizeof(encoding))) errcnt++;
   }
   {
      const OSOCTET encoding[] = { 0x3a, 0x7f, 0xff, 0xff, 0xff };
      if (!testValue (&ctxt, OSINT32_MIN, encoding, sizeof(encoding))) errcnt++;
   }

   /* Here the value-as-encoded is 2**31, so the encoded value is -2**31 - 1,
      which is out of range.  Test that we get TOO BIG error.
   */
   {
      const OSOCTET encoding[] = { 0x3a, 0x80, 0x00, 0x00, 0x00 };
      if (!testTooBigValue (&ctxt, encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
