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

#include <chucho/async_writer.h>
#include <chucho/async_writer.hpp>
#include <chucho/c_util.hpp>
#include <chucho/c_writer.hpp>
#include <chucho/error.h>

extern "C"
{

int chucho_create_async_writer(chucho_writer** async,
                               chucho_writer* wrt,
                               size_t capacity,
                               const chucho_level* discard_threshold)
{
    if (async == nullptr || wrt == nullptr || discard_threshold == nullptr) 
        return CHUCHO_NULL_POINTER;
    *async = new chucho_writer();
    (*async)->writer_ = std::make_shared<chucho::async_writer>(wrt->writer_,
                                                               capacity,
                                                               discard_threshold->level_);
    chucho_release_writer(wrt);
    return CHUCHO_NO_ERROR;
}

int chucho_aswrt_get_discard_threshold(const chucho_writer* wrt, const chucho_level** lvl)
{
    if (wrt == nullptr || lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto aswrt = std::dynamic_pointer_cast<chucho::async_writer>(wrt->writer_);
    if (!aswrt) 
        return CHUCHO_TYPE_MISMATCH;
    const chucho_level* clvl = chucho::c_util::cpp_level_to_c(aswrt->get_discard_threshold());
    if (clvl == nullptr) 
        return CHUCHO_NO_SUCH_LEVEL;
    *lvl = clvl;
    return CHUCHO_NO_ERROR;
}

int chucho_aswrt_get_queue_capacity(const chucho_writer* wrt, size_t* cap)
{
    if (wrt == nullptr || cap == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto aswrt = std::dynamic_pointer_cast<chucho::async_writer>(wrt->writer_);
    if (!aswrt) 
        return CHUCHO_TYPE_MISMATCH;
    *cap = aswrt->get_queue_capacity();
    return CHUCHO_NO_ERROR;
}

int chucho_aswrt_get_queue_size(const chucho_writer* wrt, size_t* sz)
{
    if (wrt == nullptr || sz == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto aswrt = std::dynamic_pointer_cast<chucho::async_writer>(wrt->writer_);
    if (!aswrt) 
        return CHUCHO_TYPE_MISMATCH;
    *sz = aswrt->get_queue_size();
    return CHUCHO_NO_ERROR;
}

int chucho_aswrt_get_writer(const chucho_writer* async, chucho_writer** wrt)
{
    if (async == nullptr || wrt == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto aswrt = std::dynamic_pointer_cast<chucho::async_writer>(async->writer_);
    if (!aswrt) 
        return CHUCHO_TYPE_MISMATCH;
    *wrt = new chucho_writer();
    (*wrt)->writer_ = aswrt->get_writer();
    return CHUCHO_NO_ERROR;
}

}
