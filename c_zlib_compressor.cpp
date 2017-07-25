/*
 * Copyright 2013-2017 Will Mason
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

#include <chucho/zlib_compressor.hpp>
#include <chucho/zlib_compressor.h>
#include <chucho/c_compressor.hpp>
#include <chucho/exception.hpp>

extern "C"
{

chucho_rc chucho_create_zlib_compressor(chucho_compressor** cmp, int compression_level)
{
    if (cmp == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        chucho_compressor* loc = new chucho_compressor();
        loc->cmp_ = std::make_shared<chucho::zlib_compressor>(compression_level);
        *cmp = loc;
    }
    catch (chucho::exception&)
    {
        return CHUCHO_VERSION_MISMATCH;
    }
    catch (std::invalid_argument&)
    {
        return CHUCHO_INVALID_ARGUMENT;
    }
    catch (...)
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
