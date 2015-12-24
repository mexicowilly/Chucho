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

#if !defined(CHUCHO_CALENDAR_HPP__)
#define CHUCHO_CALENDAR_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/export.hpp>
#include <ctime>
#include <string>

namespace chucho
{

namespace calendar
{

struct CHUCHO_PRIV_EXPORT pieces : std::tm
{
    pieces();
    pieces(const struct std::tm& t);

    pieces& operator= (const struct std::tm& t);

    bool is_utc;
};

CHUCHO_PRIV_EXPORT std::string format(const pieces& cal,
                                      const std::string& pattern);
CHUCHO_PRIV_EXPORT pieces get_local(std::time_t t);
CHUCHO_PRIV_EXPORT long get_time_zone_offset_in_minutes();
CHUCHO_PRIV_EXPORT pieces get_utc(std::time_t t);
CHUCHO_PRIV_EXPORT std::time_t to_time_t(const pieces& cal);

}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
