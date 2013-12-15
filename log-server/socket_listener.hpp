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

#if !defined(CHUCHO_SOCKET_LISTENER_HPP__)
#define CHUCHO_SOCKET_LISTENER_HPP__

#include "socket_reader.hpp"
#include <chucho/logger.hpp>
#include <chucho/non_copyable.hpp>
#include <atomic>

namespace chucho
{

namespace server
{

struct socket_listener_handle;

class socket_listener : non_copyable
{
public:
    socket_listener(std::uint16_t port);
    ~socket_listener();

    std::shared_ptr<socket_reader> accept();

private:
    int socket_;
    std::shared_ptr<chucho::logger> logger_;
    std::unique_ptr<socket_listener_handle> handle_;
};

}

}

#endif
