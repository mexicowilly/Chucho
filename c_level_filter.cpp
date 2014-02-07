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

#include <chucho/level_filter.hpp>
#include <chucho/level_filter.h>
#include <chucho/c_filter.hpp>
#include <chucho/c_util.hpp>

namespace
{

chucho::filter::result c_to_cpp(chucho_filter_result res)
{
    if (res == CHUCHO_FILTER_RESULT_DENY) 
        return chucho::filter::result::DENY;
    else if (res == CHUCHO_FILTER_RESULT_ACCEPT) 
        return chucho::filter::result::ACCEPT;
    return chucho::filter::result::NEUTRAL;
}

chucho_filter_result cpp_to_c(chucho::filter::result res)
{
    if (res == chucho::filter::result::DENY) 
        return CHUCHO_FILTER_RESULT_DENY;
    else if (res == chucho::filter::result::ACCEPT) 
        return CHUCHO_FILTER_RESULT_ACCEPT;
    return CHUCHO_FILTER_RESULT_NEUTRAL;
}

}

extern "C"
{

chucho_rc chucho_create_level_filter(chucho_filter** flt,
                               const chucho_level* lvl,
                               chucho_filter_result on_match,
                               chucho_filter_result on_mismatch)
{
    if (flt == nullptr || lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *flt = new chucho_filter();
        (*flt)->flt_ = std::make_shared<chucho::level_filter>(lvl->level_,
                                                              c_to_cpp(on_match),
                                                              c_to_cpp(on_mismatch));
    }
    catch (...) 
    {
        delete *flt;
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lflt_get_level(const chucho_filter* flt, const chucho_level** lvl)
{
    if (flt == nullptr || lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto lflt = std::dynamic_pointer_cast<chucho::level_filter>(flt->flt_);
    if (!lflt) 
        return CHUCHO_TYPE_MISMATCH;
    *lvl = chucho::c_util::cpp_level_to_c(lflt->get_level());
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lflt_get_on_match(const chucho_filter* flt, chucho_filter_result* res)
{
    if (flt == nullptr || res == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto lflt = std::dynamic_pointer_cast<chucho::level_filter>(flt->flt_);
    if (!lflt) 
        return CHUCHO_TYPE_MISMATCH;
    *res = cpp_to_c(lflt->get_on_match());
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lflt_get_on_mismatch(const chucho_filter* flt, chucho_filter_result* res)
{
    if (flt == nullptr || res == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto lflt = std::dynamic_pointer_cast<chucho::level_filter>(flt->flt_);
    if (!lflt) 
        return CHUCHO_TYPE_MISMATCH;
    *res = cpp_to_c(lflt->get_on_mismatch());
    return CHUCHO_NO_ERROR;
}

}
