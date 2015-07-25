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

#include <chucho/writer.h>
#include <chucho/c_writer.hpp>
#include <chucho/c_filter.hpp>
#include <chucho/c_formatter.hpp>

extern "C"
{

chucho_rc chucho_release_writer(chucho_writer* wrt)
{
    delete wrt;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_wrt_add_filter(chucho_writer* wrt, chucho_filter* flt)
{
    if (wrt == nullptr || flt == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        wrt->writer_->add_filter(flt->flt_);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return chucho_release_filter(flt);
}

chucho_rc chucho_wrt_clear_filters(chucho_writer* wrt)
{
    if (wrt == nullptr)
        return CHUCHO_NULL_POINTER; 
    wrt->writer_->clear_filters();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_wrt_get_filters(const chucho_writer* wrt, chucho_filter** buf, size_t buf_size, size_t* count)
{
    if (wrt == nullptr || count == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        auto flts = wrt->writer_->get_filters();
        *count = flts.size();
        if (buf_size < flts.size()) 
            return CHUCHO_INSUFFICIENT_BUFFER;
        if (buf == nullptr) 
            return CHUCHO_NULL_POINTER;
        unsigned idx = 0;
        for (auto flt : flts)
        {
            buf[idx] = new chucho_filter();
            buf[idx]->flt_ = flt;
            idx++;
        }
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_wrt_get_formatter(const chucho_writer* wrt, chucho_formatter** fmt)
{
    if (wrt == nullptr || fmt == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *fmt = new chucho_formatter();
        (*fmt)->fmt_ = wrt->writer_->get_formatter();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
