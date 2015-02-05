/*
 * Copyright 2013-2014 Will Mason
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

#include "util.hpp"
#include <string.h>

namespace
{

std::vector<std::string> get_words(const std::string& text)
{
    std::vector<std::string> result;
    std::vector<char> cp(text.begin(), text.end());
    cp.push_back(0);
    char* tok = strtok(&cp[0], " ");
    while (tok != nullptr)
    {
        result.push_back(tok);
        tok = strtok(nullptr, " ");
    }
    return result;
}

}

namespace chucho
{

namespace config_tool
{

namespace util
{

std::vector<std::string> word_break(const std::string& text, std::size_t len)
{
    std::string line;
    std::vector<std::string> result;
    auto words = get_words(text);
    for (auto w : words)
    {
        if (w.length() + 1 > len - line.length())
        {
            result.push_back(line);
            line = w;
        }
        else
        {
            if (!line.empty())
                line += " ";
            line += w; 
        }
    }
    return result;
}

}

}

}
