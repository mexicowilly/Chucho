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

#if !defined(CHUCHO_SELECTOR_HPP__)
#define CHUCHO_SELECTOR_HPP__

#include "socket_reader.hpp"
#include <chucho/log.hpp>
#include <condition_variable>
#include <thread>
#include <map>
#include <atomic>

namespace chucho
{

namespace server
{

class selector
{
public:
    selector(std::function<void(std::shared_ptr<socket_reader>)> was_selected);
    ~selector();

    void add(std::shared_ptr<socket_reader> reader);
    void remove(std::shared_ptr<socket_reader> reader);
    void start();
    void stop();

private:
    void main();

    std::shared_ptr<chucho::logger> logger_;
    std::mutex guard_;
    std::condition_variable condition_;
    std::map<int, std::shared_ptr<socket_reader>> readers_;
    bool stop_;
    std::function<void(std::shared_ptr<socket_reader>)> was_selected_;
    std::thread thread_;
};

}

}

#endif
