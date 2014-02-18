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

#if !defined(CHUCHO_NUMBERED_FILE_ROLLER_H__)
#define CHUCHO_NUMBERED_FILE_ROLLER_H__

/**
 * @file 
 * Functions related to numbered file rollers. 
 * @copydoc chucho::numbered_file_roller 
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
 * Create a numbered file roller.
 * 
 * @post Ownership of the rlr parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_file_roller() function.
 * @post Ownership of the cmp parameter is transferred to the 
 *       callee, if it is not NULL.
 * 
 * @param[out] rlr the numbered file roller to create
 * @param[in] min_index the minimum index at which to start 
 *                  numbering files
 * @param[in] max_index the maximum index at which to stop 
 *                  numbering file
 * @param[in] cmp the file compressor, which may be NULL if no 
 *            compression is desired
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_numbered_file_roller(chucho_file_roller** rlr,
                                                           int min_index,
                                                           int max_index,
                                                           chucho_file_compressor* cmp);
//@}
/**
 * Return the maximum index at which rolling stops.
 * 
 * @param[in] rlr the numbered file roller
 * @param[out] idx the maximum index
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_nrlr_get_max_index(const chucho_file_roller* rlr, int* idx);
/**
 * Return the minimum index at which numbered rolled files 
 * starts. 
 * 
 * @param[in] rlr the numbered file roller
 * @param[out] idx the minimum index
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_nrlr_get_min_index(const chucho_file_roller* rlr, int* idx);

#if defined(__cplusplus)
}
#endif

#endif
