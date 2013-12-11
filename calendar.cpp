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
#include <chucho/regex.hpp>
#include <thread>
#include <iomanip>
#include <sstream>

namespace
{

#if !defined(CHUCHO_HAVE_PUT_TIME)

const std::size_t FORMAT_BUF_SIZE_MAX = 20 * 1024;

#endif

struct static_data
{
    static_data();

    chucho::regex::expression offset_re_;
    chucho::regex::expression name_re_;
};

static_data::static_data()
    : offset_re_("%+z"),
      name_re_("%+Z")
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
    static_data& sd = data(); 
    regex::iterator itor(pattern, sd.offset_re_);
    regex::iterator end;
    std::size_t offset = 0;
    std::string result = pattern;
    while (itor != end)
    {
        const regex::match& m(*itor);
        if ((m[0].length() & 1) == 0)
        {
            result.replace(offset + m[0].begin() + m[0].length() - 2, 2, "+0000");
            offset += 3;
        }
        ++itor;
    }
    std::string new_pat = result;
    // std::regex has a bug in VS2012 where you can't assign
    // to the already existing iterator.
    regex::iterator itor2(new_pat, sd.name_re_);
    offset = 0;
    while (itor2 != end)
    {
        const regex::match& m(*itor2);
        if ((m[0].length() & 1) == 0)
        {
            result.replace(offset + m[0].begin() + m[0].length() - 2, 2, "UTC");
            offset += 1;
        }
        ++itor2;
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

}

}
