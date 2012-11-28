#include <gtest/gtest.h>
#include <chucho/level.hpp>

TEST(level_test, compare)
{
    auto trace = chucho::trace_level();
    auto debug = chucho::debug_level();
    auto info = chucho::info_level();
    auto warn = chucho::warn_level();
    auto error = chucho::error_level();
    auto fatal = chucho::fatal_level();
    auto off = chucho::off_level();

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
    EXPECT_STREQ("TRACE", chucho::trace_level().get_name());
    EXPECT_STREQ("DEBUG", chucho::debug_level().get_name());
    EXPECT_STREQ("INFO", chucho::info_level().get_name());
    EXPECT_STREQ("WARN", chucho::warn_level().get_name());
    EXPECT_STREQ("ERROR", chucho::error_level().get_name());
    EXPECT_STREQ("FATAL", chucho::fatal_level().get_name());
    EXPECT_STREQ("OFF", chucho::off_level().get_name());
}
