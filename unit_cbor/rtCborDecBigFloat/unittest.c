#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBigFloat.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"
#include "rtxsrc/rtxMemory.h"

static OSBOOL testValue 
(OSCTXT* pctxt, OSBOOL expBase10, OSINT64 expExp, const char* expMantissa, 
 const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {      
         OSBigFloat bigfloat;
         rtxBigFloatInit(&bigfloat);
         ret = rtCborDecBigFloat (pctxt, ub, &bigfloat);
         if (0 == ret) {
            OSSIZE digits;
            char* decval; /* decoded value */

            /* verify decoded value == expected value */
            digits = rtxBigIntDigitsNum (&bigfloat.mantissa, 10);
            decval = rtxMemAllocArray(pctxt, digits + 1 /* add one for sign */,
                                             char);
            if ( decval == 0 ) {
               ret = LOG_RTERR(pctxt, RTERR_NOMEM);
            }

            if ( ret == 0 ) {
               ret = rtxBigIntToString
                  (pctxt, &bigfloat.mantissa, 10, decval, digits + 1);
            }

            if (ret == 0 && OSCRTLSTRCMP (decval, expMantissa)) {
               printf ("decoded mantissa does not match expected\n");
               printf ("expected: %s\n", expMantissa);
               printf ("actual: %s\n", decval);
               return FALSE;
            }

            if (ret == 0 && bigfloat.exponent != expExp ) {
               printf ("decoded exponent does not match expected\n");
               printf ("expected: " OSINT64FMT "\n", expExp);
               printf ("actual: " OSINT64FMT "\n", bigfloat.exponent);
               return FALSE;               
            }

            if (ret == 0 && bigfloat.base10 != expBase10 ) {
               printf ("decoded base10 flag does not match expected\n");
               printf ("expected: %d\n", expBase10);
               printf ("actual: %d\n", bigfloat.base10);
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
      const char* mantissa = "123456789012345678901234567890";
      OSINT64 exponent = -255;
      OSBOOL base10 = TRUE;

      const OSOCTET encoding[] = { 
         0xc4, 0x82, 0x38, 0xfe,
         0xc2, 0x4d, 0x01, 0x8e, 0xe9, 0x0f, 0xf6, 0xc3, 
         0x73, 0xe0, 0xee, 0x4e, 0x3f, 0x0a, 0xd2
      };
      if (!testValue (&ctxt, base10, exponent, mantissa, encoding, sizeof(encoding))) errcnt++;
   }

   {
      const char* mantissa = "45";
      OSINT64 exponent = -255;
      OSBOOL base10 = TRUE;

      const OSOCTET encoding[] = { 
         0xc4, 0x82, 
         0x38, 0xfe,
         0x18, 0x2D
      };
      if (!testValue (&ctxt, base10, exponent, mantissa, encoding, sizeof(encoding))) errcnt++;
   }

   {
      const char* mantissa = "123456789012345678901234567890";
      OSINT64 exponent = -255;
      OSBOOL base10 = FALSE;

      const OSOCTET encoding[] = { 
         0xc5, 0x82, 0x38, 0xfe,
         0xc2, 0x4d, 0x01, 0x8e, 0xe9, 0x0f, 0xf6, 0xc3, 
         0x73, 0xe0, 0xee, 0x4e, 0x3f, 0x0a, 0xd2
      };
      if (!testValue (&ctxt, base10, exponent, mantissa, encoding, sizeof(encoding))) errcnt++;
   }

   {
      const char* mantissa = "45";
      OSINT64 exponent = -255;
      OSBOOL base10 = FALSE;

      const OSOCTET encoding[] = { 
         0xc5, 0x82, 
         0x38, 0xfe,
         0x18, 0x2D
      };
      if (!testValue (&ctxt, base10, exponent, mantissa, encoding, sizeof(encoding))) errcnt++;
   }

   return errcnt;
}
