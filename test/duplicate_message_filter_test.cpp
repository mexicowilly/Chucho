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
#include <chucho/duplicate_message_filter.hpp>
#include <chucho/logger.hpp>

namespace
{

class duplicate_message_filter_test : public ::testing::Test
{
public:
    duplicate_message_filter_test()
        : logger_(chucho::logger::get("duplicate_message_filter_test"))
    {
    }

    chucho::event get_event(const std::string& msg)
    {
        return chucho::event(logger_, chucho::level::INFO_(), msg, __FILE__, __LINE__, __FUNCTION__);
    }

private:
    std::shared_ptr<chucho::logger> logger_;
};

}

TEST_F(duplicate_message_filter_test, evaluate)
{
    chucho::duplicate_message_filter f;
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("one")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("two")));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event("two")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("two")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("one")));
}
