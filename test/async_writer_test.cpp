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

#include <gtest/gtest.h>
#include <chucho/async_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chrono>
#include <vector>

namespace
{

class slow_writer : public chucho::writer
{
public:
    slow_writer(std::chrono::milliseconds delay);

    const std::vector<std::string>& get_events() const;

protected:
    virtual void write_impl(const chucho::event& evt) override;

private:
    std::vector<std::string> events_;
    std::chrono::milliseconds delay_;
};

class async_writer_test : public ::testing::Test
{
public:
    chucho::event get_event(const std::string& msg, std::shared_ptr<chucho::level> lvl = chucho::level::INFO_());
    std::unique_ptr<chucho::async_writer> get_writer(std::chrono::milliseconds millis, bool flush = true);
};

slow_writer::slow_writer(std::chrono::milliseconds delay)
    : chucho::writer("slow", std::make_unique<chucho::pattern_formatter>("%m")),
      delay_(delay)
{
}

const std::vector<std::string>& slow_writer::get_events() const
{
    return events_;
}

void slow_writer::write_impl(const chucho::event& evt)
{
    std::this_thread::sleep_for(delay_);
    events_.push_back(formatter_->format(evt));
}

chucho::event async_writer_test::get_event(const std::string& msg, std::shared_ptr<chucho::level> lvl)
{
    return chucho::event(chucho::logger::get("will"),
                         lvl,
                         msg,
                         __FILE__,
                         __LINE__,
                         __FUNCTION__);
}

std::unique_ptr<chucho::async_writer> async_writer_test::get_writer(std::chrono::milliseconds millis, bool flush)
{
    auto wrt = std::make_unique<slow_writer>(millis);
    auto as = std::make_unique<chucho::async_writer>("async", std::move(wrt), flush);
    return std::move(as);
}

}

using namespace std::chrono_literals;

TEST_F(async_writer_test, flush_on_destruct)
{
    auto as = get_writer(1ms);
    auto stats = as->get_cache_stats();
    EXPECT_EQ(chucho::async_writer::DEFAULT_CHUNK_SIZE, stats.get_chunk_size());
    EXPECT_EQ(chucho::async_writer::DEFAULT_MAX_CHUNKS, stats.get_max_size() / stats.get_chunk_size());
    EXPECT_EQ(0, stats.get_current_size());
    for (int i = 0; i < 1000; i++)
        as->write(get_event(std::to_string(i)));
    // we don't know how many are there
    stats = as->get_cache_stats();
    EXPECT_GT(stats.get_current_size(), 0U);
    auto now = std::chrono::steady_clock::now();
    as.reset();
    auto then = std::chrono::steady_clock::now();
    auto elap = std::chrono::duration_cast<std::chrono::milliseconds>(then - now);
    EXPECT_GT(elap.count(), 500);
    as = get_writer(1ms, false);
    stats = as->get_cache_stats();
    EXPECT_EQ(chucho::async_writer::DEFAULT_CHUNK_SIZE, stats.get_chunk_size());
    EXPECT_EQ(chucho::async_writer::DEFAULT_MAX_CHUNKS, stats.get_max_size() / stats.get_chunk_size());
    EXPECT_EQ(0, stats.get_current_size());
    for (int i = 0; i < 1000; i++)
        as->write(get_event(std::to_string(i)));
    // we don't know how many are there
    stats = as->get_cache_stats();
    EXPECT_GT(stats.get_current_size(), 0U);
    now = std::chrono::steady_clock::now();
    as.reset();
    then = std::chrono::steady_clock::now();
    elap = std::chrono::duration_cast<std::chrono::milliseconds>(then - now);
    EXPECT_LT(elap.count(), 50);
}

TEST_F(async_writer_test, slow)
{
    auto as = get_writer(50ms);
    auto stats = as->get_cache_stats();
    EXPECT_EQ(chucho::async_writer::DEFAULT_CHUNK_SIZE, stats.get_chunk_size());
    EXPECT_EQ(chucho::async_writer::DEFAULT_MAX_CHUNKS, stats.get_max_size() / stats.get_chunk_size());
    EXPECT_EQ(0, stats.get_current_size());
    for (int i = 0; i < 10; i++)
        as->write(get_event(std::to_string(i)));
    // we don't know how many are there
    stats = as->get_cache_stats();
    EXPECT_GT(stats.get_current_size(), 0U);
    std::this_thread::sleep_for(2s);
    stats = as->get_cache_stats();
    EXPECT_EQ(0, stats.get_current_size());
    auto& slow = dynamic_cast<slow_writer&>(as->get_writer());
    ASSERT_EQ(10, slow.get_events().size());
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(i, std::stoi(slow.get_events()[i]));
}
