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

#if !defined(CHUCHO_WRITER_H__)
#define CHUCHO_WRITER_H__

#include <chucho/filter.h>
#include <chucho/formatter.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct chucho_writer chucho_writer;

CHUCHO_EXPORT chucho_rc chucho_release_writer(chucho_writer* wrt);

CHUCHO_EXPORT chucho_rc chucho_wrt_add_filter(chucho_writer* wrt, chucho_filter* flt);
CHUCHO_EXPORT chucho_rc chucho_wrt_clear_filters(chucho_writer* wrt);
CHUCHO_EXPORT chucho_rc chucho_wrt_get_filters(const chucho_writer* wrt, chucho_filter** buf, size_t buf_size, size_t* count);
CHUCHO_EXPORT chucho_rc chucho_wrt_get_formatter(const chucho_writer* wrt, chucho_formatter** fmt);

#if defined(__cplusplus)
}
#endif

#endif
