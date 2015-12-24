/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_NAMED_PIPE_WRITER_H__)
#define CHUCHO_NAMED_PIPE_WRITER_H__

/**
 * @file 
 * @copydoc chucho::named_pipe_writer
 *  
 * @ingroup c_writers 
 */

#include <chucho/writer.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Create a named pipe writer. The named pipe will never be
 * created by the writer.
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
 * @param[in] name the name of the pipe, which on Windows can
 * be an unadorned name
 * @param[in] flush if non-zero, the buffer will be flushed after 
 *       every write
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_named_pipe_writer(chucho_writer** wrt,
                                                        chucho_formatter* fmt,
                                                        const char* const name,
                                                        int flush);

/**
 * Return whether the writer flushes the buffer after every write.
 * 
 * @param[in] wrt the file writer
 * @param[out] flush non-zero if the writer flushes the buffer 
 *       after every write
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_npwrt_get_flush(const chucho_writer* wrt, int* flush);

#if defined(__cplusplus)
}
#endif

#endif
