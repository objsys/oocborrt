#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const OSRTGenPtrArray* pExpArray, 
 const OSOCTET* encoding, OSSIZE encsize)
{
   int ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      OSBOOL errFlag = FALSE;

      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         OSRTGenPtrArray decval;
         ret = rtCborDecArray (pctxt, ub, &decval);
         if (0 == ret) {
            OSSIZE i;
            /* verify decode value == expected value */
            if (decval.nitems != pExpArray->nitems) {
               printf ("decode failed, decoded count " OSSIZEFMT " does not "
                       "match expected count " OSSIZEFMT "\n", decval.nitems, 
                       pExpArray->nitems);
               return FALSE;
            }
            /* loop to compare decoded array values with expected */
            for (i = 0; i < decval.nitems; i++) {
               ret = rtxGenValueCompare 
                  (pctxt, decval.values[i], pExpArray->values[i]);
               if (0 != ret) {
                  printf ("comparison of item " OSSIZEFMT " failed\n", i);
                  rtxErrPrint (pctxt);
                  errFlag = TRUE;
               }
            }
            if (errFlag) return FALSE;
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
      OSRTGenPtrArray ptrArray;

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

      ptrArray.nitems = 4;
      ptrArray.values = values;
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
         if (!testValue (&ctxt, &ptrArray, encoding, sizeof(encoding))) 
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
         if (!testValue (&ctxt, &ptrArray, encoding, sizeof(encoding))) 
            errcnt++;
      }
   }

   return errcnt;
}
