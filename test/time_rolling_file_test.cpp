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
#include <chucho/rolling_file_writer.hpp>
#include <chucho/time_file_roller.hpp>
#include <chucho/logger.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/file.hpp>
#include <chucho/calendar.hpp>
#include <chucho/gzip_file_compressor.hpp>
#include <chucho/zip_file_compressor.hpp>
#include <chucho/bzip2_file_compressor.hpp>
#include <chucho/noop_file_compressor.hpp>
#include <chucho/configuration.hpp>
#include <chrono>
#include <vector>
#include <thread>
#include <iostream>
#include <queue>

namespace
{

class test
{
public:
    template <typename duration_type>
    test(const std::string& pattern,
         std::size_t max_history,
         const duration_type& dur);
    template <typename duration_type>
    test(const std::string& active,
         const std::string& pattern,
         std::size_t max_history,
         const duration_type& dur);
    virtual ~test();

    bool operator< (const test& tst) const;

    void check();
    const std::chrono::system_clock::time_point& end() const;
    void sleep();

protected:
    static std::string TOP_LEVEL_DIR;

    virtual void check_impl() = 0;
    std::deque<std::string>& expected_file_names();
    std::string format_file_name(const std::string& pattern);
    bool is_compressing() const;
    std::chrono::system_clock::time_point& next();
    std::vector<std::string>& unexpected_file_names();
    void write();

private:
    chucho::rolling_file_writer writer_;
    std::chrono::system_clock::time_point next_;
    std::deque<std::string> expected_file_names_;
    std::vector<std::string> unexpected_file_names_;
    std::chrono::system_clock::time_point end_;
};

class date_only_test : public test
{
public:
    template <typename duration_type, typename next_type>
    date_only_test(const std::string& pattern,
                   std::size_t max_history,
                   const duration_type& dur,
                   const next_type& nxt);

protected:
    virtual void check_impl() override;

private:
    std::chrono::seconds until_next_;
    std::string pattern_;
    std::size_t max_history_;
};

class fixed_active_test : public test
{
public:
    template <typename duration_type, typename next_type>
    fixed_active_test(const std::string& active,
                      const std::string& pattern,
                      std::size_t max_history,
                      const duration_type& dur,
                      const next_type& nxt);

protected:
    virtual void check_impl() override;

private:
    std::chrono::seconds until_next_;
    std::string active_;
    std::string pattern_;
    std::size_t max_history_;
};

class minute_test : public date_only_test
{
public:
    minute_test();
};

class active_minute_test : public fixed_active_test
{
public:
    active_minute_test();
};

class hour_test : public date_only_test
{
public:
    hour_test();
};

class active_hour_test : public fixed_active_test
{
public:
    active_hour_test();
};

std::string test::TOP_LEVEL_DIR("time_rolling_file_test/");
bool no_compression = false;

std::shared_ptr<chucho::file_compressor> create_compressor(int min_index)
{
    std::shared_ptr<chucho::file_compressor> result;
    if (no_compression)
    {
        result = std::make_shared<chucho::noop_file_compressor>();
    }
    else
    {
#if defined(CHUCHO_HAVE_ZLIB)
        result = std::make_shared<chucho::gzip_file_compressor>(min_index);
#elif defined(CHUCHO_HAVE_BZIP2)
        result = std::make_shared<chucho::bzip2_file_compressor>(min_index);
#elif defined(CHUCHO_HAVE_MINIZIP)
        result = std::make_shared<chucho::zip_file_compressor>(min_index);
#else
        result = std::make_shared<chucho::noop_file_compressor>();
#endif
    }
    return result;
}

template <typename duration_type>
test::test(const std::string& pattern,
           std::size_t max_history,
           const duration_type& dur)
    : writer_(std::shared_ptr<chucho::formatter>(new chucho::pattern_formatter("%m%n")),
              std::shared_ptr<chucho::file_roller>(new chucho::time_file_roller(TOP_LEVEL_DIR + pattern,
                                                                                max_history,
                                                                                create_compressor(max_history)))),
      next_(std::chrono::system_clock::now()),
      end_(std::chrono::system_clock::now() + dur)
{
    write();
}

template <typename duration_type>
test::test(const std::string& active,
           const std::string& pattern,
           std::size_t max_history,
           const duration_type& dur)
    : writer_(std::shared_ptr<chucho::formatter>(new chucho::pattern_formatter("%m%n")),
              TOP_LEVEL_DIR + active,
              std::shared_ptr<chucho::file_roller>(new chucho::time_file_roller(TOP_LEVEL_DIR + pattern,
                                                                                max_history,
                                                                                create_compressor(max_history)))),
      next_(std::chrono::system_clock::now()),
      end_(std::chrono::system_clock::now() + dur)
{
    write();
}

test::~test()
{
}

bool test::operator< (const test& tst) const
{
    return next_ < tst.next_;
}

void test::check()
{
    write();
    check_impl();
}

const std::chrono::system_clock::time_point& test::end() const
{
    return end_;
}

std::deque<std::string>& test::expected_file_names()
{
    return expected_file_names_;
}

std::string test::format_file_name(const std::string& pattern)
{
    chucho::calendar::pieces cal = chucho::calendar::get_utc(std::time(nullptr));
    std::ostringstream stream;
    stream << TOP_LEVEL_DIR << chucho::calendar::format(cal, pattern);
    return stream.str();
}

bool test::is_compressing() const
{
    return !std::dynamic_pointer_cast<chucho::noop_file_compressor>(writer_.get_file_roller()->get_file_compressor());
}

std::chrono::system_clock::time_point& test::next()
{
    return next_;
}

void test::sleep()
{
    std::this_thread::sleep_until(next_);
}

std::vector<std::string>& test::unexpected_file_names()
{
    return unexpected_file_names_;
}

void test::write()
{
    chucho::event evt(chucho::logger::get("time_rolling_file_test"),
                      chucho::level::INFO_(),
                      "hello",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    writer_.write(evt);
}

bool file_doesnt_exist(const std::string& file_name)
{
    return !chucho::file::exists(file_name);
}

template <typename duration_type, typename next_type>
date_only_test::date_only_test(const std::string& pattern,
                               std::size_t max_history,
                               const duration_type& dur,
                               const next_type& nxt)
    : test(pattern, max_history, dur),
      until_next_(std::chrono::duration_cast<std::chrono::seconds>(nxt)),
      pattern_(pattern),
      max_history_(max_history)
{
    std::size_t pos = pattern_.find("%d{");
    if (pos != std::string::npos)
        pattern_.erase(pos, 3);
    pos = pattern_.find("}");
    if (pos != std::string::npos)
        pattern_.erase(pos, 1);
    expected_file_names().push_back(format_file_name(pattern_));
    next() += until_next_;
}

void date_only_test::check_impl()
{
    if (!is_compressing())
    {
        expected_file_names().push_back(format_file_name(pattern_));
        if (expected_file_names().size() > max_history_ + 1)
        {
            unexpected_file_names().push_back(expected_file_names().front());
            expected_file_names().pop_front();
        }
        for (auto f : expected_file_names())
            EXPECT_PRED1(chucho::file::exists, f);
        for (auto f : unexpected_file_names())
            EXPECT_PRED1(file_doesnt_exist, f);
    }
    next() += until_next_;
}

template <typename duration_type, typename next_type>
fixed_active_test::fixed_active_test(const std::string& active,
                                     const std::string& pattern,
                                     std::size_t max_history,
                                     const duration_type& dur,
                                     const next_type& nxt)
    : test(active, pattern, max_history, dur),
      until_next_(std::chrono::duration_cast<std::chrono::seconds>(nxt)),
      active_(active),
      pattern_(pattern),
      max_history_(max_history)
{
    std::size_t pos = pattern_.find("%d{");
    if (pos != std::string::npos)
        pattern_.erase(pos, 3);
    pos = pattern_.find("}");
    if (pos != std::string::npos)
        pattern_.erase(pos, 1);
    next() += until_next_;
}

void fixed_active_test::check_impl()
{
    if (!is_compressing())
    {
        expected_file_names().push_back(format_file_name(pattern_));
        if (expected_file_names().size() > max_history_)
        {
            unexpected_file_names().push_back(expected_file_names().front());
            expected_file_names().pop_front();
        }
        EXPECT_PRED1(chucho::file::exists, TOP_LEVEL_DIR + active_);
        for (auto f : expected_file_names())
            EXPECT_PRED1(chucho::file::exists, f);
        for (auto f : unexpected_file_names())
            EXPECT_PRED1(file_doesnt_exist, f);
    }
    next() += until_next_;
}

minute_test::minute_test()
    : date_only_test("minute/test-%d{%H-%M}",
                     2,
                     std::chrono::minutes(5),
                     std::chrono::seconds(61))
{
}

active_minute_test::active_minute_test()
    : fixed_active_test("active-minute/active",
                        "active-minute/test-%d{%H-%M}",
                        2,
                        std::chrono::minutes(5),
                        std::chrono::seconds(62))
{
}

hour_test::hour_test()
    : date_only_test("hour/test-%d{%d-%H}",
                     2,
                     std::chrono::hours(5),
                     std::chrono::minutes(61))
{
}

active_hour_test::active_hour_test()
    : fixed_active_test("active-hour/active",
                        "active-hour/test-%d{%d-%H}",
                        2,
                        std::chrono::hours(5),
                        std::chrono::minutes(62))
{
}

void run(std::shared_ptr<test> tst)
{
    while (std::chrono::system_clock::now() < tst->end())
    {
        tst->sleep();
        tst->check();
    }
}

}

TEST(time_rolling_file_test, all)
{
    chucho::configuration::set_style(chucho::configuration::style::OFF);
    chucho::file::remove_all("time_rolling_file_test");
    // No initializer lists in VS 2012
    std::vector<std::shared_ptr<std::thread>> threads;
    threads.push_back(std::make_shared<std::thread>(run, std::make_shared<minute_test>()));
    threads.push_back(std::make_shared<std::thread>(run, std::make_shared<active_minute_test>()));
    threads.push_back(std::make_shared<std::thread>(run, std::make_shared<hour_test>()));
    threads.push_back(std::make_shared<std::thread>(run, std::make_shared<active_hour_test>()));
    for (auto thrd : threads)
        thrd->join();
}
