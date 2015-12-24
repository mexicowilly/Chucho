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

#include <chucho/windows_event_log_writer.hpp>
#include <chucho/windows_event_log_writer.h>
#include <chucho/c_writer.hpp>
#include <chucho/c_formatter.hpp>

extern "C"
{

chucho_rc chucho_create_windows_event_log_writer(chucho_writer** wrt,
                                                 chucho_formatter* fmt,
                                                 const char* const log,
                                                 const char* const source,
                                                 const char* const host)
{
    if (wrt == nullptr || fmt == nullptr || log == nullptr || source == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer();
        if (host == nullptr)
        {
            (*wrt)->writer_ = std::make_shared<chucho::windows_event_log_writer>(fmt->fmt_,
                                                                                 log,
                                                                                 source);
        }
        else
        {
            (*wrt)->writer_ = std::make_shared<chucho::windows_event_log_writer>(fmt->fmt_,
                                                                                 log,
                                                                                 source,
                                                                                 host);
        }
    }
    catch (std::bad_alloc&)
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    catch (std::invalid_argument&)
    {
        delete *wrt;
        *wrt = nullptr;
        return CHUCHO_INVALID_ARGUMENT;
    }
    catch (std::exception&)
    {
        delete *wrt;
        *wrt = nullptr;
        return CHUCHO_CONNECTION_ERROR;
    }
    chucho_release_formatter(fmt);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_welwrt_get_host(const chucho_writer* wrt, const char** host)
{
    if (wrt == nullptr || host == nullptr)
        return CHUCHO_NULL_POINTER;
    auto welwrt = std::dynamic_pointer_cast<chucho::windows_event_log_writer>(wrt->writer_);
    if (!welwrt) 
        return CHUCHO_TYPE_MISMATCH;
    const std::string& h = welwrt->get_host();
    *host = h.empty() ? nullptr : h.c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_welwrt_get_log(const chucho_writer* wrt, const char** log)
{
    if (wrt == nullptr || log == nullptr)
        return CHUCHO_NULL_POINTER;
    auto welwrt = std::dynamic_pointer_cast<chucho::windows_event_log_writer>(wrt->writer_);
    if (!welwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *log = welwrt->get_log().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_welwrt_get_source(const chucho_writer* wrt, const char** source)
{
    if (wrt == nullptr || source == nullptr)
        return CHUCHO_NULL_POINTER;
    auto welwrt = std::dynamic_pointer_cast<chucho::windows_event_log_writer>(wrt->writer_);
    if (!welwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *source = welwrt->get_source().c_str();
    return CHUCHO_NO_ERROR;
}

}
