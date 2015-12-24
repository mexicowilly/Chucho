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

#include <chucho/file_writer.hpp>
#include <chucho/file_writer.h>
#include <chucho/c_writer.hpp>
#include <chucho/c_formatter.hpp>

extern "C"
{

chucho_rc chucho_create_file_writer(chucho_writer** wrt,
                              chucho_formatter* fmt,
                              const char* const file_name,
                              chucho_on_start on_start,
                              int flush)
{
    if (wrt == nullptr || fmt == nullptr || file_name == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        chucho_writer* loc = new chucho_writer();
        chucho::file_writer::on_start ons = on_start == CHUCHO_ON_START_APPEND ?
            chucho::file_writer::on_start::APPEND : chucho::file_writer::on_start::TRUNCATE;
        loc->writer_ = std::make_shared<chucho::file_writer>(fmt->fmt_,
                                                             file_name,
                                                             ons,
                                                             flush == 0 ? false : true);
        *wrt = loc;
        chucho_release_formatter(fmt);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_fwrt_get_file_name(const chucho_writer* wrt, const char** name)
{
    if (wrt == nullptr || name == nullptr)
        return CHUCHO_NULL_POINTER;
    auto fwrt = std::dynamic_pointer_cast<chucho::file_writer>(wrt->writer_);
    if (!fwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *name = fwrt->get_file_name().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_fwrt_get_flush(const chucho_writer* wrt, int* flush)
{
    if (wrt == nullptr || flush == nullptr)
        return CHUCHO_NULL_POINTER;
    auto fwrt = std::dynamic_pointer_cast<chucho::file_writer>(wrt->writer_);
    if (!fwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *flush = fwrt->get_flush() ? 1 : 0;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_fwrt_get_on_start(const chucho_writer* wrt, chucho_on_start* on_start)
{
    if (wrt == nullptr || on_start == nullptr)
        return CHUCHO_NULL_POINTER;
    auto fwrt = std::dynamic_pointer_cast<chucho::file_writer>(wrt->writer_);
    if (!fwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *on_start = fwrt->get_on_start() == chucho::file_writer::on_start::APPEND ?
        CHUCHO_ON_START_APPEND : CHUCHO_ON_START_TRUNCATE;
    return CHUCHO_NO_ERROR;
}

}
