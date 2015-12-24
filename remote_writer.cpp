/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/remote_writer.hpp>
#include <chucho/socket_connector.hpp>
#include <chucho/exception.hpp>
#include <chucho/yaml_formatter.hpp>
#if defined(CHUCHO_HAVE_ARPA_INET_H)
#include <arpa/inet.h>
#elif defined(CHUCHO_HAVE_WINSOCK2_H)
#include <winsock2.h>
#endif
#include <cstring>

namespace chucho
{

const std::uint16_t remote_writer::DEFAULT_PORT(21012);
const std::size_t remote_writer::DEFAULT_UNSENT_CACHE_MAX(1000);

remote_writer::remote_writer(const std::string& host,
                             std::uint16_t port,
                             std::size_t unsent_cache_max)
    : writer(std::make_shared<yaml_formatter>()),
      host_(host),
      port_(port),
      unsent_cache_max_(unsent_cache_max)
{
    set_status_origin("remote_writer");
    if (host.empty())
        throw std::invalid_argument("The host name cannot be empty");
    if (port == 0)
        throw std::invalid_argument("The port cannot be zero");
    connector_.reset(new socket_connector(host, port));
}

// If I don't put this empty destructor here, then the connector_
// object causes remote_writer to fail to build, since the
// socket_connector type is undefined.
remote_writer::~remote_writer()
{
}

void remote_writer::write_impl(const event& evt)
{
    if (connector_->can_write())
    {
        try
        {
            std::string yaml(sizeof(std::uint32_t), 0);
            std::uint32_t size;
            for (const event& e : unsent_events_)
            {
                yaml.append(formatter_->format(e));
                if (yaml.length() > 1024 * 1024)
                {
                    size = htonl(yaml.length() - sizeof(std::uint32_t));
                    std::memcpy(const_cast<char*>(yaml.data()), &size, sizeof(std::uint32_t));
                    connector_->write(reinterpret_cast<const std::uint8_t*>(yaml.data()), yaml.length());
                    yaml = std::string(sizeof(std::uint32_t), 0);
                }
            }
            yaml.append(formatter_->format(evt));
            size = htonl(yaml.length() - sizeof(std::uint32_t));
            std::memcpy(const_cast<char*>(yaml.data()), &size, sizeof(std::uint32_t));
            connector_->write(reinterpret_cast<const std::uint8_t*>(yaml.data()), yaml.length());
            if (!unsent_events_.empty())
                report_info(std::to_string(unsent_events_.size()) + " cached events were sent");
            unsent_events_.clear();
        }
        catch (exception& e)
        {
            report_warning("There was a problem with the connection to " +
                host_ + ':' + std::to_string(port_) + ": " + e.what());
            unsent_events_.push_back(evt);
        }
    }
    else
    {
        if (unsent_events_.empty())
            report_info("The connection has been lost, so as many as " + std::to_string(unsent_cache_max_) + " events will be cached");
        unsent_events_.push_back(evt);
        if (unsent_events_.size() > unsent_cache_max_)
            unsent_events_.pop_front();
    }
}

}
