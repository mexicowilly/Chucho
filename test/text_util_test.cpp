/*
 * Copyright 2013-2021 Will Mason
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
#include <chucho/text_util.hpp>

TEST(text_util, tokenize)
{
    auto tokes = chucho::text_util::tokenize("one,two,three", ',');
    ASSERT_EQ(3, tokes.size());
    EXPECT_STREQ("one", tokes[0].c_str());
    EXPECT_STREQ("two", tokes[1].c_str());
    EXPECT_STREQ("three", tokes[2].c_str());
    tokes = chucho::text_util::tokenize("@@@@@@my@dog@@has@@@fleas@@@@@@@@@@@@@@@@", '@');
    ASSERT_EQ(4, tokes.size());
    EXPECT_STREQ("my", tokes[0].c_str());
    EXPECT_STREQ("dog", tokes[1].c_str());
    EXPECT_STREQ("has", tokes[2].c_str());
    EXPECT_STREQ("fleas", tokes[3].c_str());
}

TEST(text_util, trim)
{
    std::string s("hello");
    chucho::text_util::trim(s);
    EXPECT_STREQ("hello", s.c_str());
    s = "    \tis it me\t\t    ";
    chucho::text_util::trim(s);
    EXPECT_STREQ("is it me", s.c_str());
    s = "you're looking        \t";
    chucho::text_util::trim(s);
    EXPECT_STREQ("you're looking", s.c_str());
    s = "\t     for";
    chucho::text_util::trim(s);
    EXPECT_STREQ("for", s.c_str());
    s.clear();
    chucho::text_util::trim(s);
    EXPECT_TRUE(s.empty());
}
