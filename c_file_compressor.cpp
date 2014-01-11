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

#include <chucho/file_compressor.h>
#include <chucho/c_file_compressor.hpp>
#include <chucho/error.h>

extern "C"
{

int chucho_release_file_compressor(chucho_file_compressor* cmp)
{
    delete cmp;
    return CHUCHO_NO_ERROR;
}

int chucho_cmp_compress(chucho_file_compressor* cmp, const char* const file_name)
{
    if (cmp == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        cmp->compressor_->compress(file_name);
    }
    catch (...) 
    {
        return CHUCHO_COMPRESSION_FAILURE;
    }
    return CHUCHO_NO_ERROR;
}

int chucho_cmp_get_extension(const chucho_file_compressor* cmp, const char** ext)
{
    if (cmp == nullptr || ext == nullptr)
        return CHUCHO_NULL_POINTER; 
    *ext = cmp->compressor_->get_extension();
    return CHUCHO_NO_ERROR;
}

int chucho_cmp_get_min_index(const chucho_file_compressor* cmp, unsigned* idx)
{
    if (cmp == nullptr || idx == nullptr)
        return CHUCHO_NULL_POINTER; 
    *idx = cmp->compressor_->get_min_index();
    return CHUCHO_NO_ERROR;
}

}
