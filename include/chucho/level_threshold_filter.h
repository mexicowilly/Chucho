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

#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_H__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_H__

/**
 * @file 
 * Functions related to level threshold filters. 
 * @copydoc chucho::level_threshold_filter 
 *  
 * @ingroup c_filters 
 */

#include <chucho/filter.h>
#include <chucho/level.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation
 */
//@{
/**
 * Create a level threshold filter.
 * 
 * @post Ownership of the flt parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_filter() function.
 * 
 * @param[out] flt the filter to create
 * @param[in] lvl the level threshold
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_level_threshold_filter(chucho_filter** flt,
                                                             const chucho_level* lvl);
//@}
/**
 * Return the level associated with a level threshold filter.
 * 
 * @param[in] flt the level threshold filter
 * @param[out] lvl the level
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ltflt_get_level(const chucho_filter* flt, const chucho_level** lvl);

#if defined(__cplusplus)
}
#endif

#endif
