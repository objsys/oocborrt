/**
 * Copyright (c) 2016-2017 by Objective Systems, Inc.
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

#include "rtxsrc/rtxBuffer.h"
#include "rtxsrc/rtxCtype.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxIntStack.h"
#include "rtxsrc/rtxMemBuf.h"
#include "rtxsrc/rtxUTF8.h"
#include "rtjsonsrc/osrtjson.h"

/* Adapted from json_parser (http://fara.cs.uni-potsdam.de/~jsg/json_parser/)

   Modified to match a JSON value, not just JSON array or JSON object.
   Also modified to allow E/e to appear directly after leading 0 (e.g. 0E0).

   Characters are mapped into these character classes. This allows for
   a significant reduction in the size of the state transition table.
*/
enum classes {
    C_SPACE,  /* space */
    C_WHITE,  /* other whitespace */
    C_LCURB,  /* {  */
    C_RCURB,  /* } */
    C_LSQRB,  /* [ */
    C_RSQRB,  /* ] */
    C_COLON,  /* : */
    C_COMMA,  /* , */
    C_QUOTE,  /* " */
    C_BACKS,  /* \ */
    C_SLASH,  /* / */
    C_PLUS,   /* + */
    C_MINUS,  /* - */
    C_POINT,  /* . */
    C_ZERO ,  /* 0 */
    C_DIGIT,  /* 123456789 */
    C_LOW_A,  /* a */
    C_LOW_B,  /* b */
    C_LOW_C,  /* c */
    C_LOW_D,  /* d */
    C_LOW_E,  /* e */
    C_LOW_F,  /* f */
    C_LOW_L,  /* l */
    C_LOW_N,  /* n */
    C_LOW_R,  /* r */
    C_LOW_S,  /* s */
    C_LOW_T,  /* t */
    C_LOW_U,  /* u */
    C_ABCDF,  /* ABCDF */
    C_E,      /* E */
    C_ETC,    /* everything else */
    C_STAR,   /* * */
    NR_CLASSES
};

/*
  These modes can be pushed on the stack.
*/
typedef enum {
   MODE_UNDEF = 0,
   MODE_ARRAY = 1,
   MODE_DONE = 2,
   MODE_KEY = 3,
   MODE_OBJECT = 4
} JsonParserMode;

#define __   -1     /* the universal error code */

static const signed char ascii_class[128] = {
/*
    This array maps the 128 ASCII characters into character classes.
    The remaining Unicode characters should be mapped to C_ETC.
    Non-whitespace control characters are errors.
*/
    __,      __,      __,      __,      __,      __,      __,      __,
    __,      C_WHITE, C_WHITE, __,      __,      C_WHITE, __,      __,
    __,      __,      __,      __,      __,      __,      __,      __,
    __,      __,      __,      __,      __,      __,      __,      __,

    C_SPACE, C_ETC,   C_QUOTE, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_STAR,   C_PLUS,  C_COMMA, C_MINUS, C_POINT, C_SLASH,
    C_ZERO,  C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT,
    C_DIGIT, C_DIGIT, C_COLON, C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,

    C_ETC,   C_ABCDF, C_ABCDF, C_ABCDF, C_ABCDF, C_E,     C_ABCDF, C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_LSQRB, C_BACKS, C_RSQRB, C_ETC,   C_ETC,

    C_ETC,   C_LOW_A, C_LOW_B, C_LOW_C, C_LOW_D, C_LOW_E, C_LOW_F, C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_ETC,   C_LOW_L, C_ETC,   C_LOW_N, C_ETC,
    C_ETC,   C_ETC,   C_LOW_R, C_LOW_S, C_LOW_T, C_LOW_U, C_ETC,   C_ETC,
    C_ETC,   C_ETC,   C_ETC,   C_LCURB, C_ETC,   C_RCURB, C_ETC,   C_ETC
};

/*
    The state codes.
*/
enum states {
    GO,  /* start    */
    OK,  /* ok       */
    OB,  /* object   */
    KE,  /* key      */
    CO,  /* colon    */
    VA,  /* value    */
    AR,  /* array    */
    ST,  /* string   */
    ES,  /* escape   */
    U1,  /* u1       */
    U2,  /* u2       */
    U3,  /* u3       */
    U4,  /* u4       */
    MI,  /* minus    */
    ZE,  /* zero     */
    IT,  /* integer  */
    FR,  /* fraction */
    E1,  /* e        */
    E2,  /* ex       */
    E3,  /* exp      */
    T1,  /* tr       */
    T2,  /* tru      */
    T3,  /* true     */
    F1,  /* fa       */
    F2,  /* fal      */
    F3,  /* fals     */
    F4,  /* false    */
    N1,  /* nu       */
    N2,  /* nul      */
    N3,  /* null     */
    C1,  /* /        */
    C2,  /* / *     */
    C3,  /* *        */
    FX,  /* *.* *eE* */
    D1,  /* second UTF-16 character decoding started by \ */
    D2,  /* second UTF-16 character proceeded by u */
    NR_STATES
};

enum actions
{
    CB = -10, /* comment begin */
    CE = -11, /* comment end */
    FA = -12, /* false */
    TR = -13, /* false */
    NU = -14, /* null */
    DE = -15, /* double detected by exponent e E */
    DF = -16, /* double detected by fraction . */
    SB = -17, /* string begin */
    MX = -18, /* integer detected by minus */
    ZX = -19, /* integer detected by zero */
    IX = -20, /* integer detected by 1-9 */
    EX = -21, /* next char is escaped */
    UC = -22  /* Unicode character read */
};


static const signed char state_transition_table[NR_STATES][NR_CLASSES] = {
/*
    The state transition table takes the current state and the current symbol,
    and returns either a new state or an action. An action is represented as a
    negative number. A JSON text is accepted if at the end of the text the
    state is OK and if the mode is MODE_DONE.

 sp ws {  }  [  ]  :  ,  "  \  /  +  -  .  0  dg
 a  b  c  d  e  f  l  n  r  s  t  u  AF E  et * */
/*start  GO*/
{GO,GO,-6,__,-5,__,__,__,SB,__,CB,__,MX,__,ZX,IX,
 __,__,__,__,__,FA,__,NU,__,__,TR,__,__,__,__,__},

/*ok     OK*/
{OK,OK,__,-8,__,-7,__,-3,__,__,CB,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*object OB*/
{OB,OB,__,-9,__,__,__,__,SB,__,CB,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*key    KE*/
{KE,KE,__,__,__,__,__,__,SB,__,CB,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*colon  CO*/
{CO,CO,__,__,__,__,-2,__,__,__,CB,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*value  VA*/
{VA,VA,-6,__,-5,__,__,__,SB,__,CB,__,MX,__,ZX,IX,
 __,__,__,__,__,FA,__,NU,__,__,TR,__,__,__,__,__},

/*array  AR*/
{AR,AR,-6,__,-5,-7,__,__,SB,__,CB,__,MX,__,ZX,IX,
 __,__,__,__,__,FA,__,NU,__,__,TR,__,__,__,__,__},

/*string ST*/
{ST,__,ST,ST,ST,ST,ST,ST,-4,EX,ST,ST,ST,ST,ST,ST,
 ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST,ST},

/*escape ES*/
{__,__,__,__,__,__,__,__,ST,ST,ST,__,__,__,__,__,
 __,ST,__,__,__,ST,__,ST,ST,__,ST,U1,__,__,__,__},

/*u1     U1*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,U2,U2,
 U2,U2,U2,U2,U2,U2,__,__,__,__,__,__,U2,U2,__,__},

/*u2     U2*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,U3,U3,
 U3,U3,U3,U3,U3,U3,__,__,__,__,__,__,U3,U3,__,__},

/*u3     U3*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,U4,U4,
 U4,U4,U4,U4,U4,U4,__,__,__,__,__,__,U4,U4,__,__},

/*u4     U4*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,UC,UC,
 UC,UC,UC,UC,UC,UC,__,__,__,__,__,__,UC,UC,__,__},

/*minus  MI*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,ZE,IT,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*zero   ZE*/
{OK,OK,__,-8,__,-7,__,-3,__,__,CB,__,__,DF,__,__,
 __,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__,__},

/*int    IT*/
{OK,OK,__,-8,__,-7,__,-3,__,__,CB,__,__,DF,IT,IT,
 __,__,__,__,DE,__,__,__,__,__,__,__,__,DE,__,__},

/*frac   FR*/
{OK,OK,__,-8,__,-7,__,-3,__,__,CB,__,__,__,FR,FR,
 __,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__,__},

/*e      E1*/
{__,__,__,__,__,__,__,__,__,__,__,E2,E2,__,E3,E3,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*ex     E2*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,E3,E3,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*exp    E3*/
{OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,__,E3,E3,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*tr     T1*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 __,__,__,__,__,__,__,__,T2,__,__,__,__,__,__,__},

/*tru    T2*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,T3,__,__,__,__},

/*true   T3*/
{__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,
 __,__,__,__,OK,__,__,__,__,__,__,__,__,__,__,__},

/*fa     F1*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 F2,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*fal    F2*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 __,__,__,__,__,__,F3,__,__,__,__,__,__,__,__,__},

/*fals   F3*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,F4,__,__,__,__,__,__},

/*false  F4*/
{__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,
 __,__,__,__,OK,__,__,__,__,__,__,__,__,__,__,__},

/*nu     N1*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,N2,__,__,__,__},

/*nul    N2*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 __,__,__,__,__,__,N3,__,__,__,__,__,__,__,__,__},

/*null   N3*/
{__,__,__,__,__,__,__,__,__,__,CB,__,__,__,__,__,
 __,__,__,__,__,__,OK,__,__,__,__,__,__,__,__,__},

/*/      C1*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,C2},

/*/*     C2*/
{C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,
 C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C3},

/**      C3*/
{C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,CE,C2,C2,C2,C2,C2,
 C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C2,C3},

/*_.     FX*/
{OK,OK,__,-8,__,-7,__,-3,__,__,__,__,__,__,FR,FR,
 __,__,__,__,E1,__,__,__,__,__,__,__,__,E1,__,__},

/*\      D1*/
{__,__,__,__,__,__,__,__,__,D2,__,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__},

/*\      D2*/
{__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
 __,__,__,__,__,__,__,__,__,__,__,U1,__,__,__,__}
};

static int getErrCode (OSCTXT* pctxt)
{
   switch (pctxt->state) {
   case GO:
      switch (pctxt->lastChar) {
      case '{': case '}': case '[': case ']':
         return RTERR_UNBAL;
      default:
         return RTERR_INVCHAR;
      }

   case KE:
   case OB: return RTERR_EXPNAME;
   case AR: return RTERR_UNBAL;
   case CO:
      rtxErrAddStrnParm (pctxt, ":", 1);
      rtxErrAddStrnParm (pctxt, (const char*)&pctxt->lastChar, 1);
      return RTERR_IDNOTFOU;

   /* \uXXXX\uYYYY */
   case U1: case U2: case U3:case U4: case D1: case D2:
      return RTERR_UNICODE;

   /* true, false */
   case T1: case T2: case T3: case F1: case F2: case F3: case F4:
      return RTERR_INVBOOL;

   /* null */
   case N1: case N2: case N3:
      return RTERR_INVNULL;

   /* minus, integer, fraction, exponent */
   case MI: case ZE: case IT: case FR: case E1: case E2: case E3:
      return RTERR_INVREAL;

   default:;
   }

   /* Default code if matches none above */

   return RTERR_INVCHAR;
}

#define IS_HIGH_SURROGATE(uc) (((uc) & 0xFC00) == 0xD800)
#define IS_LOW_SURROGATE(uc)  (((uc) & 0xFC00) == 0xDC00)
#define DECODE_SURROGATE_PAIR(hi,lo) \
((((hi) & 0x3FF) << 10) + ((lo) & 0x3FF) + 0x10000)
static const OSOCTET utf8_lead_bits[4] = { 0x00, 0xC0, 0xE0, 0xF0 };

static int parseUnicodeChar
(OSCTXT* pctxt, const OSOCTET* buffer, size_t buflen, OSUINT16* pUTF16hs)
{
   OSINT32  i;
   OSUINT32 uc = 0;
   char* p;

   if (0 == buffer || 0 == pUTF16hs)
      return RTERR_NULLPTR;

   if (buflen < 6)
      return LOG_RTERR (pctxt, RTERR_ENDOFBUF);

   p = (char*) &buffer[buflen - 4];

   for (i = 12; i >= 0; i -= 4, ++p) {
      OSUINT32 x = *p;

      if (x >= 'a') {
         x -= ('a' - 10);
      }
      else if (x >= 'A') {
         x -= ('A' - 10);
      }
      else {
         x &= ~0x30u;
      }

      if (x >= 16) return LOG_RTERR (pctxt, RTERR_INVCHAR);

      uc |= x << i;
   }

   /* Decode */
   if (0 != *pUTF16hs) { /* high surrogate */
      if (IS_LOW_SURROGATE (uc)) {
         *pUTF16hs = 0;
      }
      else {
         /* high surrogate without a following low surrogate */
         return LOG_RTERR (pctxt, RTERR_UNICODE);
      }
   }
   else {
      if (uc < 0x80) {
      }
      else if (uc < 0x800) {
      }
      else if (IS_HIGH_SURROGATE(uc)) {
         /* save the high surrogate and wait for the low surrogate */
         *pUTF16hs = (OSUINT16) uc;
         return 0;
      }
      else if (IS_LOW_SURROGATE(uc)) {
         /* low surrogate without a preceding high surrogate */
         return LOG_RTERR (pctxt, RTERR_UNICODE);
      }
      else {
      }
   }

   return 0;
}

static int popMode (OSRTIntStack* pstack, JsonParserMode expectedMode)
{
   OSINT32 mode = 0;
   int stat = rtxIntStackPeek (pstack, &mode);
   if (0 != stat) return LOG_RTERR (pstack->pctxt, stat);

   if ((JsonParserMode)mode == expectedMode) {
      rtxIntStackPop (pstack, 0);
      return 0;
   }
   else return LOG_RTERR (pstack->pctxt, RTERR_IDNOTFOU);
}

#define MATCHING_NUMBER(pctxt) ( (pctxt)->state == ZE || \
                              (pctxt)->state == IT || \
                              (pctxt)->state == FR || \
                              (pctxt)->state == E3 )

/* return codes: 0 = OK, continue; 1 = done; negative = error
   (stack underflow) */
static int jsonParserDone (OSCTXT* pctxt, OSRTIntStack* pstack)
{
   if (pctxt->state == OK || pctxt->state == GO) {
      OSINT32 mode = 0;
      int stat = rtxIntStackPeek (pstack, &mode);
      if (0 != stat) return LOG_RTERR (pstack->pctxt, stat);

      if (mode == MODE_DONE) {
         rtxIntStackPop (pstack, 0);
         return 1;
      }
   }

   return 0;
}


/* Match JSON object, array, or value. (Previously, matched only object or
   array.)
*/
static int jsonDecAnyType (OSCTXT* pctxt, OSRTMEMBUF* pmembuf)
{
   OSRTIntStack parseStack;
   OSINT32 next_class, next_state, mode, stat;
   OSUINT16 utf16hs = 0;
   OSOCTET savedState = pctxt->state;
   OSOCTET beforeCommentState = OK;

   rtxIntStackInit (pctxt, &parseStack, 100);

   rtJsonDecSkipWhitespace (pctxt);
   pctxt->state = GO;

   stat = rtxIntStackPush (&parseStack, MODE_DONE);
   if (0 != stat) return LOG_RTERR (pctxt, stat);

   for (;;) {
      stat = rtxReadBytes (pctxt, &pctxt->lastChar, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);

      /* Determine the character class */
      if (pctxt->lastChar >= 128) {
         next_class = C_ETC;
      }
      else {
         next_class = ascii_class[pctxt->lastChar];
         if (next_class <= __) {
            return LOG_RTERR (pctxt, getErrCode(pctxt));
         }
      }

      stat = rtxMemBufAppend (pmembuf, &pctxt->lastChar, 1);
      if (0 != stat) return LOG_RTERR (pctxt, stat);

      /* Get the next state from the state transition table */

      next_state = state_transition_table[pctxt->state][next_class];
      if (next_state >= 0) {
         rtxIntStackPeek(&parseStack, &mode);

         if (MATCHING_NUMBER(pctxt) && next_state == OK && mode == MODE_DONE)
         {
            /* We're just matching a number. Push character back to be
            reread again and chop it from the buffer. */
            OSRTASSERT(pctxt->buffer.byteIndex > 0);
            pctxt->buffer.byteIndex--;
            rtxMemBufCut(pmembuf, rtxMemBufGetDataLen(pmembuf)-1, 1);
         }
         pctxt->state = (signed char)next_state;
      }
      else {
         /* Perform action */
         switch (next_state) {
         case UC: {
            int buflen;
            OSOCTET* bufptr = rtxMemBufGetData (pmembuf, &buflen);

            stat = parseUnicodeChar (pctxt, bufptr, buflen, &utf16hs);
            if (0 != stat) return LOG_RTERR (pctxt, stat);

            /* check if we need to read a second UTF-16 char */
            pctxt->state = (OSOCTET)((0 != utf16hs) ? D1 : ST);
         }
         break;

         /* escaped char */
         case EX:
            pctxt->state = ES;
            break;

         /* integer detected by minus */
         case MX:
            pctxt->state = MI;
            break;

         /* integer detected by zero */
         case ZX:
            pctxt->state = ZE;
            break;

         /* integer detected by 1-9 */
         case IX:
            pctxt->state = IT;
            break;

         /* floating point number detected by exponent */
         case DE:
            pctxt->state = E1;
            break;

         /* floating point number detected by fraction */
         case DF:
            pctxt->state = FX;
            break;

         /* string begin " */
         case SB:
            pctxt->state = ST;
            break;

         /* n */
         case NU:
            pctxt->state = N1;
            break;

         /* f */
         case FA:
            pctxt->state = F1;
            break;

         /* t */
         case TR:
            pctxt->state = T1;
            break;

         /* closing comment */
         case CE:
            pctxt->state = beforeCommentState;
            break;

         /* opening comment  */
         case CB:
            rtxIntStackPeek (&parseStack, &mode);
            switch (mode) {
            case MODE_ARRAY:
            case MODE_OBJECT:
               switch (pctxt->state) {
               case VA:
               case AR:
                  beforeCommentState = pctxt->state;
                  break;
               default:
                  beforeCommentState = OK;
                  break;
               }
               break;
            default:
               beforeCommentState = pctxt->state;
               break;
            }
            pctxt->state = C1;
            break;

         /* empty } */
         case -9:
            stat = popMode (&parseStack, MODE_KEY);
            if (0 != stat) return LOG_RTERR (pctxt, stat);
            pctxt->state = OK;
            break;

         /* } */
         case -8:
            stat = rtxIntStackPeek(&parseStack, &mode);
            if (0 != stat) return LOG_RTERR(pctxt, stat);

            if (MATCHING_NUMBER(pctxt) && mode == MODE_DONE) {
               /* We're just matching a number. Push character back to be
                  reread again and chop it from the buffer. */
               OSRTASSERT(pctxt->buffer.byteIndex > 0);
               pctxt->buffer.byteIndex--;
               rtxMemBufCut(pmembuf, rtxMemBufGetDataLen(pmembuf) - 1, 1);
            }
            else {
               stat = popMode(&parseStack, MODE_OBJECT);
               if (0 != stat) return LOG_RTERR(pctxt, RTERR_UNBAL);
            }

            pctxt->state = OK;
            break;

         /* ] */
         case -7:
            stat = rtxIntStackPeek(&parseStack, &mode);
            if (0 != stat) return LOG_RTERR(pctxt, stat);

            if (MATCHING_NUMBER(pctxt) && mode == MODE_DONE) {
               /* We're just matching a number. Push character back to be
               reread again and chop it from the buffer. */
               OSRTASSERT(pctxt->buffer.byteIndex > 0);
               pctxt->buffer.byteIndex--;
               rtxMemBufCut(pmembuf, rtxMemBufGetDataLen(pmembuf) - 1, 1);
            }
            else {
               stat = popMode(&parseStack, MODE_OBJECT);
               if (0 != stat) return LOG_RTERR(pctxt, RTERR_UNBAL);
            }

            pctxt->state = OK;
            break;

         /* { */
         case -6:
            stat = rtxIntStackPush (&parseStack, MODE_KEY);
            if (0 != stat) return LOG_RTERR (pctxt, stat);
            pctxt->state = OB;
            break;

         /* [ */
         case -5:
            stat = rtxIntStackPush (&parseStack, MODE_ARRAY);
            if (0 != stat) return LOG_RTERR (pctxt, stat);
            pctxt->state = AR;
            break;

         /* string end " */
         case -4:
            stat = rtxIntStackPeek (&parseStack, &mode);
            if (0 != stat) return LOG_RTERR (pctxt, stat);

            switch (mode) {
            case MODE_KEY:
               pctxt->state = CO;
               break;
            case MODE_DONE:
            case MODE_ARRAY:
            case MODE_OBJECT:
               pctxt->state = OK;
               break;
            default:
               return LOG_RTERR (pctxt, RTERR_UNEXPELEM);
            }
            break;

         /* , */
         case -3:
            stat = rtxIntStackPeek (&parseStack, &mode);
            if (0 != stat) return LOG_RTERR (pctxt, stat);

            switch (mode) {
            case MODE_OBJECT:
               /* A comma causes a switch from object to key mode */
               rtxIntStackPop (&parseStack, 0);
               stat = rtxIntStackPush (&parseStack, MODE_KEY);
               if (0 != stat) return LOG_RTERR (pctxt, stat);
               pctxt->state = KE;
               break;
            case MODE_ARRAY:
               pctxt->state = VA;
               break;
            case MODE_DONE:
               if (MATCHING_NUMBER(pctxt))
               {
                  /* We're just matching a number. Push character back to be
                  reread again and chop it from the buffer. */
                  OSRTASSERT(pctxt->buffer.byteIndex > 0);
                  pctxt->buffer.byteIndex--;
                  rtxMemBufCut(pmembuf, rtxMemBufGetDataLen(pmembuf) - 1, 1);
                  pctxt->state = OK;
                  break;
               }
               //else : fall-through to the default case and report error

            default:
               return LOG_RTERR (pctxt, RTERR_UNEXPELEM);
            }
            break;

         /* : */
         case -2:
            /* A colon causes a switch from key to object mode */
            stat = popMode (&parseStack, MODE_KEY);
            if (0 != stat) return LOG_RTERR (pctxt, stat);

            stat = rtxIntStackPush (&parseStack, MODE_OBJECT);
            if (0 != stat) return LOG_RTERR (pctxt, stat);

            pctxt->state = VA;
            break;

         /* Bad action */
         default:
            return LOG_RTERR (pctxt, getErrCode(pctxt));
         }
      }

      stat = jsonParserDone (pctxt, &parseStack);
      if (0 != stat) break;
   }

   pctxt->state = savedState;

   return (stat < 0) ? stat : 0;
}

int rtJsonDecAnyElem (OSCTXT* pctxt, OSUTF8CHAR** ppvalue)
{
   OSRTMEMBUF membuf;
   OSOCTET ub;
   int stat;

   rtxMemBufInit (pctxt, &membuf, 128);

   /* Decode element name. Name is assumed to not contain any escaped
      characters. */
   stat = rtJsonDecMatchChar (pctxt, '"');

   if (0 == stat) {
      stat = rtxMemBufAppend (&membuf, OSUTF8("\""), 1);
   }

   if (0 == stat) {
      for (;;) {
         stat = rtxReadBytes (pctxt, &ub, 1);
         if (0 != stat) break;

         stat = rtxMemBufAppend (&membuf, &ub, 1);
         if (0 != stat) break;

         else if ('"' == ub) break;
      }
   }

   /* Match colon character */
   if (0 == stat) {
      stat = rtJsonDecMatchChar (pctxt, ':');
   }
   if (0 == stat) {
      stat = rtxMemBufAppend (&membuf, OSUTF8(":"), 1);
   }

   /* Decode content */
   if (0 == stat) {
      stat = jsonDecAnyType (pctxt, &membuf);
   }

   if (0 == stat) {
      stat = rtxMemBufAppend (&membuf, OSUTF8("\0"), 1);
   }

   if (0 != stat) {
      rtxMemBufAppend (&membuf, OSUTF8("\0"), 1);
      rtxMemBufFree (&membuf);
      return LOG_RTERR (pctxt, stat);
   }
   else if (0 != ppvalue) {
      *ppvalue = rtxMemBufGetData (&membuf, 0);
   }
   else {
      rtxMemBufFree (&membuf);
   }

   return stat;
}

/* This version of rtJsonDecAnyElem assumes the element name has been
   pushed on the element name stack in the context.  This will be the
   case if rtJsonGetElemIdx is called prior to calling this function. */

int rtJsonDecAnyElem2 (OSCTXT* pctxt, OSUTF8CHAR** ppvalue)
{
   const OSUTF8CHAR* elemName = rtxCtxtPeekElemName (pctxt);
   OSRTMEMBUF membuf;
   int stat = 0;

   rtxMemBufInit (pctxt, &membuf, 128);

   if (0 != elemName) {
      stat = rtxMemBufAppend (&membuf, OSUTF8("\""), 1);
      if (0 == stat) {
         stat = rtxMemBufAppend
            (&membuf, (OSOCTET*)elemName, rtxUTF8LenBytes(elemName));
      }
      if (0 == stat) {
         stat = rtxMemBufAppend (&membuf, OSUTF8("\":"), 2);
      }
   }

   /* Decode content */
   if (0 == stat) {
      stat = jsonDecAnyType (pctxt, &membuf);
   }

   if (0 == stat) {
      stat = rtxMemBufAppend (&membuf, OSUTF8("\0"), 1);
   }

   if (0 != stat) {
      rtxMemBufAppend (&membuf, OSUTF8("\0"), 1);
      rtxMemBufFree (&membuf);
      return LOG_RTERR (pctxt, stat);
   }
   else if (0 != ppvalue) {
      *ppvalue = rtxMemBufGetData (&membuf, 0);
   }
   else {
      rtxMemBufFree (&membuf);
   }

   return stat;
}

int rtJsonDecAnyType (OSCTXT* pctxt, OSUTF8CHAR** ppvalue)
{
   OSRTMEMBUF membuf;
   int stat;

   rtxMemBufInit (pctxt, &membuf, 128);

   /* Decode content */
   stat = jsonDecAnyType (pctxt, &membuf);

   if (0 == stat) {
      stat = rtxMemBufAppend (&membuf, OSUTF8("\0"), 1);
   }

   if (0 != stat) {
      rtxMemBufFree (&membuf);
      return LOG_RTERR (pctxt, stat);
   }
   else if (0 != ppvalue) {
      *ppvalue = rtxMemBufGetData (&membuf, 0);
   }
   else {
      rtxMemBufFree (&membuf);
   }

   return stat;
}
