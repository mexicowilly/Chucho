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

#if !defined(CHUCHO_EVENT_CACHE_STATS_HPP__)
#define CHUCHO_EVENT_CACHE_STATS_HPP__

#include <chucho/export.h>
#include <cstdint>
#include <functional>
#include <chrono>

namespace chucho
{

class CHUCHO_EXPORT event_cache_stats
{
public:
    enum class progress_direction
    {
        UP,
        DOWN
    };

    using progress_callback = std::function<void(const event_cache_stats&,
                                                 progress_direction,
                                                 double,
                                                 std::size_t)>;

    std::size_t get_average_event_size() const;
    std::size_t get_bytes_culled() const;
    std::size_t get_chunk_size() const;
    std::size_t get_current_size() const;
    std::size_t get_events_read() const;
    std::size_t get_events_written() const;
    std::size_t get_files_created() const;
    std::size_t get_files_destroyed() const;
    std::size_t get_largest_event_size() const;
    std::size_t get_largest_size() const;
    std::size_t get_max_size() const;
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
