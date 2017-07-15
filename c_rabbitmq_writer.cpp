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

#include <chucho/rabbitmq_writer.hpp>
#include <chucho/rabbitmq_writer.h>
#include <chucho/c_formatter.hpp>
#include <chucho/c_serializer.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/c_compressor.hpp>
#include <chucho/exception.hpp>

extern "C"
{

chucho_rc chucho_create_rabbitmq_writer(chucho_writer** wrt,
                                        chucho_formatter* fmt,
                                        chucho_serializer* ser,
                                        chucho_compressor* cmp,
                                        const char* const url,
                                        const char* const exchange,
                                        const char* const routing_key)
{
    if (wrt == nullptr || fmt == nullptr || ser == nullptr ||
        url == nullptr || exchange == nullptr)
    {
        return CHUCHO_NULL_POINTER;
    }
    try
    {
        *wrt = new chucho_writer();
        std::shared_ptr<chucho::compressor> pcmp;
        if (cmp != nullptr)
            pcmp = cmp->cmp_;
        chucho::optional<std::string> rk;
        if (routing_key != nullptr)
            rk = routing_key;
        (*wrt)->writer_ = std::make_shared<chucho::rabbitmq_writer>(fmt->fmt_,
                                                                    ser->ser_,
                                                                    pcmp,
                                                                    url,
                                                                    exchange,
                                                                    rk);
    }
    catch (chucho::exception&)
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

chucho_rc chucho_rmqwrt_get_exchange(const chucho_writer* wrt, const char** ex)
{
    if (wrt == nullptr || ex == nullptr)
        return CHUCHO_NULL_POINTER;
    auto rmqwrt = std::dynamic_pointer_cast<chucho::rabbitmq_writer>(wrt->writer_);
    if (!rmqwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *ex = rmqwrt->get_exchange().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rmqwrt_get_routing_key(const chucho_writer* wrt, const char** routing_key)
{
    if (wrt == nullptr || routing_key == nullptr)
        return CHUCHO_NULL_POINTER;
    auto rmqwrt = std::dynamic_pointer_cast<chucho::rabbitmq_writer>(wrt->writer_);
    if (!rmqwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *routing_key = rmqwrt->get_routing_key().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rmqwrt_get_url(const chucho_writer* wrt, const char** url)
{
    if (wrt == nullptr || url == nullptr)
        return CHUCHO_NULL_POINTER;
    auto rmqwrt = std::dynamic_pointer_cast<chucho::rabbitmq_writer>(wrt->writer_);
    if (!rmqwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *url = rmqwrt->get_url().c_str();
    return CHUCHO_NO_ERROR;
}

}
