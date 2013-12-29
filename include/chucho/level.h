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

#if !defined(CHUCHO_LEVEL_H__)
#define CHUCHO_LEVEL_H__

#include <chucho/export.h>
#include <chucho/syslog_constants.h>

#if defined(__cplusplus)
extern "C"
{
#endif

struct chucho_level;

CHUCHO_EXPORT int chucho_get_level(const chucho_level** lvl, const char* const name);
CHUCHO_EXPORT const chucho_level* chucho_trace_level(void);
CHUCHO_EXPORT const chucho_level* chucho_debug_level(void);
CHUCHO_EXPORT const chucho_level* chucho_info_level(void);
CHUCHO_EXPORT const chucho_level* chucho_warn_level(void);
CHUCHO_EXPORT const chucho_level* chucho_error_level(void);
CHUCHO_EXPORT const chucho_level* chucho_fatal_level(void);
CHUCHO_EXPORT const chucho_level* chucho_off_level(void);

CHUCHO_EXPORT const char* chucho_lvl_get_name(const chucho_level* lvl);
CHUCHO_EXPORT chucho_syslog_severity chucho_lvl_get_syslog_severity(const chucho_level* lvl);
CHUCHO_EXPORT int chucho_lvl_get_value(const chucho_level* lvl);

#if defined(__cplusplus)
}
#endif

#endif
