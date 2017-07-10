#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBase64.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemory.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const OSOCTET* pvalue, OSSIZE nbytes, 
 const OSOCTET* encoding, OSSIZE encsize, OSSIZE segsize, OSBOOL url)
{
   OSOCTET buffer[1000];

   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      if ( url ) ret = rtCborEncBase64Url (pctxt, pvalue, nbytes, segsize);
      else ret = rtCborEncBase64 (pctxt, pvalue, nbytes, segsize);
      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode byte string value failed\n");
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
      const OSOCTET value[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
      const OSOCTET encodingUrl[] = { 0xD8, 0x21, 0x68, 'A', 'Q', 'I', 'D', 'B',
         'A', 'U', '=' };
      const OSOCTET encoding[] = { 0xD8, 0x22, 0x68, 'A', 'Q', 'I', 'D', 'B',
         'A', 'U', '=' };

      if (!testValue (&ctxt, value, sizeof(value), encodingUrl, 
                      sizeof(encodingUrl), 0, TRUE)) errcnt++;

      if (!testValue (&ctxt, value, sizeof(value), encoding, 
                      sizeof(encoding), 0, FALSE)) errcnt++;

   }

   {
      OSSIZE i, j = 0;
      OSOCTET value[255];
      /* 255 bytes requires 340 base64 characters.
         Add 2 bytes for the base64 tag, and 3 bytes for the text major type
         and length (req's 2 bytes) = 345 total bytes
      */
      OSOCTET* pbase64;
      OSOCTET encoding[ 345 ];
      OSOCTET encodingUrl[ 345 ];
      long chars;

      encoding[0] = encodingUrl[0] = 0xD8;
      encodingUrl[1] = 0x21;
      encoding[1] = 0x22;
      encoding[2] = encodingUrl[2] = 0x79;
      encoding[3] = encodingUrl[3] = 0x01;
      encoding[4] = encodingUrl[4] = 0x54;

      for (i = 0; i < sizeof(value); i++) {
         value[i] = (OSOCTET)i;
      }

      chars = rtxBase64UrlEncodeData(&ctxt, value, sizeof(value), &pbase64);
      if ( chars != 340 ) {
         printf("base64 data not expected length: %d\n", chars);
         errcnt++;
      }

      memcpy(encodingUrl + 5, pbase64, chars);
      rtxMemFreePtr(&ctxt, pbase64);

      chars = rtxBase64EncodeData(&ctxt, value, sizeof(value), &pbase64);
      if ( chars != 340 ) {
         printf("base64 data not expected length: %d\n", chars);
         errcnt++;
      }

      memcpy(encoding + 5, pbase64, chars);
      rtxMemFreePtr(&ctxt, pbase64);

      if (!testValue (&ctxt, value, sizeof(value), encodingUrl, 
                      sizeof(encodingUrl), 0, TRUE) ) errcnt++;

      if (!testValue (&ctxt, value, sizeof(value), encoding, 
                      sizeof(encoding), 0, FALSE) ) errcnt++;

   }

   {
      /* indefinite length test */
      const OSOCTET value[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
      const OSOCTET encodingUrl[] = { 
         0xd8, 0x21, 0x7f, 0x62, 0x41, 0x51, 0x62, 0x49, 0x44, 0x62, 0x42, 0x41,
         0x62, 0x55, 0x3d, 0xff };
      const OSOCTET encoding[] = { 
         0xd8, 0x22, 0x7f, 0x62, 0x41, 0x51, 0x62, 0x49, 0x44, 0x62, 0x42, 0x41,
         0x62, 0x55, 0x3d, 0xff };

      if (!testValue (&ctxt, value, sizeof(value), encodingUrl, 
                      sizeof(encodingUrl), 2, TRUE)) errcnt++;

      if (!testValue (&ctxt, value, sizeof(value), encoding, 
                      sizeof(encoding), 2, FALSE)) errcnt++;
   }

   return errcnt;
}
