/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_ZEROMQ_WRITER_H__)
#define CHUCHO_ZEROMQ_WRITER_H__

/**
 * @file 
 * Functions for ZeroMQ writers
 * @copydoc chucho::zeromq_writer
 *  
 * @ingroup c_writers c_message_queue
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>
#include <chucho/serializer.h>

#if defined(__cplusplus)
extern "C"
{
#endif

CHUCHO_EXPORT chucho_rc chucho_create_zeromq_writer(chucho_writer** wrt,
                                                    chucho_formatter* fmt,
                                                    chucho_serializer* ser,
                                                    const char* const endpoint,
                                                    const unsigned char* const prefix,
                                                    size_t prefix_len);

CHUCHO_EXPORT chucho_rc chucho_zmqwrt_get_endpoint(const chucho_writer* wrt, const char** ep);
CHUCHO_EXPORT chucho_rc chucho_zmqwrt_get_prefix(const chucho_writer* wrt,
                                                 const unsigned char** pfx,
                                                 size_t* prefix_len);

#if defined(__cplusplus)
}
#endif

#endif