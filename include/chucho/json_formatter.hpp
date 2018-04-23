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
        LOGGER,
        LEVEL,
        MESSAGE,
        FILE_NAME,
        LINE_NUMBER,
        FUNCTION_NAME,
        MARKER,
        THREAD,
        TIMESTAMP,
        HOST_NAME,
        DIAGNOSTIC_CONTEXT
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

    json_formatter(style styl = style::COMPACT,
                   time_zone tz = time_zone::LOCAL,
                   const std::string& time_format = "%Y%m%dT%H%M%S");
    json_formatter(field_disposition dis,
                   const std::vector<field>& fields,
                   style styl = style::COMPACT,
                   time_zone tz = time_zone::LOCAL,
                   const std::string& time_format = "%Y%m%dT%H%M%S");

    virtual std::string format(const event& evt) override;

private:
    style style_;
    std::bitset<11> fields_;
    std::unique_ptr<calendar::formatter> fmt_;
};

}

#endif
