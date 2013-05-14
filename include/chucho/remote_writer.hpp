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
#include <chrono>
#include <atomic>
#include <thread>
#include <queue>

namespace chucho
{

class socket_connector;

class CHUCHO_EXPORT remote_writer : public writer
{
public:
    static const std::uint16_t DEFAULT_PORT = 32123;

    remote_writer(const std::string& host,
                  std::uint16_t port = DEFAULT_PORT);
    remote_writer(const std::string& host,
                  std::uint16_t port,
                  const std::chrono::seconds& connect_interval,
                  std::size_t unsent_cache_size);
    ~remote_writer();

protected:
    virtual void write_impl(const event& evt) override;

private:
    void retry_until_connected();

    std::unique_ptr<socket_connector> connector_;
    std::chrono::seconds connect_interval_;
    std::string host_;
    std::uint16_t port_;
    std::atomic<bool> is_thread_interrupted_;
    std::thread connector_thread_;
    std::deque<event> unsent_events_;
    std::size_t unsent_cache_size_;
    std::atomic<bool> is_thread_running_;
};

}

#endif
