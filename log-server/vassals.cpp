/*
 * Copyright 2013-2016 Will Mason
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
#include <algorithm>

namespace
{

const auto REDISTRIBUTION_INTERVAL = std::chrono::minutes(1);

}

namespace chucho
{

namespace server
{

vassals::vassals(std::size_t count, std::function<void(std::shared_ptr<socket_reader>)> processor)
    : processor_(processor),
      logger_(chucho::logger::get("chuchod.vassals")),
      next_redistribution_(std::chrono::steady_clock::now() + REDISTRIBUTION_INTERVAL)
{
    start(count);
}

vassals::~vassals()
{
    stop();
}

void vassals::redistribute_load()
{
    if (vassals_.size() < 2)
        return;
    CHUCHO_INFO_STR(logger_, "Redistributing the vassal workload");
    std::list<std::weak_ptr<socket_reader>> all;
    std::vector<std::size_t> load_distribution;
    for (std::unique_ptr<worker_queue>& v : vassals_)
    {
        std::list<std::weak_ptr<socket_reader>> cur = v->remove_all_readers();
        load_distribution.push_back(cur.size());
        all.insert(all.end(), cur.begin(), cur.end());
    }
    if (logger_->permits(chucho::level::INFO_()))
    {
        std::ostringstream stream;
        stream << "Current load distribution: ";
        for (auto num : load_distribution)
            stream << num << ' ';
        CHUCHO_INFO_STR(logger_, stream.str());
    }
    std::size_t vindex = 0;
    for (auto r : all)
    {
        vassals_[vindex]->add(r.lock());
        if (++vindex == vassals_.size())
            vindex = 0;
    }
    if (logger_->permits(chucho::level::INFO_()))
    {
        std::ostringstream stream;
        stream << "Modified load distribution: ";
        for (std::unique_ptr<worker_queue>& v : vassals_)
            stream << v->get_reader_count() << ' ';
        CHUCHO_INFO_STR(logger_, stream.str());
    }
    next_redistribution_ = std::chrono::steady_clock::now() + REDISTRIBUTION_INTERVAL;
}

void vassals::start(std::size_t count)
{
    if (vassals_.empty())
    {
        for (std::size_t i = 0; i < count; i++)
            vassals_.emplace_back(new worker_queue(processor_));
        CHUCHO_INFO(logger_, "Started " << count << " worker threads");
    }
}

void vassals::stop()
{
    if (!vassals_.empty())
    {
        for (std::unique_ptr<worker_queue>& q : vassals_)
            q->stop();
        CHUCHO_INFO(logger_, "Joined " << vassals_.size() << " worker threads");
        vassals_.clear();
    }
}

void vassals::submit(std::shared_ptr<socket_reader> reader)
{
    for (std::size_t i = 0; i < vassals_.size(); i++)
        vassals_[i]->get_reader_count();
    if (std::chrono::steady_clock::now() >= next_redistribution_)
        redistribute_load();
    for (std::unique_ptr<worker_queue>& q : vassals_)
    {
        if (q->is_mine(reader))
        {
            q->submit_for_reading(reader);
            return;
        }
    }
    std::size_t index_of_least_used;
    std::size_t least_used = std::numeric_limits<std::size_t>::max();
    for (std::size_t i = 0; i < vassals_.size(); i++)
    {
        std::size_t cur = vassals_[i]->remove_unused_readers();
        if (cur < least_used)
        {
            index_of_least_used = i;
            least_used = cur;
        }
    }
    vassals_[index_of_least_used]->add(reader);
    vassals_[index_of_least_used]->submit_for_reading(reader);
}

vassals::worker_queue::worker_queue(std::function<void(std::shared_ptr<socket_reader>)> processor)
    : processor_(processor),
      logger_(logger::get("chuchod.vassals")),
      stop_(false)
{
    thread_.reset(new std::thread(std::bind(&worker_queue::work, this)));
}

void vassals::worker_queue::add(std::shared_ptr<socket_reader> reader)
{
    std::lock_guard<std::mutex> lg(guard_);
    my_readers_.push_front(reader);
}

std::size_t vassals::worker_queue::get_reader_count()
{
    std::lock_guard<std::mutex> lg(guard_);
    return my_readers_.size();
}

bool vassals::worker_queue::is_mine(std::shared_ptr<socket_reader> reader)
{
    std::lock_guard<std::mutex> lg(guard_);
    return std::find_if(my_readers_.begin(),
                        my_readers_.end(),
                        [&] (std::weak_ptr<socket_reader> wp) { return wp.lock() == reader; }) != my_readers_.end();
}

std::list<std::weak_ptr<socket_reader>> vassals::worker_queue::remove_all_readers()
{
    std::lock_guard<std::mutex> lg(guard_);
    my_readers_.remove_if([] (std::weak_ptr<socket_reader> wp) { return wp.expired(); });
    return std::move(my_readers_);
}

std::size_t vassals::worker_queue::remove_unused_readers()
{
    std::lock_guard<std::mutex> lg(guard_);
    my_readers_.remove_if([] (std::weak_ptr<socket_reader> wp) { return wp.expired(); });
    return my_readers_.size();
}

void vassals::worker_queue::stop()
{
    guard_.lock();
    stop_ = true;
    condition_.notify_one();
    guard_.unlock();
    thread_->join();
}

void vassals::worker_queue::submit_for_reading(std::shared_ptr<socket_reader> reader)
{
    std::lock_guard<std::mutex> lg(guard_);
    readers_with_work_.push(reader);
    condition_.notify_one();
}

void vassals::worker_queue::work()
{
    while (true)
    {
        std::unique_lock<std::mutex> ul(guard_);
        while (readers_with_work_.empty())
        {
            if (stop_)
                return;
            condition_.wait(ul);
        }
        if (stop_)
            break;
        std::shared_ptr<socket_reader> reader = readers_with_work_.front();
        readers_with_work_.pop();
        ul.unlock();
        CHUCHO_DEBUG(logger_, "Processing " << reader->get_full_host());
        processor_(reader);
    }
}

}

}
