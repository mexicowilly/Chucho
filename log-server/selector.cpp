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

#include "selector.hpp"

namespace chucho
{

namespace server
{

selector::selector(std::function<void(std::shared_ptr<socket_reader>)> was_selected)
    : logger_(chucho::logger::get("chuchod.selector")),
      stop_(false),
      was_selected_(was_selected),
      thread_(std::bind(&selector::main, this))
{
}

selector::~selector()
{
    guard_.lock();
    stop_ = true;
    guard_.unlock();
    condition_.notify_all();
    thread_.join();
    CHUCHO_INFO_STR(logger_, "Joined the selector thread");
}

void selector::add(std::shared_ptr<socket_reader> reader)
{
    std::lock_guard<std::mutex> lg(guard_);
    readers_[reader->get_socket()] = reader;
    condition_.notify_one();
}

void selector::remove(std::shared_ptr<socket_reader> reader)
{
    std::lock_guard<std::mutex> lg(guard_);
    readers_.erase(reader->get_socket());
    condition_.notify_one();
}

}

}
