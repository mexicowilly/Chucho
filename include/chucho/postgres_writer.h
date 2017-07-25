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

#if !defined(CHUCHO_POSTGRES_WRITER_H__)
#define CHUCHO_POSTGRES_WRITER_H__

/**
 * @file 
 * Creation of PostgreSQL writers. 
 * @copydoc chucho::postgres_writer
 *  
 * @ingroup c_writers c_database
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation 
 * @{ 
 */
/**
 * Create a PostgreSQL writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and must be released with the @ref
 *       chucho_release_writer() function.
 *  
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @param[out] wrt the PostgreSQL writer to create
 * @param[in] fmt the formatter
 * @param[in] uri the <a 
 *       href="http://www.postgresql.org/docs/9.3/static/libpq-connect.html#LIBPQ-CONNSTRING">connection
 *       URI</a>
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_postgres_writer(chucho_writer** wrt,
                                                      chucho_formatter* fmt,
                                                      const char* const uri);
/** @}  */

/**
 * Return the connection URI.
 * 
 * @param[in] wrt the PostgreSQL writer
 * @param[out] uri the connection URI
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_pgwrt_get_uri(const chucho_writer* wrt, const char** uri);

#if defined(__cplusplus)
}
#endif

#endif
