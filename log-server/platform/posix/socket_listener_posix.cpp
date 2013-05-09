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

#include "socket_listener.hpp"
#include "eof_exception.hpp"
#include "is_shut_down.hpp"
#include <chucho/socket_exception.hpp>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <netdb.h>

namespace chucho
{

namespace server
{

socket_listener::socket_listener(std::uint16_t port)
    : socket_(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
    if (socket_ == -1)
        throw chucho::socket_exception("Error creating socket", errno);
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(socket_, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        int err = errno;
        close(socket_);
        throw chucho::socket_exception("Unable to bind to local address", err);
    }
    if (listen(socket_, SOMAXCONN) == -1)
    {
        int err = errno;
        close(socket_);
        throw chucho::socket_exception("Unable to listen to local address", err);
    }
}

socket_listener::~socket_listener()
{
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
        close(socket_);
    }
}

std::shared_ptr<socket_reader> socket_listener::accept()
{
    struct pollfd pfd;
    pfd.fd = socket_;
    pfd.events = POLLIN;
    pfd.revents = 0;
    struct sockaddr_in addr;
    int rc;
    do
    {
        rc = poll(&pfd, 1, 250);
        if (is_shut_down)
            throw shutdown_exception();
        if (rc > 0)
        {
            if (pfd.revents & (POLLHUP | POLLERR | POLLNVAL))
                throw eof_exception();
            socklen_t length = sizeof(addr);
            int sock = ::accept(pfd.fd, reinterpret_cast<struct sockaddr*>(&addr), &length);
            if (sock == -1)
            {
                int err = errno;
                if (err == EBADF)
                    throw eof_exception();
                throw socket_exception("Unable to accept a connection", err);
            }
            char host_name[NI_MAXHOST + 1];
            if (getnameinfo(reinterpret_cast<struct sockaddr*>(&addr), length,
                            host_name, NI_MAXHOST,
                            nullptr, 0,
                            0) != 0)
            {
                host_name[0] = 0;
            }
            return std::make_shared<socket_reader>(sock, host_name);
        }
    } while (rc == 0);
    return std::shared_ptr<socket_reader>();
}

}

}
