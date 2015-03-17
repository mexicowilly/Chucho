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
#include <chucho/log_stream.hpp>
#include <chucho/writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <sstream>

namespace
{
    
class string_writer : public chucho::writer
{
public:
    string_writer(std::shared_ptr<chucho::formatter> fmt)
        : writer(fmt) { }
    
    std::string get_text()
    {
        return stream_.str();
    }
    
protected:
    void write_impl(const chucho::event& evt)
    {
        stream_ << formatter_->format(evt);
    }
    
private:
    std::stringstream stream_;
};
    
class stream_test : public ::testing::Test
{
public:
    stream_test()
        : lgr_(chucho::logger::get("stream_test")),
          wrt_(std::make_shared<string_writer>(std::make_shared<chucho::pattern_formatter>("%m%k")))
    {
        lgr_->add_writer(wrt_);
    }
    
    ~stream_test()
    {
        lgr_.reset();
        chucho::logger::remove_unused_loggers();
    }
    
    std::string get_text()
    {
        return std::dynamic_pointer_cast<string_writer>(wrt_)->get_text();
    }
    
protected:
    std::shared_ptr<chucho::logger> lgr_;
    std::shared_ptr<chucho::writer> wrt_;
};

}

TEST_F(stream_test, debug)
{
    chucho::log_stream ls(lgr_);
    lgr_->set_level(chucho::level::DEBUG_());
    CHUCHO_M(ls) << chucho::debug << "hello" << chucho::endm;
    CHUCHO_M(ls) << chucho::trace << "goodbye" << chucho::endm;
    EXPECT_EQ(std::string("hello"), get_text());
}

TEST_F(stream_test, error)
{
    chucho::log_stream ls(lgr_);
    lgr_->set_level(chucho::level::ERROR_());
    CHUCHO_M(ls) << chucho::error << "hello" << chucho::endm;
    CHUCHO_M(ls) << chucho::warn << "goodbye" << chucho::endm;
    EXPECT_EQ(std::string("hello"), get_text());
}

TEST_F(stream_test, fatal)
{
    chucho::log_stream ls(lgr_);
    lgr_->set_level(chucho::level::FATAL_());
    CHUCHO_M(ls) << chucho::fatal << "hello" << chucho::endm;
    CHUCHO_M(ls) << chucho::error << "goodbye" << chucho::endm;
    EXPECT_EQ(std::string("hello"), get_text());
}

TEST_F(stream_test, info)
{
    chucho::log_stream ls(lgr_);
    lgr_->set_level(chucho::level::INFO_());
    CHUCHO_M(ls) << chucho::info << "hello" << chucho::endm;
    CHUCHO_M(ls) << chucho::debug << "goodbye" << chucho::endm;
    EXPECT_EQ(std::string("hello"), get_text());
}

TEST_F(stream_test, set_level)
{
    chucho::log_stream ls(lgr_);
    lgr_->set_level(chucho::level::FATAL_());
    CHUCHO_M(ls) << chucho::set_level(chucho::level::FATAL_()) << "hello" << chucho::endm;
    CHUCHO_M(ls) << chucho::error << "goodbye" << chucho::endm;
    EXPECT_EQ(std::string("hello"), get_text());
}

TEST_F(stream_test, set_marker)
{
    chucho::log_stream ls(lgr_);
    lgr_->set_level(chucho::level::INFO_());
    CHUCHO_M(ls) << chucho::info << chucho::set_marker(chucho::marker("marky")) << chucho::endm;
    EXPECT_EQ(std::string("marky"), get_text());
}

TEST_F(stream_test, trace)
{
    chucho::log_stream ls(lgr_);
    lgr_->set_level(chucho::level::TRACE_());
    CHUCHO_M(ls) << chucho::trace << "hello" << chucho::endm;
    EXPECT_EQ(std::string("hello"), get_text());
}

TEST_F(stream_test, warn)
{
    chucho::log_stream ls(lgr_);
    lgr_->set_level(chucho::level::WARN_());
    CHUCHO_M(ls) << chucho::warn << "hello" << chucho::endm;
    CHUCHO_M(ls) << chucho::info << "goodbye" << chucho::endm;
    EXPECT_EQ(std::string("hello"), get_text());
}