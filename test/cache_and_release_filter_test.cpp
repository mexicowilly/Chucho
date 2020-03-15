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
#include <chucho/cache_and_release_filter.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>

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

class cache_and_release_filter_test : public ::testing::Test
{
protected:
    cache_and_release_filter_test()
      : logger_(chucho::logger::get("cache_and_release_filter_test"))
    {
        logger_->add_writer(std::move(std::make_unique<string_writer>()));
    }

    chucho::event get_event(std::shared_ptr<chucho::level> lvl)
    {
        return chucho::event(logger_, lvl, "hello", __FILE__, __LINE__, __FUNCTION__);
    }

    std::unique_ptr<chucho::cache_and_release_filter> get_carf(std::shared_ptr<chucho::level> c, std::shared_ptr<chucho::level> r)
    {
        return std::make_unique<chucho::cache_and_release_filter>("howdy", logger_->get_writer("string"), c, r);
    }

    std::string get_writer_text()
    {
        auto& wrt = static_cast<string_writer&>(logger_->get_writer("string"));
        auto result = wrt.get_text();
        wrt.clear();
        return result;
    }

private:
    std::shared_ptr<chucho::logger> logger_;
};

}

TEST_F(cache_and_release_filter_test, simple)
{
    auto carf = get_carf(chucho::level::DEBUG_(), chucho::level::ERROR_());
    EXPECT_EQ(chucho::filter::result::NEUTRAL, carf->evaluate(get_event(chucho::level::INFO_())));
    EXPECT_EQ(chucho::filter::result::DENY, carf->evaluate(get_event(chucho::level::DEBUG_())));
    EXPECT_EQ(chucho::filter::result::ACCEPT, carf->evaluate(get_event(chucho::level::ERROR_())));
    EXPECT_STREQ("hello", get_writer_text().c_str());
}
