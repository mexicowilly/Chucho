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

#include <chucho/door_writer.hpp>
#include <chucho/door_writer.h>
#include <chucho/c_writer.hpp>
#include <chucho/c_formatter.hpp>

extern "C"
{

chucho_rc chucho_create_door_writer(chucho_writer** wrt,
                                    chucho_formatter* fmt,
                                    const char* const file_name)
{
    if (wrt == nullptr || fmt == nullptr || file_name == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        chucho_writer* loc = new chucho_writer();
        loc->writer_ = std::make_shared<chucho::door_writer>(fmt->fmt_,
                                                             file_name);
        *wrt = loc;
        chucho_release_formatter(fmt);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_dwrt_get_file_name(const chucho_writer* wrt, const char** name)
{
    if (wrt == nullptr || name == nullptr)
        return CHUCHO_NULL_POINTER;
    auto dwrt = std::dynamic_pointer_cast<chucho::door_writer>(wrt->writer_);
    if (!dwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *name = dwrt->get_file_name().c_str();
    return CHUCHO_NO_ERROR;
}

}