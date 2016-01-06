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

#include <chucho/level_threshold_email_trigger.hpp>
#include <chucho/level_threshold_email_trigger.h>
#include <chucho/c_email_trigger.hpp>
#include <chucho/c_util.hpp>

extern "C"
{

chucho_rc chucho_create_level_threshold_email_trigger(chucho_email_trigger** trg,
                                                      const chucho_level* lvl)
{
    if (trg == nullptr || lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *trg = new chucho_email_trigger();
        (*trg)->trg_ = std::make_shared<chucho::level_threshold_email_trigger>(lvl->level_);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ltetrg_get_level(const chucho_email_trigger* trg, const chucho_level** lvl)
{
    if (trg == nullptr || lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto lttrg = std::dynamic_pointer_cast<chucho::level_threshold_email_trigger>(trg->trg_);
    if (!lttrg) 
        return CHUCHO_TYPE_MISMATCH;
    *lvl = chucho::c_util::cpp_level_to_c(lttrg->get_level());
    return CHUCHO_NO_ERROR;
}

}
