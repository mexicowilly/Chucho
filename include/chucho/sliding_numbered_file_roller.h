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

#if !defined(CHUCHO_SLIDING_NUMBERED_FILE_ROLLER_H__)
#define CHUCHO_SLIDING_NUMBERED_FILE_ROLLER_H__

#include <chucho/file_roller.h>

#if defined(__cplusplus)
extern "C"
{
#endif

CHUCHO_EXPORT chucho_rc chucho_create_sliding_numbered_file_roller(chucho_file_roller** rlr,
                                                             int min_index,
                                                             size_t max_count,
                                                             chucho_file_compressor* cmp);

CHUCHO_EXPORT chucho_rc chucho_snfrlr_get_current_index(const chucho_file_roller* rlr, int* idx);
CHUCHO_EXPORT chucho_rc chucho_snfrlr_get_max_count(const chucho_file_roller* rlr, size_t* cnt);
CHUCHO_EXPORT chucho_rc chucho_snfrlr_get_min_index(const chucho_file_roller* rlr, int* idx);

#if defined(__cplusplus)
}
#endif

#endif
