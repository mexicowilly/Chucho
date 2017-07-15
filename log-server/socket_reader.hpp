/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_SOCKET_READER_HPP__)
#define CHUCHO_SOCKET_READER_HPP__

#include <cstdint>
#include <cstddef>
#include <string>

namespace chucho
{

namespace server
{

class socket_reader
{
public:
    socket_reader(int sock,
                  const std::string& base_host,
                  const std::string& full_host);
    ~socket_reader();

    const std::string& get_base_host() const;
    const std::string& get_full_host() const;
    int get_socket() const;
    void read(std::uint8_t* dest, std::size_t count);

private:
    std::size_t read_impl(std::uint8_t* dest, std::size_t count);

    int socket_;
    std::string base_host_;
    std::string full_host_;
};

inline const std::string& socket_reader::get_base_host() const
{
    return base_host_;
}

inline const std::string& socket_reader::get_full_host() const
{
    return full_host_;
}

inline int socket_reader::get_socket() const
{
    return socket_;
}

}

}

#endif
