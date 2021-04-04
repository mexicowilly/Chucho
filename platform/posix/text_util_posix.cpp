/*
 * Copyright 2013-2021 Will Mason
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
#include <cstdio>

namespace chucho
{

namespace text_util
{

std::string format(const char* const fmt, va_list args)
{
    char buf[1024];
    int rc = std::vsnprintf(buf, 1024, fmt, args);
    if (rc > 1023 && rc < MAX_MESSAGE_SIZE)
    {
        std::vector<char> chars(rc + 1);
        std::vsnprintf(&chars[0], chars.size(), fmt, args);
        return std::string(&chars[0]);
    }
    return std::string(buf);
}

}

}
