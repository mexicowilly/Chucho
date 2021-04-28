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

#include <chucho/serialization_formatter.hpp>
#include <chucho/calendar.hpp>

namespace chucho
{

serialization_formatter::serialization_formatter(style styl,
                                                 time_zone tz,
                                                 const std::string& time_format)
  : style_(styl),
    cal_fmt_(std::make_unique<calendar::formatter>(time_format,
                                                   (tz == time_zone::LOCAL) ? calendar::formatter::location::LOCAL : calendar::formatter::location::UTC))
{
    fields_.set();
}

serialization_formatter::serialization_formatter(field_disposition dis,
                                                 const std::vector<field>& fields,
                                                 style styl,
                                                 time_zone tz,
                                                 const std::string& time_format)
  : serialization_formatter(styl, tz, time_format)
{
    if (dis == field_disposition::INCLUDED)
    {
        fields_.reset();
        for (auto f : fields)
            fields_.set(static_cast<std::size_t>(f));
    }
    else
    {
        fields_.set();
        for (auto f : fields)
            fields_.reset(static_cast<std::size_t>(f));
    }
}

serialization_formatter::~serialization_formatter()
{
}

}
