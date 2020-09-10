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

#include <chucho/host.hpp>
#include <windows.h>

namespace chucho
{

void host::get_base_impl(std::string& result)
{
    DWORD size = 0;
    GetComputerNameExA(ComputerNameNetBIOS, nullptr, &size);
    std::string tmp(size, 0);
    if (GetComputerNameExA(ComputerNameNetBIOS,
                           const_cast<char*>(tmp.data()),
                           &size))
    {
        // remove trailing zero
        tmp.pop_back();
        result = tmp;
    }
}

void host::get_full_impl(std::string& result)
{
    DWORD size = 0;
    GetComputerNameExA(ComputerNameDnsFullyQualified, nullptr, &size);
    std::string tmp(size, 0);
    if (GetComputerNameExA(ComputerNameDnsFullyQualified,
                           const_cast<char*>(tmp.data()),
                           &size))
    {
        // remove trailing zero
        tmp.pop_back();
        result = tmp;
    }
}

}
