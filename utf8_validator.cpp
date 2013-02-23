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

#include <chucho/utf8_validator.hpp>
#include <chucho/exception.hpp>
#include "utf8.h"
#include <string>

namespace chucho
{

namespace utf8
{

void validate(std::istream& stream)
{
    std::string line;
    int cur = 0;
    while (std::getline(stream, line))
    {
        ++cur;
        auto invalid = ::utf8::find_invalid(line.begin(), line.end());
        if (invalid != line.end())
        {
            throw exception("Found invalid UTF-8 at line " + std::to_string(cur) + ", column " +
                std::to_string(std::distance(line.begin(), invalid)));
        }
    }
}

}

}
