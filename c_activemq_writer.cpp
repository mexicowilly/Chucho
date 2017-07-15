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

#include <chucho/activemq_writer.hpp>
#include <chucho/activemq_writer.h>
#include <chucho/c_formatter.hpp>
#include <chucho/c_serializer.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/c_compressor.hpp>

extern "C"
{

chucho_rc chucho_create_activemq_writer(chucho_writer** wrt,
                                        chucho_formatter* fmt,
                                        chucho_serializer* ser,
                                        chucho_compressor* cmp,
                                        const char* const broker,
                                        chucho_activemq_consumer_type tp,
                                        const char* const topic_or_queue)
{
    if (wrt == nullptr || fmt == nullptr || ser == nullptr ||
        broker == nullptr || topic_or_queue == nullptr)
    {
        return CHUCHO_NULL_POINTER;
    }
    try
    {
        *wrt = new chucho_writer();
        chucho::activemq_writer::consumer_type cxxtp = (tp == CHUCHO_ACTIVEMQ_QUEUE) ?
            chucho::activemq_writer::consumer_type::QUEUE : chucho::activemq_writer::consumer_type::TOPIC;
        std::shared_ptr<chucho::compressor> pcmp;
        if (cmp != nullptr)
            pcmp = cmp->cmp_;
        (*wrt)->writer_ = std::make_shared<chucho::activemq_writer>(fmt->fmt_,
                                                                    ser->ser_,
                                                                    pcmp,
                                                                    broker,
                                                                    cxxtp,
                                                                    topic_or_queue);
    }
    catch (cms::CMSException& cmse)
    {
        delete *wrt;
        *wrt = nullptr;
        return CHUCHO_CONNECTION_ERROR;
    }
    catch (...)
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    chucho_release_formatter(fmt);
    chucho_release_serializer(ser);
    if (cmp != nullptr)
        chucho_release_compressor(cmp);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_amqwrt_get_broker(const chucho_writer* wrt, const char** brk)
{
    if (wrt == nullptr || brk == nullptr)
        return CHUCHO_NULL_POINTER;
    auto amqwrt = std::dynamic_pointer_cast<chucho::activemq_writer>(wrt->writer_);
    if (!amqwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *brk = amqwrt->get_broker().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_amqwrt_get_consumer_type(const chucho_writer* wrt,
                                          chucho_activemq_consumer_type* tp)
{
    if (wrt == nullptr || tp == nullptr)
        return CHUCHO_NULL_POINTER;
    auto amqwrt = std::dynamic_pointer_cast<chucho::activemq_writer>(wrt->writer_);
    if (!amqwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *tp = (amqwrt->get_consumer_type() == chucho::activemq_writer::consumer_type::QUEUE) ?
        CHUCHO_ACTIVEMQ_QUEUE : CHUCHO_ACTIVEMQ_TOPIC;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_amqwrt_get_topic_or_queue(const chucho_writer* wrt, const char** toq)
{
    if (wrt == nullptr || toq == nullptr)
        return CHUCHO_NULL_POINTER;
    auto amqwrt = std::dynamic_pointer_cast<chucho::activemq_writer>(wrt->writer_);
    if (!amqwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *toq = amqwrt->get_topic_or_queue().c_str();
    return CHUCHO_NO_ERROR;
}

}
