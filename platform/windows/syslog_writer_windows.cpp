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

#include <chucho/syslog_writer.hpp>
#include <chucho/socket_exception.hpp>
#include <chucho/host.hpp>
#include <chucho/calendar.hpp>
#include "winsock_startup.hpp"
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace
{

std::string format_time(const chucho::event::time_type& when)
{
    static const char* english_months[] =
    {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    struct std::tm cal = chucho::calendar::get_local(std::chrono::system_clock::to_time_t(when));
    std::string result = english_months[cal.tm_mon];
    result += chucho::calendar::format(cal, " %d %H:%M:%S");
    return result;
}

}

namespace chucho
{

class syslog_transport_handle
{
public:
    syslog_transport_handle(const std::string& host, std::uint16_t port);
    ~syslog_transport_handle();

    std::string format(syslog::facility fcl,
                       syslog::severity sev,
                       const event::time_type& when,
                       const std::string& message);
    void send(syslog::facility fcl,
              syslog::severity sev,
              const std::string& message);

private:
    SOCKET socket_;
    std::vector<std::uint8_t> address_;
};

syslog_transport_handle::syslog_transport_handle(const std::string& host,
                                                 std::uint16_t port)
    : socket_(INVALID_SOCKET)
{
    std::call_once(winsock::once, winsock::startup);
    struct addrinfo* info;
    int rc = getaddrinfo(host.c_str(), std::to_string(port).c_str(), nullptr, &info);
    if (rc != 0)
        throw chucho::exception("Could not resolve address of " + host + ": " + gai_strerror(rc));
    address_.resize(info->ai_addrlen);
    std::memcpy(&address_[0], info->ai_addr, info->ai_addrlen);
    socket_ = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    freeaddrinfo(info);
    if (socket_ == INVALID_SOCKET)
        throw socket_exception("Could not create socket", WSAGetLastError());
}

syslog_transport_handle::~syslog_transport_handle()
{
    closesocket(socket_);
}

std::string syslog_transport_handle::format(syslog::facility fcl,
                                            syslog::severity sev,
                                            const chucho::event::time_type& when,
                                            const std::string& message)
{
    std::ostringstream stream;
    stream << '<' << (static_cast<int>(fcl) | static_cast<int>(sev)) << '>' <<
        format_time(when) << ' ' << host::get_base_name() << ' ' << message;
    return stream.str();
}

void syslog_transport_handle::send(chucho::syslog::facility fcl,
                                   chucho::syslog::severity sev,
                                   const std::string& message)
{
    if (socket_ != INVALID_SOCKET &&
        sendto(socket_,
               message.data(),
               message.length(),
               0,
               reinterpret_cast<struct sockaddr*>(&address_[0]),
               address_.size()) == SOCKET_ERROR)
    {
        throw socket_exception("Unable to send syslog data: ", WSAGetLastError());
    }
}

syslog_writer::transport::transport()
    : handle_(new syslog_transport_handle(host::get_base_name(), syslog::DEFAULT_PORT))
{
}

syslog_writer::transport::transport(const std::string& host, std::uint16_t port)
    : handle_(new syslog_transport_handle(host, port))
{
}

syslog_writer::transport::~transport()
{
    delete handle_;
}

std::string syslog_writer::transport::format(syslog::facility fcl,
                                             syslog::severity sev,
                                             const chucho::event::time_type& when,
                                             const std::string& message)
{
    return handle_->format(fcl, sev, when, message);
}

void syslog_writer::transport::send(syslog::facility fcl,
                                    syslog::severity sev,
                                    const std::string& message)
{
    handle_->send(fcl, sev, message);
}

}
