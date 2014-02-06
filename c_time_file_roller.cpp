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

#include <chucho/time_file_roller.hpp>
#include <chucho/time_file_roller.h>
#include <chucho/c_file_roller.hpp>
#include <chucho/c_file_compressor.hpp>
#include <chucho/file_compressor.h>
#include <chucho/error.h>

extern "C"
{

int chucho_create_time_file_roller(chucho_file_roller** rlr,
                                   const char* const file_name_pattern,
                                   size_t max_history,
                                   chucho_file_compressor* cmp)
{
    if (rlr == nullptr || file_name_pattern == nullptr) 
        return CHUCHO_NULL_POINTER;
    std::shared_ptr<chucho::file_compressor> cpp_cmp;
    try
    {
        if (cmp != nullptr) 
            cpp_cmp = cmp->compressor_;
        *rlr = new chucho_file_roller;
        (*rlr)->rlr_ = std::make_shared<chucho::time_file_roller>(file_name_pattern,
                                                                  max_history,
                                                                  cpp_cmp);
        chucho_release_file_compressor(cmp);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

int chucho_trlr_get_file_name_pattern(const chucho_file_roller* rlr, const char** pat)
{
    if (rlr == nullptr || pat == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto tfr = std::dynamic_pointer_cast<chucho::time_file_roller>(rlr->rlr_);
    if (!tfr) 
        return CHUCHO_TYPE_MISMATCH;
    *pat = tfr->get_file_name_pattern().c_str();
    return CHUCHO_NO_ERROR;
}

int chucho_trlr_get_max_history(const chucho_file_roller* rlr, size_t* hist)
{
    if (rlr == nullptr || hist == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto tfr = std::dynamic_pointer_cast<chucho::time_file_roller>(rlr->rlr_);
    if (!tfr) 
        return CHUCHO_TYPE_MISMATCH;
    *hist = tfr->get_max_history();
    return CHUCHO_NO_ERROR;
}

}
