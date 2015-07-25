/*
 * Copyright 2013-2015 Will Mason
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

#include <chucho/security_policy.hpp>

namespace chucho
{

security_policy::security_policy()
    : default_text_max_(2048)
{
}

optional<std::pair<std::intmax_t, std::intmax_t>> security_policy::get_integer_range(const std::string& key) const
{
    optional<std::pair<std::intmax_t, std::intmax_t>> result;
    auto found = int_ranges_.find(key);
    if (found != int_ranges_.end())
        result = found->second->get_range();
    return result;
}

std::size_t security_policy::get_text_max(const std::string& key) const
{
    auto found = text_maxes_.find(key);
    return (found == text_maxes_.end()) ? default_text_max_ : found->second;
}

void security_policy::set_text(const std::string& key, std::size_t max_len)
{
    if (text_maxes_.count(key) == 0)
        override_text(key, max_len);
}

std::string security_policy::validate(const std::string& key, const std::string& val) const
{
    auto found = text_maxes_.find(key);
    std::size_t max_len = default_text_max_;
    if (found != text_maxes_.end())
        max_len = found->second;
    if (val.length() > max_len)
    {
        std::string val_cp;
        bool chopped = false;
        if (val.length() > 50)
        {
            val_cp = val.substr(0, 50);
            val_cp += "...";
            chopped = true;
        }
        std::ostringstream stream;
        stream << "The value, ";
        if (chopped)
            stream << "beginning ";
        stream << val_cp << ", is longer than the maximum length, " << max_len <<
            ", allowed by the security policy";
        throw exception(stream.str());
    }
    return val;
}

}
