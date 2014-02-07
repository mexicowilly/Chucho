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

#include <chucho/pattern_formatter.hpp>
#include <chucho/pattern_formatter.h>
#include <chucho/exception.hpp>
#include <chucho/c_formatter.hpp>

extern "C"
{

chucho_rc chucho_create_pattern_formatter(chucho_formatter** fmt, const char* const pattern)
{
    if (fmt == nullptr || pattern == nullptr)
        return CHUCHO_NULL_POINTER;
    chucho_formatter* loc = new chucho_formatter();
    try
    {
        loc->fmt_ = std::make_shared<chucho::pattern_formatter>(pattern);
        *fmt = loc;
    }
    catch (chucho::exception&) 
    {
        delete loc;
        return CHUCHO_INVALID_PATTERN;
    }
    catch (...)
    {
        delete loc;
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
