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

#if !defined(CHUCHO_DB2_WRITER_H__)
#define CHUCHO_DB2_WRITER_H__

/**
 * @file 
 * Functions for DB2 writers. 
 * @copydoc chucho::db2_writer
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
 * Create a DB2 writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and must be released with the @ref
 *       chucho_release_writer() function.
 *  
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @param[out] wrt the DB2 writer to create
 * @param[in] fmt the formatter
 * @param[in] database the database name
 * @param[in] user the user name for the database
 * @param[in] password the password of the user for the database
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_db2_writer(chucho_writer** wrt,
                                                 chucho_formatter* fmt,
                                                 const char* const database,
                                                 const char* const user,
                                                 const char* const password);
/** @}  */

/**
 * Return the name of the database.
 * 
 * @param[in] wrt the DB2 writer
 * @param[out] database the name of the connected database
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_db2wrt_get_database(const chucho_writer* wrt, const char** database);

/**
 * Return the password.
 * 
 * @param[in] wrt the DB2 writer
 * @param[out] password the password
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_db2wrt_get_password(const chucho_writer* wrt, const char** password);

/**
 * Return the DB2 user name.
 * 
 * @param[in] wrt the DB2 writer
 * @param[out] user the user
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_db2wrt_get_user(const chucho_writer* wrt, const char** user);

#if defined(__cplusplus)
}
#endif

#endif
