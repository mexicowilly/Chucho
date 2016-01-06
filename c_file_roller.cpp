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

#include <chucho/file_roller.h>
#include <chucho/c_file_roller.hpp>
#include <chucho/c_file_compressor.hpp>
#include <cstring>

extern "C"
{

chucho_rc chucho_release_file_roller(chucho_file_roller* rlr)
{
    delete rlr;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rlr_get_active_file_name(const chucho_file_roller* rlr, char* buf, size_t buf_size, size_t* count)
{
    if (rlr == nullptr || count == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        std::string fn = rlr->rlr_->get_active_file_name();
        *count = fn.length() + 1;
        if (buf == nullptr) 
            return CHUCHO_NULL_POINTER;
        if (*count > buf_size) 
            return CHUCHO_INSUFFICIENT_BUFFER;
        std::strcpy(buf, fn.c_str());
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rlr_get_file_compressor(const chucho_file_roller* rlr, chucho_file_compressor** cmp)
{
    if (rlr == nullptr || cmp == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        auto cpp = rlr->rlr_->get_file_compressor();
        if (cpp)
        {
            *cmp = new chucho_file_compressor;
            (*cmp)->compressor_ = cpp;
        }
        else
        {
            *cmp = nullptr;
        }
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
