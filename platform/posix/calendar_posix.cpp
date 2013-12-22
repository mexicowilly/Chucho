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

#include <chucho/calendar.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <mutex>

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
