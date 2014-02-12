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

#if !defined(CHUCHO_FORMATTER_H__)
#define CHUCHO_FORMATTER_H__

/**
 * @file 
 * Functions that can be used with any formatter. 
 *  
 * @ingroup c formatters 
 */

#include <chucho/return_code.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque structure that you don't care about.
 */
typedef struct chucho_formatter chucho_formatter;

/**
 * Release a formatter.
 *  
 * @post Ownership of the fmt parameters is transferred to the 
 *       callee.
 *
 * @param fmt the formatter
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_formatter(chucho_formatter* fmt);

#if defined(__cplusplus)
}
#endif

#endif
