/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/sqlite_writer.hpp>
#include <chucho/sqlite_writer.h>
#include <chucho/c_formatter.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/exception.hpp>

extern "C"
{

chucho_rc chucho_create_sqlite_writer(chucho_writer** wrt,
                                      chucho_formatter* fmt,
                                      const char* const file_name)
{
    if (wrt == nullptr || fmt == nullptr || file_name == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer();
        (*wrt)->writer_ = std::make_shared<chucho::sqlite_writer>(fmt->fmt_,
                                                                  file_name);
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
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sltwrt_get_file_name(const chucho_writer* wrt, const char** file_name)
{
    if (wrt == nullptr || file_name == nullptr)
        return CHUCHO_NULL_POINTER;
    auto sltwrt = std::dynamic_pointer_cast<chucho::sqlite_writer>(wrt->writer_);
    if (!sltwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *file_name = sltwrt->get_file_name().c_str();
    return CHUCHO_NO_ERROR;
}

}