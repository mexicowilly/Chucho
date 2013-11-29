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
#include <mutex>
#include <sstream>
#include <vector>
#include <iomanip>

#if !defined(CHUCHO_HAVE_GMTIME_R) || !defined(CHUCHO_HAVE_LOCALTIME_R)

namespace
{

std::once_flag once;

std::mutex& calendar_guard()
{
    static std::mutex* guard;

    std::call_once(once, [&] () { guard = new std::mutex();
                                  chucho::garbage_cleaner::get().add([&] () { delete guard; }) });
    return *guard;
}

}

#endif

#if !defined(CHUCHO_HAVE_PUT_TIME)

namespace
{

const std::size_t FORMAT_BUF_SIZE_MAX = 20 * 1024;

}

#endif

namespace chucho
{

namespace calendar
{

std::string format_time_zone(const pieces&, const std::string&);

std::string format(const pieces& cal, const std::string& pattern)
{
    std::string new_pat = format_time_zone(cal, pattern);

    #if defined(CHUCHO_HAVE_PUT_TIME)

    std::ostringstream stream;
    stream << std::put_time(&cal, new_pat.c_str());
    return stream.str();

    #else

    std::vector<char> buf(1024);
    std::string result;
    while (true)
    {
        std::size_t rc = std::strftime(&buf[0],
                                       buf.size(),
                                       new_pat.c_str(),
                                       &cal);
        if (rc > 0)
        {
            result = &buf[0];
            break;
        }
        if (buf.size() > FORMAT_BUF_SIZE_MAX)
            break;
        buf.resize(buf.size() + 1024);
    }
    return result;

    #endif
}

pieces get_local(std::time_t t)
{
    pieces result;

    #if defined(CHUCHO_HAVE_LOCALTIME_R)

    ::localtime_r(&t, &result);

    #else

    std::lock_guard<std::mutex> lg(calendar_guard());
    result = *std::localtime(&t);

    #endif

    result.is_utc = false;
    return result;
}

pieces get_utc(std::time_t t)
{
    pieces result;

    #if defined(CHUCHO_HAVE_GMTIME_R)

    ::gmtime_r(&t, &result);

    #else

    std::lock_guard<std::mutex> lg(calendar_guard());
    result = *std::gmtime(&t);

    #endif

    result.is_utc = true;
    return result;
}

}

}
