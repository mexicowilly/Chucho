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

#if !defined(CHUCHO_ASYNC_WRITER_H__)
#define CHUCHO_ASYNC_WRITER_H__

#include <chucho/writer.h>
#include <chucho/export.h>
#include <chucho/level.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

CHUCHO_EXPORT int chucho_create_async_writer(chucho_writer** async,
                                             chucho_writer* wrt,
                                             size_t capacity,
                                             const chucho_level* discard_threshold);

CHUCHO_EXPORT int chucho_aswrt_get_discard_threshold(const chucho_writer* wrt, const chucho_level** lvl);
CHUCHO_EXPORT int chucho_aswrt_get_queue_capacity(const chucho_writer* wrt, size_t* cap);
CHUCHO_EXPORT int chucho_aswrt_get_queue_size(const chucho_writer* wrt, size_t* sz);
CHUCHO_EXPORT int chucho_aswrt_get_writer(const chucho_writer* async, chucho_writer** wrt);

#if defined(__cplusplus)
}
#endif

#endif
