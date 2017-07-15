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

#if !defined(CHUCHO_SYSLOG_WRITER_H__)
#define CHUCHO_SYSLOG_WRITER_H__

/**
 * @file 
 * @copydoc chucho::syslog_writer
 * 
 * @ingroup c_syslog
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>
#include <chucho/syslog_constants.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation
 * @{
 */
/**
 * Create a syslog writer that writes to the local host.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_writer() function.
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @param[out] wrt the syslog writer to create
 * @param[in] fmt the formatter
 * @param[in] fcl the syslog facility
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_local_syslog_writer(chucho_writer** wrt,
                                                          chucho_formatter* fmt,
                                                          chucho_syslog_facility fcl);
/**
 * Create a syslog writer that writes to a remote host.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_writer() function.
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @param[out] wrt the syslog writer to create
 * @param[in] fmt the formatter
 * @param[in] fcl the syslog facility
 * @param[in] host the host name to which to write
 * @param[in] port the port to which to connect
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_remote_syslog_writer(chucho_writer** wrt,
                                                           chucho_formatter* fmt,
                                                           chucho_syslog_facility fcl,
                                                           const char* const host,
                                                           unsigned port);
/** @}  */
/**
 * Return the facility of a syslog writer.
 * 
 * @param[in] wrt the syslog writer
 * @param[out] fcl the facility
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_slwrt_get_facility(const chucho_writer* wrt, chucho_syslog_facility* fcl);
/**
 * Return the host name of a syslog writer.
 * 
 * @note @ref CHUCHO_NO_SUCH_VALUE is returned if this writer 
 *       was not configured to write to a remote host.
 * 
 * @param[in] wrt the syslog writer
 * @param[out] name the name of the host, if the writer was 
 *       configured to write to a remote host
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_slwrt_get_host_name(const chucho_writer* wrt, const char** name);
/**
 * Return the port to which a syslog writer is meant to connect.
 * 
 * @note @ref CHUCHO_NO_SUCH_VALUE is returned if this writer 
 *       was not configured to write to a remote host.
 * 
 * @param[in] wrt the syslog writer
 * @param[out] port the port number, if the writer was 
 *       configured to write to a remote host
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_slwrt_get_port(const chucho_writer* wrt, unsigned* port);

#if defined(__cplusplus)
}
#endif

#endif
