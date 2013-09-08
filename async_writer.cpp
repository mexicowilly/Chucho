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

#include <chucho/async_writer.hpp>

namespace
{

class noop_formatter : public chucho::formatter
{

public:
    virtual std::string format(const chucho::event& evt) override;
};

std::string noop_formatter::format(const chucho::event& evt)
{
    return std::string();
}

}

namespace chucho
{

const std::size_t async_writer::DEFAULT_QUEUE_CAPACITY = 256;

async_writer::async_writer(std::shared_ptr<writer> wrt,
                           std::size_t capacity,
                           std::shared_ptr<level> discard_threshold)
    : writer(std::make_shared<noop_formatter>()),
      writer_(wrt),
      capacity_(capacity),
      discard_threshold_(discard_threshold),
      stop_(false)
{
    set_status_origin("async_writer");
    if (writer_)
        worker_.reset(new std::thread(std::bind(&async_writer::thread_main, this)));
}

async_writer::~async_writer()
{
    if (worker_)
    {
        guard_.lock();
        stop_ = true;
        queue_.clear();
        empty_condition_.notify_one();
        full_condition_.notify_all();
        guard_.unlock();
        worker_->join();
    }
}

std::size_t async_writer::get_queue_size()
{
    std::lock_guard<std::mutex> lg(guard_);
    return queue_.size();
}

void async_writer::thread_main()
{
    while (true)
    {
        std::unique_lock<std::mutex> ul(guard_);
        empty_condition_.wait(ul, [this]{ return stop_ || !queue_.empty(); });
        if (stop_)
            break;
        event evt = queue_.front();
        queue_.pop_front();
        full_condition_.notify_all();
        std::shared_ptr<level> dt = discard_threshold_;
        double full = static_cast<double>(queue_.size()) / static_cast<double>(capacity_);
        ul.unlock();
        if (full <= .8 || dt == level::OFF_() || evt.get_level() > dt)
            writer_->write(evt);
    }
}

void async_writer::write_impl(const event& evt)
{
    std::unique_lock<std::mutex> ul(guard_);
    full_condition_.wait(ul, [this]{ return queue_.size() <= capacity_; });
    queue_.push_back(evt);
    empty_condition_.notify_one();
}

}
