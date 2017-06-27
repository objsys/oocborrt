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
 * @file rtxStack.h
 * Simple FIFO stack for storing void pointers to any type of data.
 */
#ifndef _RTXSTACK_H_
#define _RTXSTACK_H_


#include "rtxsrc/rtxDList.h"


/**
 * @defgroup ccfStack Stack Utility Functions
 *
 * This is a simple stack structure with supporting push and pop functions.
 * Different initialization routines are provided to permit different
 * memory management schemes.
 *
 *@{
 */
/**
 * @struct _OSRTStack
 * This is the main stack structure.  It uses a linked list structure.
 */
typedef struct _OSRTStack {
   struct OSCTXT*   pctxt;
   OSRTDList dlist;
} OSRTStack;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function creates a new stack structure. It allocates memory for the
 * structure and calls rtxStackInit to initialize the structure.
 *
 * @param pctxt        A pointer to the context with which the stack is
 *                      associated.
 * @return             A pointer to an allocated stack structure.
 */
EXTERNRT OSRTStack* rtxStackCreate (struct OSCTXT* pctxt);

/**
 * This function initializes a stack structure. It sets the number of elements
 * to zero and sets all internal pointer values to NULL.
 *
 * @param pctxt        A pointer to the context with which the stack is
 *                      associated.
 * @param pStack       A pointer to a stack structure to be initialized.
 */
EXTERNRT void  rtxStackInit (struct OSCTXT* pctxt, OSRTStack* pStack);

/**
 * This macro tests if the stack is empty.
 *
 * @param stack        Stack structure variable to be tested.
 */
#define rtxStackIsEmpty(stack) (OSBOOL)((stack).dlist.count == 0)

/**
 * This function pops an item off the stack.
 *
 * @param pStack       The pointer to the stack structure from which the value
 *                       is to be popped. Pointer to the updated stack
 *                       structure.
 * @return             The pointer to the item popped from the stack
 */
EXTERNRT void* rtxStackPop (OSRTStack* pStack);

/**
 * This function pushes an item onto the stack.
 *
 * @param pStack       A pointer to the structure onto which the data item is
 *                       to be pushed. The pointer updated to the stack
 *                       structure
 * @param pData        A pointer to the data item to be pushed on the stack.
 * @return             Completion status of operation:
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtxStackPush (OSRTStack* pStack, void* pData);

/**
 * This functions returns the data item on the top of the stack.
 *
 * @param pStack       A pointer to the structure onto which the data item is
 *                       to be pushed. The pointer updated to the stack
 *                       structure
 * @return             Pointer to data item at top of stack or NULL if
 *                       stack empty.
 *                       - 0 (0) = success,
 *                       - negative return value is error.
 */
EXTERNRT void* rtxStackPeek (OSRTStack* pStack);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
