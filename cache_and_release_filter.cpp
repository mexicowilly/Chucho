/*
 * Copyright 2013-2021 Will Mason
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

#include <chucho/cache_and_release_filter.hpp>
#include <chucho/event_cache.hpp>
#include <chucho/logger.hpp>

namespace chucho
{

using namespace std::chrono_literals;

cache_and_release_filter::cache_and_release_filter(const std::string& name,
                                                   writer& wrt,
                                                   std::shared_ptr<level> cache_threshold,
                                                   std::shared_ptr<level> release_threshold,
                                                   std::size_t chunk_size,
                                                   std::size_t max_chunks)
  : writeable_filter(name, wrt),
    event_cache_provider(chunk_size, chunk_size * max_chunks),
    release_threshold_(release_threshold),
    cache_threshold_(cache_threshold)
{
}

cache_and_release_filter::cache_and_release_filter(const std::string& name,
                                                   std::shared_ptr<level> cache_threshold,
                                                   std::shared_ptr<level> release_threshold,
                                                   std::size_t chunk_size,
                                                   std::size_t max_chunks)
    : writeable_filter(name),
      event_cache_provider(chunk_size, chunk_size * max_chunks),
      release_threshold_(release_threshold),
      cache_threshold_(cache_threshold)
{
}

filter::result cache_and_release_filter::evaluate(const event& evt)
{
    result rs = result::NEUTRAL;
    if (*evt.get_level() >= *release_threshold_)
    {
        if (!has_writer())
            throw std::runtime_error("No writer has been set in the cache_filter '" + get_name() + "'");
        auto e = cache_->pop(1ms);
        while (e)
        {
            write(*e);
            e = cache_->pop(1ms);
        }
        rs = result::ACCEPT;
    }
    else if (*evt.get_level() <= *cache_threshold_)
    {
        cache_->push(evt);
        rs = result::DENY;
    }
    return rs;
}

}
