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
#include <array>
#include <algorithm>

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
        return chucho::event(logger_, chucho::level::INFO, msg, __FILE__, __LINE__, __FUNCTION__);
    }

    std::string get_file_name(const std::string& base)
    {
        return dir_name_ + '/' + base;
    }

    std::string get_line(const std::string& file_name)
    {
        std::ifstream stream(file_name);
        std::string line;
        std::getline(stream, line);
        return line;
    }

    std::string get_time(const std::string& fmt)
    {
        struct std::tm t = chucho::calendar::get_utc(
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

TEST_F(rolling_file_writer_test, time_names)
{
    std::shared_ptr<chucho::formatter> fmt(new chucho::pattern_formatter("%m%n"));
    std::shared_ptr<chucho::file_roller> roll;
    std::unique_ptr<chucho::rolling_file_writer> w;
    std::string fname;
    for (char c : std::string("MHdVmY"))
    {
        chucho::status_manager::get()->clear();
        std::string simple("%d{%$}");
        std::replace(simple.begin(), simple.end(), '$', c);
        roll.reset(new chucho::time_file_roller(get_file_name(simple), 1));
        w.reset(new chucho::rolling_file_writer(fmt, std::move(roll)));
        w->write(get_event("one"));
        EXPECT_EQ(0, chucho::status_manager::get()->get_count());
        fname = get_file_name(get_time(std::string("%") + c));
        EXPECT_TRUE(chucho::file::exists(fname));
        chucho::file::remove(fname);
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
        r.reset(new chucho::time_file_roller(name, 1));
        EXPECT_GE(smgr->get_count(), 1);
    }
}
