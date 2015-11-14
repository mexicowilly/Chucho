/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_PIPE_WRITER_H__)
#define CHUCHO_PIPE_WRITER_H__

/**
 * @file 
 * @copydoc chucho::pipe_writer
 *  
 * @ingroup c_writers 
 */

#include <chucho/writer.h>
#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * A platform-dependent type for holding a pipe descriptor.
 * This is HANDLE on Windows and int on other operating
 * systems.
 */
#if defined(CHUCHO_DOXYGEN_SPECIAL)
typedef <platform dependent> pipe_type;
#elif defined(_WIN32)
typedef HANDLE pipe_writer_pipe_type;
#else
typedef int pipe_writer_pipe_type;
#endif

/**
 * Create a pipe writer. This function creates a pipe and
 * attaches the returned writer to the output end of it.
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
 * @param[in] flush if non-zero, the buffer will be flushed after 
 *       every write
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_pipe_writer(chucho_writer** wrt,
                                                  chucho_formatter* fmt,
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
CHUCHO_EXPORT chucho_rc chucho_pwrt_get_flush(const chucho_writer* wrt, int* flush);
/**
 * Return the input end of the pipe.
 * 
 * @param[in] wrt the file writer
 * @param[out] p the input end of the pipe
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_pwrt_get_input(const chucho_writer* wrt, pipe_writer_pipe_type* p);
/**
 * Return the output end of the pipe.
 * 
 * @param[in] wrt the file writer
 * @param[out] p the output end of the pipe
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_pwrt_get_output(const chucho_writer* wrt, pipe_writer_pipe_type* p);

#if defined(__cplusplus)
}
#endif

#endif