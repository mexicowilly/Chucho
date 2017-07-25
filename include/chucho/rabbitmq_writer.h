/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_RABBITMQ_WRITER_H__)
#define CHUCHO_RABBITMQ_WRITER_H__

/**
 * @file 
 * Functions for RabbitMQ writers
 * @copydoc chucho::rabbitmq_writer
 *  
 * @ingroup c_writers c_mq
 */

#include <chucho/writer.h>
#include <chucho/formatter.h>
#include <chucho/serializer.h>
#include <chucho/compressor.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation 
 * @{ 
 */
/**
 * Create a RabbitMQ writer.
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
 * @post Ownership of the cmp parameter is transferred to the
 *       callee.
 * 
 * @note The cmp parameter may be NULL.
 * 
 * @note The routing_key parameter may be NULL.
 * 
 * @param[out] wrt the RabbitMQ writer to create
 * @param[in] fmt the formatter
 * @param[in] ser the serializer
 * @param[in] cmp the compressor, which may be NULL
 * @param[in] url the URL of the RabbitMQ instance
 * @param[in] exchange the exchange to which to post messages
 * @param[in] routing_key the routing key, which may be NULL
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_rabbitmq_writer(chucho_writer** wrt,
                                                      chucho_formatter* fmt,
                                                      chucho_serializer* ser,
                                                      chucho_compressor* cmp,
                                                      const char* const url,
                                                      const char* const exchange,
                                                      const char* const routing_key);
/**
 * @}
 */

/**
 * Return the exchange
 * 
 * @param[in] wrt the RabbitMQ writer
 * @param[out] ex the exchange
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rmqwrt_get_exchange(const chucho_writer* wrt, const char** ex);
/**
 * Return the routing key.
 * 
 * @param[in] wrt the RabbitMQ writer
 * @param[out] routing_key the routing key
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rmqwrt_get_routing_key(const chucho_writer* wrt, const char** routing_key);
/**
 * Return the URL.
 * 
 * @param[in] wrt the RabbitMQ writer
 * @param[out] url the URL
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rmqwrt_get_url(const chucho_writer* wrt, const char** url);

#if defined(__cplusplus)
}
#endif

#endif
