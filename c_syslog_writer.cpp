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

#include <chucho/syslog_writer.hpp>
#include <chucho/syslog_writer.h>
#include <chucho/c_writer.hpp>
#include <chucho/c_formatter.hpp>
#include <chucho/exception.hpp>

extern "C"
{

chucho_rc chucho_create_local_syslog_writer(chucho_writer** wrt,
                                            chucho_formatter* fmt,
                                            chucho_syslog_facility fcl)
{
    if (wrt == nullptr || fmt == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer();
        (*wrt)->writer_ = std::make_shared<chucho::syslog_writer>(fmt->fmt_,
                                                                  static_cast<chucho::syslog::facility>(fcl));
        chucho_release_formatter(fmt);
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
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_create_remote_syslog_writer(chucho_writer** wrt,
                                             chucho_formatter* fmt,
                                             chucho_syslog_facility fcl,
                                             const char* const host,
                                             unsigned port)
{
    if (wrt == nullptr || fmt == nullptr || host == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer();
        (*wrt)->writer_ = std::make_shared<chucho::syslog_writer>(fmt->fmt_,
                                                                  static_cast<chucho::syslog::facility>(fcl),
                                                                  host,
                                                                  port);
        chucho_release_formatter(fmt);
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
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_slwrt_get_facility(const chucho_writer* wrt, chucho_syslog_facility* fcl)
{
    if (wrt == nullptr || fcl == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto sl = std::dynamic_pointer_cast<chucho::syslog_writer>(wrt->writer_);
    if (!sl) 
        return CHUCHO_TYPE_MISMATCH;
    *fcl = static_cast<chucho_syslog_facility>(sl->get_facility());
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_slwrt_get_host_name(const chucho_writer* wrt, const char** name)
{
    if (wrt == nullptr || name == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto sl = std::dynamic_pointer_cast<chucho::syslog_writer>(wrt->writer_);
    if (!sl) 
        return CHUCHO_TYPE_MISMATCH;
    const std::string& hn(sl->get_host_name());
    if (hn.empty()) 
        return CHUCHO_NO_SUCH_VALUE;
    *name = hn.c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_slwrt_get_port(const chucho_writer* wrt, unsigned* port)
{
    if (wrt == nullptr || port == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto sl = std::dynamic_pointer_cast<chucho::syslog_writer>(wrt->writer_);
    if (!sl) 
        return CHUCHO_TYPE_MISMATCH;
    const chucho::optional<std::uint16_t> p(sl->get_port());
    if (!p)
        return CHUCHO_NO_SUCH_VALUE;
    *port = *p;
    return CHUCHO_NO_ERROR;
}

}
