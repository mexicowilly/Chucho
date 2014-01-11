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

#if !defined(CHUCHO_FILE_COMPRESSOR_H__)
#define CHUCHO_FILE_COMPRESSOR_H__

#include <chucho/export.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct chucho_file_compressor chucho_file_compressor;

CHUCHO_EXPORT int chucho_release_file_compressor(chucho_file_compressor* cmp);

CHUCHO_EXPORT int chucho_cmp_compress(chucho_file_compressor* cmp, const char* const file_name);
CHUCHO_EXPORT int chucho_cmp_get_extension(const chucho_file_compressor* cmp, const char** ext);
CHUCHO_EXPORT int chucho_cmp_get_min_index(const chucho_file_compressor* cmp, unsigned* idx);

#if defined(__cplusplus)
}
#endif

#endif
