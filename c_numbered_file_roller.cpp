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

#include <chucho/numbered_file_roller.hpp>
#include <chucho/numbered_file_roller.h>
#include <chucho/c_file_roller.hpp>
#include <chucho/c_file_compressor.hpp>
#include <chucho/file_compressor.h>
#include <stdexcept>

extern "C"
{

chucho_rc chucho_create_numbered_file_roller(chucho_file_roller** rlr,
                                       int min_index,
                                       int max_index,
                                       chucho_file_compressor* cmp)
{
    if (rlr == nullptr) 
        return CHUCHO_NULL_POINTER;
    std::shared_ptr<chucho::file_compressor> cpp_cmp;
    try
    {
        if (cmp != nullptr) 
            cpp_cmp = cmp->compressor_;
        *rlr = new chucho_file_roller;
        (*rlr)->rlr_ = std::make_shared<chucho::numbered_file_roller>(min_index, max_index, cpp_cmp);
        chucho_release_file_compressor(cmp);
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

chucho_rc chucho_nrlr_get_max_index(const chucho_file_roller* rlr, int* idx)
{
    if (rlr == nullptr || idx == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto cpp = std::dynamic_pointer_cast<chucho::numbered_file_roller>(rlr->rlr_);
    if (!cpp) 
        return CHUCHO_TYPE_MISMATCH;
    *idx = cpp->get_max_index();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_nrlr_get_min_index(const chucho_file_roller* rlr, int* idx)
{
    if (rlr == nullptr || idx == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto cpp = std::dynamic_pointer_cast<chucho::numbered_file_roller>(rlr->rlr_);
    if (!cpp) 
        return CHUCHO_TYPE_MISMATCH;
    *idx = cpp->get_min_index();
    return CHUCHO_NO_ERROR;
}

}
