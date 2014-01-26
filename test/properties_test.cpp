/*
 * Copyright 2013-2014 Will Mason
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
#include <chucho/properties.hpp>
#include <sstream>

TEST(properties, iterate)
{
    std::istringstream in("one=\n"
                          "two=\n"
                          "three=");
    chucho::properties props(in);
    int i = 0;
    for (auto p : props)
    {
        EXPECT_TRUE(props.get_one(p.first));
        i++;
    }
    EXPECT_EQ(3, i);
}

TEST(properties, multiple)
{
    std::istringstream in("one=\n"
                          "two=\n"
                          "two=\n"
                          "three=\n"
                          "three=\n"
                          "three=");
    chucho::properties props(in);
    auto range = props.get("zero");
    EXPECT_EQ(range.first, range.second);
    range = props.get("one");
    ASSERT_EQ(1, std::distance(range.first, range.second));
    EXPECT_EQ(std::string("one"), range.first->first);
    range = props.get("two");
    ASSERT_EQ(2, std::distance(range.first, range.second));
    EXPECT_EQ(std::string("two"), range.first->first);
    range = props.get("three");
    ASSERT_EQ(3, std::distance(range.first, range.second));
    EXPECT_EQ(std::string("three"), range.first->first);
}

TEST(properties, read)
{
    std::istringstream in("a=b\n"
                          "      b     =        c\n"
                          "\tc=\n"
                          "       # a comment\n"
                          "# another comment\n"
                          "\t    \t# and another comment\n"
                          "d\t=                   my dog has fleas\n"
                          "e=         and=so=do=I\n"
                          "f.g.h = what\n"
                          "great.balls.of.fire    \t  =      jerry\n"
                          "trailing=\tspaces \n"
                          "this should be ignored = yes\n"
                          "only.spaces=       \t");
    chucho::properties props(in);
    auto v = props.get_one("a"); 
    ASSERT_TRUE(v);
    EXPECT_EQ(std::string("b"), *v);
    v = props.get_one("b");
    ASSERT_TRUE(v);
    EXPECT_EQ(std::string("c"), *v);
    v = props.get_one("c");
    ASSERT_TRUE(v);
    EXPECT_TRUE(v->empty());
    v = props.get_one("d");
    ASSERT_TRUE(v);
    EXPECT_EQ(std::string("my dog has fleas"), *v);
    v = props.get_one("e");
    ASSERT_TRUE(v);
    EXPECT_EQ(std::string("and=so=do=I"), *v);
    v = props.get_one("f.g.h");
    ASSERT_TRUE(v);
    EXPECT_EQ(std::string("what"), *v);
    v = props.get_one("great.balls.of.fire");
    ASSERT_TRUE(v);
    EXPECT_EQ(std::string("jerry"), *v);
    v = props.get_one("trailing");
    ASSERT_TRUE(v);
    EXPECT_EQ(std::string("spaces "), *v);
    v = props.get_one("this one's not there");
    EXPECT_FALSE(v);
    v = props.get_one("this should be ingored");
    EXPECT_FALSE(v);
    v = props.get_one("only.spaces");
    ASSERT_TRUE(v);
    EXPECT_TRUE(v->empty());
}

TEST(properties, subset)
{
    std::istringstream in("without=\n"
                          "unwanted.prefix.one=\n"
                          "wanted.prefix.two=\n"
                          "unwanted.prefix.three=\n"
                          "wanted.prefix.four=");
    chucho::properties props(in);
    chucho::properties sub = props.get_subset("wanted.prefix.");
    auto v = sub.get_one("one");
    ASSERT_FALSE(v);
    v = sub.get_one("two");
    ASSERT_TRUE(v);
    v = sub.get_one("three");
    ASSERT_FALSE(v);
    v = sub.get_one("four");
    ASSERT_TRUE(v);
    v = sub.get_one("without");
    ASSERT_FALSE(v);
}
