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

#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/size_file_roll_trigger.h>
#include <chucho/c_file_roll_trigger.hpp>

extern "C"
{

chucho_rc chucho_create_size_file_roll_trigger(chucho_file_roll_trigger** trg,
                                         off_t max_size)
{
    if (trg == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *trg = new chucho_file_roll_trigger;
        (*trg)->trg_ = std::make_shared<chucho::size_file_roll_trigger>(max_size);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sftrg_get_max_size(const chucho_file_roll_trigger* trg, off_t* sz)
{
    if (trg == nullptr || sz == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto sft = std::dynamic_pointer_cast<chucho::size_file_roll_trigger>(trg->trg_);
    if (!sft) 
        return CHUCHO_TYPE_MISMATCH;
    *sz = static_cast<off_t>(sft->get_max_size());
    return CHUCHO_NO_ERROR;
}

}
