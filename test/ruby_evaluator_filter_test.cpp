/*
 * Copyright 2013-2020 Will Mason
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
#include <chucho/ruby_evaluator_filter.hpp>
#include <chucho/logger.hpp>

#include <iostream>

namespace
{

class ruby_evaluator_filter_test : public ::testing::Test
{
public:
    ruby_evaluator_filter_test()
        : logger_(chucho::logger::get("ruby_evaluator_filter_test"))
    {
    }

    chucho::event get_event(const std::string& msg)
    {
        return chucho::event(logger_, chucho::level::INFO_(), msg, __FILE__, __LINE__, "monkey", "marky");
    }

private:
    std::shared_ptr<chucho::logger> logger_;
};

}

TEST_F(ruby_evaluator_filter_test, file_name)
{
    chucho::ruby_evaluator_filter f1("ruby", "$file_name.instance_of? String");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f1.evaluate(get_event("one")));
    chucho::ruby_evaluator_filter f2("ruby", std::string("$file_name == '") + __FILE__ + "'");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f2.evaluate(get_event("two")));
}

TEST_F(ruby_evaluator_filter_test, function_name)
{
    chucho::ruby_evaluator_filter f1("ruby", "$function_name.instance_of? String");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f1.evaluate(get_event("one")));
    chucho::ruby_evaluator_filter f2("ruby", "$function_name == 'monkey'");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f2.evaluate(get_event("two")));
}

TEST_F(ruby_evaluator_filter_test, level)
{
    chucho::ruby_evaluator_filter f1("ruby", "$level.instance_of? String");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f1.evaluate(get_event("one")));
    chucho::ruby_evaluator_filter f2("ruby", "$level == 'INFO'");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f2.evaluate(get_event("two")));
}

TEST_F(ruby_evaluator_filter_test, line_number)
{
    chucho::ruby_evaluator_filter f1("ruby", "$line_number.instance_of? Fixnum");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f1.evaluate(get_event("one")));
    chucho::ruby_evaluator_filter f2("ruby", "$line_number == 36");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f2.evaluate(get_event("two")));
}

TEST_F(ruby_evaluator_filter_test, logger)
{
    chucho::ruby_evaluator_filter f1("ruby", "$logger.instance_of? String");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f1.evaluate(get_event("one")));
    chucho::ruby_evaluator_filter f2("ruby", "$logger == 'ruby_evaluator_filter_test'");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f2.evaluate(get_event("two")));
}

TEST_F(ruby_evaluator_filter_test, marker)
{
    chucho::ruby_evaluator_filter f1("ruby", "$marker.instance_of? String");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f1.evaluate(get_event("one")));
    chucho::ruby_evaluator_filter f2("ruby", "$marker == 'marky'");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f2.evaluate(get_event("two")));
}

TEST_F(ruby_evaluator_filter_test, message)
{
    chucho::ruby_evaluator_filter f1("ruby", "$message.instance_of? String");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f1.evaluate(get_event("one")));
    chucho::ruby_evaluator_filter f2("ruby", "$message == 'two'");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f2.evaluate(get_event("two")));
}

TEST_F(ruby_evaluator_filter_test, timestamp)
{
    chucho::ruby_evaluator_filter f1("ruby", "$timestamp.instance_of? Time");
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f1.evaluate(get_event("one")));
    auto evt = get_event("two");
    auto secs = chucho::event::clock_type::to_time_t(evt.get_time());
    chucho::ruby_evaluator_filter f2("ruby", "$timestamp.to_i == " + std::to_string(secs));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f2.evaluate(evt));
}
