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

#if !defined(CHUCHO_SLIDING_NUMBERED_FILE_ROLLER_H__)
#define CHUCHO_SLIDING_NUMBERED_FILE_ROLLER_H__

/**
 * @file 
 * @copydoc chucho::sliding_numbered_file_roller 
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
 */
//@{
/**
 * Create a sliding numbered file roller.
 * 
 * @post Ownership of the rlr parameter is transferred to the 
 *       caller, and it must be release with the @ref
 *       chucho_release_file_roller() function.
 * 
 * @param rlr the sliding numbered file roller to create
 * @param min_index the number at which to start counting when 
 *                  rolling is performed
 * @param max_count the maximum number of rolled files that can 
 *                  exist
 * @param cmp a file compressor, which may be NULL if no 
 *            compression is desired
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_sliding_numbered_file_roller(chucho_file_roller** rlr,
                                                                   int min_index,
                                                                   size_t max_count,
                                                                   chucho_file_compressor* cmp);
//@}
/**
 * Return the index of the file that is current being written.
 * 
 * @param[in] rlr the sliding numbered file roller
 * @param[out] idx the current index
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_snfrlr_get_current_index(const chucho_file_roller* rlr, int* idx);
/**
 * Return the maximum number of files that can be present at a 
 * time. 
 * 
 * @param[in] rlr the sliding numbered file roller
 * @param[out] cnt the maximum count
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_snfrlr_get_max_count(const chucho_file_roller* rlr, size_t* cnt);
/**
 * Return the starting index at which files will be rolled.
 * 
 * @param[in] rlr the sliding numbered file roller
 * @param[out] idx the starting index
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_snfrlr_get_min_index(const chucho_file_roller* rlr, int* idx);

#if defined(__cplusplus)
}
#endif

#endif
