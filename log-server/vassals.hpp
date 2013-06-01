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

#if !defined(CHUCHO_VASSALS_HPP__)
#define CHUCHO_VASSALS_HPP__

#include "socket_reader.hpp"
#include <chucho/logger.hpp>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>

namespace chucho
{

namespace server
{

class vassals
{
public:
    vassals(std::size_t count, std::function<void(std::shared_ptr<socket_reader>)> processor);
    vassals(const vassals&) = delete;
    ~vassals();

    vassals& operator= (const vassals&) = delete;

    void start();
    void stop();
    void submit(std::shared_ptr<socket_reader> reader);

private:
    void work();

    std::vector<std::thread> vassals_;
    std::queue<std::shared_ptr<socket_reader>> readers_;
    std::mutex guard_;
    std::condition_variable condition_;
    std::function<void(std::shared_ptr<socket_reader>)> processor_;
    std::shared_ptr<chucho::logger> logger_;
    std::size_t count_;
    bool stop_;
};

}

}

#endif
