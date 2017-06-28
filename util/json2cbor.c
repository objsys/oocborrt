/* JSON-to-CBOR translation utility */

#include "rtjsonsrc/osrtjson.h"
#include "rtcborsrc/osrtcbor.h"
#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxFile.h"
#include "rtxsrc/rtxHexDump.h"

#include <stdio.h>

static int json2cbor (OSCTXT* pJsonCtxt, OSCTXT* pCborCtxt)
{
   int ret = 0;
   char ch = rtJsonDecPeekChar2 (pJsonCtxt);
   if (ch == 0) return 0;

   switch (ch) {
   case '{':   /* object */
   case '[': { /* array */
      /* JSON object maps to CBOR map, array to CBOR array */
      OSOCTET ub;
      OSOCTET tag = (ch == '{') ? OSRTCBOR_MAP : OSRTCBOR_ARRAY;
      pJsonCtxt->buffer.byteIndex++; /* move buffer index past { */
      pJsonCtxt->level++;

      /* Encode start tag using indefinite length encoding */
      ub = (OSOCTET)((tag << 5)|OSRTCBOR_INDEF);
      ret = rtxWriteBytes (pCborCtxt, &ub, 1);
      if (0 != ret) return LOG_RTERR (pCborCtxt, ret);

      for (;;) {
         if (tag == OSRTCBOR_MAP) {
            OSUTF8CHAR* pElemName = 0;

            /* Parse element name */
            ret = rtJsonDecStringValue (pJsonCtxt, &pElemName);
            if (0 != ret) return LOG_RTERR (pJsonCtxt, ret);

            ret = rtJsonDecMatchChar (pJsonCtxt, ':');
            if (0 != ret) return LOG_RTERR (pJsonCtxt, ret);

            /* Encode CBOR map element name as UTF8 string */
            ret = rtCborEncUTF8Str (pCborCtxt, (const char*)pElemName, 0);
            rtxMemFreePtr (pJsonCtxt, pElemName);
            if (0 != ret) return LOG_RTERR (pCborCtxt, ret);
         }

         /* Make recursive call to parse value */
         ret = json2cbor (pJsonCtxt, pCborCtxt);
         if (0 != ret) {
            OSCTXT* pctxt = 
               (rtxErrGetErrorCnt(pJsonCtxt) > 0) ? pJsonCtxt : pCborCtxt;
            return LOG_RTERR (pctxt, ret);
         }

         /* Check for comma or closing brace */
         ch = rtJsonDecPeekChar2 (pJsonCtxt);
         if (ch == ',' || ch == '}' || ch == ']')  {
            pJsonCtxt->buffer.byteIndex++; /* move buffer index past char */
            if (ch == ',') {
               /* no action */
               continue;
            } 
            else if (pJsonCtxt->level > 0) {
               /* Check for proper closing character */
               if (tag == OSRTCBOR_MAP && ch == ']' ||
                   tag == OSRTCBOR_ARRAY && ch == '}') {
                  const char* expected = (tag == OSRTCBOR_MAP) ? "}" : "]";
                  rtxErrAddStrParm (pJsonCtxt, expected);
                  rtxErrAddStrnParm (pJsonCtxt, &ch, 1);
                  return LOG_RTERR (pJsonCtxt, RTERR_IDNOTFOU);
               }
               pJsonCtxt->level--;

               /* Encode end of indefinite length marker */
               ub = 0xFF;
               ret = rtxWriteBytes (pCborCtxt, &ub, 1);
               if (0 != ret) return LOG_RTERR (pCborCtxt, ret);
               break;
            }
            else return LOG_RTERR (pJsonCtxt, RTERR_UNBAL);
         }
         else if (ch != 0) {
            rtxErrAddStrParm (pJsonCtxt, ", or }");
            rtxErrAddStrnParm (pJsonCtxt, &ch, 1);
            return LOG_RTERR (pJsonCtxt, RTERR_IDNOTFOU);
         }
         else return LOG_RTERR (pJsonCtxt, RTERR_ENDOFBUF);
      }
      break;
   }

   case '"': { /* string */
      OSUTF8CHAR* pString = 0;

      ret = rtJsonDecStringValue (pJsonCtxt, &pString);
      if (0 != ret) return LOG_RTERR (pJsonCtxt, ret);

      ret = rtCborEncUTF8Str (pCborCtxt, (const char*)pString, 0);
      rtxMemFreePtr (pJsonCtxt, pString);
      if (0 != ret) return LOG_RTERR (pCborCtxt, ret);

      break;
   }

   default:
      if (ch == '-' || (ch >= '0' && ch <= '9')) {
         char* pNumberStr;
         ret = rtJsonDecNumberString (pJsonCtxt, &pNumberStr);
         if (0 != ret) return LOG_RTERR (pJsonCtxt, ret);

         ret = rtCborEncNumberStr (pCborCtxt, pNumberStr);
         if (0 != ret) return LOG_RTERR (pCborCtxt, ret);

         rtxMemFreePtr (pJsonCtxt, pNumberStr);
      }
      else if (0 == rtJsonDecMatchToken2 (pJsonCtxt, "true", 4)) {
         ret = rtCborEncBool (pCborCtxt, TRUE);
         if (0 != ret) return LOG_RTERR (pCborCtxt, ret);
      }
      else if (0 == rtJsonDecMatchToken2 (pJsonCtxt, "false", 5)) {
         ret = rtCborEncBool (pCborCtxt, FALSE);
         if (0 != ret) return LOG_RTERR (pCborCtxt, ret);
      }
      else if (0 == rtJsonDecMatchToken2 (pJsonCtxt, "null", 4)) {
         ret = rtCborEncNull (pCborCtxt);
         if (0 != ret) return LOG_RTERR (pCborCtxt, ret);
      }
      else ret = RTERR_INVFORMAT;
   }

   return (0 == ret) ? 0 : LOG_RTERR (pJsonCtxt, ret);
}

int main (int argc, char** argv)
{
   OSCTXT      jsonCtxt, cborCtxt;
   OSOCTET*    pMsgBuf = 0;
   size_t      msglen;
   OSBOOL      verbose = FALSE;
   const char* filename = "message.json";
   const char* outfname = "message.cbor";
   const char* dumpfname = "message.hex";
   int         ret;

   /* Process command line arguments */
   if (argc > 1) {
      int i;
      for (i = 1; i < argc; i++) {
         if (!strcmp (argv[i], "-v")) verbose = TRUE;
         else if (!strcmp (argv[i], "-i")) filename = argv[++i];
         else if (!strcmp (argv[i], "-o")) outfname = argv[++i];
         else if (!strcmp (argv[i], "-dumpfile")) dumpfname = argv[++i];
         else {
            printf ("usage: json2cbor [-v] [-i <filename>] [-o filename]\n");
            printf ("   -v  verbose mode: print trace info\n");
            printf ("   -i <filename>  read encoded msg from <filename>\n");
            printf ("   -o <filename>  write CBOR data to <filename>\n");
            printf ("   -dumpfile <filename>  write CBOR hex dump to <filename>\n");
            return 1;
         }
      }
   }

   /* Initialize context structures */
   ret = rtxInitContext (&jsonCtxt);
   if (ret != 0) {
      rtxErrPrint (&jsonCtxt);
      return ret;
   }
   /* rtxSetDiag (&jsonCtxt, verbose); */

   ret = rtxInitContext (&cborCtxt);
   if (ret != 0) {
      rtxErrPrint (&cborCtxt);
      return ret;
   }
   /* rtxSetDiag (&cborCtxt, verbose); */

   /* Create file input stream */
#if 0
   /* Streaming not supported in open source version
   ret = rtxStreamFileCreateReader (&jsonCtxt, filename);
   */
#else
   /* Read input file into memory buffer */
   ret = rtxFileReadText (&jsonCtxt, filename, &pMsgBuf, &msglen);
   if (0 == ret) {
      ret = rtxInitContextBuffer (&jsonCtxt, pMsgBuf, msglen);
   }
#endif
   if (0 != ret) {
      rtxErrPrint (&jsonCtxt);
      rtxFreeContext (&jsonCtxt);
      rtxFreeContext (&cborCtxt);
      return ret;
   }

   /* Init CBOR output buffer */
   ret = rtxInitContextBuffer (&cborCtxt, 0, 0);
   if (0 != ret) {
      rtxErrPrint (&cborCtxt);
      rtxFreeContext (&jsonCtxt);
      rtxFreeContext (&cborCtxt);
      return ret;
   }

   /* Invoke the translation function */
   ret = json2cbor (&jsonCtxt, &cborCtxt);

   if (0 == ret && jsonCtxt.level != 0) 
      ret = LOG_RTERR (&jsonCtxt, RTERR_UNBAL);

   if (0 == ret && 0 != outfname) {
      /* Write encoded CBOR data to output file */
      int fileret = rtxFileWriteBinary 
         (outfname, cborCtxt.buffer.data, cborCtxt.buffer.byteIndex);

      if (0 != fileret) {
         printf ("unable to write message data to '%s', status = %d\n", 
                 outfname, fileret);
      }
   }

   if (0 == ret && 0 != dumpfname) {
      /* Write hex dump of CBOR data to output file */
      int fileret = rtxHexDumpToNamedFileNoAscii 
         (dumpfname, cborCtxt.buffer.data, cborCtxt.buffer.byteIndex);

      if (0 != fileret) {
         printf ("unable to write hex dump to '%s', status = %d\n", 
                 dumpfname, fileret);
      }
   }

   if (0 != ret) {
      rtxErrPrint (&jsonCtxt);
      rtxErrPrint (&cborCtxt);
   }

   rtxFreeContext (&jsonCtxt);
   rtxFreeContext (&cborCtxt);

   return ret;
}
