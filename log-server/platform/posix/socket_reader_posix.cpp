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
#include "eof_exception.hpp"
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

namespace chucho
{

namespace server
{

socket_reader::~socket_reader()
{
    shutdown(socket_, SHUT_RD);
    close(socket_);
}

std::size_t socket_reader::read_impl(std::uint8_t* dest, std::size_t count)
{
    struct pollfd pfd;
    pfd.fd = socket_;
    pfd.events = POLLIN;
    pfd.revents = 0;
    std::size_t result = 0;
    int rc;
    do
    {
        rc = poll(&pfd, 1, 250);
        if (rc > 0)
        {
            if (pfd.revents & (POLLHUP | POLLERR | POLLNVAL))
                throw eof_exception();
            ssize_t amount = recv(pfd.fd, dest, count, MSG_DONTWAIT);
            if (amount > 0)
            {
                result = amount;
            }
            else if (amount < 0)
            {
                int err = errno;
                if (errno == EBADF)
                    throw eof_exception();
                throw chucho::exception(std::string("Unable to read from socket") + std::strerror(err));
            }
            else
            {
                throw eof_exception();
            }
        }
    } while (rc == 0);
    return result;
}

}

}
