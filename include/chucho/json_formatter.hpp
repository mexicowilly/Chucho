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

#ifndef CHUCHO_JSON_FORMATTER_HPP__
#define CHUCHO_JSON_FORMATTER_HPP__

#include <chucho/formatter.hpp>
#include <bitset>
#include <vector>

namespace chucho
{

namespace calendar
{

class formatter;

}

class CHUCHO_EXPORT json_formatter : public formatter
{
public:
    enum class style
    {
        PRETTY,
        COMPACT
    };

    enum class field
    {
        DIAGNOSTIC_CONTEXT,
        FILE_NAME,
        FUNCTION_NAME,
        HOST_NAME,
        LEVEL,
        LINE_NUMBER,
        LOGGER,
        MARKER,
        MESSAGE,
        PROCESS_ID,
        THREAD,
        TIMESTAMP
    };

    enum class field_disposition
    {
        INCLUDED,
        EXCLUDED
    };

    enum class time_zone
    {
        LOCAL,
        UTC
    };

    static constexpr style DEFAULT_STYLE = style::COMPACT;
    static constexpr time_zone DEFAULT_TIME_ZONE = time_zone::LOCAL;
    static constexpr char const* DEFAULT_TIME_FORMAT = "%Y%m%dT%H%M%S";

    json_formatter(style styl = DEFAULT_STYLE,
                   time_zone tz = DEFAULT_TIME_ZONE,
                   const std::string& time_format = DEFAULT_TIME_FORMAT);
    json_formatter(field_disposition dis,
                   const std::vector<field>& fields,
                   style styl = DEFAULT_STYLE,
                   time_zone tz = DEFAULT_TIME_ZONE,
                   const std::string& time_format = DEFAULT_TIME_FORMAT);
    virtual ~json_formatter();

    virtual std::string format(const event& evt) override;

private:
    style style_;
    std::bitset<12> fields_;
    std::unique_ptr<calendar::formatter> fmt_;
};

}

#endif
