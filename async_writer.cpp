/*
 * Copyright 2013-2019 Will Mason
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

constexpr std::size_t async_writer::DEFAULT_CHUNK_SIZE;
constexpr std::size_t async_writer::DEFAULT_MAX_CHUNKS;

async_writer::async_writer(const std::string& name,
                           std::unique_ptr<writer>&& wrt,
                           bool flush_on_destruct)
    : async_writer(name,
                   std::move(wrt),
                   DEFAULT_CHUNK_SIZE,
                   DEFAULT_MAX_CHUNKS,
                   flush_on_destruct)
{
}

async_writer::async_writer(const std::string& name,
                           std::unique_ptr<writer>&& wrt,
                           std::size_t chunk_size,
                           std::size_t max_chunks,
                           bool flush_on_destruct)
    : writer(name, std::move(std::make_unique<noop_formatter>())),
      event_cache_provider(chunk_size, max_chunks * chunk_size),
      writer_(std::move(wrt)),
      stop_(false),
      flush_on_destruct_(flush_on_destruct)
{
    if (max_chunks < 2)
        throw std::invalid_argument("max_chunks must be 2 or greater");
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
    report_info("The writer thread is exiting");
}

void async_writer::write_impl(const event& evt)
{
    cache_->push(evt);
}

}
