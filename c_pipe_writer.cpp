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

#include <chucho/pipe_writer.hpp>
#include <chucho/pipe_writer.h>
#include <chucho/c_writer.hpp>
#include <chucho/c_formatter.hpp>

extern "C"
{

chucho_rc chucho_create_pipe_writer(chucho_writer** wrt,
                                    chucho_formatter* fmt,
                                    int flush)
{
    if (wrt == nullptr || fmt == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer();
        (*wrt)->writer_ = std::make_shared<chucho::pipe_writer>(fmt->fmt_,
                                                                flush == 0 ? false : true);
        chucho_release_formatter(fmt);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_pwrt_get_flush(const chucho_writer* wrt, int* flush)
{
    if (wrt == nullptr || flush == nullptr)
        return CHUCHO_NULL_POINTER;
    auto pwrt = std::dynamic_pointer_cast<chucho::pipe_writer>(wrt->writer_);
    if (!pwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *flush = pwrt->get_flush() ? 1 : 0;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_pwrt_get_input(const chucho_writer* wrt, pipe_writer_pipe_type* p)
{
    if (wrt == nullptr || p == nullptr)
        return CHUCHO_NULL_POINTER;
    auto pwrt = std::dynamic_pointer_cast<chucho::pipe_writer>(wrt->writer_);
    if (!pwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *p = pwrt->get_input();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_pwrt_get_output(const chucho_writer* wrt, pipe_writer_pipe_type* p)
{
    if (wrt == nullptr || p == nullptr)
        return CHUCHO_NULL_POINTER;
    auto pwrt = std::dynamic_pointer_cast<chucho::pipe_writer>(wrt->writer_);
    if (!pwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *p = pwrt->get_output();
    return CHUCHO_NO_ERROR;
}

}
