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

#include <chucho/named_pipe_writer.hpp>

namespace chucho
{

std::string named_pipe_writer::normalize_name(const std::string& name)
{
    if (name.find("\\\\") != 0)
    {
        std::string loc(name);
        auto ns = loc.find_first_not_of('\\');
        if (ns != std::string::npos)
            loc.erase(0, ns);
        return "\\\\.\\pipe\\" + loc;
    }
    return name;
}

}

