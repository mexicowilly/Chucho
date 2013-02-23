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

#include <chucho/configurator.hpp>
#include <chucho/exception.hpp>
#include <chucho/cerr_writer_factory.hpp>
#include <chucho/cout_writer_factory.hpp>
#include <chucho/file_writer_factory.hpp>
#include <chucho/level_filter_factory.hpp>
#include <chucho/level_threshold_filter_factory.hpp>
#include <chucho/logger_factory.hpp>
#include <chucho/numbered_file_roller_factory.hpp>
#include <chucho/pattern_formatter_factory.hpp>
#include <chucho/rolling_file_writer_factory.hpp>
#include <chucho/size_file_roll_trigger_factory.hpp>
#include <chucho/time_file_roller_factory.hpp>
#include <chucho/regex.hpp>
#include <cstring>

namespace chucho
{

configurator::configurator()
{
    set_status_origin("configurator");
}

std::map<std::string, std::shared_ptr<configurable_factory>>& configurator::get_factories()
{
    static std::map<std::string, std::shared_ptr<configurable_factory>> factories;

    return factories;
}

void configurator::initialize()
{
    std::shared_ptr<configurable_factory> fact(new cerr_writer_factory());
    add_configurable_factory("chucho::cerr_writer", fact);
    fact.reset(new cout_writer_factory());
    add_configurable_factory("chucho::cout_writer", fact);
    fact.reset(new file_writer_factory());
    add_configurable_factory("chucho::file_writer", fact);
    fact.reset(new level_filter_factory());
    add_configurable_factory("chucho::level_filter", fact);
    fact.reset(new level_threshold_filter_factory());
    add_configurable_factory("chucho::level_threshold_filter", fact);
    fact.reset(new logger_factory());
    add_configurable_factory("chucho::logger", fact);
    fact.reset(new numbered_file_roller_factory());
    add_configurable_factory("chucho::numbered_file_roller", fact);
    fact.reset(new pattern_formatter_factory());
    add_configurable_factory("chucho::pattern_formatter", fact);
    fact.reset(new rolling_file_writer_factory());
    add_configurable_factory("chucho::rolling_file_writer", fact);
    fact.reset(new size_file_roll_trigger_factory());
    add_configurable_factory("chucho::size_file_roll_trigger", fact);
    fact.reset(new time_file_roller_factory());
    add_configurable_factory("chucho::time_file_roller", fact);
}

std::string configurator::resolve_variables(const std::string& val)
{
    static regex::expression re("\\$([Ee][Nn][Vv])?\\{(.+)\\}");

    std::string result(val);
    int pos_offset = 0;
    regex::iterator itor(val, re);
    regex::iterator end;
    while (itor != end)
    {
        const regex::match& m(*itor);
        if (m[1].begin() > 0)
        {
            char* env = std::getenv(val.substr(m[2].begin(), m[2].length()).c_str());
            if (env != nullptr)
            {
                result.replace(m[0].begin() + pos_offset, m[0].length(), env);
                pos_offset += std::strlen(env) - m[0].length();
            }
        }
        else
        {
            auto found = variables_.find(val.substr(m[2].begin(), m[2].length()));
            if (found != variables_.end())
            {
                result.replace(m[0].begin() + pos_offset, m[0].length(), found->second);
                pos_offset += found->second.length() - m[0].length();
            }
        }
        ++itor;
    }
    return result;
}

}
