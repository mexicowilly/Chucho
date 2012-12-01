#include <gtest/gtest.h>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <sstream>
#include <thread>
#include <array>
#if !defined(_WIN32)
#include <sys/utsname.h>
#endif

namespace
{

#if defined(_WIN32)
const char* file_name = "\\one\\two";
#else
const char* file_name = "/one/two";
#endif

}

class pattern_formatter_test : public ::testing::Test
{
public:
    pattern_formatter_test()
        : evt_(chucho::logger::get_logger("pattern logger"),
               "hi",
               file_name,
               10,
               "dowdy")
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

TEST_F(pattern_formatter_test, logger)
{
    chucho::pattern_formatter f("%c");
    EXPECT_STREQ("pattern logger", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, full_file)
{
    chucho::pattern_formatter f("%F");
    EXPECT_STREQ(file_name, f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, line_number)
{
    chucho::pattern_formatter f("%L");
    EXPECT_STREQ("10", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, message)
{
    chucho::pattern_formatter f("%m");
    EXPECT_STREQ("hi", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, function)
{
    chucho::pattern_formatter f("%M");
    EXPECT_STREQ("dowdy", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, end_of_line)
{
    std::ostringstream stream;
    stream << std::endl;
    chucho::pattern_formatter f("%n");
    EXPECT_EQ(stream.str(), f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, level)
{
    chucho::pattern_formatter f("%p");
    EXPECT_STREQ("INFO", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, pid)
{
    #if defined(_WIN32)
    pid_t p = _getpid();
    #else
    pid_t p = getpid();
    #endif
    chucho::pattern_formatter f("%i");
    EXPECT_EQ(std::to_string(p), f.format(evt_));
}

TEST_F(pattern_formatter_test, base_host)
{
    #if defined(_WIN32)
    #else
    struct utsname info;
    uname(&info);
    std::string h(info.nodename);
    #endif
    chucho::pattern_formatter f("%h");
    EXPECT_EQ(h, f.format(evt_));
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
    struct std::tm cal = *std::gmtime(&secs);
    std::array<char, 1024> buf;
    std::strftime(buf.data(), buf.size(), "%Y-%m-%d %H:%M:%S", &cal);
    EXPECT_STREQ(buf.data(), f.format(evt_).c_str());
    f = chucho::pattern_formatter("%d{%Y}");
    std::strftime(buf.data(), buf.size(), "%Y", &cal);
    EXPECT_STREQ(buf.data(), f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, local_date_time)
{
    chucho::pattern_formatter f("%D");
    std::time_t secs = std::chrono::duration_cast<std::chrono::seconds>(evt_.get_time().time_since_epoch()).count();
    struct std::tm cal = *std::localtime(&secs);
    std::array<char, 1024> buf;
    std::strftime(buf.data(), buf.size(), "%Y-%m-%d %H:%M:%S", &cal);
    EXPECT_STREQ(buf.data(), f.format(evt_).c_str());
    f = chucho::pattern_formatter("%D{%Y}");
    std::strftime(buf.data(), buf.size(), "%Y", &cal);
    EXPECT_STREQ(buf.data(), f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, with_literal)
{
    chucho::pattern_formatter f(" %m");
    EXPECT_STREQ(" hi", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%m ");
    EXPECT_STREQ("hi ", f.format(evt_).c_str());
    f = chucho::pattern_formatter(" %m ");
    EXPECT_STREQ(" hi ", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%m %L");
    EXPECT_STREQ("hi 10", f.format(evt_).c_str());
    f = chucho::pattern_formatter(" %m %L");
    EXPECT_STREQ(" hi 10", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%m %L ");
    EXPECT_STREQ("hi 10 ", f.format(evt_).c_str());
    f = chucho::pattern_formatter(" %m %L ");
    EXPECT_STREQ(" hi 10 ", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, format_params)
{
    chucho::pattern_formatter f("%5m");
    EXPECT_STREQ("   hi", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%-5m");
    EXPECT_STREQ("hi   ", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%.1m");
    EXPECT_STREQ("i", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%5.1m");
    EXPECT_STREQ("i", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%1.1m");
    EXPECT_STREQ("i", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%1.10m");
    EXPECT_STREQ("hi", f.format(evt_).c_str());
    f = chucho::pattern_formatter("%-5.10m");
    EXPECT_STREQ("hi   ", f.format(evt_).c_str());
}

TEST_F(pattern_formatter_test, invalid)
{
    ASSERT_THROW(chucho::pattern_formatter("%z"), chucho::pattern_exception);
    ASSERT_THROW(chucho::pattern_formatter("%1.m"), chucho::pattern_exception);
    ASSERT_THROW(chucho::pattern_formatter(""), chucho::pattern_exception);
    ASSERT_THROW(chucho::pattern_formatter("%d{%Ym"), chucho::pattern_exception);
}
