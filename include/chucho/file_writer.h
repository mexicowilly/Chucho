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

#if !defined(CHUCHO_FILE_WRITER_H__)
#define CHUCHO_FILE_WRITER_H__

#include <chucho/writer.h>
#include <chucho/formatter.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef enum
{
    CHUCHO_ON_START_APPEND,
    CHUCHO_ON_START_TRUNCATE
} chucho_on_start;

CHUCHO_EXPORT chucho_rc chucho_create_file_writer(chucho_writer** wrt,
                                            chucho_formatter* fmt,
                                            const char* const file_name,
                                            chucho_on_start on_start,
                                            int flush);

CHUCHO_EXPORT chucho_rc chucho_fwrt_get_file_name(const chucho_writer* wrt, const char** name);
CHUCHO_EXPORT chucho_rc chucho_fwrt_get_flush(const chucho_writer* wrt, int* flush);
CHUCHO_EXPORT chucho_rc chucho_fwrt_get_on_start(const chucho_writer* wrt, chucho_on_start* on_start);

#if defined(__cplusplus)
}
#endif

#endif
