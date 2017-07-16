/*
 * Copyright 2013-2017 Will Mason
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
#include <chucho/streamable.hpp>
#include <chucho/pattern_formatter.hpp>

namespace
{

class string_writer : public chucho::writer
{
public:
    string_writer(std::shared_ptr<chucho::formatter> fmt)
        : writer(fmt) { }

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

}

class streamable_test : public ::testing::Test, public chucho::streamable<streamable_test>
{
protected:
    streamable_test()
        : wrt_(std::make_shared<string_writer>(std::make_shared<chucho::pattern_formatter>("%m")))
    {
        get_logger()->add_writer(wrt_);
    }

    ~streamable_test()
    {
        get_logger()->remove_writer(wrt_);
    }

    std::shared_ptr<string_writer> wrt_;
};

TEST_F(streamable_test, info)
{
    get_logger()->set_level(chucho::level::INFO_());
    CHUCHO_MS << chucho::info << "hello" << chucho::endm;
    CHUCHO_MS << chucho::debug << "goodbye" << chucho::endm;
    EXPECT_EQ(std::string("hello"), wrt_->get_text());
}

TEST_F(streamable_test, logger_name)
{
    EXPECT_EQ(std::string("streamable_test"), get_logger()->get_name());
}
