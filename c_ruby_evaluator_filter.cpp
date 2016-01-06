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

#include <chucho/ruby_evaluator_filter.hpp>
#include <chucho/ruby_evaluator_filter.h>
#include <chucho/c_filter.hpp>

extern "C"
{

chucho_rc chucho_create_ruby_evaluator_filter(chucho_filter** flt,
                                              const char* const expression)
{
    if (flt == nullptr || expression == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *flt = new chucho_filter();
        (*flt)->flt_ = std::make_shared<chucho::ruby_evaluator_filter>(expression);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_rbflt_get_expression(const chucho_filter* flt, const char** exp)
{
    if (flt == nullptr || exp == nullptr)
        return CHUCHO_NULL_POINTER;
    auto rf = std::dynamic_pointer_cast<chucho::ruby_evaluator_filter>(flt->flt_);
    if (!rf) 
        return CHUCHO_TYPE_MISMATCH;
    *exp = rf->get_expression().c_str();
    return CHUCHO_NO_ERROR;
}

}
