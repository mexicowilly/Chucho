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

#include <chucho/serialization_formatter_memento.hpp>
#include <chucho/text_util.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

serialization_formatter_memento::serialization_formatter_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("serialization_formatter_memento");
    cfg.get_security_policy().set_text("serialization_formatter::time_zone", 5);
    cfg.get_security_policy().set_text("serialization_formatter::style", 7);
    set_handler("time_zone",
                std::bind(&serialization_formatter_memento::handle_time_zone, this, std::placeholders::_1));
    set_handler("style",
                std::bind(&serialization_formatter_memento::handle_style, this, std::placeholders::_1));
    set_handler("excluded_fields",
                std::bind(&serialization_formatter_memento::handle_excluded_fields, this, std::placeholders::_1));
    set_handler("included_fields",
                std::bind(&serialization_formatter_memento::handle_included_fields, this, std::placeholders::_1));
    set_handler("time_format", [this] (const std::string& val) { time_format_ = validate("serialization_formatter::time_format", val); });
}

void serialization_formatter_memento::handle_excluded_fields(const std::string& str)
{
    excluded_fields_ = parse_fields(validate("serialization_formatter::excluded_fields", str));
}

void serialization_formatter_memento::handle_included_fields(const std::string& str)
{
    included_fields_ = parse_fields(validate("serialization_formatter::included_fields", str));
}

void serialization_formatter_memento::handle_style(const std::string& str)
{
    auto low = text_util::to_lower(validate("serialization_formatter::style", str));
    if (low == "compact")
        style_ = serialization_formatter::style::COMPACT;
    else if (low == "pretty")
        style_ = serialization_formatter::style::PRETTY;
    else
        throw exception("style has an invalid value of " + str);
}

void serialization_formatter_memento::handle_time_zone(const std::string& str)
{
    auto low = text_util::to_lower(validate("serialization_formatter::time_zone", str));
    if (low == "local")
        time_zone_ = serialization_formatter::time_zone::LOCAL;
    else if (low == "utc")
        time_zone_ = serialization_formatter::time_zone::UTC;
    else
        throw exception("time_zone has an invalid value of " + str);
}

std::vector<serialization_formatter::field> serialization_formatter_memento::parse_fields(const std::string& str) const
{
    std::vector<serialization_formatter::field> result;
    auto fields = text_util::tokenize(str, ',');
    for (auto& field : fields)
    {
        text_util::trim(field);
        auto low = text_util::to_lower(field);
        if (low == "diagnostic_context")
            result.push_back(serialization_formatter::field::DIAGNOSTIC_CONTEXT);
        else if (low == "file_name")
            result.push_back(serialization_formatter::field::FILE_NAME);
        else if (low == "function_name")
            result.push_back(serialization_formatter::field::FUNCTION_NAME);
        else if (low == "host_name")
            result.push_back(serialization_formatter::field::HOST_NAME);
        else if (low == "level")
            result.push_back(serialization_formatter::field::LEVEL);
        else if (low == "line_number")
            result.push_back(serialization_formatter::field::LINE_NUMBER);
        else if (low == "logger")
            result.push_back(serialization_formatter::field::LOGGER);
        else if (low == "marker")
            result.push_back(serialization_formatter::field::MARKER);
        else if (low == "message")
            result.push_back(serialization_formatter::field::MESSAGE);
        else if (low == "process_id")
            result.push_back(serialization_formatter::field::PROCESS_ID);
        else if (low == "thread")
            result.push_back(serialization_formatter::field::THREAD);
        else if (low == "timestamp")
            result.push_back(serialization_formatter::field::TIMESTAMP);
        else
            throw exception("serialization_formatter invalid field: " + str);
    }
    return result;
}

}
