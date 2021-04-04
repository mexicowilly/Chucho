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

#include <chucho/event_cache_provider.hpp>
#include <chucho/event_cache.hpp>

namespace chucho
{

constexpr std::size_t event_cache_provider::DEFAULT_CHUNK_SIZE;
constexpr std::size_t event_cache_provider::DEFAULT_MAX_CHUNKS;

event_cache_provider::event_cache_provider(std::size_t chunk_size, std::size_t max_size)
    : cache_(std::make_unique<event_cache>(chunk_size, max_size))
{
}

event_cache_provider::~event_cache_provider()
{
}

event_cache_stats event_cache_provider::get_cache_stats()
{
    return cache_->get_stats();
}

void event_cache_provider::set_cache_progress_callback(event_cache_stats::progress_callback cb)
{
    cache_->set_progress_callback(cb);
}

}
