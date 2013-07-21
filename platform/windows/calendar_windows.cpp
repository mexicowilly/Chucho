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

std::string format(const struct std::tm& cal, const std::string& pattern)
{
    std::ostringstream stream;
    stream << std::put_time(const_cast<struct std::tm*>(&cal), pattern.c_str());
    return stream.str();
}

struct std::tm get_local(std::time_t t)
{
    struct std::tm result;
    localtime_s(&result, &t);
    return result;
}

struct std::tm get_utc(std::time_t t)
{
    struct std::tm result;
    gmtime_s(&result, &t);
    return result;
}

}

}
