/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/text_util.hpp>
#include <chucho/exception.hpp>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cctype>

namespace chucho
{

namespace text_util
{

std::uintmax_t parse_byte_size(const std::string& spec)
{
    if (spec.empty() || !std::isdigit(spec[0]))
        throw exception("The size specification must start with a digit");
    std::istringstream stream(spec);
    std::uintmax_t tmp;
    stream >> tmp;
    std::string suffix;
    stream >> suffix;
    if (!suffix.empty())
    {
        std::string lower = to_lower(suffix);
        if (lower.length() > 2 ||
            (lower.length() == 2 && lower[1] != 'b') ||
            (lower.length() == 2 && lower[0] == 'b') ||
            (std::string("bkmg").find(lower[0]) == std::string::npos))
        {
            throw exception("The suffix '" + suffix + "' is invalid (case-insensitive b, k[b], m[b], g[b])");
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
    return tmp;
}

std::string to_lower(const std::string& text)
{
    std::string low;
    std::transform(text.begin(),
                   text.end(),
                   std::back_inserter(low),
                   [] (char c) { return std::tolower(c); });
    return low;
}

std::vector<std::string> tokenize(const std::string& text, char delim)
{
    std::vector<std::string> result;
    std::size_t start = text.find_first_not_of(delim);
    while (start != std::string::npos)
    {
        std::size_t end = text.find_first_of(delim, start);
        result.push_back(text.substr(start, end - start));
        start = text.find_first_not_of(delim, end);
    }
    return result;
}

void trim(std::string& text)
{
    text.erase(text.begin(),
               std::find_if(text.begin(), text.end(), [] (int ch) { return !std::isblank(ch); }));
    text.erase(std::find_if(text.rbegin(), text.rend(), [] (int ch) { return !std::isblank(ch); }).base(),
               text.end());
}

}

}
