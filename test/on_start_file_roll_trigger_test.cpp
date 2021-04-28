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
#include <chucho/on_start_file_roll_trigger.hpp>
#include <chucho/logger.hpp>

TEST(on_start_file_roll_trigger_test, trigger)
{
    chucho::on_start_file_roll_trigger t;
    chucho::event evt(chucho::logger::get("size_file_roll_trigger_test"),
                      chucho::level::INFO_(),
                      "hello",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    std::string name("size_file_roll_trigger_test");
    EXPECT_TRUE(t.is_triggered(name, evt));
    EXPECT_FALSE(t.is_triggered(name, evt));
    EXPECT_FALSE(t.is_triggered(name, evt));
    EXPECT_FALSE(t.is_triggered(name, evt));
}
