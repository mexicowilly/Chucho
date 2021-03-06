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

#if !defined(CHUCHO_EVENT_CACHE_HPP_)
#define CHUCHO_EVENT_CACHE_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/export.h>
#include <chucho/non_copyable.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/event.hpp>
#include <chucho/event_cache_stats.hpp>
#include <string>
#include <memory>
#include <mutex>
#include <fstream>
#include <vector>
#include <cstring>
#include <condition_variable>

namespace chucho
{

class CHUCHO_PRIV_EXPORT event_cache : non_copyable, public status_reporter
{
public:
    event_cache(std::size_t chunk_size, std::size_t max_size);
    virtual ~event_cache();

    event_cache_stats get_stats();
    optional<event> pop(std::chrono::milliseconds to_wait);
    void push(const event& evt);
    void set_progress_callback(event_cache_stats::progress_callback cb);

private:
    // NOTE: guard_ must be locked on entry
    void cull();
    // NOTE: guard_ must be locked on entry
    std::string find_oldest_file();
    template <typename int_type>
    int_type get_mem_buf(std::size_t idx)
    {
        int_type result;
        std::memcpy(&result, read_pos_ + idx, sizeof(result));
        return result;
    }
    std::string get_mem_buf_str(std::size_t idx, std::size_t len);
    void report_progress(event_cache_stats::progress_direction dir);
    // I would prefer to use Flatbuffers, but I can't make Chucho
    // permanently depend on Flatbuffers.
    // The serialized format looks like:
    // 4 | Length of serialized event
    // 2 | Length of logger name
    // <len> | Logger name
    // 2 | Length of level name
    // <len> | Level name
    // 4 | Length of message
    // <len> | Message
    // 8 | Microseconds since epoch
    // 2 | Length of file name
    // <len> | File name
    // 4 | Line number
    // 2 | Length of function name
    // <len> | Function name
    // 2 | Length of marker
    // <len> | Marker
    // 2 | Length of thread ID
    // <len> | Thread ID
    //
    // NOTE: guard_ must be locked on entry
    std::size_t serialize(const event& evt);
    std::size_t serialized_size(const event& evt, std::string& mrk_text, std::string& thr_text);
    template <typename int_type>
    void set_ser_buf(std::size_t idx, int_type val)
    {
        std::memcpy(&ser_buf_[idx], &val, sizeof(val));
    }
    void stop();
    // NOTE: guard_ must be locked on entry
    event unserialize(std::size_t& sz);

    std::string directory_;
    std::mutex guard_;
    std::unique_ptr<std::uint8_t[]> mem_chunk_;
    std::uint8_t* read_pos_;
    std::uint8_t* write_pos_;
    std::unique_ptr<std::ofstream> write_file_;
    std::uint32_t current_sequence_;
    std::vector<std::uint8_t> ser_buf_;
    std::condition_variable read_cond_;
    std::size_t mem_chunk_occupied_;
    event_cache_stats::progress_callback progress_cb_;
    event_cache_stats stats_;
    std::size_t total_bytes_written_;
    double last_fullness_threshold_;
};

inline std::string event_cache::get_mem_buf_str(std::size_t idx, std::size_t len)
{
    return std::string(reinterpret_cast<char*>(read_pos_ + idx), len);
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
