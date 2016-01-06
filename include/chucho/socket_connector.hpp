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

#if !defined(CHUCHO_SOCKET_CONNECTOR_HPP__)
#define CHUCHO_SOCKET_CONNECTOR_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/status_reporter.hpp>
#include <string>
#include <cstdint>
#include <vector>

namespace chucho
{

class socket_connector : public status_reporter
{
public:
    socket_connector(const std::string& host, std::uint16_t port);
    ~socket_connector();

    bool can_write();
    void write(const std::uint8_t* buf, std::size_t length);

private:
    void open_socket();

    int socket_;
    std::string host_;
    std::uint16_t port_;
    bool is_blocking_mode_;
};

}

#endif
