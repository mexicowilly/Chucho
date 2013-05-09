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

#if !defined(CHUCHO_SOCKET_CONNECTOR_HPP__)
#define CHUCHO_SOCKET_CONNECTOR_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <string>
#include <cstdint>

namespace chucho
{

class socket_connector
{
public:
    socket_connector(const std::string& host, std::uint16_t port);
    ~socket_connector();

    void write(const std::uint8_t* buf, std::size_t length);

private:
    int socket_;
};

}

#endif
