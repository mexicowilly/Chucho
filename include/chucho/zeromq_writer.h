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
 * @ingroup c_writers c_mq
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>
#include <chucho/serializer.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation 
 * @{ 
 */
/**
 * Create a ZeroMQ writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and must be released with the @ref
 *       chucho_release_writer() function.
 *  
 * @post Ownership of the fmt parameter is transferred to the 
 *       callee.
 * 
 * @post Ownership of the ser parameter is transferred to the 
 *       callee.
 * 
 * @param[out] wrt the Oracle writer to create
 * @param[in] fmt the formatter
 * @param[in] ser the serializer
 * @param[in] endpoint the ZeroMQ endpoint to which to publish messages
 * @param[in] prefix the message "topic", which, if provided, will cause
 * all events to be published as two-part messages, first prefix, then
 * event.
 * @param[in] prefix_len the length of the prefix
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_zeromq_writer(chucho_writer** wrt,
                                                    chucho_formatter* fmt,
                                                    chucho_serializer* ser,
                                                    const char* const endpoint,
                                                    const unsigned char* const prefix,
                                                    size_t prefix_len);
/**
 * @}
 */

/**
 * Return the endpoint.
 * 
 * @param[in] wrt the ZeroMQ writer
 * @param[out] ep the endpoint
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_zmqwrt_get_endpoint(const chucho_writer* wrt, const char** ep);
/**
 * Return the prefix or "topic".
 * 
 * @param[in] wrt the ZeroMQ writer
 * @param[out] pfx the prefix
 * @param[out] prefix_len the length of the prefix
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_zmqwrt_get_prefix(const chucho_writer* wrt,
                                                 const unsigned char** pfx,
                                                 size_t* prefix_len);

#if defined(__cplusplus)
}
#endif

#endif