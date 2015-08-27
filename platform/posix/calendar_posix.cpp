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
#include <chucho/garbage_cleaner.hpp>
#include <mutex>
#include <cstdlib>

#if !defined(CHUCHO_HAVE_GMTIME_R) || !defined(CHUCHO_HAVE_LOCALTIME_R)

namespace
{

std::mutex& calendar_guard()
{
    static std::mutex* guard;
    static std::once_flag once;

    std::call_once(once, [&] () { guard = new std::mutex();
                                  chucho::garbage_cleaner::get().add([&] () { delete guard; }); });
    return *guard;
}

}

#endif

namespace chucho
{

namespace calendar
{

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

long get_time_zone_offset_in_minutes()
{
    #if defined(CHUCHO_HAVE_TM_GMTOFF)

    pieces broke = get_local(0);
    return broke.tm_gmtoff / 60;

    #elif defined(CHUCHO_HAVE_TIMEZONE)

    static std::once_flag once;

    std::call_once(once, tzset);
    return -(timezone / 60);

    #else

    const std::time_t whatever = 1000000;
    pieces broke = get_utc(whatever);
    std::time_t loc = std::mktime(&broke);
    return (whatever - loc) / 60;

    #endif
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

std::time_t to_time_t(const pieces& cal)
{
    time_t result;
    if (cal.is_utc)
    {
        #if defined(CHUCHO_HAVE_TIMEGM)

        result = timegm(&const_cast<pieces&>(cal));

        #else

        char* tz = std::getenv("TZ");
        setenv("TZ", "", 1);
        tzset();
        result = std::mktime(&const_cast<pieces&>(cal));
        if (tz != nullptr)
            setenv("TZ", tz, 1);
        else
            unsetenv("TZ");
        tzset();

        #endif
    }
    else
    {
        result = std::mktime(&const_cast<pieces&>(cal));
    }
    return result;
}

}

}
