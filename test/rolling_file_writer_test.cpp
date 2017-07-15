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
#include <chucho/numbered_file_roller.hpp>
#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/file.hpp>
#include <chucho/logger.hpp>
#include <chucho/calendar.hpp>
#include <chucho/time_file_roller.hpp>
#include <chucho/status_manager.hpp>
#include <chucho/gzip_file_compressor.hpp>
#include <chucho/sliding_numbered_file_roller.hpp>
#include <chucho/exception.hpp>
#include <array>
#include <algorithm>
#include <fstream>

class rolling_file_writer_test : public ::testing::Test
{
protected:
    rolling_file_writer_test()
        : logger_(chucho::logger::get("rolling_file_writer_test")),
          dir_name_("rolling_file_writer_test")
    {
        if (chucho::file::exists(dir_name_))
            chucho::file::remove_all(dir_name_);
        chucho::file::create_directory(dir_name_);
        chucho::status_manager::get()->clear();
    }

    ~rolling_file_writer_test()
    {
        try
        {
            chucho::file::remove_all(dir_name_);
        }
        catch (...)
        {
        }
    }

    chucho::event get_event(const std::string& msg)
    {
        return chucho::event(logger_, chucho::level::INFO_(), msg, __FILE__, __LINE__, __FUNCTION__);
    }

    std::string get_file_name(const std::string& base)
    {
        return dir_name_ + chucho::file::dir_sep + base;
    }

    std::string get_line(const std::string& file_name)
    {
        std::ifstream stream(file_name);
        std::string line;
        std::getline(stream, line);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        return line;
    }

    std::string get_time(const std::string& fmt)
    {
        chucho::calendar::pieces t = chucho::calendar::get_utc(
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        return chucho::calendar::format(t, fmt);
    }

private:
    std::shared_ptr<chucho::logger> logger_;
    std::string dir_name_;
};

TEST_F(rolling_file_writer_test, numbered)
{
    std::shared_ptr<chucho::file_roll_trigger> trig(std::make_shared<chucho::size_file_roll_trigger>(5));
    std::shared_ptr<chucho::file_roller> roll(new chucho::numbered_file_roller(1, 2));
    std::string fn = get_file_name("num");
    std::shared_ptr<chucho::formatter> fmt(new chucho::pattern_formatter("%m%n"));
    chucho::rolling_file_writer w(fmt, fn, std::move(roll), trig);
    w.write(get_event("one:hello"));
    w.write(get_event("two:hello"));
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("two:hello", get_line(fn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".1"));
    EXPECT_STREQ("one:hello", get_line(fn + ".1").c_str());
    w.write(get_event("three:hello"));
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("three:hello", get_line(fn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".1"));
    EXPECT_STREQ("two:hello", get_line(fn + ".1").c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".2"));
    EXPECT_STREQ("one:hello", get_line(fn + ".2").c_str());
    w.write(get_event("four:hello"));
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("four:hello", get_line(fn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".1"));
    EXPECT_STREQ("three:hello", get_line(fn + ".1").c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".2"));
    EXPECT_STREQ("two:hello", get_line(fn + ".2").c_str());
    EXPECT_FALSE(chucho::file::exists(fn + ".3"));
}

#if defined(CHUCHO_HAVE_ZLIB)

TEST_F(rolling_file_writer_test, numbered_gzip)
{
    auto trig = std::make_shared<chucho::size_file_roll_trigger>(5);
    auto comp = std::make_shared<chucho::gzip_file_compressor>(1);
    auto roll = std::make_shared<chucho::numbered_file_roller>(1, comp);
    auto fn = get_file_name("num_gzip");
    auto fmt = std::make_shared<chucho::pattern_formatter>("%m%n");
    chucho::rolling_file_writer w(fmt, fn, roll, trig);
    w.write(get_event("one:hello"));
    w.write(get_event("two:hello"));
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("two:hello", get_line(fn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".1.gz"));
}

TEST_F(rolling_file_writer_test, numbered_gzip_with_gap)
{
    auto trig = std::make_shared<chucho::size_file_roll_trigger>(5);
    auto comp = std::make_shared<chucho::gzip_file_compressor>(2);
    auto roll = std::make_shared<chucho::numbered_file_roller>(-1, 1, comp);
    auto fn = get_file_name("num_gzip");
    auto fmt = std::make_shared<chucho::pattern_formatter>("%m%n");
    chucho::rolling_file_writer w(fmt, fn, roll, trig);
    w.write(get_event("one:hello"));
    w.write(get_event("two:hello"));
    w.write(get_event("three:hello"));
    w.write(get_event("four:hello"));
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("four:hello", get_line(fn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".-1"));
    EXPECT_STREQ("three:hello", get_line(fn + ".-1").c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".0.gz"));
    EXPECT_TRUE(chucho::file::exists(fn + ".1.gz"));
}

#endif

TEST_F(rolling_file_writer_test, sliding_numbered)
{
    std::shared_ptr<chucho::file_roll_trigger> trig(std::make_shared<chucho::size_file_roll_trigger>(5));
    std::shared_ptr<chucho::file_roller> roll(new chucho::sliding_numbered_file_roller(3));
    std::string fn = get_file_name("slide");
    std::shared_ptr<chucho::formatter> fmt(new chucho::pattern_formatter("%m%n"));
    chucho::rolling_file_writer w(fmt, fn, roll, trig);
    w.write(get_event("one:hello"));
    w.write(get_event("two:hello"));
    auto fnn = fn + ".1";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("two:hello", get_line(fnn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("one:hello", get_line(fn).c_str());
    w.write(get_event("three:hello"));
    fnn = fn + ".2";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("three:hello", get_line(fnn).c_str());
    fnn = fn + ".1";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("two:hello", get_line(fnn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("one:hello", get_line(fn).c_str());
    w.write(get_event("four:hello"));
    fnn = fn + ".3";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("four:hello", get_line(fnn).c_str());
    fnn = fn + ".2";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("three:hello", get_line(fnn).c_str());
    fnn = fn + ".1";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("two:hello", get_line(fnn).c_str());
    EXPECT_FALSE(chucho::file::exists(fn));
}

#if defined(CHUCHO_HAVE_ZLIB)

TEST_F(rolling_file_writer_test, sliding_numbered_gzip)
{
    auto trig = std::make_shared<chucho::size_file_roll_trigger>(5);
    auto comp = std::make_shared<chucho::gzip_file_compressor>(1);
    auto roll = std::make_shared<chucho::sliding_numbered_file_roller>(2, comp);
    auto fn = get_file_name("slide_gzip");
    auto fmt = std::make_shared<chucho::pattern_formatter>("%m%n");
    chucho::rolling_file_writer w(fmt, fn, roll, trig);
    w.write(get_event("one:hello"));
    w.write(get_event("two:hello"));
    std::string fnn = fn + ".1";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("two:hello", get_line(fnn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".gz"));
}

TEST_F(rolling_file_writer_test, sliding_numbered_gzip_with_gap)
{
    auto trig = std::make_shared<chucho::size_file_roll_trigger>(5);
    auto comp = std::make_shared<chucho::gzip_file_compressor>(2);
    auto roll = std::make_shared<chucho::sliding_numbered_file_roller>(-1, 2, comp);
    auto fn = get_file_name("slide_gzip");
    auto fmt = std::make_shared<chucho::pattern_formatter>("%m%n");
    chucho::rolling_file_writer w(fmt, fn, roll, trig);
    w.write(get_event("one:hello"));
    w.write(get_event("two:hello"));
    w.write(get_event("three:hello"));
    w.write(get_event("four:hello"));
    std::string fnn = fn + ".1";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("four:hello", get_line(fnn).c_str());
    fnn = fn + ".0";
    EXPECT_TRUE(chucho::file::exists(fnn));
    EXPECT_STREQ("three:hello", get_line(fnn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".-1.gz"));
    EXPECT_TRUE(chucho::file::exists(fn + ".gz"));
}

#endif

TEST_F(rolling_file_writer_test, time_names)
{
    std::shared_ptr<chucho::formatter> fmt(new chucho::pattern_formatter("%m%n"));
    std::shared_ptr<chucho::file_roller> roll;
    std::unique_ptr<chucho::rolling_file_writer> w;
    std::string fname;
    for (char c : std::string("MHdmY"))
    {
        chucho::status_manager::get()->clear();
        std::string simple("%d{%$}");
        std::replace(simple.begin(), simple.end(), '$', c);
        roll.reset(new chucho::time_file_roller(get_file_name(simple), 1));
        w.reset(new chucho::rolling_file_writer(fmt, roll));
        w->write(get_event("one"));
        EXPECT_EQ(0, chucho::status_manager::get()->get_count());
        fname = get_file_name(get_time(std::string("%") + c));
        EXPECT_TRUE(chucho::file::exists(fname));
        w.reset();
        EXPECT_NO_THROW(chucho::file::remove(fname));
    }
    roll.reset(new chucho::time_file_roller(get_file_name("sub1/sub2/%d{%Y,aux}/%d{%m}"), 1));
    w.reset(new chucho::rolling_file_writer(fmt, std::move(roll)));
    w->write(get_event("one"));
    EXPECT_EQ(0, chucho::status_manager::get()->get_count());
    fname = get_file_name("sub1/sub2/" + get_time("%Y") + "/" + get_time("%m"));
    EXPECT_TRUE(chucho::file::exists(fname));
}

TEST_F(rolling_file_writer_test, time_name_errors)
{
    auto smgr = chucho::status_manager::get();
    std::array<const char*, 5> names = { "%d{%m, aux}", "%d{%m} %d{%Y}", "%d{%S}", "", "my dog has fleas" };
    std::shared_ptr<chucho::time_file_roller> r;
    for (auto name : names)
    {
        smgr->clear();
        EXPECT_THROW(r.reset(new chucho::time_file_roller(name, 1)), chucho::exception);
    }
}
