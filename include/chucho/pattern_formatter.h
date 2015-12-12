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

#if !defined(CHUCHO_PATTERN_FORMATTER_H__)
#define CHUCHO_PATTERN_FORMATTER_H__

/**
 * @file 
 * @copydoc chucho::pattern_formatter 
 *  
 * @ingroup c_formatters 
 */

#include <chucho/formatter.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Create a pattern formatter.
 * 
 * @post Ownership of the fmt paramter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_formatter() function.
 * 
 * @param fmt the formatter to create
 * @param pattern the pattern
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_pattern_formatter(chucho_formatter** fmt, const char* const pattern);

#if defined(__cplusplus)
}
#endif

#endif