/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/zeromq_writer.hpp>
#include <chucho/zeromq_writer.h>
#include <chucho/c_formatter.hpp>
#include <chucho/c_serializer.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/exception.hpp>

extern "C"
{

chucho_rc chucho_create_zeromq_writer(chucho_writer** wrt,
                                      chucho_formatter* fmt,
                                      chucho_serializer* ser,
                                      const char* const endpoint,
                                      const unsigned char* const prefix,
                                      size_t prefix_len)
{
    if (wrt == nullptr || fmt == nullptr || ser == nullptr || endpoint == nullptr ||
        (prefix_len > 0 && prefix == nullptr))

        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer();
        std::vector<std::uint8_t> pfx;
        if (prefix_len > 0)
            pfx.assign(prefix, prefix + prefix_len);
        (*wrt)->writer_ = std::make_shared<chucho::zeromq_writer>(fmt->fmt_,
                                                                  ser->ser_,
                                                                  endpoint,
                                                                  pfx);
    }
    catch (chucho::exception&) 
    {
        delete *wrt;
        return CHUCHO_CONNECTION_ERROR;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    chucho_release_formatter(fmt);
    chucho_release_serializer(ser);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_zmqwrt_get_endpoint(const chucho_writer* wrt, const char** ep)
{
    if (wrt == nullptr || ep == nullptr)
        return CHUCHO_NULL_POINTER;
    auto zwrt = std::dynamic_pointer_cast<chucho::zeromq_writer>(wrt->writer_);
    if (!zwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *ep = zwrt->get_endpoint().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_zmqwrt_get_prefix(const chucho_writer* wrt,
                                   const unsigned char** pfx,
                                   size_t* prefix_len)
{
    if (wrt == nullptr || pfx == nullptr || prefix_len == nullptr)
        return CHUCHO_NULL_POINTER;
    auto zwrt = std::dynamic_pointer_cast<chucho::zeromq_writer>(wrt->writer_);
    if (!zwrt) 
        return CHUCHO_TYPE_MISMATCH;
    if (zwrt->get_prefix().empty())
    {
        *pfx = nullptr;
        *prefix_len = 0;
    }
    else
    {
        *pfx = &zwrt->get_prefix()[0];
        *prefix_len = zwrt->get_prefix().size();
    }
    return CHUCHO_NO_ERROR;
}

}
