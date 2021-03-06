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
#include <chucho/level_threshold_filter.hpp>
#include <chucho/logger.hpp>

class level_threshold_filter_test : public ::testing::Test
{
protected:
    level_threshold_filter_test()
        : logger_(chucho::logger::get("level_threshold_filter_test"))
    {
    }

    chucho::event get_event(std::shared_ptr<chucho::level> lvl)
    {
        return chucho::event(logger_, lvl, "hello", __FILE__, __LINE__, __FUNCTION__);
    }

private:
    std::shared_ptr<chucho::logger> logger_;
};

TEST_F(level_threshold_filter_test, evaluate)
{
    chucho::level_threshold_filter f("thresh", chucho::level::INFO_());
    EXPECT_EQ(std::string("thresh"), f.get_name());
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::TRACE_())));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::DEBUG_())));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::INFO_())));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::WARN_())));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::ERROR_())));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::FATAL_())));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::OFF_())));
}
