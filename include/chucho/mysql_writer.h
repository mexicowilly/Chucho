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

#if !defined(CHUCHO_MYSQL_WRITER_H__)
#define CHUCHO_MYSQL_WRITER_H__

/**
 * @file 
 * Creation of MySQL writers. 
 * @copydoc chucho::mysql_writer
 *  
 * @ingroup c_writers 
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>
#include <chucho/level.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @copydoc chucho::mysql_writer::DEFAULT_PORT
 */
extern unsigned CHUCHO_DEFAULT_MYSQL_PORT;

/**
 * Create a MySQL writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and must be released with the @ref
 *       chucho_release_writer() function.
 *  
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @param[out] wrt the MySQL writer to create
 * @param[in] fmt the formatter
 * @param[in] host the host where the database is running
 * @param[in] user the user name for the database
 * @param[in] password the password of the user for the database
 * @param[in] database the database name
 * @param[in] port the port on which to connect 
 * @param[in] capacity the default queue capacity of the 
 *       underlying async writer
 * @param[in] discard_threshold the discard threshold of the 
 *       underlying async writer
 * @param[in] flush_on_destruct whether the underlying
 *       async writer will flush its queue when it is destroyed
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_mysql_writer(chucho_writer** wrt,
                                                   chucho_formatter* fmt,
                                                   const char* const host,
                                                   const char* const user,
                                                   const char* const password,
                                                   const char* const database,
                                                   unsigned port,
                                                   size_t capacity,
                                                   const chucho_level* discard_threshold,
                                                   int flush_on_destruct);

/**
 * Return the async writer that underlies this writer. The 
 * async writer has the effect of channeling all MySQL 
 * traffic for this writer through a single thread, which 
 * guarantees that the thread has been prepared for use with 
 * MySQL. 
 *  
 * @post Ownership of the async writer is transferred to the 
 *       caller and must be released with the @ref
 *       chucho_release_writer() function.
 * 
 * @param[in] wrt the MySQL writer
 * @param[out] async the underlying async writer
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_mwrt_get_async_writer(const chucho_writer* wrt, chucho_writer** async);

/**
 * Return the name of the database.
 * 
 * @param[in] wrt the MySQL writer
 * @param[out] database the name of the connected database
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_mwrt_get_database(const chucho_writer* wrt, const char** database);

/**
 * Return the name of the host.
 * 
 * @param[in] wrt the MySQL writer
 * @param[out] host the name of the connected host
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_mwrt_get_host(const chucho_writer* wrt, const char** host);

/**
 * Return the password.
 * 
 * @param[in] wrt the MySQL writer
 * @param[out] password the password
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_mwrt_get_password(const chucho_writer* wrt, const char** password);

/**
 * Return the port on which the database connection has been 
 * made. 
 * 
 * @param[in] wrt the MySQL writer
 * @param[out] port the port
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_mwrt_get_port(const chucho_writer* wrt, unsigned* port);

/**
 * Return the MySQL user name.
 * 
 * @param[in] wrt the MySQL writer
 * @param[out] user the user
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_mwrt_get_user(const chucho_writer* wrt, const char** user);

#if defined(__cplusplus)
}
#endif

#endif
