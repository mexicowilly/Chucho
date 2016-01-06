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

#include "error_message.hpp"
#include <windows.h>

namespace chucho
{

namespace server
{

namespace windows
{

std::string error_message(int err)
{
    char* msg;
    if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                       nullptr,
                       err,
                       0,
                       reinterpret_cast<char*>(&msg),
                       1,
                       nullptr) == 0)
    {
        return std::string();
    }
    std::string result("(" + std::to_string(err) + ") " + msg);
    LocalFree(msg);
    return result;
}

}

}

}
