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

#include "log.hpp"
#include <chucho/configuration.hpp>

namespace chucho
{

namespace server
{

void log::configure(properties& props)
{
    chucho::configuration::set_unknown_handler(std::bind(&properties::handle_config_value,
                                                         props,
                                                         std::placeholders::_1,
                                                         std::placeholders::_2));
    chucho::configuration::set_file_name(get_config_file_name());
    chucho::configuration::set_environment_variable("");
    chucho::configuration::set_fallback("- chucho::logger:\n"
                                        "    - name: chuchod\n"
                                        "    - level: debug\n"
                                        "    - chucho::rolling_file_writer:\n"
                                        "        - file_name: " + get_file_name() + "\n"
                                        "        - chucho::formatter:\n"
                                        "            - chucho::pattern_formatter:\n"
                                        "                - pattern: '%d{%Y/%m/%d %H:%M:%S.%q} %c %-5p %m%n'\n"
                                        "        - chucho::numbered_file_roller:\n"
                                        "            - max_index: 1\n"
                                        "        - chucho::size_file_roll_trigger:\n"
                                        "            - max_size: 10m\n");
}

}

}
