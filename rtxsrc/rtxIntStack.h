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
/**
 * @file rtxIntStack.h
 * Simple FIFO stack for storing integer values.
 */
#ifndef _RTXINTSTACK_
#define _RTXINTSTACK_

#include "rtxsrc/rtxContext.h"

/**
 * @defgroup ccfIntStack Integer Stack Utility Functions
 *
 * This is a simple stack structure with supporting push, pop, and
 * peek functions.
 *
 *@{
 */
/**
 * This is the default capacity that is used if zero is passed as the
 * capacity argument to rtxIntStackInit.
 */
#define OSRTISTK_DEFAULT_CAPACITY 100

/**
 * @struct _OSRTIntStack
 * This is the main stack structure.  It uses an expandable array for
 * storing integer values.
 */
typedef struct _OSRTIntStack {
   OSCTXT*      pctxt;      /* context pointer */
   OSINT32*     pdata;      /* stack data */
   size_t       index;      /* index to top element */
   size_t       size;       /* current size of the stack */
} OSRTIntStack;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function initializes a stack structure. It allocates the initial
 * amount of memory required to store data and sets all working variables
 * to their initil state.
 *
 * @param pctxt        A pointer to the context with which the stack is
 *                      associated.
 * @param pstack       A pointer to a stack structure to be initialized.
 * @param capacity     Initial capacity of the stack.  This is the number
 *                       of integer values that can be stored before the
 *                       stack is expanded.  Each expansion doubles the
 *                       initial capacity value.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxIntStackInit
(OSCTXT* pctxt, OSRTIntStack* pstack, size_t capacity);

/**
 * This function pushes an item onto the stack.
 *
 * @param pstack       A pointer to the stack structure.
 * @param value        A pointer to the data item to be pushed on the stack.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxIntStackPush (OSRTIntStack* pstack, OSINT32 value);

/**
 * This functions returns the data item on the top of the stack.
 *
 * @param pstack       A pointer to the stack structure.
 * @param pvalue       A pointer to a variable to store the integer value
 *                       of the item at the top of the stack.
 * @return             Status of peek operation:
 *                       - 0 (0) = success,
 *                       - RTERR_ENDOFBUF if stack is empty
 */
EXTERNRT int rtxIntStackPeek (OSRTIntStack* pstack, OSINT32* pvalue);

/**
 * This functions pops the data item on the top of the stack.
 *
 * @param pstack       A pointer to the stack structure.
 * @param pvalue       A pointer to a variable to store the integer value
 *                       of the item at the top of the stack.
 * @return             Status of pop operation:
 *                       - 0 (0) = success,
 *                       - RTERR_ENDOFBUF if stack is empty
 */
EXTERNRT int rtxIntStackPop (OSRTIntStack* pstack, OSINT32* pvalue);

/**
 * This macro tests if the stack is empty.
 *
 * @param stack        Stack structure variable to be tested.
 */
#define rtxIntStackIsEmpty(stack) (OSBOOL)((stack).index == 0)

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
