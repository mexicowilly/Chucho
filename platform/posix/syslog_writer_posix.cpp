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
#include <chucho/exception.hpp>
#include <chucho/calendar.hpp>
#include <chucho/host.hpp>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>
#include <syslog.h>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <array>
#include <cerrno>

namespace chucho
{

class syslog_transport_handle
{
public:
    virtual ~syslog_transport_handle() { }

    virtual std::string format(syslog::facility fcl,
                               syslog::severity sev,
                               const event::time_type& when,
                               const std::string& message) = 0;
    virtual void send(syslog::facility fcl,
                      syslog::severity sev,
                      const std::string& message) = 0;
};

}

namespace
{

std::string formatTime(const chucho::event::time_type& when)
{
    static std::array<std::string, 12> english_months =
    {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    static std::string pattern(" %e %H:%M:%S");

    struct std::tm cal = chucho::calendar::get_local(std::chrono::system_clock::to_time_t(when));
    std::string result = english_months[cal.tm_mon];
    result += chucho::calendar::format(cal, pattern);
    return result;
}

class local_syslog_transport_handle : public chucho::syslog_transport_handle
{
public:
    virtual std::string format(chucho::syslog::facility fcl,
                               chucho::syslog::severity sev,
                               const chucho::event::time_type& when,
                               const std::string& message) override;
    virtual void send(chucho::syslog::facility fcl,
                      chucho::syslog::severity sev,
                      const std::string& message) override;
};

class remote_syslog_transport_handle : public chucho::syslog_transport_handle
{
public:
    remote_syslog_transport_handle(const std::string& host, std::uint16_t port);
    ~remote_syslog_transport_handle();

    virtual std::string format(chucho::syslog::facility fcl,
                               chucho::syslog::severity sev,
                               const chucho::event::time_type& when,
                               const std::string& message) override;
    virtual void send(chucho::syslog::facility fcl,
                      chucho::syslog::severity sev,
                      const std::string& message) override;

private:
    std::string get_timestamp();

    int socket_;
    std::vector<std::uint8_t> address_;
};

std::string local_syslog_transport_handle::format(chucho::syslog::facility fcl,
                                                  chucho::syslog::severity sev,
                                                  const chucho::event::time_type& when,
                                                  const std::string& message)
{
    return message;
}

void local_syslog_transport_handle::send(chucho::syslog::facility fcl,
                                         chucho::syslog::severity sev,
                                         const std::string& message)
{
    syslog(static_cast<int>(fcl) | static_cast<int>(sev), "%s", message.c_str());
}

remote_syslog_transport_handle::remote_syslog_transport_handle(const std::string& host, std::uint16_t port)
    : socket_(-1)
{
    struct addrinfo* info;
    int rc = getaddrinfo(host.c_str(), std::to_string(port).c_str(), nullptr, &info);
    if (rc != 0)
        throw chucho::exception("Could not resolve address of " + host + ": " + gai_strerror(rc));
    address_.resize(info->ai_addrlen);
    std::memcpy(&address_[0], info->ai_addr, info->ai_addrlen);
    socket_ = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    freeaddrinfo(info);
    if (socket_ == -1)
        throw chucho::exception(std::string("Could not create socket: ") + std::strerror(errno));
}

remote_syslog_transport_handle::~remote_syslog_transport_handle()
{
    close(socket_);
}

std::string remote_syslog_transport_handle::format(chucho::syslog::facility fcl,
                                                   chucho::syslog::severity sev,
                                                   const chucho::event::time_type& when,
                                                   const std::string& message)
{
    std::ostringstream stream;
    stream << '<' << (static_cast<int>(fcl) | static_cast<int>(sev)) << '>' <<
        formatTime(when) << ' ' << chucho::host::get_base_name() << ' ' << message;
    return stream.str();
}

void remote_syslog_transport_handle::send(chucho::syslog::facility fcl,
                                          chucho::syslog::severity sev,
                                          const std::string& message)
{
    if (socket_ != -1 &&
        sendto(socket_,
               message.data(),
               message.length(),
               0,
               reinterpret_cast<struct sockaddr*>(&address_[0]),
               address_.size()) == -1)
    {
        throw chucho::exception(std::string("Unable to send syslog data: ") + std::strerror(errno));
    }
}

}

namespace chucho
{

syslog_writer::transport::transport()
    : handle_(new local_syslog_transport_handle())
{
}

syslog_writer::transport::transport(const std::string& host, std::uint16_t port)
    : handle_(new remote_syslog_transport_handle(host, port))
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

void syslog_writer::transport::send(syslog::facility fcl, syslog::severity sev, const std::string& message)
{
    handle_->send(fcl, sev, message);
}

}
