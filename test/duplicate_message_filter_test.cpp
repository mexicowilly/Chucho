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
#include <chucho/duplicate_message_filter.hpp>
#include <chucho/logger.hpp>
#include <chucho/pattern_formatter.hpp>

namespace
{

class string_writer : public chucho::writer
{
public:
    string_writer()
      : writer("string", std::move(std::make_unique<chucho::pattern_formatter>("%m")))
    {
    }

    void clear()
    {
        stream_.str("");
    }

    std::string get_text()
    {
        return stream_.str();
    }

protected:
    virtual void write_impl(const chucho::event& evt) override
    {
        stream_ << formatter_->format(evt);
    }

private:
    std::stringstream stream_;
};

class duplicate_message_filter_test : public ::testing::Test
{
public:
    duplicate_message_filter_test()
        : logger_(chucho::logger::get("duplicate_message_filter_test"))
    {
        logger_->add_writer(std::move(std::make_unique<string_writer>()));
    }

    chucho::event get_event(const std::string& msg)
    {
        return chucho::event(logger_, chucho::level::INFO_(), msg, __FILE__, __LINE__, __FUNCTION__);
    }

    std::string get_writer_text()
    {
        auto& wrt = static_cast<string_writer&>(logger_->get_writer("string"));
        auto result = wrt.get_text();
        wrt.clear();
        return result;
    }

protected:
    std::shared_ptr<chucho::logger> logger_;
};

}

TEST_F(duplicate_message_filter_test, evaluate)
{
    chucho::duplicate_message_filter f("dup", logger_->get_writer("string"));
    EXPECT_STREQ("dup", f.get_name().c_str());
    EXPECT_TRUE(f.has_writer());
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("one")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("two")));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event("two")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("three")));
    EXPECT_STREQ("The last message was logged 2 times in a row", get_writer_text().c_str());
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("two")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event("three")));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event("one")));
    EXPECT_STREQ("The last message was logged 4 times in a row", get_writer_text().c_str());
}
