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

#if !defined(CHUCHO_REMOTE_WRITER_H__)
#define CHUCHO_REMOTE_WRITER_H__

#include <chucho/writer.h>

#if defined(__cplusplus)
extern "C"
{
#endif

extern unsigned CHUCHO_DEFAULT_REMOTE_WRITER_PORT;
extern size_t CHUCHO_DEFAULT_REMOTE_UNSENT_CACHE_MAX;

CHUCHO_EXPORT int chucho_create_remote_writer(chucho_writer** wrt,
                                              const char* const host,
                                              unsigned port,
                                              size_t unsent_cache_max);

CHUCHO_EXPORT int chucho_rwrt_get_host(const chucho_writer* wrt, const char** host);
CHUCHO_EXPORT int chucho_rwrt_get_port(const chucho_writer* wrt, unsigned* port);
CHUCHO_EXPORT int chucho_rwrt_get_unsent_cache_max(const chucho_writer* wrt, size_t* cmax);
CHUCHO_EXPORT int chucho_rwrt_get_unsent_cache_size(const chucho_writer* wrt, size_t* csize);

#if defined(__cplusplus)
}
#endif

#endif
