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
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/exception.hpp>
#include <chucho/status_manager.hpp>
#include <chucho/marker.hpp>
#include <chucho/diagnostic_context.hpp>
#include <chucho/calendar.hpp>
#include <chucho/host.hpp>
#include <chucho/line_ending.hpp>
#include <sstream>
#include <thread>
#if defined(CHUCHO_WINDOWS)
#include <process.h>
#endif

namespace
{

#if defined(CHUCHO_WINDOWS)
const char* file_name = "\\one\\two";
#else
const char* file_name = "/one/two";
#endif

}

class pattern_formatter_test : public ::testing::Test
{
public:
    pattern_formatter_test()
        : evt_(chucho::logger::get("pattern logger"),
               chucho::level::INFO_(),
               "hi",
               file_name,
               10,
               "dowdy",
               "chucho")
    {
    }

protected:
    chucho::event evt_;
};

TEST_F(pattern_formatter_test, base_file)
{
    chucho::pattern_formatter f("%b");
    EXPECT_STREQ("two", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, base_host)
{
    chucho::pattern_formatter f("%h");
    EXPECT_EQ(chucho::host::get_base_name(), f.format(evt_));
}

TEST_F(pattern_formatter_test, end_of_line)
{
    chucho::pattern_formatter f("%n");
    EXPECT_STREQ(chucho::line_ending::EOL, f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, full_file)
{
    chucho::pattern_formatter f("%F");
    EXPECT_STREQ(file_name, f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, full_host)
{
    chucho::pattern_formatter f("%H");
    EXPECT_EQ(chucho::host::get_full_name(), f.format(evt_));
}

TEST_F(pattern_formatter_test, function)
{
    chucho::pattern_formatter f("%M");
    EXPECT_STREQ("dowdy", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, level)
{
    chucho::pattern_formatter f("%p");
    EXPECT_STREQ("INFO", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, line_number)
{
    chucho::pattern_formatter f("%L");
    EXPECT_STREQ("10", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, local_date_time)
{
    chucho::pattern_formatter f("%D");
    std::time_t secs = std::chrono::duration_cast<std::chrono::seconds>(evt_.get_time().time_since_epoch()).count();
    chucho::calendar::pieces cal = *std::localtime(&secs);
    cal.is_utc = false;
    EXPECT_EQ(chucho::calendar::format(cal, "%Y-%m-%d %H:%M:%S"), f.format(evt_));
    chucho::pattern_formatter f2("%D{%Y}");
    EXPECT_EQ(chucho::calendar::format(cal, "%Y"), f2.format(evt_));
}

TEST_F(pattern_formatter_test, logger)
{
    chucho::pattern_formatter f("%c");
    EXPECT_STREQ("pattern logger", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, message)
{
    chucho::pattern_formatter f("%m");
    EXPECT_STREQ("hi", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, microseconds)
{
    chucho::pattern_formatter f("%d{%Q}");
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(evt_.get_time().time_since_epoch()).count() % 1000000;
    auto fmt = f.format(evt_);
    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(6) << micros;
    EXPECT_EQ(stream.str(), fmt);
    chucho::pattern_formatter f2("%d{-%Q%%Qdoggy%%Q%%%Q}");
    fmt = f2.format(evt_);
    stream.str("");
    stream << std::setfill('0') << '-' << std::setw(6) << micros << "%Qdoggy%Q%" << std::setw(6) << micros;
    EXPECT_EQ(stream.str(), fmt);
}

TEST_F(pattern_formatter_test, milliseconds)
{
    chucho::pattern_formatter f("%d{%q}");
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(evt_.get_time().time_since_epoch());
    auto fmt = f.format(evt_);
    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(3) << (millis.count() % 1000);
    EXPECT_EQ(stream.str(), fmt);
}

TEST_F(pattern_formatter_test, pid)
{
    #if defined(CHUCHO_WINDOWS)
    int p = _getpid();
    #else
    pid_t p = getpid();
    #endif
    chucho::pattern_formatter f("%i");
    EXPECT_EQ(std::to_string(p), f.format(evt_));
}

TEST_F(pattern_formatter_test, thread)
{
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    chucho::pattern_formatter f("%t");
    EXPECT_EQ(stream.str(), f.format(evt_));
}

TEST_F(pattern_formatter_test, utc_date_time)
{
    chucho::pattern_formatter f("%d");
    std::time_t secs = std::chrono::duration_cast<std::chrono::seconds>(evt_.get_time().time_since_epoch()).count();
    chucho::calendar::pieces cal = *std::gmtime(&secs);
    cal.is_utc = true;
    EXPECT_EQ(chucho::calendar::format(cal, "%Y-%m-%d %H:%M:%S"), f.format(evt_));
    chucho::pattern_formatter f2("%d{%Y}");
    EXPECT_EQ(chucho::calendar::format(cal, "%Y"), f2.format(evt_));
}

TEST_F(pattern_formatter_test, with_literal)
{
    auto f = std::make_unique<chucho::pattern_formatter>(" %m");
    EXPECT_STREQ(" hi", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%m ");
    EXPECT_STREQ("hi ", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>(" %m ");
    EXPECT_STREQ(" hi ", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%m %L");
    EXPECT_STREQ("hi 10", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>(" %m %L");
    EXPECT_STREQ(" hi 10", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%m %L ");
    EXPECT_STREQ("hi 10 ", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>(" %m %L ");
    EXPECT_STREQ(" hi 10 ", f->format(evt_).c_str());
}

TEST_F(pattern_formatter_test, format_params)
{
    auto f = std::make_unique<chucho::pattern_formatter>("%5m");
    EXPECT_STREQ("   hi", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%-17m");
    EXPECT_STREQ("hi               ", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%.1m");
    EXPECT_STREQ("i", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%5.1m");
    EXPECT_STREQ("i", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%1.1m");
    EXPECT_STREQ("i", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%1.10m");
    EXPECT_STREQ("hi", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%-5.10m");
    EXPECT_STREQ("hi   ", f->format(evt_).c_str());
}

TEST_F(pattern_formatter_test, invalid)
{
    auto& smgr = chucho::status_manager::get();
    smgr.clear();
    auto f = std::make_unique<chucho::pattern_formatter>("%z");
    EXPECT_EQ(1, smgr.get_count());
    EXPECT_EQ(chucho::status::level::ERROR_, smgr.get_level());
    EXPECT_STREQ("%z", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%1.m");
    EXPECT_EQ(2, smgr.get_count());
    EXPECT_EQ(chucho::status::level::ERROR_, smgr.get_level());
    EXPECT_STREQ("%1.m", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("");
    EXPECT_EQ(3, smgr.get_count());
    EXPECT_EQ(chucho::status::level::ERROR_, smgr.get_level());
    EXPECT_STREQ("", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%d{%Ym");
    EXPECT_EQ(5, smgr.get_count());
    EXPECT_EQ(chucho::status::level::ERROR_, smgr.get_level());
    f = std::make_unique<chucho::pattern_formatter>("%C");
    EXPECT_EQ(6, smgr.get_count());
    EXPECT_EQ(chucho::status::level::ERROR_, smgr.get_level());
}

TEST_F(pattern_formatter_test, marker)
{
    chucho::pattern_formatter f("%k");
    EXPECT_STREQ("chucho", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, diagnostic_context)
{
    auto f = std::make_unique<chucho::pattern_formatter>("%C{name}");
    EXPECT_STREQ("", f->format(evt_).c_str());
    f = std::make_unique<chucho::pattern_formatter>("%C{name}");
    chucho::diagnostic_context::at("name") = "funky";
    EXPECT_STREQ("funky", f->format(evt_).c_str());
}

TEST_F(pattern_formatter_test, regex_replace)
{
    auto f = std::make_unique<chucho::pattern_formatter>("%R{\"%m %M\", \"i|d\", \"\"}");
    EXPECT_STREQ("h owy", f->format(evt_).c_str());
}
