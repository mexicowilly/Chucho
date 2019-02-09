/*
 * Copyright 2013-2019 Will Mason
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
#include <chucho/cloudwatch_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/function_name.hpp>
#include <chucho/logger.hpp>

TEST(cloudwatch_writer, many)
{
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
    chucho::cloudwatch_writer wrt("cloudwatch", std::move(fmt), "Application", "UnitTest", 10);
    for (int i = 0; i < 11; i++)
    {
        chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), std::to_string(i), __FILE__, __LINE__,
                        CHUCHO_FUNCTION_NAME);
        wrt. write(e);
    }
    EXPECT_EQ(1, wrt.get_current_batch_size());
    wrt.flush();
    EXPECT_EQ(0, wrt.get_current_batch_size());
}

TEST(cloudwatch_writer, one)
{
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
    chucho::cloudwatch_writer wrt("cloudwatch", std::move(fmt), "Application", "UnitTest");
    chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), "Hello, World! (monkies)", __FILE__, __LINE__,
                    CHUCHO_FUNCTION_NAME);
    wrt.write(e);
    EXPECT_EQ(1, wrt.get_current_batch_size());
    wrt.flush();
    EXPECT_EQ(0, wrt.get_current_batch_size());
}
