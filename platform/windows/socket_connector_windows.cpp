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

#include <chucho/socket_connector.hpp>
#include <chucho/socket_exception.hpp>
#include <chucho/garbage_cleaner.hpp>
#include "winsock_startup.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

namespace chucho
{

socket_connector::socket_connector(const std::string& host, std::uint16_t port)
    : socket_(INVALID_SOCKET),
      host_(host),
      port_(port),
      is_blocking_mode_(true)
{
    std::call_once(winsock::once, winsock::startup);
    set_status_origin("socket_connector");
    try
    {
        open_socket();
    }
    catch (std::exception&)
    {
        report_warning("Unable to open socket", std::current_exception());
    }
}

socket_connector::~socket_connector()
{
    if (socket_ != INVALID_SOCKET)
    {
        shutdown(socket_, SD_BOTH);
        closesocket(socket_);
    }
}

bool socket_connector::can_write()
{
    bool result = false;
    if (socket_ == INVALID_SOCKET)
    {
        try
        {
            open_socket();
        }
        catch (std::exception&)
        {
            report_warning("Unable to open socket", std::current_exception());
            return result;
        }
    }
    fd_set writes;
    FD_ZERO(&writes);
    FD_SET(socket_, &writes);
    fd_set errs;
    FD_ZERO(&errs);
    FD_SET(socket_, &errs);
    timeval wait;
    wait.tv_sec = 0;
    wait.tv_usec = 0;
    int rc = select(0, nullptr, &writes, &errs, &wait);
    if (rc == SOCKET_ERROR)
        throw socket_exception("Could not select socket for writing", WSAGetLastError());
    else if (rc > 0)
    {
        if (FD_ISSET(socket_, &errs))
        {
            report_info("The connection to " + host_ + ":" + std::to_string(port_) + " could not be made. Reconnecting...");
            open_socket();
        }
        else if (FD_ISSET(socket_, &writes))
        {
            if (!is_blocking_mode_)
            {
                u_long nblock = 0;
                ioctlsocket(socket_, FIONBIO, &nblock);
                is_blocking_mode_ = true;
                report_info("The connection to " + host_ + ":" + std::to_string(port_) + " was established");
            }
            result = true;
        }
    }
    return result;
}

void socket_connector::open_socket()
{
    if (socket_ != INVALID_SOCKET)
    {
        shutdown(socket_, SD_BOTH);
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
    }
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* addrs;
    int rc = getaddrinfo(host_.c_str(), std::to_string(port_).c_str(), &hints, &addrs);
    if (rc != 0)
        throw exception("Could not resolve address of " + host_ + ": " + gai_strerror(rc));
    struct sentry
    {
        sentry(addrinfo* addrs) : addrs_(addrs) { }
        ~sentry() { freeaddrinfo(addrs_); }
        addrinfo* addrs_;
    } sntry(addrs);
    do
    {
        socket_ = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
        if (socket_ != INVALID_SOCKET)
        {
            u_long nblock = 1;
            ioctlsocket(socket_, FIONBIO, &nblock);
            int rc = connect(socket_, addrs->ai_addr, addrs->ai_addrlen);
            if (rc == 0 || WSAGetLastError() == WSAEALREADY)
            {
                is_blocking_mode_ = false;
                return;
            }
            closesocket(socket_);
        }
        addrs = addrs->ai_next;
    } while (addrs != nullptr);
    int err = WSAGetLastError();
    if (socket_ != INVALID_SOCKET)
        closesocket(socket_);
    std::string msg;
    if (socket_ == INVALID_SOCKET)
        msg = "Could not create socket";
    else
        msg = "Could not connect to " + host_;
    throw socket_exception(msg, err);
}

void socket_connector::write(const std::uint8_t* buf, std::size_t length)
{
    if (socket_ != INVALID_SOCKET)
    {
        std::size_t remaining = length;
        while (remaining > 0)
        {
            int sent = send(socket_,
                            reinterpret_cast<const char*>(buf + length - remaining),
                            remaining,
                            0);
            if (sent == SOCKET_ERROR)
                throw socket_exception("Error sending data over socket", WSAGetLastError());
            remaining -= sent;
        }
    }
}

}

