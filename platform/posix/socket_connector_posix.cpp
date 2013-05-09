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
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

namespace chucho
{

socket_connector::socket_connector(const std::string& host, std::uint16_t port)
{
    std::string ptext(std::to_string(port));
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    struct addrinfo* addrs;
    int rc = getaddrinfo(host.c_str(), ptext.c_str(), nullptr, &addrs);
    if (rc != 0)
        throw exception("Could not resolve address of " + host + ": " + gai_strerror(rc));
    struct Sentry
    {
        Sentry(addrinfo* addrs) : addrs_(addrs) { }
        ~Sentry() { freeaddrinfo(addrs_); }
        addrinfo* addrs_;
    } sentry(addrs);
    do
    {
        socket_ = socket(addrs->ai_family, addrs->ai_socktype, addrs->ai_protocol);
        if (socket_ != -1)
        {
            if (connect(socket_, addrs->ai_addr, addrs->ai_addrlen) == 0)
                return;
        }
        addrs = addrs->ai_next;
    } while (addrs != nullptr);
    int err = errno;
    std::string msg;
    if (socket_ == -1)
        msg = "Could not create socket";
    else
        msg = "Could not connect to " + host;
    throw socket_exception(msg, err);
}

socket_connector::~socket_connector()
{
    if (socket_ != -1)
    {
        shutdown(socket_, SHUT_RDWR);
        close(socket_);
    }
}

void socket_connector::write(const std::uint8_t* buf, std::size_t length)
{
    if (socket_ != -1)
    {
        std::size_t remaining = length;
        while (remaining > 0)
        {
            ssize_t sent = send(socket_, buf + length - remaining, remaining, 0);
            if (sent == -1)
                throw socket_exception("Error sending data over socket", errno);
            remaining -= sent;
        }
    }
}

}
