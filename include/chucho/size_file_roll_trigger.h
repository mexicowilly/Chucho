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

#if !defined(CHUCHO_SIZE_FILE_ROLL_TRIGGER_H__)
#define CHUCHO_SIZE_FILE_ROLL_TRIGGER_H__

/**
 * @file 
 * @copydoc chucho::size_file_roll_trigger 
 *  
 * @ingroup c_rolling 
 */

#include <chucho/file_roll_trigger.h>
#include <sys/types.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation
 * @{
 */
/**
 * Create a size file roll trigger.
 * 
 * @post Ownership of the trg parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_file_roll_trigger() function.
 * 
 * @param[out] trg the size file roll trigger to create
 * @param[in] max_size the size at which files will roll
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_size_file_roll_trigger(chucho_file_roll_trigger** trg,
                                                             off_t max_size);
/** @} */
/**
 * Return the size at which files will roll.
 * 
 * @param[in] trg the size file roll trigger
 * @param[out] sz the size at which files roll
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sftrg_get_max_size(const chucho_file_roll_trigger* trg, off_t* sz);

#if defined(__cplusplus)
}
#endif

#endif
