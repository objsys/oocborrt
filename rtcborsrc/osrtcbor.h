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
/*! \mainpage
 * <h1>C CBOR Runtime Library Functions</h1>
 *
 * The <b>C run-time Concise Binary Object Representation (CBOR) library</b>
 * contains functions used to encode/decode CBOR data as defined in IETF RFC
 * 7049. These functions are identified by their <i>rtCbor</i> prefixes.
 */
/**
 * @file osrtcbor.h
 * CBOR low-level C encode/decode functions.
 */
#ifndef _OSRTCBOR_H_
#define _OSRTCBOR_H_

#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxGenValueType.h"

/* Major types (2.1) */
typedef enum {
   OSRTCBOR_UINT,
   OSRTCBOR_NEGINT,
   OSRTCBOR_BYTESTR,
   OSRTCBOR_UTF8STR,
   OSRTCBOR_ARRAY,
   OSRTCBOR_MAP,
   OSRTCBOR_TAGGED,
   OSRTCBOR_FLOAT
} OSRTCBOR_MajorType;

/* Unsigned integer tags */
typedef enum {
   OSRTCBOR_UINT8_ID = 24,
   OSRTCBOR_UINT16_ID,
   OSRTCBOR_UINT32_ID,
   OSRTCBOR_UINT64_ID
} OSRTCBOR_IntType;

/* CBOR Boolean and Null values */
#define OSRTCBOR_FALSE 20
#define OSRTCBOR_TRUE  21
#define OSRTCBOR_NULL  22

/* CBOR Encoded Boolean, Null, Unknown, and Float values */
#define OSRTCBOR_FALSEENC 0xF4
#define OSRTCBOR_TRUEENC  0xF5
#define OSRTCBOR_NULLENC  0xF6
#define OSRTCBOR_UNKENC   0xF7
#define OSRTCBOR_FLT16ENC 0xF9
#define OSRTCBOR_FLT32ENC 0xFA
#define OSRTCBOR_FLT64ENC 0xFB

/* Indefinite length special values */
#define OSRTCBOR_INDEF 0x1F
#define OSRTCBOR_EOC   0xFF

/* This macro will test for an EOC (end-of-context) marker */
#define OSRTCBOR_MATCHEOC(pctxt) \
(( (pctxt)->buffer.byteIndex + 1 <= (pctxt)->buffer.size ) && \
 ( (pctxt)->buffer.data[(pctxt)->buffer.byteIndex] == OSRTCBOR_EOC ))

/* Flag mask values */
#define OSRTCBORM_INDEF 0x80000000  /* indefinite length encoding */
#define OSRTCBORM_CANON 0x40000000  /* canonical encoding */

struct OSBigInt;
struct OSBigFloat;

/**
 * This function will decode array content into a pointer array data variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pPtrArray   Pointer to a pointer array structure to receive
 *                       decoded content.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecArray
(OSCTXT* pctxt, OSOCTET byte1, OSRTGenPtrArray* pPtrArray);

/**
 * This function will decode either a CBOR decimal fraction (tag 4) or CBOR
 * bigfloat (tag 5) into the given OSBigFloat.  The OSBigFloat's base10 flag
 * will be true if a decimal fraction was decoded and false if a bigfloat
 * was decoded.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream. This must
 *                    indicate major type 6 with tag 4 or 5.
 * @param pvalue      Value to encode.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecBigFloat (OSCTXT* pctxt, OSOCTET byte1,
                                 struct OSBigFloat* pvalue);

/**
 * This function will decode either a CBOR int or CBOR bignum into an OSBigInt.
 *
 * @param pctxt       Pointer to context block structure.
 * @param byte1       Initial byte parsed from the input stream. This must
 *                    indicate major type 0 or 1, or else indicate major type
 *                    6 with tag 2 or 3.
 * @param pvalue      Pointer to the OSBigInt into which the value should be
 *                    decoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecBigInt (OSCTXT* pctxt, OSOCTET byte1,
                              struct OSBigInt* pvalue);

/**
 * This function will decode a boolean value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pvalue      Pointer to boolean variable to receive decoded result.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecBool (OSCTXT* pctxt, OSOCTET byte1, OSBOOL* pvalue);

/**
 * This function will decode byte string content into a static buffer.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pnumocts    Pointer to size-typed variable to receive decoded length.
 * @param buffer      Static byte array into which data will be read.
 * @param bufsiz      Size of the static byte array into which the data will
 *                       be read.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecByteStr
(OSCTXT* pctxt, OSOCTET byte1, OSSIZE* pnumocts, OSOCTET* buffer, OSSIZE bufsiz);

/**
 * This function will decode byte string content into a dynamic buffer.
 * Memory for the data is allocated using the rtxMemAlloc function and will
 * be freed when the context is freed or a specifc call to rtmMemFree or
 * rtxMemFreePtr is done.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pOctStr     Pointer to structure to recieve decoded data.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecDynByteStr
(OSCTXT* pctxt, OSOCTET byte1, OSDynOctStr64* pOctStr);

/**
 * This function will decode an ISO 8601 date/time string into the given
 * date/time structure.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pvalue      Pointer to date/time structure variable to receive
 *                       decoded result.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecDateTimeStr
(OSCTXT* pctxt, OSOCTET byte1, OSNumDateTime* pvalue);

/**
 * This function will decode a floating-pont value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pvalue      Pointer to float variable to receive decoded result.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecFloat (OSCTXT* pctxt, OSOCTET byte1, OSDOUBLE* pvalue);

/**
 * This function will decode an integer value into a 16-bit signed
 * integer variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pvalue      Pointer to integer variable to receive decode value.
 *                       May be set to null to skip field.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecInt16 (OSCTXT* pctxt, OSOCTET byte1, OSINT16* pvalue);

/**
 * This function will decode an integer value into a 32-bit signed
 * integer variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pvalue      Pointer to integer variable to receive decode value.
 *                       May be set to null to skip field.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecInt32 (OSCTXT* pctxt, OSOCTET byte1, OSINT32* pvalue);

/**
 * This function will decode an integer value into a 32-bit signed
 * integer variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pvalue      Pointer to integer variable to receive decode value.
 *                       May be set to null to skip field.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecInt64 (OSCTXT* pctxt, OSOCTET byte1, OSINT64* pvalue);

/**
 * This function will decode CBOR map content into a map data variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pmap        Pointer to a map data structure to receive
 *                       decoded content.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecMap (OSCTXT* pctxt, OSOCTET byte1, OSRTGenValueMap* pmap);

/**
 * This function will decode a null value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecNull (OSCTXT* pctxt, OSOCTET byte1);

/**
 * This function will decode unsigned integer content into a size-typed
 * variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param value       The value to be encoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecSize (OSCTXT* pctxt, OSOCTET byte1, OSSIZE* pvalue);

/**
 * This function will skip the field at the current buffer or stream
 * location.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecSkipField (OSCTXT* pctxt, OSOCTET byte1);

/**
 * This function will decode a time value.  Time is measured as the
 * numerical representation of seconds relative to 1970-01-01T00:00Z in UTC
 * time.  This function decodes the out tag and the value itself
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pvalue      Pointer to time variable to received decoded content.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecTime (OSCTXT* pctxt, OSOCTET byte1, time_t* pvalue);

/**
 * This function will decode a time value.  Time is measured as the
 * numerical representation of seconds relative to 1970-01-01T00:00Z in UTC
 * time.  This function decodes content only, not tags.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param pvalue      Pointer to time variable to received decoded content.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecTimeValue (OSCTXT* pctxt, OSOCTET byte1, time_t* pvalue);

/**
 * This function will decode the given unsigned integer into a 16-bit
 * unsigned integer variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param value       The value to be encoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecUInt16 (OSCTXT* pctxt, OSOCTET byte1, OSUINT16* pvalue);

/**
 * This function will decode the given unsigned integer into a 32-bit
 * unsigned integer variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param value       The value to be encoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecUInt32 (OSCTXT* pctxt, OSOCTET byte1, OSUINT32* pvalue);

/**
 * This function will decode UTF8 string content into a static character
 * array.  The array must be large enough to hold the decoded contents plus
 * a null-terminator byte.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param buffer      Character array buffer to receive decoded content.
 * @param bufsiz      Size of the buffer.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecUTF8Str
(OSCTXT* pctxt, OSOCTET byte1, char* buffer, OSSIZE bufsiz);

/**
 * This function will decode UTF8 string content into a dynamic character
 * string variable. Memory for the data is allocated using the rtxMemAlloc
 * function and will be freed when the context is freed or a specific call
 * to rtmMemFree or rtxMemFreePtr is done.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.
 * @param ppUtf8Str   Pointer to a char* variable to recieve decoded content.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecDynUTF8Str
(OSCTXT* pctxt, OSOCTET byte1, char** ppUtf8Str);

/**
 * This function will decode a CBOR encoded value of any type.  The result
 * wll be returned in a generic CBOR value structure.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Pointer to generic value structure to receive decoded
 *                       result.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecValue (OSCTXT* pctxt, OSRTGenValue* pvalue);

/**
 * This function will encode an array of generic values.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param values      Pointer to array of pointers to values to be encoded.
 * @param nitems      Number of items in the array to encode.
 * @param flags       Flags: indef len, canonical, etc.. See "Flag mask values"
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncArray
(OSCTXT* pctxt, const OSRTGenValue** values, OSSIZE nitems, OSUINT32 flags);


/**
 * This function will encode either a CBOR decimal fraction (tag 4) or CBOR
 * bigfloat (tag 5), depending on the OSBigFloat's base10 flag.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Value to encode.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncBigFloat (OSCTXT* pctxt, struct OSBigFloat* pvalue);

/**
 * This function will encode either a CBOR int or a CBOR bignum, using a
 * CBOR int when possible. To force encoding of a CBOR bignum, use
 * rtCborEncBigNum instead.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Big integer value to encode.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncBigInt (OSCTXT* pctxt, struct OSBigInt* pvalue);

/**
 * This function will encode a CBOR bignum.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Big integer value to encode.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncBigNum (OSCTXT* pctxt, struct OSBigInt* pvalue);

/**
 * This function will encode a boolean value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       Boolean value to encode.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncBool (OSCTXT* pctxt, OSBOOL value);

/**
 * This function will encode the given byte string value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Pointer to array of bytes to be encoded.
 * @param nbytes      Number of bytes to encode.
 * @param segsize     Size of a segment to be encoded.  If zero, standard
 *                       length encoding will be done.  Otherwise, indefinite
 *                       length encoding will be done (2.2.2) with each
 *                       segment being of this length.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncByteStr
(OSCTXT* pctxt, const OSOCTET* value, OSSIZE nbytes, OSSIZE segsize);

/**
 * This function will encode the given date/time value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Pointer to date/time structure containing value to encode.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncDateTimeStr (OSCTXT* pctxt, const OSNumDateTime* pvalue);

/**
 * This function will encode the given floating point value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       Value to encode.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncFloat (OSCTXT* pctxt, OSDOUBLE value);

/**
 * This function will encode the given signed integer value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       The value to be encoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncInt32 (OSCTXT* pctxt, OSINT32 value);

/**
 * This function will encode a map of generic values.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pmap        Pointer to map data structure to be encoded.
 * @param flags       Flags: indef len, canonical, etc.. See "Flag mask values"
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncMap
(OSCTXT* pctxt, const OSRTGenValueMap* pmap, OSUINT32 flags);

/**
 * This function will encode a null value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncNull (OSCTXT* pctxt);

/**
 * This function will encode a numeric string value.  The string is assumed
 * to be a base-10 stirng in JSON number format.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Pointer to character string to be encoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncNumberStr (OSCTXT* pctxt, const char* pvalue);

/**
 * This function will encode the given size value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       The value to be encoded.
 * @param majorType   Major type code.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncSize
(OSCTXT* pctxt, OSSIZE value, OSRTCBOR_MajorType majorType);

/**
 * This function will encode the given time value.  Time is measured as the
 * numerical representation of seconds relative to 1970-01-01T00:00Z in UTC
 * time.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       The value to be encoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncTime (OSCTXT* pctxt, time_t value);

/**
 * This function will encode the given time value.  Time is measured as the
 * numerical representation of seconds relative to 1970-01-01T00:00Z in UTC
 * time.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       The value to be encoded.
 * @param majorType   Major type code.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncTimeValue
(OSCTXT* pctxt, time_t value, OSRTCBOR_MajorType majorType);

/**
 * This function will encode the given unsigned integer.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       The value to be encoded.
 * @param majorType   Major type code.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncUInt32
(OSCTXT* pctxt, OSUINT32 value, OSRTCBOR_MajorType majorType);

/**
 * This function will encode the given UTF-8 character string value.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Pointer to character string to be encoded.
 * @param segsize     Size of a segment to be encoded.  If zero, standard
 *                       length encoding will be done.  Otherwise, indefinite
 *                       length encoding will be done (2.2.2) with each
 *                       segment being of this length.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncUTF8Str
(OSCTXT* pctxt, const char* pvalue, OSSIZE segsize);

#ifndef _NO_INT64_SUPPORT
/**
 * This function will decode the given unsigned integer into a 64-bit
 * unsigned integer variable.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param byte1       Initial byte parsed from the input stream.  The major
 *                    type must be zero.
 * @param value       The value to be encoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborDecUInt64 (OSCTXT* pctxt, OSOCTET byte1, OSUINT64* pvalue);

/**
 * This function will encode the given 64-bit signed integer.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       The value to be encoded.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncInt64 (OSCTXT* pctxt, OSINT64 value);

/**
 * This function will encode the given 64-bit unsigned integer.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param value       The value to be encoded.
 * @param majorType   Major type code.
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncUInt64
(OSCTXT* pctxt, OSUINT64 value, OSRTCBOR_MajorType majorType);

/**
 * This function will encode a CBOR value of any type.
 *
 * @param pctxt       Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pvalue      Pointer to generic value structure containing data
 *                       to be encoded.
 * @param flags       Flags: indef len, canonical, etc.. See "Flag mask values"
 * @return            Status of operation: 0 = success, negative value = error.
 */
EXTERNRT int rtCborEncValue
(OSCTXT* pctxt, const OSRTGenValue* pvalue, OSUINT32 flags);

#endif

#endif
