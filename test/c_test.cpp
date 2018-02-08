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
#include <chucho/file.hpp>
#include <fstream>

TEST(c, simple)
{
#if defined(CHUCHO_YAML_CONFIG)
    std::string helper_name(std::string("test") + chucho::file::dir_sep + "c-test-helper");
    #if defined(CHUCHO_WINDOWS)
    helper_name += ".exe";
    #endif
    ASSERT_TRUE(chucho::file::exists(helper_name));
    ASSERT_EQ(0, std::system((helper_name + " c-test.log").c_str()));
    ASSERT_TRUE(chucho::file::exists("c-test.log"));
    std::ifstream stream("c-test.log");
    ASSERT_TRUE(stream.is_open());
    std::vector<std::string> lines;
    while (stream)
    {
        lines.emplace_back("");
        std::getline(stream, lines.back());
    }
    stream.close();
    lines.pop_back();
    ASSERT_EQ(12, lines.size());
    EXPECT_STREQ("TRACE 1", lines[0].c_str());
    EXPECT_STREQ("TRACE mark 2", lines[1].c_str());
    EXPECT_STREQ("DEBUG 3", lines[2].c_str());
    EXPECT_STREQ("DEBUG mark 4", lines[3].c_str());
    EXPECT_STREQ("INFO 5", lines[4].c_str());
    EXPECT_STREQ("INFO mark 6", lines[5].c_str());
    EXPECT_STREQ("WARN 7", lines[6].c_str());
    EXPECT_STREQ("WARN mark 8", lines[7].c_str());
    EXPECT_STREQ("ERROR 9", lines[8].c_str());
    EXPECT_STREQ("ERROR mark 10", lines[9].c_str());
    EXPECT_STREQ("FATAL 11", lines[10].c_str());
    EXPECT_STREQ("FATAL mark 12", lines[11].c_str());
    chucho::file::remove("c-test.log");
#endif
}
