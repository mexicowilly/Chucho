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

#if !defined(CHUCHO_SYSLOG_WRITER_HPP__)
#define CHUCHO_SYSLOG_WRITER_HPP__

#include <chucho/writer.hpp>
#include <chucho/optional.hpp>
#include <chucho/syslog_constants.hpp>

namespace chucho
{

class syslog_transport_handle;

class CHUCHO_EXPORT syslog_writer : public writer
{
public:
    syslog_writer(std::shared_ptr<formatter> fmt,
                  syslog::facility fcl);
    syslog_writer(std::shared_ptr<formatter> fmt,
                  syslog::facility fcl,
                  const std::string& host,
                  std::uint16_t port = syslog::DEFAULT_PORT,
                  const std::string& app_name = "");

protected:
    virtual void write_impl(const event& evt) override;

private:
    class CHUCHO_NO_EXPORT transport
    {
    public:
        transport();
        transport(const std::string& host, std::uint16_t port);
        ~transport();

        std::string format(syslog::facility fcl,
                           syslog::severity sev,
                           const event::time_type& when,
                           const std::string& app_name,
                           const std::string& message);
        void send(syslog::facility fcl,
                  syslog::severity sev,
                  const std::string& message);

    private:
        syslog_transport_handle* handle_;
    };

    CHUCHO_NO_EXPORT std::string remote_message(syslog::facility fcl,
                                                syslog::severity sev,
                                                const event::time_type& when,
                                                const std::string& local_message);

    transport transport_;
    syslog::facility facility_;
    std::string app_name_;
};

}

#endif