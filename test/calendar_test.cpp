/*
 * Copyright 2013-2015 Will Mason
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
#include <chucho/calendar.hpp>

TEST(calendar, time_zone_format_local)
{
    auto p = chucho::calendar::get_local(std::time(nullptr));
    char buf[1024];
    std::strftime(buf, sizeof(buf), "%z", &p);
    auto res = chucho::calendar::format(p, "%z");
    EXPECT_EQ(std::string(buf), res);
    std::strftime(buf, sizeof(buf), "%%z", &p);
    res = chucho::calendar::format(p, "%%z");
    EXPECT_EQ(std::string(buf), res);
    std::strftime(buf, sizeof(buf), "%%%z", &p);
    res = chucho::calendar::format(p, "%%%z");
    EXPECT_EQ(std::string(buf), res);
    std::strftime(buf, sizeof(buf), "my %z has %%%z fleas %%%%z", &p);
    res = chucho::calendar::format(p, "my %z has %%%z fleas %%%%z");
    EXPECT_EQ(std::string(buf), res);
    std::strftime(buf, sizeof(buf), "%Z", &p);
    res = chucho::calendar::format(p, "%Z");
    EXPECT_EQ(std::string(buf), res);
    std::strftime(buf, sizeof(buf), "%%Z", &p);
    res = chucho::calendar::format(p, "%%Z");
    EXPECT_EQ(std::string(buf), res);
    std::strftime(buf, sizeof(buf), "%%%Z", &p);
    res = chucho::calendar::format(p, "%%%Z");
    EXPECT_EQ(std::string(buf), res);
    std::strftime(buf, sizeof(buf), "my %Z has %%%Z fleas %%%%Z", &p);
    res = chucho::calendar::format(p, "my %Z has %%%Z fleas %%%%Z");
    EXPECT_EQ(std::string(buf), res);
}

TEST(calendar, time_zone_format_utc)
{
    auto p = chucho::calendar::get_utc(std::time(nullptr));
    auto res = chucho::calendar::format(p, "%z");
    EXPECT_EQ(std::string("+0000"), res);
    res = chucho::calendar::format(p, "%%z");
    EXPECT_EQ(std::string("%z"), res);
    res = chucho::calendar::format(p, "%%%z");
    EXPECT_EQ(std::string("%+0000"), res);
    res = chucho::calendar::format(p, "my %z has %%%z fleas %%%%z");
    EXPECT_EQ(std::string("my +0000 has %+0000 fleas %%z"), res);
    res = chucho::calendar::format(p, "%Z");
    EXPECT_EQ(std::string("UTC"), res);
    res = chucho::calendar::format(p, "%%Z");
    EXPECT_EQ(std::string("%Z"), res);
    res = chucho::calendar::format(p, "%%%Z");
    EXPECT_EQ(std::string("%UTC"), res);
    res = chucho::calendar::format(p, "my %Z has %%%Z fleas %%%%Z");
    EXPECT_EQ(std::string("my UTC has %UTC fleas %%Z"), res);
}
