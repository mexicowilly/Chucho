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

#if !defined(CHUCHO_FILE_ROLLER_H__)
#define CHUCHO_FILE_ROLLER_H__

/**
 * @file 
 * Types and functions common to all file rollers. 
 * @copydoc chucho::file_roller
 *  
 * @ingroup c_rolling 
 */

#include <chucho/file_compressor.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque type that you don't care about.
 */
typedef struct chucho_file_roller chucho_file_roller;

/**
 * Release a file roller that was created by one of the file 
 * roller creation functions. 
 * 
 * @post Ownership of the rlr parameter is transferred to the 
 *       callee.
 * 
 * @param[in] rlr the roller to release
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_file_roller(chucho_file_roller* rlr);
/**
 * Return this roller's currently active file name. This is the 
 * name of the file to which the writer is currently writing. 
 * The active file name is copied into the buf parameter, and 
 * the count parameter is set to the size of the file name. This
 * count includes the terminating zero of the name. You may pass 
 * a NULL for buf if all you need to know is the length.
 *
 * @param[in] rlr the file roller 
 * @param[out] buf the buffer to which to copy the file name 
 * @param[in] buf_size the size of the buffer 
 * @param[out] count the number of characters including the 
 *       terminating zero that must be copied
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rlr_get_active_file_name(const chucho_file_roller* rlr, char* buf, size_t buf_size, size_t* count);
/**
 * Return the file compressor associated with this roller.
 * 
 * @post Ownership of the cmp parameter is transferred to the 
 *       caller and it must be released with the @ref
 *       chucho_release_file_compressor() function.
 * 
 * @param[in] rlr the file roller
 * @param[out] cmp the file compressor
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rlr_get_file_compressor(const chucho_file_roller* rlr, chucho_file_compressor** cmp);

#if defined(__cplusplus)
}
#endif

#endif
