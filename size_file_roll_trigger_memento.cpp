/*
 * Copyright 2013-2017 Will Mason
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

#include <chucho/size_file_roll_trigger_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/text_util.hpp>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace chucho
{

size_file_roll_trigger_memento::size_file_roll_trigger_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("size_file_roll_trigger_memento");
    cfg.get_security_policy().set_integer("size_file_roll_trigger::max_size", static_cast<std::uintmax_t>(1), static_cast<std::uintmax_t>(1024 * 1024 * 1024));
    cfg.get_security_policy().set_text("size_file_roll_trigger::max_size(text)", 50);
    set_handler("max_size", std::bind(&size_file_roll_trigger_memento::parse, this, std::placeholders::_1));
}

void size_file_roll_trigger_memento::parse(const std::string& spec)
{
    if (spec.empty() || !std::isdigit(spec[0]))
        throw exception(get_status_origin() + ": The size specification must start with a digit");
    std::istringstream stream(validate("size_file_roll_trigger::max_size(text)", spec));
    std::uintmax_t tmp;
    stream >> tmp;
    std::string suffix;
    stream >> suffix;
    if (!suffix.empty())
    {
        std::string lower = text_util::to_lower(suffix);
        if (lower.length() > 2 ||
            (lower.length() == 2 && lower[1] != 'b') ||
            (lower.length() == 2 && lower[0] == 'b') ||
            (std::string("bkmg").find(lower[0]) == std::string::npos))
        {
            throw exception(get_status_origin() + ": The suffix '" + suffix + "' is invalid (case-insensitive b, k[b], m[b], g[b])");
        }
        switch (lower[0])
        {
        case 'b':
            break;
        case 'k':
            tmp *= 1024;
            break;
        case 'm':
            tmp *= 1024 * 1024;
            break;
        case 'g':
            tmp *= 1024 * 1024 * 1024;
            break;
        }
    }
    max_size_ = validate("size_file_roll_trigger::max_size", tmp);
}

}
