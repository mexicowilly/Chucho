/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_EVENT_CACHE_STATS_HPP_)
#define CHUCHO_EVENT_CACHE_STATS_HPP_

#include <chucho/export.h>
#include <cstdint>
#include <functional>
#include <chrono>

namespace chucho
{

/**
 * @class event_cache_stats event_cache_stats.hpp chucho/event_cache_stats.hpp
 * Information about event caches. An event cache is a disk-backed queue
 * of log events, which is used by @ref event_cache_provider. The cache will
 * report information about its state via the stats.
 *
 * When the cache is full, a chunk of the oldest events is deleted. This
 * can be monitored using a progress callback. The callback is invoked when
 * the cache is 80%, 90% and 95% full.
 *
 * @sa event_cache_provider
 * @ingroup miscellaneous
 */
class CHUCHO_EXPORT event_cache_stats
{
public:
    /**
     * Direction that the size of the cache is moving when progress
     * is reported.
     */
    enum class progress_direction
    {
        UP,     /**< The cache is growing. */
        DOWN    /**< The cache is shrinking. */
    };

    /**
     * Function used for progress callbacks. If a callback has been set,
     * it will be called when the cache is at 80%, 90% and 95% capacity.
     * Additionally, it will be called after bytes have been culled,
     * which occurs when the cache is full.
     *
     * @param stats the stats at the time
     * @param dir whether the cache is growing or shrinking
     * @param full how full the cache is in the range [0, 1]
     * @param bytes_culled the number of bytes that were culled immediately
     *        before the callback was invoked
     */
    using progress_callback = std::function<void(const event_cache_stats& stats,
                                                 progress_direction dir,
                                                 double full,
                                                 std::size_t bytes_culled)>;

    /**
     * Return the average serialized size of events.
     * @return the average
     */
    std::size_t get_average_event_size() const;
    /**
     * Return the number of bytes deleted after the cache has
     * become full.
     * @return the culled bytes
     */
    std::size_t get_bytes_culled() const;
    /**
     * Return the size in bytes of a chunk. The event cache deals
     * with discrete chunks. One chunk is always held in memory and
     * other chunks are stored on disk.
     * @return the chunk size
     */
    std::size_t get_chunk_size() const;
    /**
     * Return the size in bytes of the cache.
     * @return the size
     */
    std::size_t get_current_size() const;
    /**
     * Return the number of times that events have been culled due
     * to the cache being full.
     * @return the number of culls
     */
    std::size_t get_cull_events() const;
    /**
     * Return the number of events that have been read from the cache.
     * @return the number of read events
     */
    std::size_t get_events_read() const;
    /**
     * Return the number of events that have been written to the cache.
     * @return the number of written events
     */
    std::size_t get_events_written() const;
    /**
     * Return the number of files created.
     * @return the number of created files
     */
    std::size_t get_files_created() const;
    /**
     * Return the number of files destroyed.
     * @return the number of destroyed files
     */
    std::size_t get_files_destroyed() const;
    /**
     * Return the size of the largest serialized event.
     * @return the largest event size
     */
    std::size_t get_largest_event_size() const;
    /**
     * Return the largest size that this cache has reached.
     * @return the largest that this cache has ever been
     */
    std::size_t get_largest_size() const;
    /**
     * Return the maximum size.
     * @return the maximum
     */
    std::size_t get_max_size() const;
    /**
     * Return the size of the smallest serialized event.
     * @return the smallest event size
     */
    std::size_t get_smallest_event_size() const;

private:
    friend class event_cache;

    event_cache_stats(std::size_t chunk_size, std::size_t max_size);

    std::size_t chunk_size_{0};
    std::size_t max_size_{0};
    std::size_t current_size_{0};
    std::size_t largest_size_{0};
    std::size_t files_created_{0};
    std::size_t files_destroyed_{0};
    std::size_t bytes_culled_{0};
    std::size_t events_read_{0};
    std::size_t events_written_{0};
    std::size_t smallest_event_size_{std::numeric_limits<std::size_t>::max()};
    std::size_t largest_event_size_{0};
    std::size_t average_event_size_{0};
    std::size_t cull_events_{0};
};

inline event_cache_stats::event_cache_stats(std::size_t chunk_size, std::size_t max_size)
    : chunk_size_(chunk_size),
      max_size_(max_size)
{
}

inline std::size_t event_cache_stats::get_average_event_size() const
{
    return average_event_size_;
}

inline std::size_t event_cache_stats::get_bytes_culled() const
{
    return bytes_culled_;
}

inline std::size_t event_cache_stats::get_chunk_size() const
{
    return chunk_size_;
}

inline std::size_t event_cache_stats::get_cull_events() const
{
    return cull_events_;
}

inline std::size_t event_cache_stats::get_current_size() const
{
    return current_size_;
}

inline std::size_t event_cache_stats::get_events_read() const
{
    return events_read_;
}

inline std::size_t event_cache_stats::get_events_written() const
{
    return events_written_;
}

inline std::size_t event_cache_stats::get_files_created() const
{
    return files_created_;
}

inline std::size_t event_cache_stats::get_files_destroyed() const
{
    return files_destroyed_;
}

inline std::size_t event_cache_stats::get_largest_size() const
{
    return largest_size_;
}

inline std::size_t event_cache_stats::get_largest_event_size() const
{
    return largest_event_size_;
}

inline std::size_t event_cache_stats::get_max_size() const
{
    return max_size_;
}

inline std::size_t event_cache_stats::get_smallest_event_size() const
{
    return smallest_event_size_;
}

}

#endif
