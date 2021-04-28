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
#include <chucho/streamable.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/log.hpp>

namespace
{

class string_writer : public chucho::writer
{
public:
    string_writer()
        : writer("string", std::move(std::make_unique<chucho::pattern_formatter>("%m"))) { }

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

class my_streamable : public chucho::streamable<my_streamable>
{
public:
    my_streamable(const std::string& logger_name = std::string())
    {
        if (!logger_name.empty())
            rename_stream(logger_name);
    }

    std::shared_ptr<chucho::level> get_level() const
    {
        return get_log_stream().get_level();
    }

    void set_level(std::shared_ptr<chucho::level> lvl)
    {
        get_log_stream().set_level(lvl);
    }

    const std::string& get_logger_name() const
    {
        return get_logger()->get_name();
    }
};

}

class streamable_test : public ::testing::Test, public chucho::streamable<streamable_test>
{
protected:
    streamable_test()
    {
        get_logger()->add_writer(std::move(std::make_unique<string_writer>()));
    }

    ~streamable_test()
    {
        get_logger()->remove_writer("string");
    }

    std::string get_text()
    {
        return dynamic_cast<string_writer&>(get_logger()->get_writer("string")).get_text();
    }
};

TEST_F(streamable_test, copy)
{
    my_streamable s1("s1");
    EXPECT_EQ(std::string("s1"), s1.get_logger_name());
    EXPECT_FALSE(s1.get_level());
    my_streamable s2("s2");
    s2.set_level(chucho::level::INFO_());
    EXPECT_EQ(std::string("s2"), s2.get_logger_name());
    EXPECT_EQ(chucho::level::INFO_(), s2.get_level());
    my_streamable s3(s2);
    EXPECT_EQ(std::string("s2"), s3.get_logger_name());
    EXPECT_EQ(chucho::level::INFO_(), s3.get_level());
    s2 = s1;
    EXPECT_EQ(std::string("s1"), s2.get_logger_name());
    EXPECT_FALSE(s2.get_level());
}

TEST_F(streamable_test, info)
{
    get_logger()->set_level(chucho::level::INFO_());
    CHUCHO_MS << chucho::info << "hello" << chucho::endm;
    CHUCHO_MS << chucho::debug << "goodbye" << chucho::endm;
    EXPECT_EQ(std::string("hello"), get_text());
}

TEST_F(streamable_test, info_lgbl)
{
    get_logger()->set_level(chucho::level::INFO_());
    CHUCHO_INFO_LGBL("hello");
    CHUCHO_DEBUG_LGBL("goodbye");
    EXPECT_EQ(std::string("hello"), get_text());
}

TEST_F(streamable_test, level)
{
    my_streamable ms;
    ms.set_level(chucho::level::FATAL_());
    EXPECT_EQ(chucho::level::FATAL_(), ms.get_level());
}

TEST_F(streamable_test, move)
{
    my_streamable s1("s1");
    s1.set_level(chucho::level::INFO_());
    my_streamable s2(std::move(s1));
    EXPECT_EQ(std::string("s1"), s2.get_logger_name());
    EXPECT_EQ(chucho::level::INFO_(), s2.get_level());
}

TEST_F(streamable_test, logger_name)
{
    EXPECT_EQ(std::string("streamable_test"), get_logger()->get_name());
}
