#include <stdio.h>
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxMemLeakCheck.h"
#include "rtxsrc/rtxPrint.h"

static OSBOOL testValue 
(OSCTXT* pctxt, const OSRTGenValueMap* pExpMap, 
 const OSOCTET* encoding, OSSIZE encsize)
{
   int ret;

   DECLARE_MEMLEAK_DETECTOR;

   ret = rtxInitContextBuffer (pctxt, (OSOCTET*)encoding, encsize);
   if (0 == ret) {
      OSOCTET ub;
      OSBOOL errFlag = FALSE;

      ret = rtxReadBytes (pctxt, &ub, 1);
      if (0 == ret) {
         OSRTGenValueMap decval;
         ret = rtCborDecMap (pctxt, ub, &decval);
         if (0 == ret) {
            OSSIZE i;
            /* verify decode value == expected value */
            if (decval.nitems != pExpMap->nitems) {
               printf ("decode failed, decoded count " OSSIZEFMT " does not "
                       "match expected count " OSSIZEFMT "\n", decval.nitems, 
                       pExpMap->nitems);
               return FALSE;
            }
            /* loop to compare decoded map values with expected */
            for (i = 0; i < decval.nitems; i++) {
               /* key */
               ret = rtxGenValueCompare 
                  (pctxt, decval.values[i].key, pExpMap->values[i].key);
               if (0 != ret) {
                  printf ("comparison of key " OSSIZEFMT " failed\n", i);
                  rtxErrPrint (pctxt);
                  errFlag = TRUE;
               }

               /* value */
               ret = rtxGenValueCompare 
                  (pctxt, decval.values[i].value, pExpMap->values[i].value);
               if (0 != ret) {
                  printf ("comparison of value " OSSIZEFMT " failed\n", i);
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
      OSRTGenValue key1, key2, key3, key4;
      OSRTGenValue value1, value2, value3, value4;
      OSRTGenValue* innerValues[3];
      OSRTGenKeyValuePair values[4];
      OSRTGenValueMap map;

      key1.t = OSRTDataType_CharStr;
      key1.u.utf8str = "key1";
      value1.t = OSRTDataType_UnsignedInt;
      value1.u.uintval = 22;
      values[0].key = &key1;
      values[0].value = innerValues[0] = &value1;

      key2.t = OSRTDataType_UnsignedInt;
      key2.u.uintval = 2;
      value2.t = OSRTDataType_ByteArray;
      value2.u.bytestr.numocts = sizeof(bytestr_data);
      value2.u.bytestr.data = bytestr_data;
      values[1].key = &key2;
      values[1].value = innerValues[1] = &value2;

      key3.t = OSRTDataType_CharStr;
      key3.u.utf8str = "key3";
      value3.t = OSRTDataType_CharStr;
      value3.u.utf8str = "this is a test";
      values[2].key = &key3;
      values[2].value = innerValues[2] = &value3;

      key4.t = OSRTDataType_SignedInt;
      key4.u.intval = -4;
      value4.t = OSRTDataType_Array;
      value4.u.array.nitems = 3;
      value4.u.array.values = innerValues;
      values[3].key = &key4;
      values[3].value = &value4;

      map.nitems = 4;
      map.values = values;
      {
         /* definite length test */
         const OSOCTET encoding[] = { 
            0xa4, 0x64, 0x6b, 0x65, 0x79, 0x31, 0x16, 0x02, 
            0x43, 0x01, 0x02, 0x03, 0x64, 0x6b, 0x65, 0x79, 
            0x33, 0x6e, 0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 
            0x73, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74,
            0x23, 0x83, 0x16, 0x43, 0x01, 0x02, 0x03, 0x6e, 
            0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 
            0x61, 0x20, 0x74, 0x65, 0x73, 0x74 
         };
         if (!testValue (&ctxt, &map, encoding, sizeof(encoding))) 
            errcnt++;
      }
      {
         /* indefinite length test */
         const OSOCTET encoding[] = { 
            0xbf, 0x64, 0x6b, 0x65, 0x79, 0x31, 0x16, 0x02, 
            0x43, 0x01, 0x02, 0x03, 0x64, 0x6b, 0x65, 0x79, 
            0x33, 0x6e, 0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 
            0x73, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74,
            0x23, 0x9f, 0x16, 0x43, 0x01, 0x02, 0x03, 0x6e, 
            0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 
            0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0xff, 0xff 
         };
         if (!testValue (&ctxt, &map, encoding, sizeof(encoding))) 
            errcnt++;
      }
   }

   rtxFreeContext (&ctxt);

   return errcnt;
}
