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

#include <chucho/file_descriptor_writer.hpp>
#include <chucho/file_descriptor_writer.h>
#include <chucho/c_writer.hpp>
#include <chucho/c_formatter.hpp>

extern "C"
{

chucho_rc chucho_create_file_descriptor_writer(chucho_writer** wrt,
                                               chucho_formatter* fmt,
                                               int fd,
                                               int flush)
{
    if (wrt == nullptr || fmt == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        chucho_writer* loc = new chucho_writer();
        loc->writer_ = std::make_shared<chucho::file_descriptor_writer>(fmt->fmt_,
                                                                        fd,
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

chucho_rc chucho_fdwrt_get_flush(const chucho_writer* wrt, int* flush)
{
    if (wrt == nullptr || flush == nullptr)
        return CHUCHO_NULL_POINTER;
    auto fdwrt = std::dynamic_pointer_cast<chucho::file_descriptor_writer>(wrt->writer_);
    if (!fdwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *flush = fdwrt->get_flush() ? 1 : 0;
    return CHUCHO_NO_ERROR;
}

}