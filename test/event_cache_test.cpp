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

#include <gtest/gtest.h>
#include <chucho/event_cache.hpp>
#include <chucho/logger.hpp>
#include <chucho/function_name.hpp>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

namespace
{

void full_speed_main(chucho::event_cache& cache, std::size_t count, std::chrono::milliseconds delay)
{
    for (std::size_t i = 0; i < count; i++)
    {
        chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), std::to_string(i), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME);
        cache.push(e);
        std::this_thread::sleep_for(delay);
    }
}

std::ostream& operator<< (std::ostream& stream, const chucho::event_cache_stats& stats)
{
    stream <<
        "  Chunk size: " << stats.get_chunk_size() << '\n' <<
        "  Max size: " << stats.get_max_size() << '\n' <<
        "  Current size: " << stats.get_current_size() << '\n' <<
        "  Largest size: " << stats.get_largest_size() << '\n' <<
        "  Files created: " << stats.get_files_created() << '\n' <<
        "  Files destroyed: " << stats.get_files_destroyed() << '\n' <<
        "  Bytes culled: " << stats.get_bytes_culled() << '\n' <<
        "  Cull events: " << stats.get_cull_events() << '\n' <<
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
    chucho::event_cache cache(1024 * 1024, 2 * 1024 * 1024);
    cache.set_progress_callback([&culled_bytes] (const chucho::event_cache_stats& st, chucho::event_cache_stats::progress_direction , double, std::size_t cnt) { culled_bytes += cnt; });
    std::thread thr(full_speed_main, std::ref(cache), 1000000, 0ms);
    chucho::optional<chucho::event> evt;
    std::this_thread::sleep_for(1s);
    do
    {
        evt = cache.pop(250ms);
        ++popped;
    } while (evt);
    thr.join();
    EXPECT_GT(culled_bytes, 0U);
    --popped;
    EXPECT_LT(popped, 1000000U);
    auto stats = cache.get_stats();
    EXPECT_EQ(culled_bytes, stats.get_bytes_culled());
    std::cout << cache.get_stats() << std::endl;
}

TEST(event_cache, full_speed)
{
    chucho::event_cache cache(1024 * 1024, 100 * 1024 * 1024);
    std::thread thr(full_speed_main, std::ref(cache), 1000000, 5ms);
    for (std::size_t i = 0; i < 1000000; i++)
    {
        auto evt = cache.pop(5000ms);
        ASSERT_TRUE(evt);
        if (std::to_string(i) != evt->get_message())
        {
            std::cout << cache.get_stats() << std::endl;
            FAIL();
        }
    }
    thr.join();
    std::cout << cache.get_stats() << std::endl;
}

TEST(event_cache, progress)
{
    chucho::event_cache cache(1024 * 1024, 2 * 1024 * 1024);
    std::vector<std::tuple<chucho::event_cache_stats, chucho::event_cache_stats::progress_direction, double, std::size_t>> prg;
    cache.set_progress_callback([&prg] (const chucho::event_cache_stats& s,
                                        chucho::event_cache_stats::progress_direction d,
                                        double f,
                                        std::size_t c)
                                {
                                    prg.emplace_back(s, d, f, c);
                                });
    for (int i = 0; i < 1000000; i++)
    {
        chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), std::to_string(i), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME);
        cache.push(e);
        auto st = cache.get_stats();
        if (st.get_current_size() > st.get_max_size() - st.get_average_event_size())
            break;
    }
    ASSERT_EQ(3, prg.size());
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::UP, std::get<1>(prg[0]));
    EXPECT_GT(std::get<2>(prg[0]), .8);
    EXPECT_EQ(0, std::get<3>(prg[0]));
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::UP, std::get<1>(prg[1]));
    EXPECT_GT(std::get<2>(prg[1]), .9);
    EXPECT_EQ(0, std::get<3>(prg[1]));
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::UP, std::get<1>(prg[2]));
    EXPECT_GT(std::get<2>(prg[2]), .95);
    EXPECT_EQ(0, std::get<3>(prg[2]));
    prg.clear();
    chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), "full", __FILE__, __LINE__, CHUCHO_FUNCTION_NAME);
    cache.push(e);
    ASSERT_EQ(1, prg.size());
    EXPECT_LT(std::get<2>(prg[0]), .8);
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::DOWN, std::get<1>(prg[0]));
    EXPECT_GT(std::get<2>(prg[0]), 0);
    prg.clear();
    for (int i = 0; i < 1000000; i++)
    {
        chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), std::to_string(i), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME);
        cache.push(e);
        auto st = cache.get_stats();
        if (st.get_current_size() > st.get_max_size() - st.get_average_event_size())
            break;
    }
    ASSERT_EQ(3, prg.size());
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::UP, std::get<1>(prg[0]));
    EXPECT_GT(std::get<2>(prg[0]), .8);
    EXPECT_EQ(0, std::get<3>(prg[0]));
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::UP, std::get<1>(prg[1]));
    EXPECT_GT(std::get<2>(prg[1]), .9);
    EXPECT_EQ(0, std::get<3>(prg[1]));
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::UP, std::get<1>(prg[2]));
    EXPECT_GT(std::get<2>(prg[2]), .95);
    EXPECT_EQ(0, std::get<3>(prg[2]));
    prg.clear();
    chucho::optional<chucho::event> oe;
    do
    {
        oe = cache.pop(200ms);
    } while (oe);
    ASSERT_EQ(3, prg.size());
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::DOWN, std::get<1>(prg[0]));
    EXPECT_LT(std::get<2>(prg[0]), .95);
    EXPECT_EQ(0, std::get<3>(prg[0]));
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::DOWN, std::get<1>(prg[1]));
    EXPECT_LT(std::get<2>(prg[1]), .9);
    EXPECT_EQ(0, std::get<3>(prg[1]));
    EXPECT_EQ(chucho::event_cache_stats::progress_direction::DOWN, std::get<1>(prg[2]));
    EXPECT_LT(std::get<2>(prg[2]), .8);
    EXPECT_EQ(0, std::get<3>(prg[2]));
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

TEST(event_cache, slow_write)
{
    chucho::event_cache cache(1024 * 1024, 100 * 1024 * 1024);
    std::thread thr(full_speed_main, std::ref(cache), 1000, 1ms);
    for (std::size_t i = 0; i < 1000; i++)
    {
        auto evt = cache.pop(250ms);
        ASSERT_TRUE(evt);
        EXPECT_EQ(std::to_string(i), evt->get_message());
    }
    thr.join();
    std::cout << cache.get_stats() << std::endl;
}

