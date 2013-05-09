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

namespace chucho
{

namespace server
{

properties::properties()
    : vassal_count_(3),
      port_(chucho::remote_writer::DEFAULT_PORT)
{
}

void properties::handle_config_value(const std::string& key, const std::string& value)
{
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
            port_ = std::stoul(value);
        }
        catch (std::exception&)
        {
            // we can't do anything here because the logs are not fully up-and-running,
            // and if we're running as a daemon, then printing to stdout isn't going
            // to help
        }
    }
}

}

}
