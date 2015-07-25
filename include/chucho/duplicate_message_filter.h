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

#if !defined(CHUCHO_DUPLICATE_MESSAGE_FILTER_H__)
#define CHUCHO_DUPLICATE_MESSAGE_FILTER_H__

/**
 * @file 
 * Creation of duplicate message filters. 
 * @copydoc chucho::duplicate_message_filter
 *  
 * @ingroup c_filters 
 */

#include <chucho/filter.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Create a duplicate message filter.
 *  
 * @post Ownership of the flt paramter is transferred to the 
 *       caller it must be released using the @ref
 *       chucho_release_filter() function.
 * 
 * @param[out] flt the filter to create
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_duplicate_message_filter(chucho_filter** flt);

#if defined(__cplusplus)
}
#endif

#endif
