#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBigInt.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"
#include "rtxsrc/rtxMemory.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const char* expvalue, const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {      
         OSBigInt bigint;
         rtxBigIntInit(&bigint);
         ret = rtCborDecBigInt (pctxt, ub, &bigint);
         if (0 == ret) {
            OSSIZE digits;
            char* decval; /* decoded value */

            /* verify decoded value == expected value */
            digits = rtxBigIntDigitsNum (&bigint, 10);
            decval = rtxMemAllocArray(pctxt, digits + 1 /* add one for sign */,
                                             char);
            if ( decval == 0 ) {
               ret = LOG_RTERR(pctxt, RTERR_NOMEM);
            }

            if ( ret == 0 ) {
               ret = rtxBigIntToString(pctxt, &bigint, 10, decval, digits + 1);               
            }

            if (ret == 0 && OSCRTLSTRCMP (decval, expvalue)) {
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

   ret = rtxInitContext (&ctxt);
   if (ret != 0) {
      rtxErrPrint(&ctxt);
      return 1;
   }

   {
      const char* value = "123456789012345678901234567890";
      const OSOCTET encoding[] = { 
         0xc2, 0x4d, 0x01, 0x8e, 0xe9, 0x0f, 0xf6, 0xc3, 
         0x73, 0xe0, 0xee, 0x4e, 0x3f, 0x0a, 0xd2
      };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      const char* value = "-123456789012345678901234567891";
      const OSOCTET encoding[] = { 
         0xc3, 0x4d, 0x01, 0x8e, 0xe9, 0x0f, 0xf6, 0xc3, 
         0x73, 0xe0, 0xee, 0x4e, 0x3f, 0x0a, 0xd2
      };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* Test smallest CBOR nint minus 1 */
      const char* value = "-18446744073709551617";
      const OSOCTET encoding[] = { 
         0xC3, 0x49, 0x01, 0, 0, 0, 0, 0, 0, 0, 0 };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* Test smallest CBOR nint */
      const char* value = "-18446744073709551616";
      const OSOCTET encoding[] = { 
         0x3B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* Test smallest CBOR nint + 1 */
      const char* value = "-18446744073709551615";
      const OSOCTET encoding[] = { 
         0x3B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE  };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* Test biggest CBOR uint */
      const char* value = "18446744073709551615";
      const OSOCTET encoding[] = { 
         0x1B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* Test biggest CBOR uint + 1 */
      const char* value = "18446744073709551616";
      const OSOCTET encoding[] = { 
         0xC2, 0x49, 0x01, 0, 0, 0, 0, 0, 0, 0, 0 };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* Test a single byte positive value */
      const char* value = "45";
      const OSOCTET encoding[] = { 0x18, 0x2D };
   }

   {
      /* Test a single byte negative value */
      const char* value = "-45";
      const OSOCTET encoding[] = { 0x38, 0x2C };
   }

   {
      /* Test zero with leading zeros */
      const char* value = "0";
      const OSOCTET encoding[] = { 
         0xC2, 0x49, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* Test five with leading zeros */
      const char* value = "5";
      const OSOCTET encoding[] = { 
         0xC2, 0x49, 0, 0, 0, 0, 0, 0, 0, 0, 5 };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   {
      /* Test negative five with leading zeros */
      const char* value = "-5";
      const OSOCTET encoding[] = { 
         0xC3, 0x49, 0, 0, 0, 0, 0, 0, 0, 0, 4 };
      if (!testValue (&ctxt, value, encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
