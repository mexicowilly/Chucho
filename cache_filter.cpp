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

#include <chucho/cache_filter.hpp>
#include <chucho/event_cache.hpp>
#include <chucho/logger.hpp>

namespace chucho
{

using namespace std::chrono_literals;

cache_filter::cache_filter(const std::string& name,
                           writer& wrt,
                           std::shared_ptr<level> threshold,
                           std::size_t chunk_size,
                           std::size_t max_chunks)
  : filter(name),
    event_cache_provider(chunk_size, chunk_size * max_chunks),
    writer_(&wrt),
    threshold_(threshold)
{
}

cache_filter::cache_filter(const std::string& name,
                           const std::string& writer_name,
                           std::shared_ptr<level> threshold,
                           std::size_t chunk_size,
                           std::size_t max_chunks)
    : filter(name),
      event_cache_provider(chunk_size, chunk_size * max_chunks),
      writer_(nullptr),
      threshold_(threshold),
      writer_name_(writer_name)
{
}

filter::result cache_filter::evaluate(const event& evt)
{
    result rs = result::DENY;
    if (*evt.get_level() >= *threshold_)
    {
        if (writer_ == nullptr)
            writer_ = &evt.get_logger()->get_writer(writer_name_);
        auto e = cache_->pop(1ms);
        while (e)
        {
            writer_->write_impl(*e);
            e = cache_->pop(1ms);
        }
        rs = result::ACCEPT;
    }
    return rs;
}

}
