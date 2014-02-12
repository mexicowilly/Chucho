/*
 * Copyright 2013-2014 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#if !defined(CHUCHO_FILE_WRITER_H__)
#define CHUCHO_FILE_WRITER_H__

/**
 * @file 
 * Functions for file writers. 
 *  
 * @ingroup c writers 
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * The starting behavior of a file writer.
 */
typedef enum
{
    CHUCHO_ON_START_APPEND,     /**< Append to the file on start. */
    CHUCHO_ON_START_TRUNCATE    /**< Truncate the file on start. */
} chucho_on_start;

/**
 * @name Creation
 */
//@{
/**
 * Create a file writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_writer() function.
 *  
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @param[out] wrt the writer to create
 * @param[in] fmt the formatter
 * @param[in] file_name the name of the file to which to write
 * @param[in] on_start the starting behavior of the writer
 * @param[in] flush if non-zero, the file will be flushed after 
 *       every write
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_file_writer(chucho_writer** wrt,
                                            chucho_formatter* fmt,
                                            const char* const file_name,
                                            chucho_on_start on_start,
                                            int flush);
//@}
/**
 * Return the name of the file.
 * 
 * @param[in] wrt the file writer
 * @param[out] name the name of the writer's file
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_fwrt_get_file_name(const chucho_writer* wrt, const char** name);
/**
 * Return whether the writer flushes the file after every write.
 * 
 * @param[in] wrt the file writer
 * @param[out] flush non-zero if the writer flushes the file 
 *       after every write
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_fwrt_get_flush(const chucho_writer* wrt, int* flush);
/**
 * Return behavior of the writer when it starts writing to a 
 * file. 
 * 
 * @param[in] wrt the file writer
 * @param[out] on_start the starting behavior
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_fwrt_get_on_start(const chucho_writer* wrt, chucho_on_start* on_start);

#if defined(__cplusplus)
}
#endif

#endif
