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

#if !defined(CHUCHO_FILE_ROLLER_H__)
#define CHUCHO_FILE_ROLLER_H__

#include <chucho/file_compressor.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct chucho_file_roller chucho_file_roller;

CHUCHO_EXPORT int chucho_release_file_roller(chucho_file_roller* rlr);

CHUCHO_EXPORT int chucho_rlr_get_active_file_name(const chucho_file_roller* rlr, char* buf, size_t buf_size, size_t* count);
CHUCHO_EXPORT int chucho_rlr_get_file_compressor(const chucho_file_roller* rlr, chucho_file_compressor** cmp);

#if defined(__cplusplus)
}
#endif

#endif
