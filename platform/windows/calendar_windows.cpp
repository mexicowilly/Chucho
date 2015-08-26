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

#include <chucho/calendar.hpp>
#include <time.h>
#include <windows.h>

namespace chucho
{

namespace calendar
{

pieces get_local(std::time_t t)
{
    pieces result;
    localtime_s(&result, &t);
    result.is_utc = false;
    return result;
}

long get_time_zone_offset_in_minutes()
{
    TIME_ZONE_INFORMATION info;
    GetTimeZoneInformation(&info);
    return -info.Bias;
}

pieces get_utc(std::time_t t)
{
    pieces result;
    gmtime_s(&result, &t);
    result.is_utc = true;
    return result;
}

std::time_t to_time_t(const pieces& cal)
{
    return cal.is_utc ? _mkgmtime(&const_cast<pieces&>(cal)) : mktime(&const_cast<pieces&>(cal));
}

}

}
