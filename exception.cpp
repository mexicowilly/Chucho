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

#include <chucho/exception.hpp>
#include <sstream>

namespace
{

void nested_what_handler(std::ostream& stream, int level, const std::exception& e)
{
    if (level > 1)
        stream << ' ';
    stream << "{ " << level << ": " << e.what() << " }";
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (std::exception& ne)
    {
        nested_what_handler(stream, level + 1, ne);
    }
}

}

namespace chucho
{

exception::exception(const std::string& msg)
    : message_(msg)
{
}

std::string exception::nested_whats(const std::exception& e)
{
    std::ostringstream stream;
    nested_what_handler(stream, 1, e);
    return stream.str();
}

const char* exception::what() const noexcept
{
    return message_.c_str();
}

}
