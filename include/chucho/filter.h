/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_FILTER_H__)
#define CHUCHO_FILTER_H__

/**
 * @file 
 * Types and functions common to all filters.
 * @copydoc chucho::filter
 *  
 * @ingroup c_filters 
 */

#include <chucho/return_code.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque type that you don't care about.
 */
typedef struct chucho_filter chucho_filter;

/**
 * The result of a filter operation.
 */
typedef enum
{
    /**
     * The filter does not accept the event. No subsequent filters 
     * are checked, as this is a vetoing result. 
     */
    CHUCHO_FILTER_RESULT_DENY,
    /**
     * This filter doesn't care about the event. Other filters in 
     * the chain are checked. 
     */
    CHUCHO_FILTER_RESULT_NEUTRAL,
    /**
     * The filter unconditionally accepts the event. No subsequent 
     * filters are checked, as this is a vetoing result. 
     */
    CHUCHO_FILTER_RESULT_ACCEPT
} chucho_filter_result;

/**
 * Releaes a filter created by one of the filter creation 
 * functions. 
 *  
 * @post Onwership of the filter is transferred to the callee. 
 * 
 * @param[in] flt the filter to release
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_filter(chucho_filter* flt);

#if defined(__cplusplus)
}
#endif

#endif
