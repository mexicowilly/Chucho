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

#if !defined(CHUCHO_ERROR_H__)
#define CHUCHO_ERROR_H__

#include <chucho/export.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define CHUCHO_NO_ERROR                     0
#define CHUCHO_NO_SUCH_LEVEL                1
#define CHUCHO_NULL_POINTER                 2
#define CHUCHO_INSUFFICIENT_BUFFER          3
#define CHUCHO_FORMAT_ERROR                 4
#define CHUCHO_OUT_OF_MEMORY                5
#define CHUCHO_TYPE_MISMATCH                6
#define CHUCHO_INVALID_PATTERN              7
#define CHUCHO_INVALID_ARGUMENT             8

CHUCHO_EXPORT const char* chucho_error_message(int err);

#if defined(__cplusplus)
}
#endif

#endif
