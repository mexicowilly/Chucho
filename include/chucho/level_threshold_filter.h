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

#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_H__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_H__

#include <chucho/filter.h>
#include <chucho/level.h>

#if defined(__cplusplus)
extern "C"
{
#endif

CHUCHO_EXPORT int chucho_create_level_threshold_filter(chucho_filter** flt,
                                                       const chucho_level* lvl);

CHUCHO_EXPORT int chucho_ltflt_get_level(const chucho_filter* flt, const chucho_level** lvl);

#if defined(__cplusplus)
}
#endif

#endif
