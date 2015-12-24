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

#include <chucho/duplicate_message_filter.hpp>
#include <chucho/duplicate_message_filter.h>
#include <chucho/c_filter.hpp>

extern "C"
{

chucho_rc chucho_create_duplicate_message_filter(chucho_filter** flt)
{
    if (flt == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *flt = new chucho_filter();
        (*flt)->flt_ = std::make_shared<chucho::duplicate_message_filter>();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
