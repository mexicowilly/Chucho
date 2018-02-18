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

#include <gtest/gtest.h>
#include <chucho/event_cache.hpp>
#include <chucho/logger.hpp>
#include <chucho/function_name.hpp>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

namespace
{

void full_speed_main(chucho::event_cache& cache, std::size_t count)
{
    for (std::size_t i = 0; i < count; i++)
    {
        chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), std::to_string(i), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME);
        cache.push(e);
    }
}

std::ostream& operator<< (std::ostream& stream, const chucho::event_cache_stats& stats)
{
    stream <<
        "  Chunk size: " << stats.get_chunk_size() << '\n' <<
        "  Max size: " << stats.get_max_size() << '\n' <<
        "  Total size: " << stats.get_total_size() << '\n' <<
        "  Largest size: " << stats.get_largest_size() << '\n' <<
        "  Files created: " << stats.get_files_created() << '\n' <<
        "  Files destroyed: " << stats.get_files_destroyed() << '\n' <<
        "  Bytes culled: " << stats.get_bytes_culled() << '\n' <<
        "  Events read: " << stats.get_events_read() << '\n' <<
        "  Events written: " << stats.get_events_written() << '\n' <<
        "  Smallest event: " << stats.get_smallest_event_size() << '\n' <<
        "  Largest event: " << stats.get_largest_event_size() << '\n' <<
        "  Average event size: " << stats.get_average_event_size();
    return stream;
}

}

TEST(event_cache, cull)
{
    std::size_t culled_bytes = 0;
    std::size_t popped = 0;
    chucho::event_cache cache(1024 * 1024, 2 * 1024 * 1024, [&culled_bytes] (const chucho::event_cache_stats& st, std::size_t cnt) { culled_bytes += cnt; });
    std::thread thr(full_speed_main, std::ref(cache), 1000000);
    chucho::optional<chucho::event> evt;
    std::this_thread::sleep_for(1s);
    do
    {
        evt = cache.pop(250ms);
        ++popped;
    } while (evt);
    thr.join();
    EXPECT_GT(culled_bytes, 0);
    --popped;
    EXPECT_LT(popped, 1000000);
    auto stats = cache.get_stats();
    EXPECT_EQ(culled_bytes, stats.get_bytes_culled());
    std::cout << cache.get_stats() << std::endl;
}

TEST(event_cache, full_speed)
{
    chucho::event_cache cache(1024 * 1024, 100 * 1024 * 1024);
    std::thread thr(full_speed_main, std::ref(cache), 1000000);
    for (std::size_t i = 0; i < 1000000; i++)
    {
        auto evt = cache.pop(250ms);
        ASSERT_TRUE(evt);
        EXPECT_EQ(std::to_string(i), evt->get_message());
    }
    thr.join();
    std::cout << cache.get_stats() << std::endl;
}

TEST(event_cache, serialization)
{
    chucho::event_cache cache(1024 * 1024, 10 * 1024 * 1024);
    chucho::event e1(chucho::logger::get("will"), chucho::level::INFO_(), "hi", __FILE__, __LINE__, CHUCHO_FUNCTION_NAME);
    cache.push(e1);
    auto e2 = cache.pop(250ms);
    ASSERT_TRUE(e2);
    EXPECT_STREQ(e1.get_logger()->get_name().c_str(), e2->get_logger()->get_name().c_str());
    EXPECT_EQ(e1.get_level(), e2->get_level());
    EXPECT_STREQ(e1.get_message().c_str(), e2->get_message().c_str());
    EXPECT_STREQ(e1.get_file_name(), e2->get_file_name());
    EXPECT_EQ(e1.get_line_number(), e2->get_line_number());
    EXPECT_STREQ(e1.get_function_name(), e2->get_function_name());
    // close enough (the cache only records microsecond precision)
    EXPECT_EQ(std::chrono::system_clock::to_time_t(e1.get_time()), std::chrono::system_clock::to_time_t(e2->get_time()));
    ASSERT_TRUE(e2->get_thread_id());
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    EXPECT_STREQ(stream.str().c_str(), e2->get_thread_id()->c_str());
}

