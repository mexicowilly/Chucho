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

#if !defined(CHUCHO_REMOTE_WRITER_HPP__)
#define CHUCHO_REMOTE_WRITER_HPP__

#include <chucho/writer.hpp>
#include <cstdint>
#include <queue>

namespace chucho
{

class socket_connector;

class CHUCHO_EXPORT remote_writer : public writer
{
public:
    static const std::uint16_t DEFAULT_PORT;
    static const std::size_t DEFAULT_UNSENT_CACHE_MAX;

    remote_writer(const std::string& host,
                  std::uint16_t port = DEFAULT_PORT,
                  std::size_t unsent_cache_max = DEFAULT_UNSENT_CACHE_MAX);
    ~remote_writer();

    const std::string& get_host() const;
    std::uint16_t get_port() const;
    std::size_t get_unsent_cache_max() const;
    std::size_t get_unsent_cache_size() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    void retry_until_connected();

    std::unique_ptr<socket_connector> connector_;
    std::string host_;
    std::uint16_t port_;
    std::deque<event> unsent_events_;
    std::size_t unsent_cache_max_;
};

inline const std::string& remote_writer::get_host() const
{
    return host_;
}

inline std::uint16_t remote_writer::get_port() const
{
    return port_;
}

inline std::size_t remote_writer::get_unsent_cache_max() const
{
    return unsent_cache_max_;
}

inline std::size_t remote_writer::get_unsent_cache_size() const
{
    return unsent_events_.size();
}

}

#endif
