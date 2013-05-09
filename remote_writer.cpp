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

#include <chucho/remote_writer.hpp>
#include <chucho/socket_connector.hpp>
#include <chucho/exception.hpp>
#include <chucho/yaml_formatter.hpp>
#if defined(CHUCHO_HAVE_ARPA_INET_H)
#include <arpa/inet.h>
#endif

namespace chucho
{

remote_writer::remote_writer(const std::string& host,
                             std::uint16_t port)
    : remote_writer(host, port, std::chrono::seconds(30), 1000)
{
}

remote_writer::remote_writer(const std::string& host,
                             std::uint16_t port,
                             const std::chrono::seconds& connect_interval,
                             std::size_t unsent_cache_size)
    : writer(std::make_shared<yaml_formatter>()),
      connect_interval_(connect_interval),
      host_(host),
      port_(port),
      is_thread_interrupted_(false),
      unsent_cache_size_(unsent_cache_size)
{
    set_status_origin("remote_writer");
    if (host.empty())
        throw exception("The host name cannot be empty");
    if (port == 0)
        throw exception("The port cannot be zero");
    try
    {
        connector_.reset(new socket_connector(host, port));
    }
    catch (exception& e)
    {
        report_info("Could not connect to log server " + host_ + ":" +
            std::to_string(port_) + ": " + e.what());
        connector_thread_ = std::move(
           std::thread(&remote_writer::retry_until_connected, this));
    }
}

remote_writer::~remote_writer()
{
    if (connector_thread_.joinable())
    {
        is_thread_interrupted_ = true;
        connector_thread_.join();
    }
}

void remote_writer::retry_until_connected()
{
    while (!is_thread_interrupted_)
    {
        std::chrono::steady_clock::time_point wake_time =
            std::chrono::steady_clock::now() + connect_interval_;
        while (std::chrono::steady_clock::now() < wake_time)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (is_thread_interrupted_)
            {
                report_info("The connector thread has been interrupted.");
                return;
            }
        }
        report_info("Attempting connection to " + host_ + ":" + std::to_string(port_));
        try
        {
            connector_.reset(new socket_connector(host_, port_));
            report_info("Connection established. Exiting connector thread.");
            return;
        }
        catch (exception& e)
        {
            report_info(std::string("Connection failed: ") + e.what());
        }
    }
    report_info("The connector thread has been interrupted.");
}

void remote_writer::write_impl(const event& evt)
{
    if (connector_thread_.joinable())
    {
        unsent_events_.push_back(evt);
        if (unsent_events_.size() > unsent_cache_size_)
            unsent_events_.pop_front();
    }
    else
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
                    size = htonl(yaml.length());
                    std::memcpy(const_cast<char*>(yaml.data()), &size, sizeof(std::uint32_t));
                    connector_->write(reinterpret_cast<const std::uint8_t*>(yaml.data()), yaml.length());
                    yaml = std::string(sizeof(std::uint32_t), 0);
                }
            }
            yaml.append(formatter_->format(evt));
            size = htonl(yaml.length());
            std::memcpy(const_cast<char*>(yaml.data()), &size, sizeof(std::uint32_t));
            connector_->write(reinterpret_cast<const std::uint8_t*>(yaml.data()), yaml.length());
            unsent_events_.clear();
        }
        catch (exception& e)
        {
            report_warning("There was a problem with the connection to " +
                host_ + ':' + std::to_string(port_) + ": " + e.what());
            unsent_events_.push_back(evt);
            connector_thread_ = std::move(std::thread(&remote_writer::retry_until_connected, this));
        }
    }
}

}
