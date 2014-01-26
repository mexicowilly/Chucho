/*
 * Copyright 2013-2014 Will Mason
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
#include "error_message.hpp"
#include "eof_exception.hpp"
#include <winsock2.h>

namespace chucho
{

namespace server
{

socket_reader::~socket_reader()
{
    shutdown(socket_, SD_BOTH);
    closesocket(socket_);
}

std::size_t socket_reader::read_impl(std::uint8_t* dest, std::size_t count)
{
    fd_set sfd;
    FD_ZERO(&sfd);
    FD_SET(socket_, &sfd);
    timeval wait;
    wait.tv_sec = 0;
    wait.tv_usec = 250000;
    int rc;
    std::size_t result = 0;
    do
    {
        rc = select(1, &sfd, nullptr, nullptr, &wait);
        if (rc == SOCKET_ERROR)
        {
            throw chucho::exception("Error selecting socket: " + windows::error_message(WSAGetLastError()));
        }
        if (rc > 0)
        {
            int rd = recv(socket_, reinterpret_cast<char*>(dest), count, 0);
            if (rd == SOCKET_ERROR)
            {
                DWORD err = WSAGetLastError();
                if (err == WSAECONNRESET)
                    throw eof_exception();
                else
                    throw chucho::exception("Error reading socket: " + windows::error_message(err));
            }
            else if (rd == 0)
            {
                throw eof_exception();
            }
            result = rd;
        }
    } while (rc == 0);
    return result;
}

}

}
