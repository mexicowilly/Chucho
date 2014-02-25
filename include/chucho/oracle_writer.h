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

#if !defined(CHUCHO_ORACLE_WRITER_H__)
#define CHUCHO_ORACLE_WRITER_H__

/**
 * @file 
 * Creation of Oracle writers. 
 * @copydoc chucho::oracle_writer
 *  
 * @ingroup c_writers 
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Create an Oracle writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and must be released with the @ref
 *       chucho_release_writer() function.
 *  
 * @post Ownership of the fmt parameters is transferred to the 
 *       callee.
 * 
 * @param[out] the Oracle writer to create
 * @param[in] fmt the formatter
 * @param[in] user the user name for the database
 * @param[in] password the password of the user for the database
 * @param[in] database the database name, which is a TNS name, 
 *            such as myoracleserver.com/dbname
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_oracle_writer(chucho_writer** wrt,
                                                    chucho_formatter* fmt,
                                                    const char* const user,
                                                    const char* const password,
                                                    const char* const databaes);

#if defined(__cplusplus)
}
#endif

#endif
