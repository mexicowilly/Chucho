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

#if !defined(CHUCHO_ORACLE_WRITER_H__)
#define CHUCHO_ORACLE_WRITER_H__

/**
 * @file 
 * Creation of SQLite writers. 
 * @copydoc chucho::sqlite_writer
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
 * Create a SQLite writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and must be released with the @ref
 *       chucho_release_writer() function.
 *  
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @param[out] wrt the writer to create
 * @param[in] fmt the formatter
 * @param[in] file_name the name of the file in which the 
 *       database is stored
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_sqlite_writer(chucho_writer** wrt,
                                                    chucho_formatter* fmt,
                                                    const char* const file_name);
/** @}  */

/**
 * Return the file name in which the SQLite database is stored.
 * 
 * @param[in] wrt the SQLite writer
 * @param[out] file_name the name of the file in which the 
 *       database is stored
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sltwrt_get_file_name(const chucho_writer* wrt, const char** file_name);

#if defined(__cplusplus)
}
#endif

#endif
