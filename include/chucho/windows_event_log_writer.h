/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_WINDOWS_EVENT_LOG_WRITER_H__)
#define CHUCHO_WINDOWS_EVENT_LOG_WRITER_H__

/**
 * @file 
 * Functions for windows event log writers. 
 *  
 * @ingroup c_writers 
 */

#include <chucho/writer.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation
 * @{
 */
/**
 * Create a Windows event log writer.
 *
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_writer() function.
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 *
 * @note The host parameter can be NULL if connection to the
 *       local host is desired.
 *
 * @param[out] wrt the writer to create
 * @param[in] fmt the formatter
 * @param[in] log the name of the log to which to write
 * @param[in] source the name of the event source
 * @param[in] host the name of the host to which to connect,
 *            which can be NULL to connect to the local host
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_windows_event_log_writer(chucho_writer** wrt,
                                                               chucho_formatter* fmt,
                                                               const char* const log,
                                                               const char* const source,
                                                               const char* const host);
/** @} */

/**
 * Return the name of the host.
 * 
 * @param[in] wrt the Windows event log writer
 * @param[out] host the name of the host to which the writer is 
 *       connected, or NULL if it is the local host
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_welwrt_get_host(const chucho_writer* wrt, const char** host);
/**
 * Return the name of the log.
 * 
 * @param[in] wrt the Windows event log writer
 * @param[out] log the name of the log to which the writer is 
 *       writing
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_welwrt_get_log(const chucho_writer* wrt, const char** log);
/**
 * Return the name of the event source.
 * 
 * @param[in] wrt the Windows event log writer
 * @param[out] source the name of the event source
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_welwrt_get_source(const chucho_writer* wrt, const char** source);

#if defined(__cplusplus)
}
#endif

#endif
