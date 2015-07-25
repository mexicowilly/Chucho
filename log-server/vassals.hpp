/*
 * Copyright 2013-2015 Will Mason
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
#include <chucho/non_copyable.hpp>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>
#include <list>

namespace chucho
{

namespace server
{

class vassals : non_copyable
{
public:
    vassals(std::size_t count, std::function<void(std::shared_ptr<socket_reader>)> processor);
    ~vassals();

    void start(std::size_t count);
    void stop();
    void submit(std::shared_ptr<socket_reader> reader);

private:
    class worker_queue : non_copyable,
                         public std::thread
    {
    public:
        worker_queue(std::function<void(std::shared_ptr<socket_reader>)> processor);

        void add(std::shared_ptr<socket_reader> reader);
        std::list<std::weak_ptr<socket_reader>> remove_all_readers();
        // returns the number of readers left
        std::size_t remove_unused_readers();
        std::size_t get_reader_count();
        bool is_mine(std::shared_ptr<socket_reader> reader);
        void stop();
        void submit_for_reading(std::shared_ptr<socket_reader> reader);

    private:
        void work();

        std::unique_ptr<std::thread> thread_;
        std::queue<std::shared_ptr<socket_reader>> readers_with_work_;
        std::list<std::weak_ptr<socket_reader>> my_readers_;
        std::mutex guard_;
        std::condition_variable condition_;
        std::function<void(std::shared_ptr<socket_reader>)> processor_;
        std::shared_ptr<chucho::logger> logger_;
        bool stop_;
    };

    void redistribute_load();

    std::vector<std::unique_ptr<worker_queue>> vassals_;
    std::function<void(std::shared_ptr<socket_reader>)> processor_;
    std::shared_ptr<chucho::logger> logger_;
    std::chrono::steady_clock::time_point next_redistribution_;
};

}

}

#endif
