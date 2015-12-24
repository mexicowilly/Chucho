/*
 * Copyright 2013-2016 Will Mason
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
#include <vector>
#include <algorithm>
// Windows does not include the vsnprintf-style functions in
// cstdio.
#include <stdio.h>

namespace chucho
{

namespace text_util
{

std::string format(const char* const fmt, va_list args)
{
    char buf[1024];
    int rc = vsnprintf_s(buf, 1024, _TRUNCATE, fmt, args);
    if (rc == -1)
    {
        std::vector<char> chars(8 * 1024);
        rc = vsnprintf_s(&chars[0], chars.size(), _TRUNCATE, fmt, args);
        while (rc == -1 && chars.size() < MAX_MESSAGE_SIZE)
        {
            chars.resize(std::min(chars.size() * 2, MAX_MESSAGE_SIZE));
            rc = vsnprintf_s(&chars[0], chars.size(), _TRUNCATE, fmt, args);
        }
        return std::string(&chars[0]);
    }
    return std::string(buf);
}

}

}
