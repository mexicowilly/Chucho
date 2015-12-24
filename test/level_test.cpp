/*
 * Copyright 2013-2016 Will Mason
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
#include <chucho/level.hpp>

TEST(level_test, compare)
{
    auto trace = chucho::level::TRACE_();
    auto debug = chucho::level::DEBUG_();
    auto info = chucho::level::INFO_();
    auto warn = chucho::level::WARN_();
    auto error = chucho::level::ERROR_();
    auto fatal = chucho::level::FATAL_();
    auto off = chucho::level::OFF_();

    // trace
    EXPECT_TRUE(*trace == *trace);
    EXPECT_TRUE(*trace <= *trace);
    EXPECT_TRUE(*trace >= *trace);
    EXPECT_TRUE(*trace < *debug);
    EXPECT_TRUE(*trace <= *debug);

    // debug
    EXPECT_TRUE(*debug == *debug);
    EXPECT_TRUE(*debug <= *debug);
    EXPECT_TRUE(*debug >= *debug);
    EXPECT_TRUE(*debug > *trace);
    EXPECT_TRUE(*debug >= *trace);
    EXPECT_TRUE(*debug < *info);
    EXPECT_TRUE(*debug <= *info);

    // info
    EXPECT_TRUE(*info == *info);
    EXPECT_TRUE(*info <= *info);
    EXPECT_TRUE(*info >= *info);
    EXPECT_TRUE(*info > *debug);
    EXPECT_TRUE(*info >= *debug);
    EXPECT_TRUE(*info < *warn);
    EXPECT_TRUE(*info <= *warn);

    // warn
    EXPECT_TRUE(*warn == *warn);
    EXPECT_TRUE(*warn <= *warn);
    EXPECT_TRUE(*warn >= *warn);
    EXPECT_TRUE(*warn > *info);
    EXPECT_TRUE(*warn >= *info);
    EXPECT_TRUE(*warn < *error);
    EXPECT_TRUE(*warn <= *error);

    // error
    EXPECT_TRUE(*error == *error);
    EXPECT_TRUE(*error <= *error);
    EXPECT_TRUE(*error >= *error);
    EXPECT_TRUE(*error > *warn);
    EXPECT_TRUE(*error >= *warn);
    EXPECT_TRUE(*error < *fatal);
    EXPECT_TRUE(*error <= *fatal);

    // fatal
    EXPECT_TRUE(*fatal == *fatal);
    EXPECT_TRUE(*fatal <= *fatal);
    EXPECT_TRUE(*fatal >= *fatal);
    EXPECT_TRUE(*fatal > *error);
    EXPECT_TRUE(*fatal >= *error);
    EXPECT_TRUE(*fatal < *off);
    EXPECT_TRUE(*fatal <= *off);

    // off
    EXPECT_TRUE(*off == *off);
    EXPECT_TRUE(*off <= *off);
    EXPECT_TRUE(*off >= *off);
    EXPECT_TRUE(*off > *fatal);
    EXPECT_TRUE(*off >= *fatal);
}

TEST(level_test, custom)
{
    class scary_level : public chucho::level
    {
    public:
        virtual const char* get_name() const override
        {
            return "SCARY";
        }
        virtual chucho::syslog::severity get_syslog_severity() const override
        {
            return chucho::syslog::severity::INFORMATIONAL;
        }
        virtual int get_value() const override
        {
            return 100000;
        }
    };
    std::shared_ptr<chucho::level> scary(new scary_level());
    chucho::level::add(scary);
    auto gotten = chucho::level::from_text("sCarY");
    EXPECT_STREQ("SCARY", gotten->get_name());
    EXPECT_EQ(chucho::syslog::severity::INFORMATIONAL, gotten->get_syslog_severity());
    EXPECT_EQ(100000, gotten->get_value());
};

TEST(level_test, name)
{
    EXPECT_STREQ("TRACE", chucho::level::TRACE_()->get_name());
    EXPECT_STREQ("DEBUG", chucho::level::DEBUG_()->get_name());
    EXPECT_STREQ("INFO", chucho::level::INFO_()->get_name());
    EXPECT_STREQ("WARN", chucho::level::WARN_()->get_name());
    EXPECT_STREQ("ERROR", chucho::level::ERROR_()->get_name());
    EXPECT_STREQ("FATAL", chucho::level::FATAL_()->get_name());
    EXPECT_STREQ("OFF", chucho::level::OFF_()->get_name());
}

TEST(level_test, value)
{
    EXPECT_EQ(0, chucho::level::TRACE_()->get_value());
    EXPECT_EQ(10000, chucho::level::DEBUG_()->get_value());
    EXPECT_EQ(20000, chucho::level::INFO_()->get_value());
    EXPECT_EQ(30000, chucho::level::WARN_()->get_value());
    EXPECT_EQ(40000, chucho::level::ERROR_()->get_value());
    EXPECT_EQ(50000, chucho::level::FATAL_()->get_value());
    EXPECT_EQ(std::numeric_limits<int>::max(), chucho::level::OFF_()->get_value());
}
