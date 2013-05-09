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
#include <sstream>

namespace chucho
{

std::string yaml_formatter::format(const event& evt)
{
    std::ostringstream stream;
    stream.imbue(std::locale("C"));
    stream << "- event:\n";
    stream << "    version: 1\n";
    stream << "    milliseconds_since_epoch: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(evt.get_time().time_since_epoch()).count() << '\n';
    stream << "    file_name: " + utf8::escape_invalid(evt.get_file_name()) + '\n';
    stream << "    line_number: " << evt.get_line_number() << '\n';
    stream << "    function_name: " << evt.get_function_name() << '\n';
    stream << "    logger: " << utf8::escape_invalid(evt.get_logger()->get_name()) << '\n';
    stream << "    level: " << *evt.get_level() << '\n';
    if (evt.get_marker())
    {
        std::ostringstream mstream;
        mstream << *evt.get_marker();
        stream << "    marker: " << utf8::escape_invalid(mstream.str()) << '\n';
    }
    stream << "    message: " << utf8::escape_invalid(evt.get_message()) << '\n';
    return stream.str();
}

}
