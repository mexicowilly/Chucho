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

#include <chucho/cerr_writer.hpp>
#include <chucho/cerr_writer.h>
#include <chucho/c_formatter.hpp>
#include <chucho/c_writer.hpp>

extern "C"
{

chucho_rc chucho_create_cerr_writer(chucho_writer** wrt, chucho_formatter* fmt)
{
    if (wrt == nullptr || fmt == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer();
        (*wrt)->writer_ = std::make_shared<chucho::cerr_writer>(fmt->fmt_);
    }
    catch (...) 
    {
        delete *wrt;
        return CHUCHO_OUT_OF_MEMORY;
    }
    chucho_release_formatter(fmt);
    return CHUCHO_NO_ERROR;
}

}
