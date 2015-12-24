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

#include <chucho/interval_file_roll_trigger.hpp>
#include <chucho/interval_file_roll_trigger.h>
#include <chucho/c_file_roll_trigger.hpp>
#include <chucho/exception.hpp>

namespace
{

chucho::interval_file_roll_trigger::period c_to_cpp(chucho_interval_period prd)
{
    if (prd == CHUCHO_INTERVAL_PERIOD_MINUTE)
        return chucho::interval_file_roll_trigger::period::MINUTE;
    else if (prd == CHUCHO_INTERVAL_PERIOD_HOUR)
        return chucho::interval_file_roll_trigger::period::HOUR;
    else if (prd == CHUCHO_INTERVAL_PERIOD_DAY)
        return chucho::interval_file_roll_trigger::period::DAY;
    else if (prd == CHUCHO_INTERVAL_PERIOD_WEEK)
        return chucho::interval_file_roll_trigger::period::WEEK;
    else if (prd == CHUCHO_INTERVAL_PERIOD_MONTH)
        return chucho::interval_file_roll_trigger::period::MONTH;
    throw chucho::exception("Unknown chucho_interval_period");
}

}

extern "C"
{

chucho_rc chucho_create_interval_file_roll_trigger(chucho_file_roll_trigger** trg,
                                                   chucho_interval_period prd,
                                                   size_t count)
{
    if (trg == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        *trg = new chucho_file_roll_trigger;
        (*trg)->trg_ = std::make_shared<chucho::interval_file_roll_trigger>(c_to_cpp(prd), count);
    }
    catch (chucho::exception&)
    {
        delete *trg;
        *trg = nullptr;
        return CHUCHO_INVALID_ARGUMENT;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
