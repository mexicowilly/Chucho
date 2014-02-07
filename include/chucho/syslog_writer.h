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

#if !defined(CHUCHO_SYSLOG_WRITER_H__)
#define CHUCHO_SYSLOG_WRITER_H__

#include <chucho/writer.h>
#include <chucho/formatter.h>
#include <chucho/syslog_constants.h>

#if defined(__cplusplus)
extern "C"
{
#endif

CHUCHO_EXPORT chucho_rc chucho_create_local_syslog_writer(chucho_writer** wrt,
                                                    chucho_formatter* fmt,
                                                    chucho_syslog_facility fcl);
CHUCHO_EXPORT chucho_rc chucho_create_remote_syslog_writer(chucho_writer** wrt,
                                                     chucho_formatter* fmt,
                                                     chucho_syslog_facility fcl,
                                                     const char* const host,
                                                     unsigned port);

CHUCHO_EXPORT chucho_rc chucho_slwrt_get_facility(const chucho_writer* wrt, chucho_syslog_facility* fcl);
CHUCHO_EXPORT chucho_rc chucho_slwrt_get_host_name(const chucho_writer* wrt, const char** name);
CHUCHO_EXPORT chucho_rc chucho_slwrt_get_port(const chucho_writer* wrt, unsigned* port);

#if defined(__cplusplus)
}
#endif

#endif
