#include <gtest/gtest.h>
#include <chucho/level.hpp>

TEST(level_test, compare)
{
    auto trace = chucho::level::TRACE;
    auto debug = chucho::level::DEBUG;
    auto info = chucho::level::INFO;
    auto warn = chucho::level::WARN;
    auto error = chucho::level::ERROR;
    auto fatal = chucho::level::FATAL;
    auto off = chucho::level::OFF;

    // trace
    EXPECT_TRUE(trace == trace);
    EXPECT_TRUE(trace <= trace);
    EXPECT_TRUE(trace >= trace);
    EXPECT_TRUE(trace < debug);
    EXPECT_TRUE(trace <= debug);

    // debug
    EXPECT_TRUE(debug == debug);
    EXPECT_TRUE(debug <= debug);
    EXPECT_TRUE(debug >= debug);
    EXPECT_TRUE(debug > trace);
    EXPECT_TRUE(debug >= trace);
    EXPECT_TRUE(debug < info);
    EXPECT_TRUE(debug <= info);

    // info
    EXPECT_TRUE(info == info);
    EXPECT_TRUE(info <= info);
    EXPECT_TRUE(info >= info);
    EXPECT_TRUE(info > debug);
    EXPECT_TRUE(info >= debug);
    EXPECT_TRUE(info < warn);
    EXPECT_TRUE(info <= warn);

    // warn
    EXPECT_TRUE(warn == warn);
    EXPECT_TRUE(warn <= warn);
    EXPECT_TRUE(warn >= warn);
    EXPECT_TRUE(warn > info);
    EXPECT_TRUE(warn >= info);
    EXPECT_TRUE(warn < error);
    EXPECT_TRUE(warn <= error);

    // error
    EXPECT_TRUE(error == error);
    EXPECT_TRUE(error <= error);
    EXPECT_TRUE(error >= error);
    EXPECT_TRUE(error > warn);
    EXPECT_TRUE(error >= warn);
    EXPECT_TRUE(error < fatal);
    EXPECT_TRUE(error <= fatal);

    // fatal
    EXPECT_TRUE(fatal == fatal);
    EXPECT_TRUE(fatal <= fatal);
    EXPECT_TRUE(fatal >= fatal);
    EXPECT_TRUE(fatal > error);
    EXPECT_TRUE(fatal >= error);
    EXPECT_TRUE(fatal < off);
    EXPECT_TRUE(fatal <= off);

    // off
    EXPECT_TRUE(off == off);
    EXPECT_TRUE(off <= off);
    EXPECT_TRUE(off >= off);
    EXPECT_TRUE(off > fatal);
    EXPECT_TRUE(off >= fatal);
}

TEST(level_test, name)
{
    EXPECT_STREQ("TRACE", chucho::level::TRACE->get_name());
    EXPECT_STREQ("DEBUG", chucho::level::DEBUG->get_name());
    EXPECT_STREQ("INFO", chucho::level::INFO->get_name());
    EXPECT_STREQ("WARN", chucho::level::WARN->get_name());
    EXPECT_STREQ("ERROR", chucho::level::ERROR->get_name());
    EXPECT_STREQ("FATAL", chucho::level::FATAL->get_name());
    EXPECT_STREQ("OFF", chucho::level::OFF->get_name());
}

TEST(level_test, value)
{
    EXPECT_EQ(0, chucho::level::TRACE->get_value());
    EXPECT_EQ(10000, chucho::level::DEBUG->get_value());
    EXPECT_EQ(20000, chucho::level::INFO->get_value());
    EXPECT_EQ(30000, chucho::level::WARN->get_value());
    EXPECT_EQ(40000, chucho::level::ERROR->get_value());
    EXPECT_EQ(50000, chucho::level::FATAL->get_value());
    EXPECT_EQ(std::numeric_limits<int>::max(), chucho::level::OFF->get_value());
}
