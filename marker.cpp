/*
 * Copyright 2013-2020 Will Mason
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

#include <chucho/marker.hpp>

namespace chucho
{

std::ostream& operator<< (std::ostream& stream, const marker& mark)
{
    stream << mark.get_name();
    if (!mark.empty())
    {
        bool first = true;
        stream << " [ ";
        for (auto m : mark)
        {
            if (first)
                first = false;
            else
                stream << ", ";
            stream << m;
        }
        stream << " ]";
    }
    return stream;
}

marker::marker(const std::string& name)
    : name_(name)
{
}

}
