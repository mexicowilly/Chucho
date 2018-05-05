/*
 * Copyright 2013-2018 Will Mason
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
#include <iomanip>
#include <sstream>
#include <array>
#include <tuple>
#include <vector>

namespace
{

#if !defined(CHUCHO_HAVE_PUT_TIME)

const std::size_t FORMAT_BUF_SIZE_MAX = 20 * 1024;

#endif

}

namespace chucho
{

namespace calendar
{

pieces::pieces()
{
}

pieces::pieces(const struct std::tm& t)
    : std::tm(t),
      is_utc(true)
{
}

pieces& pieces::operator= (const struct std::tm& t)
{
    std::tm::operator=(t);
    is_utc = true;
    return *this;
}

std::string format_time_zone(const pieces& cal,
                             const std::string& pattern)
{
    if (!cal.is_utc)
        return pattern;
    std::string result = pattern;
    std::array<std::tuple<const char*, const char*, std::size_t>, 2> tups =
    {
        std::make_tuple("%z", "+0000", 5),
        std::make_tuple("%Z", "UTC", 3)
    };
    for (const auto& mr : tups)
    {
        std::string::size_type offset = result.find(std::get<0>(mr));
        while (offset != std::string::npos)
        {
            std::string::size_type beg = result.find_last_not_of('%', offset);
            beg = (beg == std::string::npos) ? 0 : beg + 1;
            if (((offset + 1 - beg) & 1) != 0)
                result.replace(offset, 2, std::get<1>(mr));
            offset = result.find(std::get<0>(mr), offset + std::get<2>(mr));
        }
    }
    return result; 
}

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

formatter::formatter(const std::string& pattern, location loc)
    : pattern_(pattern),
      pieces_getter_((loc == LOCAL) ? get_local : get_utc)
{
    enum class state
    {
        NORMAL,
        PERCENT
    };
    auto st = state::NORMAL;
    for (std::size_t i = 0; i < pattern.length(); i++)
    {
        if (st == state::NORMAL)
        {
            if (pattern[i] == '%')
                st = state::PERCENT;
        }
        else if (st == state::PERCENT)
        {
            if (pattern[i] == 'q')
                frac_positions_.emplace_back(frac_type::MILLI, i - 1);
            else if (pattern[i] == 'Q')
                frac_positions_.emplace_back(frac_type::MICRO, i - 1);
            st = state::NORMAL;
        }
    }
    std::reverse(frac_positions_.begin(), frac_positions_.end());
}

}

}
