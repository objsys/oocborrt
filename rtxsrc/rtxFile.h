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
 * @file rtxFile.h
 * Common runtime functions for reading from or writing to files.
 */
#ifndef _RTXFILE_H_
#define _RTXFILE_H_

#include "rtxsrc/rtxContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function tests if a file exists.
 *
 * @param filePath     Complete file path name of file to be tested.
 * @return             TRUE if file exists or FALSE if does not exist or
 *                       some other error occurred.
 */
EXTERNRT OSBOOL rtxFileExists (const char* filePath);

/**
 * This function returns the last modified time for a given file.
 * If the file does not exist or some error occurs, 0 is returned.
 */
EXTERNRT time_t rtxFileLastModified( const char* filePath );

/**
 * This function opens a file for read, write, or append access.  It
 * is basically a wrapper for the C run-time fopen function except in
 * the case of Visual Studio, the more secure fopen_s function is
 * used.
 *
 * @param ppFile       Pointer to FILE variable to receive file pointer.
 * @param filePath     Complete file path name of file to be opened.
 * @param access       File access string as defined for C fopen.
 * @return             Completion status of operation:
 *                       - 0 = success or negative status code.
 */
EXTERNRT int rtxFileOpen
(FILE** ppFile, const char* filePath, const char* access);

/**
 * This function reads the entire contents of a binary file into memory. A
 * memory buffer is allocated for the file contents using the run-time
 * memory management functions.
 *
 * @param pctxt        Pointer to context block structure.
 * @param filePath     Complete file path name of file to read.
 * @param ppMsgBuf     Pointer to message buffer to receive allocated memory
 *                       pointer.
 * @param pLength      Pointer to integer to receive length of data read.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - RTERR_FILNOTFOU = file not found
 *                       - RTERR_FILEREAD = file read error (see errno)
 */
EXTERNRT int rtxFileReadBinary
(OSCTXT* pctxt, const char* filePath, OSOCTET** ppMsgBuf, size_t* pLength);

/**
 * This function reads the entire contents of a binary file into memory. A
 * memory buffer is allocated for the file contents using the run-time
 * memory management functions.
 *
 * @param pctxt        Pointer to context block structure.
 * @param pFile        Pointer to the open file.
 * @param ppMsgBuf     Pointer to message buffer to receive allocated memory
 *                       pointer.
 * @param pLength      Pointer to integer to receive length of data read.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - RTERR_FILNOTFOU = file not found
 *                       - RTERR_FILEREAD = file read error (see errno)
 */
EXTERNRT int rtxFileReadBinary2
(OSCTXT* pctxt, FILE* pFile, OSOCTET** ppMsgBuf, size_t* pLength);

/**
 * This function reads the entire contents of a binary file into memory. A
 * memory buffer is allocated for the file contents using the standard
 * configured system memory allocation function (usually malloc).
 *
 * @param pctxt        Pointer to context block structure.
 * @param filePath     Complete file path name of file to read.
 * @param ppMsgBuf     Pointer to message buffer to receive allocated memory
 *                       pointer.
 * @param pLength      Pointer to integer to receive length of data read.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - RTERR_FILNOTFOU = file not found
 *                       - RTERR_FILEREAD = file read error (see errno)
 */
EXTERNRT int rtxFileReadBinToSysMem
(OSCTXT* pctxt, const char* filePath, OSOCTET** ppMsgBuf, size_t* pLength);

/**
 * This function reads the entire contents of an ASCII text file into
 * memory. A memory buffer is allocated for the file contents using the
 * run-time memory management functions.  This function is identical to
 * rtxReadFileBinary except that a) the file is opened in text mode, and
 * b) and extra byte is allocated at the end for a null-terminator
 * character.
 *
 * @param pctxt        Pointer to context block structure.
 * @param filePath     Complete file path name of file to read.
 * @param ppMsgBuf     Pointer to message buffer to receive allocated memory
 *                       pointer.
 * @param pLength      Pointer to integer to receive length of data read.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - RTERR_FILNOTFOU = file not found
 *                       - RTERR_FILEREAD = file read error (see errno)
 */
EXTERNRT int rtxFileReadText
(OSCTXT* pctxt, const char* filePath, OSOCTET** ppMsgBuf, size_t* pLength);

/**
 * This function writes binary data from memory to the given file.
 *
 * @param filePath     Complete file path name of file to be written to.
 * @param pMsgBuf      Pointer to buffer containing data to be written.
 * @param length       Size (in bytes) of data to be written
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative status code if error
 */
EXTERNRT int rtxFileWriteBinary
(const char* filePath, const OSOCTET* pMsgBuf, size_t length);

/**
 * This function writes text data from memory to the given file.
 * The text is expected to be terminated by a null terminator character.
 * This function will work with standard ASCII or UTF-8 encoded text.
 *
 * @param filePath     Complete file path name of file to be written to.
 * @param pMsgBuf      Pointer to buffer containing data to be written.
 * @return             Completion status of operation:
 *                       - 0 = success,
 *                       - negative status code if error
 */
EXTERNRT int rtxFileWriteText
(const char* filePath, const char* pMsgBuf);

/**
 * This function copies the contents on one text file to another.
 *
 * @param srcFilePath  Complete file path name of file to be copied.
 * @param destFilePath Complete file path name of target file.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - RTERR_FILNOTFOU = source file not found
 *                       - RTERR_FILEREAD = file read error (see errno)
 */
EXTERNRT int rtxFileCopyTextFile
(const char* srcFilePath, const char* destFilePath);

#ifdef __cplusplus
}
#endif

#endif
