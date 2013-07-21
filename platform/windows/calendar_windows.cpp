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
#include <chucho/garbage_cleaner.hpp>
#include <sstream>
#include <mutex>
#include <iomanip>

namespace
{

struct static_data
{
    static_data();

    std::mutex calendar_guard_;
};

static_data::static_data()
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

std::once_flag once;

static_data& data()
{
    // This will be cleaned in finalize()
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

}

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
    std::lock_guard<std::mutex> lg(data().calendar_guard_);
    return *std::localtime(&t);
}

struct std::tm get_utc(std::time_t t)
{
    std::lock_guard<std::mutex> lg(data().calendar_guard_);
    return *std::gmtime(&t);
}

}

}
