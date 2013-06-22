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
#include <chucho/log.hpp>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <netdb.h>

namespace chucho
{

namespace server
{

socket_listener::socket_listener(std::uint16_t port)
    : socket_(-1),
      logger_(chucho::logger::get("chuchod.socket_listener")),
      stop_(false)
{
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo* addrs;
    if (getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &addrs) != 0)
        throw chucho::socket_exception("Unable to get address information", errno);
    struct sentry
    {
        sentry(addrinfo* addrs) : addrs_(addrs) { }
        ~sentry() { freeaddrinfo(addrs_); }
        addrinfo* addrs_;
    } sntry(addrs);
    struct addrinfo* cur;
    for (cur = addrs; cur != nullptr; cur = cur->ai_next)
    {
        socket_ = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
        if (socket_ == -1)
        {
            int err = errno;
            CHUCHO_INFO(logger_, std::string("Could not create socket: ") + std::strerror(err));
            continue;
        }
        int yes = 1;
        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        {
            int err = errno;
            CHUCHO_INFO(logger_, std::string("Error setting socket options: ") + std::strerror(err));
            continue;
        }
        if (bind(socket_, cur->ai_addr, cur->ai_addrlen) == -1)
        {
            int err = errno;
            CHUCHO_INFO(logger_, std::string("Could not bind to local address: ") + std::strerror(err));
            continue;
        }
        break;
    }
    if (cur == nullptr)
        throw chucho::exception("No listening sockets could be created");
    if (listen(socket_, SOMAXCONN) == -1)
    {
        int err = errno;
        close(socket_);
        throw chucho::socket_exception("Unable to listen to local address", err);
    }
}

socket_listener::~socket_listener()
{
    // This can throw when the listener is reset during SIGHUP
    try
    {
        if (socket_ != -1)
        {
            shutdown(socket_, SHUT_RDWR);
            close(socket_);
        }
    }
    catch (exception& e)
    {
        CHUCHO_ERROR(logger_, "Error shutting down the listener: " << e.what());
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
        rc = poll(&pfd, 1, 500);
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
            std::string full_host(host_name);
            if (getnameinfo(reinterpret_cast<struct sockaddr*>(&addr), length,
                            host_name, NI_MAXHOST,
                            nullptr, 0,
                            NI_NOFQDN) != 0)
            {
                host_name[0] = 0;
            }
            return std::make_shared<socket_reader>(sock, host_name, full_host);
        }
    } while (rc == 0);
    int err = errno;
    CHUCHO_ERROR(logger_, "Polling the socket has been halted: " << std::strerror(err));
    throw eof_exception();
}

}

}
