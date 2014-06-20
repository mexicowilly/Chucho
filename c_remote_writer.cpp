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

#include <chucho/remote_writer.hpp>
#include <chucho/remote_writer.h>
#include <chucho/c_writer.hpp>

extern "C"
{

unsigned CHUCHO_DEFAULT_REMOTE_WRITER_PORT = chucho::remote_writer::DEFAULT_PORT;
size_t CHUCHO_DEFAULT_REMOTE_UNSENT_CACHE_MAX = chucho::remote_writer::DEFAULT_UNSENT_CACHE_MAX;

chucho_rc chucho_create_remote_writer(chucho_writer** wrt,
                                const char* const host,
                                unsigned port,
                                size_t unsent_cache_max)
{
    if (wrt == nullptr || host == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *wrt = new chucho_writer;
        (*wrt)->writer_ = std::make_shared<chucho::remote_writer>(host, port, unsent_cache_max);
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

chucho_rc chucho_rwrt_get_host(const chucho_writer* wrt, const char** host)
{
    if (wrt == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto rwrt = std::dynamic_pointer_cast<chucho::remote_writer>(wrt->writer_);
    if (!rwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *host = rwrt->get_host().c_str();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rwrt_get_port(const chucho_writer* wrt, unsigned* port)
{
    if (wrt == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto rwrt = std::dynamic_pointer_cast<chucho::remote_writer>(wrt->writer_);
    if (!rwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *port = rwrt->get_port();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rwrt_get_unsent_cache_max(const chucho_writer* wrt, size_t* cmax)
{
    if (wrt == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto rwrt = std::dynamic_pointer_cast<chucho::remote_writer>(wrt->writer_);
    if (!rwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *cmax = rwrt->get_unsent_cache_max();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rwrt_get_unsent_cache_size(const chucho_writer* wrt, size_t* csize)
{
    if (wrt == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto rwrt = std::dynamic_pointer_cast<chucho::remote_writer>(wrt->writer_);
    if (!rwrt) 
        return CHUCHO_TYPE_MISMATCH;
    *csize = rwrt->get_unsent_cache_size();
    return CHUCHO_NO_ERROR;
}

}
