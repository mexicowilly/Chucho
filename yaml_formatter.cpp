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

#include <chucho/yaml_formatter.hpp>
#include <chucho/utf8.hpp>
#include <chucho/logger.hpp>
#include <chucho/marker.hpp>
#include <chucho/regex.hpp>
#include <sstream>

namespace
{

std::string to_yaml_double_quoted(const std::string& msg)
{
    std::string result(1, '"');
    auto last_pos = 0;
    auto pos = msg.find_first_of("\\\"");
    while (pos != std::string::npos)
    {
        result.append(msg, last_pos, pos - last_pos);
        result.append(1, '\\');
        result.append(1, msg[pos++]);
        last_pos = pos;
        pos = msg.find_first_of("\\\"", last_pos);
    }
    if (last_pos < msg.length())
        result.append(msg, last_pos, msg.length() - last_pos);
    result.append(1, '"');
    return result;
}

std::string to_yaml_literal(const std::string& msg, std::size_t indentation = 8)
{
    std::string result("|\n");
    auto last_pos = msg.find_first_not_of(' ');
    auto pos = msg.find('\n', last_pos);
    while (pos != std::string::npos)
    {
        result.append(indentation, ' ');
        result.append(msg, last_pos, pos - last_pos + 1);
        last_pos = pos + 1;
        pos = msg.find('\n', last_pos);
    }
    if (last_pos < msg.length())
    {
        result.append(indentation, ' ');
        result.append(msg, last_pos, msg.length() - last_pos);
    }
    return result;
}

}

namespace chucho
{

const unsigned yaml_formatter::VERSION(1);

std::string yaml_formatter::format(const event& evt)
{
    static std::locale c_locale("C");

    std::ostringstream stream;
    stream.imbue(c_locale);
    stream << "- event:\n";
    stream << "    version: " << VERSION << '\n';
    stream << "    milliseconds_since_epoch: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(evt.get_time().time_since_epoch()).count() << '\n';
    stream << "    file_name: \"" << utf8::escape_invalid(evt.get_file_name()) << "\"\n";
    stream << "    line_number: " << evt.get_line_number() << '\n';
    stream << "    function_name: \"" << evt.get_function_name() << "\"\n";
    stream << "    logger: \"" << utf8::escape_invalid(evt.get_logger()->get_name()) << "\"\n";
    stream << "    level: \"" << *evt.get_level() << "\"\n";
    if (evt.get_marker())
    {
        std::ostringstream mstream;
        mstream << *evt.get_marker();
        stream << "    marker: " << utf8::escape_invalid(mstream.str()) << '\n';
    }
    stream << "    message: ";
    std::string msg = utf8::escape_invalid(evt.get_message());
    if (msg.find('\n') == std::string::npos)
        stream << to_yaml_double_quoted(msg);
    else
        stream << to_yaml_literal(msg);
    stream << '\n';
    return stream.str();
}

}
