/*
 * Copyright 2013-2017 Will Mason
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
#include <chucho/event_cache.hpp>

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

using namespace std::chrono_literals;

async_writer::async_writer(const std::string& name,
                           std::unique_ptr<writer>&& wrt,
                           bool flush_on_destruct)
    : async_writer(name,
                   std::move(wrt),
                   DEFAULT_CHUNK_SIZE,
                   DEFAULT_MAX_CHUNKS,
                   flush_on_destruct,
                   std::function<void()>(),
                   std::function<void()>())
{
}

async_writer::async_writer(const std::string& name,
                           std::unique_ptr<writer>&& wrt,
                           std::size_t chunk_size,
                           std::size_t max_chunks,
                           bool flush_on_destruct)
    : async_writer(name,
                   std::move(wrt),
                   chunk_size,
                   max_chunks,
                   flush_on_destruct,
                   std::function<void()>(),
                   std::function<void()>())
{
}

async_writer::async_writer(const std::string& name,
                           std::unique_ptr<writer>&& wrt,
                           std::size_t chunk_size,
                           std::size_t max_chunks,
                           bool flush_on_destruct,
                           std::function<void()> enter_thread_cb,
                           std::function<void()> leave_thread_cb)
    : writer(name, std::move(std::make_unique<noop_formatter>())),
      event_cache_provider(chunk_size, max_chunks * chunk_size),
      writer_(std::move(wrt)),
      stop_(false),
      enter_thread_cb_(enter_thread_cb),
      leave_thread_cb_(leave_thread_cb),
      flush_on_destruct_(flush_on_destruct)
{
    set_status_origin("async_writer");
    if (writer_)
        worker_ = std::make_unique<std::thread>(std::bind(&async_writer::thread_main, this));
}

async_writer::~async_writer()
{
    if (worker_)
    {
        stop_ = true;
        worker_->join();
    }
}

void async_writer::thread_main()
{
    if (enter_thread_cb_) 
        enter_thread_cb_();
    while (true)
    {
        auto evt = cache_->pop(250ms);
        if (evt)
        {
            if (stop_ && !flush_on_destruct_)
                break;
            writer_->write(*evt);
        }
        else if (stop_)
        {
            break;
        }
    }
    if (leave_thread_cb_) 
        leave_thread_cb_();
    report_info("The writer thread is exiting");
}

void async_writer::write_impl(const event& evt)
{
    cache_->push(evt);
}

}
