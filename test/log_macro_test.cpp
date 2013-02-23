/*
 * Copyright 2013 Will Mason
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
#include <chucho/log.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/marker.hpp>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>
#include <cstring>

namespace
{

const chucho::optional<chucho::marker> MARK(chucho::marker("marky mark"));

class string_writer : public chucho::writer
{
public:
    string_writer(std::shared_ptr<chucho::formatter> fmt)
        : writer(fmt) { }

    std::vector<std::string> get_lines()
    {
        std::vector<std::string> result;
        std::string line;
        while (stream_.good())
        {
            std::getline(stream_, line);
            if (!line.empty())
                result.push_back(line);
        }
        return result;
    }

protected:
    void write_impl(const chucho::event& evt)
    {
        stream_ << formatter_->format(evt);
    }

private:
    std::stringstream stream_;
};

class log_macro_test : public ::testing::Test
{
public:
    log_macro_test()
        : lgr_(chucho::logger::get("log_macro_test")),
          wrt_(std::make_shared<string_writer>(std::make_shared<chucho::pattern_formatter>("%p %m %k%n")))
    {
        lgr_->add_writer(wrt_);
    }

    ~log_macro_test()
    {
        lgr_.reset();
        chucho::logger::remove_unused_loggers();
    }

protected:
    void log()
    {
        CHUCHO_TRACE(lgr_, "my dog " << "has fleas");
        CHUCHO_DEBUG(lgr_, "my dog " << "has fleas");
        CHUCHO_INFO(lgr_, "my dog " << "has fleas");
        CHUCHO_WARN(lgr_, "my dog " << "has fleas");
        CHUCHO_ERROR(lgr_, "my dog " << "has fleas");
        CHUCHO_FATAL(lgr_, "my dog " << "has fleas");
    }

    void log_marker()
    {
        CHUCHO_TRACE_M(MARK, lgr_, "my dog " << "has fleas");
        CHUCHO_DEBUG_M(MARK, lgr_, "my dog " << "has fleas");
        CHUCHO_INFO_M(MARK, lgr_, "my dog " << "has fleas");
        CHUCHO_WARN_M(MARK, lgr_, "my dog " << "has fleas");
        CHUCHO_ERROR_M(MARK, lgr_, "my dog " << "has fleas");
        CHUCHO_FATAL_M(MARK, lgr_, "my dog " << "has fleas");
    }

    void log_marker_str()
    {
        CHUCHO_TRACE_STR_M(MARK->get_name(), lgr_, "my dog has fleas");
        CHUCHO_DEBUG_STR_M(MARK->get_name(), lgr_, "my dog has fleas");
        CHUCHO_INFO_STR_M(MARK->get_name(), lgr_, "my dog has fleas");
        CHUCHO_WARN_STR_M(MARK->get_name(), lgr_, "my dog has fleas");
        CHUCHO_ERROR_STR_M(MARK->get_name(), lgr_, "my dog has fleas");
        CHUCHO_FATAL_STR_M(MARK->get_name(), lgr_, "my dog has fleas");
    }

    void log_str()
    {
        CHUCHO_TRACE_STR(lgr_, "my dog has fleas");
        CHUCHO_DEBUG_STR(lgr_, "my dog has fleas");
        CHUCHO_INFO_STR(lgr_, "my dog has fleas");
        CHUCHO_WARN_STR(lgr_, "my dog has fleas");
        CHUCHO_ERROR_STR(lgr_, "my dog has fleas");
        CHUCHO_FATAL_STR(lgr_, "my dog has fleas");
    }

    void expect(std::shared_ptr<chucho::level> lvl, const chucho::optional<chucho::marker>& mark = chucho::optional<chucho::marker>())
    {
        std::array<std::shared_ptr<chucho::level>, 6> lvls =
        {
            chucho::level::TRACE,
            chucho::level::DEBUG,
            chucho::level::INFO,
            chucho::level::WARN,
            chucho::level::ERROR,
            chucho::level::FATAL
        };
        std::vector<std::string> lines(wrt_->get_lines());
        std::size_t i = std::distance(lvls.begin(), std::find(lvls.begin(), lvls.end(), lvl));
        ASSERT_EQ(lvls.size() - i, lines.size());
        for (std::size_t num = 0; i < lvls.size(); i++, num++)
        {
            EXPECT_STREQ(lvls[i]->get_name(), lines[num].substr(0, std::strlen(lvls[i]->get_name())).c_str());
            if (mark)
                EXPECT_NE(std::string::npos, lines[num].find(mark->get_name()));
        }
    }

    std::shared_ptr<chucho::logger> lgr_;
    std::shared_ptr<string_writer> wrt_;
};

}

TEST_F(log_macro_test, debug)
{
    SCOPED_TRACE("debug");
    lgr_->set_level(chucho::level::DEBUG);
    log();
    expect(chucho::level::DEBUG);
}

TEST_F(log_macro_test, debug_marker)
{
    SCOPED_TRACE("debug_marker");
    lgr_->set_level(chucho::level::DEBUG);
    log_marker();
    expect(chucho::level::DEBUG, MARK);
}

TEST_F(log_macro_test, debug_marker_str)
{
    SCOPED_TRACE("debug_str");
    lgr_->set_level(chucho::level::DEBUG);
    log_marker_str();
    expect(chucho::level::DEBUG, MARK);
}

TEST_F(log_macro_test, debug_str)
{
    SCOPED_TRACE("debug_str");
    lgr_->set_level(chucho::level::DEBUG);
    log_str();
    expect(chucho::level::DEBUG);
}

TEST_F(log_macro_test, error)
{
    SCOPED_TRACE("error");
    lgr_->set_level(chucho::level::ERROR);
    log();
    expect(chucho::level::ERROR);
}

TEST_F(log_macro_test, error_marker)
{
    SCOPED_TRACE("error_marker");
    lgr_->set_level(chucho::level::ERROR);
    log_marker();
    expect(chucho::level::ERROR, MARK);
}

TEST_F(log_macro_test, error_marker_str)
{
    SCOPED_TRACE("error_str");
    lgr_->set_level(chucho::level::ERROR);
    log_marker_str();
    expect(chucho::level::ERROR, MARK);
}

TEST_F(log_macro_test, error_str)
{
    SCOPED_TRACE("error_str");
    lgr_->set_level(chucho::level::ERROR);
    log_str();
    expect(chucho::level::ERROR);
}

TEST_F(log_macro_test, fatal)
{
    SCOPED_TRACE("fatal");
    lgr_->set_level(chucho::level::FATAL);
    log();
    expect(chucho::level::FATAL);
}

TEST_F(log_macro_test, fatal_marker)
{
    SCOPED_TRACE("fatal_marker");
    lgr_->set_level(chucho::level::FATAL);
    log_marker();
    expect(chucho::level::FATAL, MARK);
}

TEST_F(log_macro_test, fatal_marker_str)
{
    SCOPED_TRACE("fatal_str");
    lgr_->set_level(chucho::level::FATAL);
    log_marker_str();
    expect(chucho::level::FATAL, MARK);
}

TEST_F(log_macro_test, fatal_str)
{
    SCOPED_TRACE("fatal_str");
    lgr_->set_level(chucho::level::FATAL);
    log_str();
    expect(chucho::level::FATAL);
}

TEST_F(log_macro_test, info)
{
    SCOPED_TRACE("info");
    lgr_->set_level(chucho::level::INFO);
    log();
    expect(chucho::level::INFO);
}

TEST_F(log_macro_test, info_marker)
{
    SCOPED_TRACE("info_marker");
    lgr_->set_level(chucho::level::INFO);
    log_marker();
    expect(chucho::level::INFO, MARK);
}

TEST_F(log_macro_test, info_marker_str)
{
    SCOPED_TRACE("info_str");
    lgr_->set_level(chucho::level::INFO);
    log_marker_str();
    expect(chucho::level::INFO, MARK);
}

TEST_F(log_macro_test, info_str)
{
    SCOPED_TRACE("info_str");
    lgr_->set_level(chucho::level::INFO);
    log_str();
    expect(chucho::level::INFO);
}

TEST_F(log_macro_test, trace)
{
    SCOPED_TRACE("trace");
    lgr_->set_level(chucho::level::TRACE);
    log();
    expect(chucho::level::TRACE);
}

TEST_F(log_macro_test, trace_marker)
{
    SCOPED_TRACE("trace_marker");
    lgr_->set_level(chucho::level::TRACE);
    log_marker();
    expect(chucho::level::TRACE, MARK);
}

TEST_F(log_macro_test, trace_marker_str)
{
    SCOPED_TRACE("trace_str");
    lgr_->set_level(chucho::level::TRACE);
    log_marker_str();
    expect(chucho::level::TRACE, MARK);
}

TEST_F(log_macro_test, trace_str)
{
    SCOPED_TRACE("trace_str");
    lgr_->set_level(chucho::level::TRACE);
    log_str();
    expect(chucho::level::TRACE);
}

TEST_F(log_macro_test, warn)
{
    SCOPED_TRACE("warn");
    lgr_->set_level(chucho::level::WARN);
    log();
    expect(chucho::level::WARN);
}

TEST_F(log_macro_test, warn_marker)
{
    SCOPED_TRACE("warn_marker");
    lgr_->set_level(chucho::level::TRACE);
    log_marker();
    expect(chucho::level::TRACE, MARK);
}

TEST_F(log_macro_test, warn_marker_str)
{
    SCOPED_TRACE("warn_str");
    lgr_->set_level(chucho::level::TRACE);
    log_marker_str();
    expect(chucho::level::TRACE, MARK);
}

TEST_F(log_macro_test, warn_str)
{
    SCOPED_TRACE("warn_str");
    lgr_->set_level(chucho::level::TRACE);
    log_str();
    expect(chucho::level::TRACE);
}
