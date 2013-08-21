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

#include "log.hpp"
#include <windows.h>
#include <io.h>

namespace
{

std::string all_users_profile()
{
    std::string result;
    DWORD rc = GetEnvironmentVariableA("ALLUSERSPROFILE", nullptr, 0);
    if (rc > 0)
    {
        result.resize(rc);
        GetEnvironmentVariableA("ALLUSERSPROFILE",
                                const_cast<char*>(result.data()),
                                rc);
        result.pop_back();
    }
    return result;
}

std::string chuchod_directory()
{
    std::string dir(all_users_profile() + "\\chuchod");
    if (_access(dir.c_str(), 0) != 0)
        CreateDirectoryA(dir.c_str(), nullptr);
    dir += "\\";
    return dir;
}

}

namespace chucho
{

namespace server
{

std::string log::get_config_file_name()
{
    return chuchod_directory() + "chuchod.yaml";
}

std::string log::get_file_name()
{
    return chuchod_directory() + "chuchod.log";
}

}

}
