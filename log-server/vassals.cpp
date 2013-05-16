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

#include "vassals.hpp"
#include <chucho/log.hpp>

namespace chucho
{

namespace server
{

vassals::vassals(std::size_t count, std::function<void(std::shared_ptr<socket_reader>)> processor)
    : stop_(false),
      processor_(processor),
      logger_(chucho::logger::get("chuchod.vassals"))
{
    for (std::size_t i = 0; i < count; i++)
        vassals_.emplace_back(std::bind(&vassals::work, this));
    CHUCHO_INFO(logger_, "Started " << count << " worker threads");
}

vassals::~vassals()
{
    guard_.lock();
    stop_ = true;
    guard_.unlock();
    condition_.notify_all();
    for (std::thread& t : vassals_)
        t.join();
    CHUCHO_INFO(logger_, "Joined " << vassals_.size() << " worker threads");
}

void vassals::submit(std::shared_ptr<socket_reader> reader)
{
    guard_.lock();
    readers_.push(reader);
    guard_.unlock();
    condition_.notify_one();
}

void vassals::work()
{
    while (true)
    {
        std::unique_lock<std::mutex> ul(guard_);
        while (!stop_ && readers_.empty())
            condition_.wait(ul);
        if (stop_)
            break;
        std::shared_ptr<socket_reader> reader = readers_.front();
        readers_.pop();
        ul.unlock();
        CHUCHO_DEBUG(logger_, "Processing " << reader->get_full_host());
        processor_(reader);
    }
}

}

}