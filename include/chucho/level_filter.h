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

#if !defined(CHUCHO_LEVEL_FILTER_H__)
#define CHUCHO_LEVEL_FILTER_H__

/**
 * @file 
 * Functions related to level filters. 
 *  
 * @ingroup c filters 
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
 * Create a level filter.
 * 
 * @post Ownership of the flt parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_filter() function.
 * 
 * @param[out] flt the level filter to create
 * @param[in] lvl the level of the filter
 * @param[in] on_match the result to return in case of a level 
 *       match
 * @param[in] on_mismatch the result to return in case of a 
 *       level mismatch
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_level_filter(chucho_filter** flt,
                                             const chucho_level* lvl,
                                             chucho_filter_result on_match,
                                             chucho_filter_result on_mismatch);
//@}
/**
 * Return the level associated with a level filter.
 * 
 * @param[in] flt the level filter
 * @param[out] lvl the level of this filter
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lflt_get_level(const chucho_filter* flt, const chucho_level** lvl);
/**
 * Return the result this filter will return in the case of a 
 * level match. 
 * 
 * @param[in] flt the level filter
 * @param[out] res the result in the case of a level match
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lflt_get_on_match(const chucho_filter* flt, chucho_filter_result* res);
/**
 * Return the result this filter will return in the case of a 
 * level mismatch. 
 * 
 * @param[in] flt the level filter
 * @param[out] res the result in the case of a level mismatch
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lflt_get_on_mismatch(const chucho_filter* flt, chucho_filter_result* res);

#if defined(__cplusplus)
}
#endif

#endif
