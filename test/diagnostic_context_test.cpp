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
#include <chucho/diagnostic_context.hpp>
#include <thread>

namespace
{

void thread_main()
{
    EXPECT_TRUE(chucho::diagnostic_context::empty());
    chucho::diagnostic_context::at("funky") = "monkey";
    EXPECT_FALSE(chucho::diagnostic_context::empty());
    EXPECT_STREQ("monkey", chucho::diagnostic_context::at("funky").c_str());
}

}

TEST(diagnostic_context_test, basic)
{
    chucho::diagnostic_context::clear();
    EXPECT_TRUE(chucho::diagnostic_context::empty());
    chucho::diagnostic_context::at("one") = "two";
    EXPECT_FALSE(chucho::diagnostic_context::empty());
    EXPECT_STREQ("two", chucho::diagnostic_context::at("one").c_str());
    chucho::diagnostic_context::erase("one");
    EXPECT_TRUE(chucho::diagnostic_context::empty());
    EXPECT_STREQ("", chucho::diagnostic_context::at("one").c_str());
    chucho::diagnostic_context::at("one") = "two";
    auto cp = chucho::diagnostic_context::get();
    EXPECT_EQ(1, cp.size());
    EXPECT_STREQ("two", cp["one"].c_str());
    cp.clear();
    cp["three"] = "four";
    chucho::diagnostic_context::set(cp);
    EXPECT_FALSE(chucho::diagnostic_context::empty());
    EXPECT_STREQ("four", chucho::diagnostic_context::at("three").c_str());
}

TEST(diagnostic_context_test, threads)
{
    chucho::diagnostic_context::clear();
    EXPECT_TRUE(chucho::diagnostic_context::empty());
    chucho::diagnostic_context::at("one") = "two";
    EXPECT_FALSE(chucho::diagnostic_context::empty());
    EXPECT_STREQ("two", chucho::diagnostic_context::at("one").c_str());
    std::thread t(thread_main);
    t.join();
    EXPECT_FALSE(chucho::diagnostic_context::empty());
    EXPECT_STREQ("two", chucho::diagnostic_context::at("one").c_str());
}
