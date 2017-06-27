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
/**
 * @file rtxMemory.h
 * Memory management function and macro definitions.
 */
#ifndef __RTXMEMORY_H__
#define __RTXMEMORY_H__

#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup rtmem Memory Allocation Macros and Functions
 * @ingroup cruntime
 *
 * Memory allocation functions and macros handle memory management for the
 * ASN1C run-time. Special algorithms are used for allocation and deallocation
 * of memory to improve the run-time performance. @{
 */
#define RT_MH_ZEROARRAY    0x80

/**
 * Free memory associated with a context. This macro frees all memory held
 * within a context. This is all memory allocated using the rtxMemAlloc (and
 * similar macros) and the mem memory allocation functions using the given
 * context variable.
 *
 * @param pctxt        - Pointer to a context block
 */
#define MEMFREE(pctxt) do {\
memFree(pctxt); \
if ((pctxt)->buffer.dynamic && (pctxt)->buffer.data) { \
(pctxt)->buffer.data = 0; \
} \
} while (0)

/**
 * Allocate memory.  This function allocates the given number of bytes.  It is
 * similar to the C \c malloc run-time function.
 *
 * @param pctxt - Pointer to a context block
 * @param nbytes - Number of bytes of memory to allocate
 * @return - Void pointer to allocated memory or NULL if insufficient memory
 *   was available to fulfill the request.
 */
EXTERNRT void* rtxMemAlloc (OSCTXT* pctxt, size_t nbytes);

/**
 * Allocate and zero memory.  This function allocates the given number of bytes
 * and then initializes the memory block to zero.
 *
 * @param pctxt - Pointer to a context block
 * @param nbytes - Number of bytes of memory to allocate
 * @return - Void pointer to allocated memory or NULL if insufficient memory
 *   was available to fulfill the request.
 */
EXTERNRT void* rtxMemAllocZ (OSCTXT* pctxt, size_t nbytes);

/**
 * Free memory pointer. This macro frees memory at the given pointer. The
 * memory must have been allocated using the rtxMemAlloc function.
 *
 * @param pctxt        - Pointer to a context block
 * @param pmem         - Pointer to memory block to free. This must have been
 *                       allocated using the rtxMemAlloc macro or the
 *                       memHeapAlloc function.
 */
EXTERNRT void rtxMemFreePtr2 (OSCTXT* pctxt, void* pmem);

#define rtxMemFreePtr(pctxt,mem_p) rtxMemFreePtr2(pctxt,(void*)mem_p)
#define rtxMemFreeType(pctxt,mem_p) rtxMemFreePtr2(pctxt,(void*)mem_p)

/**
 * Free memory pointer.  This macro frees memory at the given pointer.
 * The memory must have been allocated using the rtxMemAlloc (or similar)
 * macros or the rtxMem memory allocation macros.  This macro is
 * similar to the C \c free function.
 *
 * @param pctxt - Pointer to a context block
 * @param mem_p - Pointer to memory block to free.  This must have
 *   been allocated using the rtxMemAlloc function.
 */
#define rtxMemFreeArray(pctxt,mem_p) rtxMemFreePtr2(pctxt, (void*)mem_p)

/**
 * Free memory associated with a context.  This macro frees all memory
 * held within a context.  This is all memory allocated using the
 * rtxMemAlloc (and similar macros) and the rtxMem memory allocation
 * functions using the given context variable.
 *
 * @param pctxt - Pointer to a context block
 */
EXTERNRT void rtxMemFree (OSCTXT* pctxt);

/**
 * Reallocate memory.  This macro reallocates a memory block (either
 * expands or contracts) to the given number of bytes.  It is
 * similar to the C \c realloc run-time function.
 *
 * @param pctxt - Pointer to a context block
 * @param mem_p - Pointer to memory block to reallocate.  This must have been
 *   allocated using the rtxMemAlloc macro or the rtxMemHeapAlloc function.
 * @param nbytes - Number of bytes of memory to which the block is to be
 *   resized.
 * @return - Void pointer to allocated memory or NULL if insufficient memory
 *   was available to fulfill the request.  This may be the same as the mem_p
 *   pointer that was passed in if the block did not need to be relocated.
 */
EXTERNRT void* rtxMemRealloc (OSCTXT* pctxt, void* pmem, size_t nbytes);

/**
 * Reallocate memory.  This macro reallocates a memory block (either
 * expands or contracts) to the given number of bytes.  It is
 * similar to the C \c realloc run-time function.
 *
 * @param pctxt - Pointer to a context block
 * @param mem_p - Pointer to memory block to reallocate.  This must have been
 *   allocated using the rtxMemAlloc macro or the rtxMemHeapAlloc function.
 * @param n - Number of items of the given type to be allocated.
 * @param type - Array element data type (for example, int).
 * @return - Void pointer to allocated memory or NULL if insufficient memory
 *   was available to fulfill the request.  This may be the same as the pmem
 *   pointer that was passed in if the block did not need to be relocated.
 */
#define rtxMemReallocArray(pctxt,mem_p,n,type) \
(type*)rtxMemRealloc(pctxt, (void*)mem_p, sizeof(type)*n)

/**
 * Reset memory associated with a context. This macro resets all memory held
 * within a context. This is all memory allocated using the rtxMemAlloc (and
 * similar macros) and the mem memory allocation functions using the given
 * context variable.
 *
 * <p>The difference between this and the MEMFREE macro is that the memory
 * blocks held within the context are not actually freed. Internal pointers are
 * reset so the existing blocks can be reused. This can provide a performace
 * improvement for repetitive tasks such as decoding messages in a loop.
 *
 * @param pctxt        - Pointer to a context block
 */
EXTERNRT void rtxMemReset (OSCTXT* pctxt);

#define rtxMemAllocType(pctxt,ctype) \
(ctype*)rtxMemAlloc(pctxt,sizeof(ctype))

#define rtxMemAllocTypeZ(pctxt,ctype) \
(ctype*)rtxMemAllocZ(pctxt,sizeof(ctype))

/**
 * Allocate a dynamic array. This macro allocates a dynamic array of
 * records of the given type. The pointer to the allocated array is
 * returned to the caller.
 *
 * @param pctxt        - Pointer to a context block
 * @param n            - Number of records to allocate
 * @param type         - Data type of an array record
 */
#define rtxMemAllocArray(pctxt,n,type) \
(type*)rtxMemAllocArray2 (pctxt, n, sizeof(type), 0)

EXTERNRT void* rtxMemAllocArray2
(OSCTXT* pctxt, OSSIZE numElements, OSSIZE typeSize, OSUINT32 flags);

/**
 * Determine if any memory records exist in the memory heap.
 *
 * @param pctxt        - Pointer to a context block
 * @return             - True if records exist
 */
EXTERNRT OSBOOL rtxMemHeapIsEmpty (OSCTXT* pctxt);

#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif /*__RTXMEMORY_H__*/
