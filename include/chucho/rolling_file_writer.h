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

#if !defined(CHUCHO_ROLLING_FILE_WRITER_H__)
#define CHUCHO_ROLLING_FILE_WRITER_H__

#include <chucho/file_writer.h>
#include <chucho/file_roller.h>
#include <chucho/file_roll_trigger.h>

#if defined(__cplusplus)
extern "C"
{
#endif

CHUCHO_EXPORT int chucho_create_rolling_file_writer(chucho_writer** wrt,
                                                    chucho_formatter* fmt,
                                                    chucho_file_roller* rlr,
                                                    chucho_file_roll_trigger* trg,
                                                    const char* const name,
                                                    chucho_on_start on_start,
                                                    int flush);

CHUCHO_EXPORT int chucho_rfwrt_get_file_roller(const chucho_writer* wrt, chucho_file_roller** rlr);
CHUCHO_EXPORT int chucho_rfwrt_get_file_roll_trigger(const chucho_writer* wrt, chucho_file_roll_trigger** trg);

#if defined(__cplusplus)
}
#endif

#endif
