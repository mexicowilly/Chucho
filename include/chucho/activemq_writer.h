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

#if !defined(CHUCHO_ACTIVEMQ_WRITER_H__)
#define CHUCHO_ACTIVEMQ_WRITER_H__

/**
 * @file 
 * Functions for ActiveMQ writers
 * @copydoc chucho::activemq_writer
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
 * Whether messages should be sent to a queue or a topic
 */
typedef enum
{
    CHUCHO_ACTIVEMQ_QUEUE,     /**< Consume from a queue */
    CHUCHO_ACTIVEMQ_TOPIC      /**< Consume from a topic */
} chucho_activemq_consumer_type;

/**
 * @name Creation 
 * @{ 
 */
/**
 * Create an ActiveMQ writer.
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
 * @param[out] wrt the ActiveMQ writer to create
 * @param[in] fmt the formatter
 * @param[in] ser the serializer
 * @param[in] coalesce_max The maximum number of log events to hold in
 * cache before sending them. If -1, then the default of 25 will be used.
 * @param[in] cmp the compressor, which may be NULL
 * @param[in] broker the URI of the ActiveMQ broker
 * @param[in] tp the type of consumer, queue or topic
 * @param[in] topic_or_queue the name of the topic or queue
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_activemq_writer(chucho_writer** wrt,
                                                      chucho_formatter* fmt,
                                                      chucho_serializer* ser,
                                                      int coalesce_max,
                                                      chucho_compressor* cmp,
                                                      const char* const broker,
                                                      chucho_activemq_consumer_type tp,
                                                      const char* const topic_or_queue);
/**
 * @}
 */

/**
 * Return the broker.
 * 
 * @param[in] wrt the ActiveMQ writer
 * @param[out] brk the broker
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_amqwrt_get_broker(const chucho_writer* wrt, const char** brk);
/**
 * Return the consumer type.
 * 
 * @param[in] wrt the ActiveMQ writer
 * @param[out] tp the consumer type
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_amqwrt_get_consumer_type(const chucho_writer* wrt,
                                                        chucho_activemq_consumer_type* tp);

/**
 * Return the topic or queue.
 * 
 * @param[in] wrt the ActiveMQ writer
 * @param[out] toq the topic or queue
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_amqwrt_get_topic_or_queue(const chucho_writer* wrt, const char** toq);

#if defined(__cplusplus)
}
#endif

#endif
