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

#if !defined(CHUCHO_LEVEL_FILTER_H__)
#define CHUCHO_LEVEL_FILTER_H__

#include <chucho/filter.h>
#include <chucho/level.h>

#if defined(__cplusplus)
extern "C"
{
#endif

CHUCHO_EXPORT chucho_rc chucho_create_level_filter(chucho_filter** flt,
                                             const chucho_level* lvl,
                                             chucho_filter_result on_match,
                                             chucho_filter_result on_mismatch);

CHUCHO_EXPORT chucho_rc chucho_lflt_get_level(const chucho_filter* flt, const chucho_level** lvl);
CHUCHO_EXPORT chucho_rc chucho_lflt_get_on_match(const chucho_filter* flt, chucho_filter_result* res);
CHUCHO_EXPORT chucho_rc chucho_lflt_get_on_mismatch(const chucho_filter* flt, chucho_filter_result* res);

#if defined(__cplusplus)
}
#endif

#endif
