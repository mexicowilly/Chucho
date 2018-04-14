/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_CACHE_PROVIDER_HPP__)
#define CHUCHO_CACHE_PROVIDER_HPP__

#include <chucho/event_cache_stats.hpp>
#include <memory>

namespace chucho
{

class event_cache;

/**
 * @class event_cache_provider event_cache_provider.hpp chucho/event_cache_provider.hpp
 * A holder of an event cache. An event cache is a disk-backed queue of log events.
 * The cache stores its data in chunks. One chunk is held in memory, and the other  chunks
 * are stored on disk. Once the cache reaches its maximum size, the oldest chunk is
 * deleted.
 *
 * Cache data can be monitored by polling with @ref get_cache_stats, or via a callback
 * function. The callback is invoked when the cache is 80%, 90% and 95% full. Additionally,
 * it is called immediately after the cache has become full and the oldest chunk is
 * deleted.
 *
 * @ingroup miscellaneous
 */
class CHUCHO_EXPORT event_cache_provider
{
public:
    /**
     * Return the cache stats.
     *
     * @return the stats
     */
    event_cache_stats get_cache_stats();
    /**
     * Set the progress callback.
     * @param cb the callback
     */
    void set_cache_progress_callback(event_cache_stats::progress_callback cb);

protected:
    /**
     * @name Constructor and Destructor
     * @{
     */
    /**
     * Construct a provider.
     * @param chunk_size the size of one chunk
     * @param max_size the maximum size
     */
    event_cache_provider(std::size_t chunk_size, std::size_t max_size);
    /**
     * Destroy a provider.
     */
    virtual ~event_cache_provider();
    /**
     * @}
     */
    /**
     * The event cache.
     */
    std::unique_ptr<event_cache> cache_;
};

}

#endif
