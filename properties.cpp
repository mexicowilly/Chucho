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

#include <chucho/properties.hpp>
#include <chucho/regex.hpp>

namespace chucho
{

properties::properties()
{
}

properties::properties(std::istream& in)
{
    regex::expression re("^[ \t]*([^ \t]+)[ \t]*=[ \t]*(.*)$");
    regex::match mch;
    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (regex::search(line, re, mch))
        {
            if (mch.size() >= 2 && mch[1].begin() != -1)
            {
                std::string k = line.substr(mch[1].begin(), mch[1].length()); 
                if (!k.empty() && k[0] != '#')
                {
                    std::string v;
                    if (mch.size() == 3 && mch[2].begin() != -1)
                        v = line.substr(mch[2].begin(), mch[2].length());
                    props_.emplace(k, v);
                }
            }
        }
    }
}

optional<std::string> properties::get_one(const std::string& key) const
{
    optional<std::string> result;
    auto found = props_.equal_range(key);
    if (found.first != found.second)
        result = found.first->second;
    return result;
}

properties properties::get_subset(const std::string prefix) const
{
    properties result;
    for (const std::multimap<std::string, std::string>::value_type& i : props_)
    {
        if (i.first.find(prefix) == 0)
            result.props_.emplace(i.first.substr(prefix.length()), i.second);
    }
    return result;
}

}
