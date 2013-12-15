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
#include "error_message.hpp"
#include "is_shut_down.hpp"
#include "log.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

namespace chucho
{

namespace server
{

struct socket_listener_handle
{
};

socket_listener::socket_listener(std::uint16_t port)
    : socket_(INVALID_SOCKET),
      logger_(chucho::logger::get("chuchod.socket_listener"))
{
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo* addrs;
    if (getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &addrs) != 0)
    {
        throw chucho::exception("Unable to get address information: " + windows::error_message(WSAGetLastError()));
    }
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
        if (socket_ == INVALID_SOCKET)
        {
            CHUCHO_INFO(logger_, std::string("Could not create socket: ") + windows::error_message(WSAGetLastError()));
            continue;
        }
        int yes = 1;
        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&yes), sizeof(yes)) == SOCKET_ERROR)
        {
            CHUCHO_INFO(logger_, std::string("Error setting socket options: ") + windows::error_message(WSAGetLastError()));
            closesocket(socket_);
            continue;
        }
        if (bind(socket_, cur->ai_addr, cur->ai_addrlen) == SOCKET_ERROR)
        {
            CHUCHO_INFO(logger_, std::string("Could not bind to local address: ") + windows::error_message(WSAGetLastError()));
            closesocket(socket_);
            continue;
        }
        break;
    }
    if (cur == nullptr)
        throw chucho::exception("No listening sockets could be created");
    if (listen(socket_, SOMAXCONN) == SOCKET_ERROR)
    {
        closesocket(socket_);
        throw chucho::exception("Unable to listen to local address: " + windows::error_message(WSAGetLastError()));
    }
}

socket_listener::~socket_listener()
{
    if (socket_ != INVALID_SOCKET)
    {
        shutdown(socket_, SD_BOTH);
        closesocket(socket_);
    }
}

std::shared_ptr<socket_reader> socket_listener::accept()
{
    fd_set sfd;
    int rc;
    struct sockaddr_storage addr;
    timeval wait;
    wait.tv_sec = 0;
    wait.tv_usec = 500000;
    do
    {
        FD_ZERO(&sfd);
        FD_SET(socket_, &sfd);
        rc = select(1, &sfd, nullptr, nullptr, &wait);
        if (is_shut_down)
            throw shutdown_exception();
        if (rc == SOCKET_ERROR)
        {
            throw chucho::exception("Unable to poll the socket: " + windows::error_message(WSAGetLastError()));
        }
        if (rc > 0)
        {
            int length = sizeof(addr);
            int sock = ::accept(socket_, reinterpret_cast<struct sockaddr*>(&addr), &length);
            if (sock == INVALID_SOCKET)
            {
                throw chucho::exception("Unable to accept a connection: " + windows::error_message(WSAGetLastError()));
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
    CHUCHO_ERROR(logger_, "Polling the socket has been halted");
    throw eof_exception();
}

}

}
