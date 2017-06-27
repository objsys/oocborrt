/**
 * Copyright (c) 1997-2017 by Objective Systems, Inc.
 * http://www.obj-sys.com
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/lgpl3.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/

/* Generic value handling functions */

#include "rtxsrc/rtxCommon.h"

const char* rtxGenValueGetIdent (OSRTDataType id)
{
   switch (id) {
   case OSRTDataType_UnsignedInt: return "UnsignedInt";
   case OSRTDataType_SignedInt: return "SignedInt";
   case OSRTDataType_ByteArray: return "ByteArray";
   case OSRTDataType_CharStr: return "CharStr";
   case OSRTDataType_Array: return "Array";
   case OSRTDataType_Bool: return "Boolean";
   case OSRTDataType_Float: return "Float";
   default: return "<unknown>";
   }
}

int rtxGenValueCompare
(OSCTXT* pctxt, const OSRTGenValue* pvalue1, const OSRTGenValue* pvalue2)
{
   if (0 == pvalue1 || 0 == pvalue2) return LOG_RTERR (pctxt, RTERR_NULLPTR);
   if (pvalue1->t != pvalue2->t) {
      rtxCtxtPushElemName (pctxt, OSUTF8("t"));
      rtxErrAddElemNameParm (pctxt);
      rtxErrAddStrParm (pctxt, rtxGenValueGetIdent(pvalue1->t));
      rtxErrAddStrParm (pctxt, rtxGenValueGetIdent(pvalue2->t));
      rtxCtxtPopElemName (pctxt);
      return LOG_RTERR (pctxt, RTERR_VALCMPERR);
   }
   switch (pvalue1->t) {
   case OSRTDataType_UnsignedInt:
      if (pvalue1->u.uintval != pvalue2->u.uintval) {
         rtxCtxtPushElemName (pctxt, OSUTF8("uintval"));
         rtxErrAddElemNameParm (pctxt);
#ifndef _NO_INT64_SUPPORT
         rtxErrAddUInt64Parm (pctxt, pvalue1->u.uintval);
         rtxErrAddUInt64Parm (pctxt, pvalue2->u.uintval);
#else
         rtxErrAddUIntParm (pctxt, pvalue1->u.uintval);
         rtxErrAddUIntParm (pctxt, pvalue2->u.uintval);
#endif
         rtxCtxtPopElemName (pctxt);
         LOG_RTERR (pctxt, RTERR_VALCMPERR);
      }
      break;

   case OSRTDataType_SignedInt:
      if (pvalue1->u.intval != pvalue2->u.intval) {
         rtxCtxtPushElemName (pctxt, OSUTF8("intval"));
         rtxErrAddElemNameParm (pctxt);
#ifndef _NO_INT64_SUPPORT
         rtxErrAddInt64Parm (pctxt, pvalue1->u.intval);
         rtxErrAddInt64Parm (pctxt, pvalue2->u.intval);
#else
         rtxErrAddIntParm (pctxt, pvalue1->u.intval);
         rtxErrAddIntParm (pctxt, pvalue2->u.intval);
#endif
         rtxCtxtPopElemName (pctxt);
         LOG_RTERR (pctxt, RTERR_VALCMPERR);
      }
      break;

   case OSRTDataType_ByteArray:
      if (pvalue1->u.bytestr.numocts != pvalue2->u.bytestr.numocts) {
         rtxCtxtPushElemName (pctxt, OSUTF8("bytestr.numocts"));
         rtxErrAddElemNameParm (pctxt);
         rtxErrAddSizeParm (pctxt, pvalue1->u.bytestr.numocts);
         rtxErrAddSizeParm (pctxt, pvalue2->u.bytestr.numocts);
         rtxCtxtPopElemName (pctxt);
         LOG_RTERRNEW (pctxt, RTERR_VALCMPERR);
      }
      else if (0 != OSCRTLMEMCMP (pvalue1->u.bytestr.data,
                                  pvalue2->u.bytestr.data,
                                  pvalue1->u.bytestr.numocts)) {
         char* diffbuf;
         rtxCtxtPushElemName (pctxt, OSUTF8("bytestr.data"));
         rtxErrAddElemNameParm (pctxt);
         diffbuf = rtxHexDiffToDynString
            (pctxt, pvalue1->u.bytestr.data, pvalue2->u.bytestr.data,
             pvalue1->u.bytestr.numocts);

         if (0 != diffbuf) {
            rtxErrAddStrParm (pctxt, diffbuf);
            rtxMemFreePtr (pctxt, diffbuf);
         }
         rtxCtxtPopElemName (pctxt);
         LOG_RTERRNEW (pctxt, RTERR_BUFCMPERR);
      }
      break;

   case OSRTDataType_CharStr:
      if (0 != OSCRTLSTRCMP (pvalue1->u.utf8str, pvalue2->u.utf8str)) {
         rtxCtxtPushElemName (pctxt, OSUTF8("utf8str.data"));
         rtxErrAddElemNameParm (pctxt);
         rtxErrAddStrParm (pctxt, pvalue1->u.utf8str);
         rtxErrAddStrParm (pctxt, pvalue2->u.utf8str);
         rtxCtxtPopElemName (pctxt);
         LOG_RTERRNEW (pctxt, RTERR_VALCMPERR);
      }
      break;

   case OSRTDataType_Array:
      if (pvalue1->u.array.nitems != pvalue2->u.array.nitems) {
         rtxCtxtPushElemName (pctxt, OSUTF8("array.nitems"));
         rtxErrAddElemNameParm (pctxt);
         rtxErrAddSizeParm (pctxt, pvalue1->u.array.nitems);
         rtxErrAddSizeParm (pctxt, pvalue2->u.array.nitems);
         rtxCtxtPopElemName (pctxt);
         LOG_RTERRNEW (pctxt, RTERR_VALCMPERR);
      }
      else {
         OSSIZE i;
         for (i = 0; i < pvalue1->u.array.nitems; i++) {
            rtxCtxtPushArrayElemName (pctxt, OSUTF8("array.values"), i);
            rtxGenValueCompare
               (pctxt, pvalue1->u.array.values[i], pvalue2->u.array.values[i]);
            rtxCtxtPopArrayElemName (pctxt);
         }
      }
      break;

   case OSRTDataType_Bool:
      if (pvalue1->u.boolval != pvalue2->u.boolval) {
         rtxCtxtPushElemName (pctxt, OSUTF8("boolval"));
         rtxErrAddElemNameParm (pctxt);
         rtxErrAddIntParm (pctxt, pvalue1->u.boolval);
         rtxErrAddIntParm (pctxt, pvalue2->u.boolval);
         rtxCtxtPopElemName (pctxt);
         LOG_RTERRNEW (pctxt, RTERR_VALCMPERR);
      }
      break;

   case OSRTDataType_Float:
      if (pvalue1->u.fltval != pvalue2->u.fltval) {
         rtxCtxtPushElemName (pctxt, OSUTF8("fltval"));
         rtxErrAddElemNameParm (pctxt);
         rtxErrAddDoubleParm (pctxt, pvalue1->u.fltval);
         rtxErrAddDoubleParm (pctxt, pvalue2->u.fltval);
         rtxCtxtPopElemName (pctxt);
         LOG_RTERRNEW (pctxt, RTERR_VALCMPERR);
      }
      break;

   default:
      rtxErrAddIntParm (pctxt, pvalue1->t);
      LOG_RTERR (pctxt, RTERR_NOTSUPP);
   }

   return rtxErrGetStatus (pctxt);
}
