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

#if !defined(CHUCHO_STATUS_MANAGER_H__)
#define CHUCHO_STATUS_MANAGER_H__

#include <chucho/export.h>
#include <chucho/status.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef void (*chucho_status_observer)(const chucho_status* st);

CHUCHO_EXPORT int chucho_status_add_observer(chucho_status_observer obs);
CHUCHO_EXPORT int chucho_status_clear(void);
CHUCHO_EXPORT int chucho_status_get_count(size_t* cnt);
CHUCHO_EXPORT int chucho_status_get_level(chucho_status_level* lvl);
CHUCHO_EXPORT int chucho_status_remove_observer(chucho_status_observer obs);

#if defined(__cplusplus)
}
#endif

#endif
