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

#include "properties.hpp"
#include <chucho/remote_writer.hpp>
#include <chucho/logger.hpp>
#include <chucho/version.hpp>
#include <sstream>

namespace
{

const std::size_t DEFAULT_VASSAL_COUNT(3);

}

namespace chucho
{

namespace server
{

std::ostream& operator<< (std::ostream& stream, properties& props)
{
    stream << "Properties:\n";
    stream << "  - console mode: " << std::boolalpha << props.console_mode() << '\n';
    stream << "  - chuchod log level: " << *chucho::logger::get("chuchod")->get_level() << '\n';
    stream << "  - port: " << props.port() << '\n';
    stream << "  - version: " << chucho::version::text() << '\n';
    stream << "  - worker threads: " << props.vassal_count();
    return stream;
}

properties::properties()
    : vassal_count_(DEFAULT_VASSAL_COUNT),
      port_(chucho::remote_writer::DEFAULT_PORT),
      console_mode_(false),
      is_service_(false)
{
}

bool properties::handle_config_value(const std::string& key, const std::string& value)
{
    bool resolved = true;
    if (key == "worker_threads")
    {
        try
        {
            vassal_count_ = std::stoul(value);
        }
        catch (std::exception&)
        {
            // we can't do anything here because the logs are not fully up-and-running,
            // and if we're running as a daemon, then printing to stdout isn't going
            // to help
        }
    }
    else if (key == "port")
    {
        try
        {
            port_ = static_cast<std::uint16_t>(std::stoul(value));
        }
        catch (std::exception&)
        {
            // we can't do anything here because the logs are not fully up-and-running,
            // and if we're running as a daemon, then printing to stdout isn't going
            // to help
        }
    }
    else if (key == "console_mode")
    {
        std::istringstream stream(value);
        stream >> std::boolalpha >> console_mode_;
    }
    else if (key == "service")
    {
        std::istringstream stream(value);
        stream >> std::boolalpha >> is_service_;
    }
    else
    {
        resolved = false;
    }
    return resolved;
}

}

}
