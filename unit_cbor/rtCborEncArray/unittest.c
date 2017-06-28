#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const OSRTGenValue** values, OSSIZE nitems,
 const OSOCTET* encoding, OSSIZE encsize, OSBOOL indef)
{
   OSOCTET buffer[1000];
   int ret = rtxInitContextBuffer (pctxt, buffer, sizeof(buffer));
   if (0 == ret) {
      OSUINT32 flags = indef ? OSRTCBORM_INDEF : 0;
      ret = rtCborEncArray (pctxt, values, nitems, flags);
      if (0 == ret) {
         /* compare to expected encoding */
         if (pctxt->buffer.byteIndex != encsize ||
             0 != memcmp (pctxt->buffer.data, encoding, encsize)) {
            printf ("encode array failed\n");
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
      OSOCTET bytestr_data[] = { 0x01, 0x02, 0x03 };
      OSRTGenValue value1, value2, value3, value4;
      OSRTGenValue* values[4], *innerValues[3];

      value1.t = OSRTDataType_UnsignedInt;
      value1.u.uintval = 22;
      values[0] = innerValues[0] = &value1;

      value2.t = OSRTDataType_ByteArray;
      value2.u.bytestr.numocts = sizeof(bytestr_data);
      value2.u.bytestr.data = bytestr_data;
      values[1] = innerValues[1] = &value2;

      value3.t = OSRTDataType_CharStr;
      value3.u.utf8str = "this is a test";
      values[2] = innerValues[2] = &value3;

      value4.t = OSRTDataType_Array;
      value4.u.array.nitems = 3;
      value4.u.array.values = innerValues;
      values[3] = &value4;
      {
         /* definite length test */
         const OSOCTET encoding[] = { 
            0x84, 0x16, 0x43, 0x01, 0x02, 0x03, 0x6e, 0x74, 
            0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 
            0x20, 0x74, 0x65, 0x73, 0x74, 0x83, 0x16, 0x43, 
            0x01, 0x02, 0x03, 0x6e, 0x74, 0x68, 0x69, 0x73, 
            0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65, 
            0x73, 0x74 
         };
         if (!testValue (&ctxt, values, 4, encoding, sizeof(encoding), FALSE))
            errcnt++;
      }
      {
         /* indefinite length test */
         const OSOCTET encoding[] = { 
            0x9f, 0x16, 0x43, 0x01, 0x02, 0x03, 0x6e, 0x74, 
            0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 
            0x20, 0x74, 0x65, 0x73, 0x74, 0x9f, 0x16, 0x43, 
            0x01, 0x02, 0x03, 0x6e, 0x74, 0x68, 0x69, 0x73, 
            0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65, 
            0x73, 0x74, 0xff, 0xff
         };

         if (!testValue (&ctxt, values, 4, encoding, sizeof(encoding), TRUE))
            errcnt++;
      }
   }

   return errcnt;
}
