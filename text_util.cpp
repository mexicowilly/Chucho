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

#include <chucho/text_util.hpp>
#include <cctype>
#include <algorithm>
#include <iterator>

namespace chucho
{

namespace text_util
{

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

}

}
