/*
 * Copyright 2013 Will Mason
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
#include <sstream>
#include <iomanip>
#include <time.h>

namespace chucho
{

namespace calendar
{

std::string format_time_zone(const pieces&, const std::string&);

std::string format(const pieces& cal, const std::string& pattern)
{
    std::string new_pat = format_time_zone(cal, pattern);
    std::ostringstream stream;
    stream << std::put_time(const_cast<pieces*>(&cal), pattern.c_str());
    return stream.str();
}

pieces get_local(std::time_t t)
{
    pieces result;
    localtime_s(&result, &t);
    result.is_utc = false;
    return result;
}

pieces get_utc(std::time_t t)
{
    pieces result;
    gmtime_s(&result, &t);
    result.is_utc = true;
    return result;
}

}

}
