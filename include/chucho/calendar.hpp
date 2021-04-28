/*
 * Copyright 2013-2021 Will Mason
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

#if !defined(CHUCHO_CALENDAR_HPP_)
#define CHUCHO_CALENDAR_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/export.h>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <functional>

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

class CHUCHO_PRIV_EXPORT formatter
{
public:
    enum location
    {
        LOCAL,
        UTC
    };

    formatter(const std::string& pattern, location loc);

    template <typename rep, typename period>
    std::string format(const std::chrono::duration<rep, period>& dur)
    {
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(dur);
        std::string pat = pattern_;
        if (!frac_positions_.empty())
        {
            std::ostringstream stream;
            stream << std::setfill('0');
            stream << std::setw(3) << ((micros.count() / 1000) % 1000);
            auto milli_str = stream.str();
            stream.str("");
            stream << std::setw(6) << (micros.count() % 1000000);
            auto micro_str = stream.str();
            for (const auto& t : frac_positions_)
                pat.replace(std::get<1>(t), 2, (std::get<0>(t) == frac_type::MILLI) ? milli_str : micro_str);
        }
        pieces cal = pieces_getter_(micros.count() / 1000000);
        return calendar::format(cal, pat);
    };

private:
    enum class frac_type
    {
        MILLI,
        MICRO
    };

    std::string pattern_;
    std::vector<std::tuple<frac_type, std::size_t>> frac_positions_;
    std::function<pieces(std::time_t)> pieces_getter_;
};
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
