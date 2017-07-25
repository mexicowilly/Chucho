/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_RUBY_EVALUATOR_FILTER_H__)
#define CHUCHO_RUBY_EVALUATOR_FILTER_H__

/**
 * @file 
 * Functions related to Ruby evaluator filters. 
 * @copydoc chucho::ruby_evaluator_filter 
 *  
 * @ingroup c_filters 
 */

#include <chucho/filter.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation
 * @{
 */
/**
 * Create a ruby evaluator filter.
 * 
 * @post Ownership of the flt parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_filter() function.
 * 
 * @param[out] flt the filter to create
 * @param[in] expression the Ruby code to run
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_ruby_evaluator_filter(chucho_filter** flt,
                                                            const char* const expression);
/** @}  */

/**
 * Return the Ruby expression
 * 
 * @param[in] flt the Ruby evaluator filter
 * @param[out] exp the Ruby expression associated with this 
 *       filter
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rbflt_get_expression(const chucho_filter* flt, const char** exp);

#if defined(__cplusplus)
}
#endif

#endif
