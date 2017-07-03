#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBigFloat.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"
#include "rtxsrc/rtxMemory.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const char* value, const OSOCTET* encoding, OSSIZE encsize)
{
   OSOCTET buffer[1000];
   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      ret = rtCborEncNumberStr (pctxt, value);

      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode number string failed\n");
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
      /* A decimal fraction ( > 15 digits ) */
      const char* value = "123456789012345678901234567890E-255";

      const OSOCTET encoding[] = { 
         0xc4, 0x82, 0x38, 0xfe,
         0xc2, 0x4d, 0x01, 0x8e, 0xe9, 0x0f, 0xf6, 0xc3, 
         0x73, 0xe0, 0xee, 0x4e, 0x3f, 0x0a, 0xd2
      };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* A bignum */
      const char* value = "123456789012345678901234567890";
      const OSOCTET encoding[] = { 
         0xc2, 0x4d, 0x01, 0x8e, 0xe9, 0x0f, 0xf6, 0xc3, 
         0x73, 0xe0, 0xee, 0x4e, 0x3f, 0x0a, 0xd2
      };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* An int */
      const char* value = "45";
      const OSOCTET encoding[] = { 0x18, 0x2D };
   }

   {
      /* An nint */
      const char* value = "-45";
      const OSOCTET encoding[] = { 0x38, 0x2C };
   }

   {
      /* A float16 */
      const OSOCTET encoding[] = { 0xf9, 0x00, 0x00 };
      if (!testValue (&ctxt, "0.0", encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* float32 */
      const OSOCTET encoding[] = { 
         0xfa, 0x48, 0x59, 0x03, 0x80
      };
      if (!testValue (&ctxt, "222222.0", encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* float64 */
      const OSOCTET encoding[] = { 
         0xfb, 0x3f, 0xb9, 0x99, 0x99, 0x99, 0x99, 0x99, 0x9a
      };
      if (!testValue (&ctxt, "0.1", encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* decimal fraction: > 15 digits */
      const OSOCTET encoding[] = { 0xc4, 0x82, 0x2d, 0x1b, 0x00, 0x2b, 0xdc, 
         0x54, 0x5d, 0x6b, 0x4b, 0x44 };

      if (!testValue (&ctxt, "123.45678901234500", encoding, sizeof(encoding)))
         errcnt++;
   }

   {
      /* decimal fraction: underflows double */
      const OSOCTET encoding[] = { 0xc4, 0x82, 0x39, 0x01, 0x4b, 0x1a, 0x00, 
         0xbc, 0x61, 0x4e };

      if (!testValue (&ctxt, "123.45678E-327", encoding, sizeof(encoding)))
         errcnt++;
   }

   {
      /* decimal fraction: overflows double */
      const OSOCTET encoding[] = { 0xc4, 0x82, 0x19, 0x01, 0x35, 0x18, 0x63 };

      if (!testValue (&ctxt, "99E309", encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
