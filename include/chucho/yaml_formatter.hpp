/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_YAML_FORMATTER_HPP__)
#define CHUCHO_YAML_FORMATTER_HPP__

#include <chucho/serialization_formatter.hpp>

namespace chucho
{

class CHUCHO_EXPORT yaml_formatter : public serialization_formatter
{
public:
    /**
     * @name Constructors
     * @{
     */
    /**
     * Construct a formatter with all fields included.
     *
     * @param styl the style, pretty or compact
     * @param tz the time zone
     * @param time_format the time format
     */
    yaml_formatter(style styl = DEFAULT_STYLE,
                   time_zone tz = DEFAULT_TIME_ZONE,
                   const std::string& time_format = DEFAULT_TIME_FORMAT);
    /**
     * Construct a formatter with only certain fields included
     * or excluded.
     *
     * @param dis whether the given fields are included or excluded
     * @param fields the fields to include or exclude
     * @param styl the style, pretty or compact
     * @param tz the time zone
     * @param time_format the time format
     */
    yaml_formatter(field_disposition dis,
                   const std::vector<field>& fields,
                   style styl = DEFAULT_STYLE,
                   time_zone tz = DEFAULT_TIME_ZONE,
                   const std::string& time_format = DEFAULT_TIME_FORMAT);
    /**
     * @}
     */

    virtual std::string format(const event& evt) override;
};

}

#endif