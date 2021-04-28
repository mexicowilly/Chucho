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

#include <chucho/host.hpp>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>

namespace chucho
{

void host::get_base_impl(std::string& result)
{
    struct utsname info;
    uname(&info);
    result = info.nodename;
}

void host::get_full_impl(std::string& result)
{
    std::string full;
    get_base_impl(full);
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;
    struct addrinfo* info;
    if (getaddrinfo(full.c_str(), nullptr, &hints, &info) == 0)
    {
        full = info->ai_canonname;
        freeaddrinfo(info);
    }
    result = full;
}

}
