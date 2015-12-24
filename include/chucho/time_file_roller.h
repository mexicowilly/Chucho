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

#if !defined(CHUCHO_TIME_FILE_ROLLER_H__)
#define CHUCHO_TIME_FILE_ROLLER_H__

/**
 * @file 
 * @copydoc chucho::time_file_roller 
 *  
 * @ingroup c_rolling 
 */

#include <chucho/file_roller.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation
 * @{
 */
/**
 * Create a time file roller.
 * 
 * @post Ownership of the rlr parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_file_roller() function.
 * @post Ownership of the cmp parameter, if not NULL, is 
 *       transferred to the callee.
 * 
 * @param[out] rlr the roller to create
 * @param[in] file_name_pattern the file name pattern, which 
 *       must include a date pattern
 * @param[in] max_history the maximum number of rolled files to 
 *       keep
 * @param[in] cmp the compressor, which can be NULL if no 
 *            compression is desired
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_time_file_roller(chucho_file_roller** rlr,
                                                       const char* const file_name_pattern,
                                                       size_t max_history,
                                                       chucho_file_compressor* cmp);
/** @} */
/**
 * Return the file name pattern in use by this roller.
 * 
 * @param[in] rlr the time file roller
 * @param[out] pat the file name pattern
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_trlr_get_file_name_pattern(const chucho_file_roller* rlr, const char** pat);
/**
 * Return the maximum number of rolled files to keep.
 * 
 * @param[in] rlr the time file roller
 * @param[out] hist the maximum history
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_trlr_get_max_history(const chucho_file_roller* rlr, size_t* hist);

#if defined(__cplusplus)
}
#endif

#endif
