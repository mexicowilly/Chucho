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

#if !defined(CHUCHO_DIAGNOSTIC_CONTEXT_H__)
#define CHUCHO_DIAGNOSTIC_CONTEXT_H__

#include <chucho/export.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct
{
    char* key;
    char* value;
} chucho_dgc_node;

CHUCHO_EXPORT int chucho_dgc_release_node(chucho_dgc_node* node);

CHUCHO_EXPORT const char* chucho_dgc_at(const char* const key);
CHUCHO_EXPORT int chucho_dgc_clear(void);
CHUCHO_EXPORT int chucho_dgc_empty(void);
CHUCHO_EXPORT int chucho_dgc_erase(const char* const key);
CHUCHO_EXPORT int chucho_dgc_get(chucho_dgc_node** buf, size_t buf_size, size_t* count);
CHUCHO_EXPORT int chucho_dgc_set(const char* const key, const char* const value);

#if defined(__cplusplus)
}
#endif

#endif
