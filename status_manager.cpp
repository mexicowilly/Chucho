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

#include <chucho/status_manager.hpp>
#include <algorithm>

namespace
{

struct level_less
{
    bool operator() (const chucho::status& st1, const chucho::status& st2) const
    {
        return st1.get_level() < st2.get_level();
    }
};

}

namespace chucho
{

status_manager::status_manager()
    : count_(0),
      level_(status::level::INFO)
{
}

void status_manager::add(const status& st)
{
    std::unique_lock<std::mutex> ul(cache_guard_);
    count_++;
    if (head_cache_.size() < HEAD_CACHE_MAX)
    {
        head_cache_.push_back(st);
    }
    else
    {
        tail_cache_.push_back(st);
        if (tail_cache_.size() > TAIL_CACHE_MAX)
            tail_cache_.pop_front();
    }
    adjust_level();
    ul.unlock();
    ul = std::unique_lock<std::mutex>(observer_guard_);
    for (std::shared_ptr<status_observer> obs : observers_)
        obs->status_reported(st);
}

void status_manager::add(std::shared_ptr<status_observer> obs)
{
    std::lock_guard<std::mutex> lg(observer_guard_);
    observers_.insert(obs);
}

void status_manager::adjust_level()
{
    status::level fixed_max = head_cache_.empty() ?
        status::level::INFO :
        (*std::max_element(head_cache_.begin(),
                           head_cache_.end(),
                           level_less())).get_level();
    status::level tailed_max = tail_cache_.empty() ?
        status::level::INFO :
        (*std::max_element(tail_cache_.begin(),
                           tail_cache_.end(),
                           level_less())).get_level();
    level_ = std::max(fixed_max, tailed_max);
}

void status_manager::clear()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    count_ = 0;
    head_cache_.clear();
    tail_cache_.clear();
    level_ = status::level::INFO;
}

std::shared_ptr<status_manager> status_manager::get()
{
    static std::shared_ptr<status_manager> smgr(new status_manager());

    return smgr;
}

std::vector<status> status_manager::get_all()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    std::vector<status> result(head_cache_);
    result.insert(result.end(), tail_cache_.begin(), tail_cache_.end());
    return result;
}

std::size_t status_manager::get_count()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    return count_;
}

status::level status_manager::get_level()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    return level_;
}

void status_manager::remove(std::shared_ptr<status_observer> obs)
{
    std::lock_guard<std::mutex> lg(observer_guard_);
    observers_.erase(obs);
}

}
