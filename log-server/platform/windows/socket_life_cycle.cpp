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

#include "socket_life_cycle.hpp"
#include "error_message.hpp"
#include <chucho/exception.hpp>
#include <winsock2.h>

namespace chucho
{

namespace server
{

socket_life_cycle::socket_life_cycle()
{
    WSADATA data;
    int rc = WSAStartup(MAKEWORD(2, 0), &data);
    if (rc != 0)
        throw chucho::exception("Could not start socket subsystem: " + windows::error_message(rc));
}

socket_life_cycle::~socket_life_cycle()
{
    WSACleanup();
}

}

}