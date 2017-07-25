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

#if !defined(CHUCHO_RETURN_CODE_H__)
#define CHUCHO_RETURN_CODE_H__

/**
 * @file 
 * Return codes of Chucho C functions. 
 */

#include <chucho/export.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Values that may be returned from Chucho C API functions.
 */
typedef enum
{
    CHUCHO_NO_ERROR,                /**< No error */
    CHUCHO_NO_SUCH_LEVEL,           /**< No such level */
    CHUCHO_NULL_POINTER,            /**< Null pointer */
    CHUCHO_INSUFFICIENT_BUFFER,     /**< Insufficient buffer */
    CHUCHO_OUT_OF_MEMORY,           /**< Out of memory */
    CHUCHO_TYPE_MISMATCH,           /**< Type mismatch */
    CHUCHO_INVALID_PATTERN,         /**< Invalid pattern */
    CHUCHO_INVALID_ARGUMENT,        /**< Invalid argument */
    CHUCHO_NO_SUCH_VALUE,           /**< No such value */
    CHUCHO_CONNECTION_ERROR,        /**< Connection error */
    CHUCHO_LEVEL_ALREADY_EXISTS,    /**< Level already exists */
    CHUCHO_VERSION_MISMATCH         /**< Incompatible version */

} chucho_rc;

/**
 * Return a message given a Chucho return code.
 * 
 * @param rc the return code
 * @return the message that corresponds to the code
 */
CHUCHO_EXPORT const char* chucho_rc_message(chucho_rc rc);

#if defined(__cplusplus)
}
#endif

#endif
