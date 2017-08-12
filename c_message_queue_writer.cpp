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

#include <chucho/message_queue_writer.h>
#include <chucho/message_queue_writer.hpp>
#include <chucho/c_writer.hpp>

extern "C"
{

chucho_rc chucho_mqwrt_get_coalesce_max(const chucho_writer* wrt, size_t* cmax)
{
    if (wrt == nullptr || cmax == nullptr)
        return CHUCHO_NULL_POINTER;
    auto mqwrt = std::dynamic_pointer_cast<chucho::message_queue_writer>(wrt->writer_);
    if (!mqwrt)
        return CHUCHO_TYPE_MISMATCH;
    *cmax = mqwrt->get_coalesce_max();
    return CHUCHO_NO_ERROR;

}

}
