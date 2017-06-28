#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBigFloat.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"
#include "rtxsrc/rtxMemory.h"

static OSBOOL testValue 
(OSCTXT* pctxt, OSBOOL base10, OSINT64 exponent, const char* mantissa, const OSOCTET* encoding, OSSIZE encsize)
{
   OSOCTET buffer[1000];
   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      OSBigFloat bigfloat;
      rtxBigFloatInit(&bigfloat);
      bigfloat.base10 = base10;
      bigfloat.exponent = exponent;
      ret = rtxBigIntSetStr( pctxt, &bigfloat.mantissa, mantissa, 10);
      if ( ret == 0 ) ret = rtCborEncBigFloat (pctxt, &bigfloat);
      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode big float value failed\n");
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
