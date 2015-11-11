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
 * Functions for pipe writers. 
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

#if defined(_WIN32)
typedef HANDLE pipe_writer_pipe_type;
#else
typedef int pipe_writer_pipe_type;
#endif

CHUCHO_EXPORT chucho_rc chucho_create_pipe_writer(chucho_writer** wrt,
                                                  chucho_formatter* fmt,
                                                  int flush);

/**
 * Return whether the writer flushes the file after every write.
 * 
 * @param[in] wrt the file writer
 * @param[out] flush non-zero if the writer flushes the file 
 *       after every write
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_pwrt_get_flush(const chucho_writer* wrt, int* flush);
CHUCHO_EXPORT chucho_rc chucho_pwrt_get_input(const chucho_writer* wrt, pipe_writer_pipe_type* p);
CHUCHO_EXPORT chucho_rc chucho_pwrt_get_output(const chucho_writer* wrt, pipe_writer_pipe_type* p);

#if defined(__cplusplus)
}
#endif

#endif