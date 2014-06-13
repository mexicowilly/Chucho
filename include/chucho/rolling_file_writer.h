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

#if !defined(CHUCHO_ROLLING_FILE_WRITER_H__)
#define CHUCHO_ROLLING_FILE_WRITER_H__

/**
 * @file 
 * @copydoc chucho::rolling_file_writer 
 *  
 * @ingroup c_writers 
 */

#include <chucho/file_writer.h>
#include <chucho/file_roller.h>
#include <chucho/file_roll_trigger.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation
 * @{
 */
/**
 * Create a rolling file writer.
 * 
 * @post Ownership of the wrt parameter if transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_writer() function.
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * @post Ownership of the rlr parameter is transferred to the 
 *       callee.
 * @post Ownership of the trg parameter is transferred to the 
 *       callee.
 *  
 * @note The trg parameter can be NULL if the given file roller 
 *       is also a file roll trigger.
 * @note The name parameter can be NULL, if the given file 
 *       roller also provides a name.
 * @note The returned writer may call any of the functions 
 *       except for the creation function from @ref
 *       file_writer.h.
 * 
 * @param[out] wrt the writer to create
 * @param[in] fmt the formatter
 * @param[in] rlr the file roller
 * @param[in] trg the file roll trigger, which can be NULL if 
 *       the file roller is also a file roll trigger
 * @param[in] name the active file name, which can be NULL if 
 *       the file roller provides an active file name
 * @param on_start whether to truncate or append to the file on 
 *                 start
 * @param flush non-zero if the writer should flush to the file 
 *              after every write
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_rolling_file_writer(chucho_writer** wrt,
                                                          chucho_formatter* fmt,
                                                          chucho_file_roller* rlr,
                                                          chucho_file_roll_trigger* trg,
                                                          const char* const name,
                                                          chucho_on_start on_start,
                                                          int flush);
/** @} */
/**
 * Return the file roller associated with a rolling file writer.
 * 
 * @post Ownership of the rlr parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_file_roller() function.
 * 
 * @param[in] wrt the writer
 * @param[out] rlr the file roller
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rfwrt_get_file_roller(const chucho_writer* wrt, chucho_file_roller** rlr);
/**
 * Return the file roll trigger associated with a rolling file 
 * writer. 
 * 
 * @post Ownership of the trg parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_file_roll_trigger() function.
 * 
 * @param[in] wrt the writer
 * @param[out] trg the file roll trigger
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rfwrt_get_file_roll_trigger(const chucho_writer* wrt, chucho_file_roll_trigger** trg);

#if defined(__cplusplus)
}
#endif

#endif
