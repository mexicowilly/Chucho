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

#if !defined(CHUCHO_JSON_FORMATTER_MEMENTO_HPP__)
#define CHUCHO_JSON_FORMATTER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/json_formatter.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class json_formatter_memento : public memento
{
public:
    json_formatter_memento(configurator& cfg);

    const optional<std::bitset<12>>& get_excluded_fields() const;
    const optional<std::bitset<12>>& get_included_fields() const;
    const optional<json_formatter::style>& get_style() const;
    const std::string& get_time_format() const;
    const optional<json_formatter::time_zone>& get_time_zone() const;

private:
    void handle_excluded_fields(const std::string& str);
    void handle_included_fields(const std::string& str);
    void handle_style(const std::string& str);
    void handle_time_zone(const std::string& str);
    std::bitset<12> parse_fields(const std::string& str) const;

    optional<json_formatter::style> style_;
    optional<std::bitset<12>> included_fields_;
    optional<std::bitset<12>> excluded_fields_;
    optional<json_formatter::time_zone> time_zone_;
    std::string time_format_;
};

inline const optional<std::bitset<12>>& json_formatter_memento::get_excluded_fields() const
{
    return excluded_fields_;
}

inline const optional<std::bitset<12>>& json_formatter_memento::get_included_fields() const
{
    return included_fields_;
}

inline const optional<json_formatter::style>& json_formatter_memento::get_style() const
{
    return style_;
}

inline const std::string& json_formatter_memento::get_time_format() const
{
    return time_format_;
}

inline const optional<json_formatter::time_zone>& json_formatter_memento::get_time_zone() const
{
    return time_zone_;
}

}

#endif
