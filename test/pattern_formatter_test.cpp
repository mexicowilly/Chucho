#include <gtest/gtest.h>
#include <chucho/pattern_formatter.hpp>
#include <sstream>

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

