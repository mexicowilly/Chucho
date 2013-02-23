/*
 * Copyright 2013 Will Mason
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
#include <chucho/marker.hpp>
#include <sstream>
#include <algorithm>

TEST(marker_test, children)
{
    chucho::marker mark("hello");
    chucho::marker one("one");
    mark.insert(one);
    chucho::marker two("two");
    mark.insert(two);
    EXPECT_FALSE(mark.empty());
    EXPECT_FALSE(std::find(mark.begin(), mark.end(), one) == mark.end());
    EXPECT_FALSE(std::find(mark.begin(), mark.end(), two) == mark.end());
    chucho::marker three("three");
    EXPECT_TRUE(std::find(mark.begin(), mark.end(), three) == mark.end());
    chucho::marker two2("two");
    EXPECT_FALSE(std::find(mark.begin(), mark.end(), two2) == mark.end());
}

TEST(marker_test, format)
{
    chucho::marker mark("one");
    mark.insert(chucho::marker("a"));
    mark.insert(chucho::marker("b"));
    std::ostringstream stream;
    stream << mark;
    EXPECT_STREQ("one [ a, b ]", stream.str().c_str());
}

TEST(marker_test, name)
{
    chucho::marker mark("one");
    EXPECT_STREQ("one", mark.get_name().c_str());
}

TEST(marker_test, sort)
{
    chucho::marker mark1("one");
    chucho::marker mark2("two");
    EXPECT_LT(mark1, mark2);
    EXPECT_FALSE(mark1 == mark2);
}
