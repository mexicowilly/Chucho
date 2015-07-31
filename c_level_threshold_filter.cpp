/*
 * Copyright 2013-2015 Will Mason
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

#include <chucho/level_threshold_filter.hpp>
#include <chucho/level_threshold_filter.h>
#include <chucho/c_filter.hpp>
#include <chucho/c_util.hpp>

extern "C"
{

chucho_rc chucho_create_level_threshold_filter(chucho_filter** flt,
                                         const chucho_level* lvl)
{
    if (flt == nullptr || lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *flt = new chucho_filter();
        (*flt)->flt_ = std::make_shared<chucho::level_threshold_filter>(lvl->level_);
    }
    catch (...) 
    {
        delete *flt;
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_ltflt_get_level(const chucho_filter* flt, const chucho_level** lvl)
{
    if (flt == nullptr || lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto ltflt = std::dynamic_pointer_cast<chucho::level_threshold_filter>(flt->flt_);
    if (!ltflt) 
        return CHUCHO_TYPE_MISMATCH;
    *lvl = chucho::c_util::cpp_level_to_c(ltflt->get_level());
    return CHUCHO_NO_ERROR;
}

}
