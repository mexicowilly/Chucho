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

#include "socket_reader.hpp"

namespace chucho
{

namespace server
{

socket_reader::socket_reader(int sock,
                             const std::string& base_host,
                             const std::string& full_host)
    : socket_(sock),
      base_host_(base_host),
      full_host_(full_host)
{
}

void socket_reader::read(std::uint8_t* dest, std::size_t count)
{
    while (count > 0)
    {
        std::size_t cur = read_impl(dest, count);
        dest += cur;
        count -= cur;
    }
}

}

}
