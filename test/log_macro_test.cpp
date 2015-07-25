/*
 * Copyright 2013-2015 Will Mason
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
#include <chucho/loggable.hpp>
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

class log_macro : protected chucho::loggable<log_macro>, public ::testing::Test
{
public:
    log_macro()
        : lgr_(chucho::logger::get("log_macro")),
          wrt_(std::make_shared<string_writer>(std::make_shared<chucho::pattern_formatter>("%p %m %k%n"))),
          lgbl_wrt_(std::make_shared<string_writer>(std::make_shared<chucho::pattern_formatter>("%p %m %k%n")))
    {
        lgr_->add_writer(wrt_);
        get_logger()->add_writer(lgbl_wrt_);
    }

    ~log_macro()
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

    void log_lgbl()
    {
        CHUCHO_TRACE_LGBL("my dog " << "has fleas");
        CHUCHO_DEBUG_LGBL("my dog " << "has fleas");
        CHUCHO_INFO_LGBL("my dog " << "has fleas");
        CHUCHO_WARN_LGBL("my dog " << "has fleas");
        CHUCHO_ERROR_LGBL("my dog " << "has fleas");
        CHUCHO_FATAL_LGBL("my dog " << "has fleas");
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

    void log_lgbl_marker()
    {
        CHUCHO_TRACE_LGBL_M(MARK, "my dog " << "has fleas");
        CHUCHO_DEBUG_LGBL_M(MARK, "my dog " << "has fleas");
        CHUCHO_INFO_LGBL_M(MARK, "my dog " << "has fleas");
        CHUCHO_WARN_LGBL_M(MARK, "my dog " << "has fleas");
        CHUCHO_ERROR_LGBL_M(MARK, "my dog " << "has fleas");
        CHUCHO_FATAL_LGBL_M(MARK, "my dog " << "has fleas");
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

    void log_lgbl_marker_str()
    {
        CHUCHO_TRACE_LGBL_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_DEBUG_LGBL_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_INFO_LGBL_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_WARN_LGBL_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_ERROR_LGBL_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_FATAL_LGBL_STR_M(MARK->get_name(), "my dog has fleas");
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

    void log_lgbl_str()
    {
        CHUCHO_TRACE_LGBL_STR("my dog has fleas");
        CHUCHO_DEBUG_LGBL_STR("my dog has fleas");
        CHUCHO_INFO_LGBL_STR("my dog has fleas");
        CHUCHO_WARN_LGBL_STR("my dog has fleas");
        CHUCHO_ERROR_LGBL_STR("my dog has fleas");
        CHUCHO_FATAL_LGBL_STR("my dog has fleas");
    }

    void expect(std::shared_ptr<chucho::level> lvl,
                std::shared_ptr<string_writer> wrt,
                const chucho::optional<chucho::marker>& mark = chucho::optional<chucho::marker>())
    {
        std::array<std::shared_ptr<chucho::level>, 6> lvls =
        {
            chucho::level::TRACE_(),
            chucho::level::DEBUG_(),
            chucho::level::INFO_(),
            chucho::level::WARN_(),
            chucho::level::ERROR_(),
            chucho::level::FATAL_()
        };
        std::vector<std::string> lines(wrt->get_lines());
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
    std::shared_ptr<string_writer> lgbl_wrt_;
};

}

TEST_F(log_macro, debug)
{
    SCOPED_TRACE("debug");
    lgr_->set_level(chucho::level::DEBUG_());
    log();
    expect(chucho::level::DEBUG_(), wrt_);
}

TEST_F(log_macro, debug_lgbl)
{
    SCOPED_TRACE("debug_lgbl");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_lgbl();
    expect(chucho::level::DEBUG_(), lgbl_wrt_);
}

TEST_F(log_macro, debug_marker)
{
    SCOPED_TRACE("debug_marker");
    lgr_->set_level(chucho::level::DEBUG_());
    log_marker();
    expect(chucho::level::DEBUG_(), wrt_, MARK);
}

TEST_F(log_macro, debug_lgbl_marker)
{
    SCOPED_TRACE("debug_lgbl_marker");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_lgbl_marker();
    expect(chucho::level::DEBUG_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, debug_marker_str)
{
    SCOPED_TRACE("debug_marker_str");
    lgr_->set_level(chucho::level::DEBUG_());
    log_marker_str();
    expect(chucho::level::DEBUG_(), wrt_, MARK);
}

TEST_F(log_macro, debug_lgbl_marker_str)
{
    SCOPED_TRACE("debug_lgbl_marker_str");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_lgbl_marker_str();
    expect(chucho::level::DEBUG_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, debug_str)
{
    SCOPED_TRACE("debug_str");
    lgr_->set_level(chucho::level::DEBUG_());
    log_str();
    expect(chucho::level::DEBUG_(), wrt_);
}

TEST_F(log_macro, debug_lgbl_str)
{
    SCOPED_TRACE("debug_lgbl_str");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_lgbl_str();
    expect(chucho::level::DEBUG_(), lgbl_wrt_);
}

TEST_F(log_macro, error)
{
    SCOPED_TRACE("error");
    lgr_->set_level(chucho::level::ERROR_());
    log();
    expect(chucho::level::ERROR_(), wrt_);
}

TEST_F(log_macro, error_lgbl)
{
    SCOPED_TRACE("error_lgbl");
    get_logger()->set_level(chucho::level::ERROR_());
    log_lgbl();
    expect(chucho::level::ERROR_(), lgbl_wrt_);
}

TEST_F(log_macro, error_marker)
{
    SCOPED_TRACE("error_marker");
    lgr_->set_level(chucho::level::ERROR_());
    log_marker();
    expect(chucho::level::ERROR_(), wrt_, MARK);
}

TEST_F(log_macro, error_lgbl_marker)
{
    SCOPED_TRACE("error_lgbl_marker");
    get_logger()->set_level(chucho::level::ERROR_());
    log_lgbl_marker();
    expect(chucho::level::ERROR_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, error_marker_str)
{
    SCOPED_TRACE("error_marker_str");
    lgr_->set_level(chucho::level::ERROR_());
    log_marker_str();
    expect(chucho::level::ERROR_(), wrt_, MARK);
}

TEST_F(log_macro, error_lgbl_marker_str)
{
    SCOPED_TRACE("error_lgbl_marker_str");
    get_logger()->set_level(chucho::level::ERROR_());
    log_lgbl_marker_str();
    expect(chucho::level::ERROR_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, error_str)
{
    SCOPED_TRACE("error_str");
    lgr_->set_level(chucho::level::ERROR_());
    log_str();
    expect(chucho::level::ERROR_(), wrt_);
}

TEST_F(log_macro, error_lgbl_str)
{
    SCOPED_TRACE("error_lgbl_str");
    get_logger()->set_level(chucho::level::ERROR_());
    log_lgbl_str();
    expect(chucho::level::ERROR_(), lgbl_wrt_);
}

TEST_F(log_macro, fatal)
{
    SCOPED_TRACE("fatal");
    lgr_->set_level(chucho::level::FATAL_());
    log();
    expect(chucho::level::FATAL_(), wrt_);
}

TEST_F(log_macro, fatal_lgbl)
{
    SCOPED_TRACE("fatal_lgbl");
    get_logger()->set_level(chucho::level::FATAL_());
    log_lgbl();
    expect(chucho::level::FATAL_(), lgbl_wrt_);
}

TEST_F(log_macro, fatal_marker)
{
    SCOPED_TRACE("fatal_marker");
    lgr_->set_level(chucho::level::FATAL_());
    log_marker();
    expect(chucho::level::FATAL_(), wrt_, MARK);
}

TEST_F(log_macro, fatal_lgbl_marker)
{
    SCOPED_TRACE("fatal_lgbl_marker");
    get_logger()->set_level(chucho::level::FATAL_());
    log_lgbl_marker();
    expect(chucho::level::FATAL_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, fatal_marker_str)
{
    SCOPED_TRACE("fatal_marker_str");
    lgr_->set_level(chucho::level::FATAL_());
    log_marker_str();
    expect(chucho::level::FATAL_(), wrt_, MARK);
}

TEST_F(log_macro, fatal_lgbl_marker_str)
{
    SCOPED_TRACE("fatal_lgbl_marker_str");
    get_logger()->set_level(chucho::level::FATAL_());
    log_lgbl_marker_str();
    expect(chucho::level::FATAL_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, fatal_str)
{
    SCOPED_TRACE("fatal_str");
    lgr_->set_level(chucho::level::FATAL_());
    log_str();
    expect(chucho::level::FATAL_(), wrt_);
}

TEST_F(log_macro, fatal_lgbl_str)
{
    SCOPED_TRACE("fatal_lgbl_str");
    get_logger()->set_level(chucho::level::FATAL_());
    log_lgbl_str();
    expect(chucho::level::FATAL_(), lgbl_wrt_);
}

TEST_F(log_macro, info)
{
    SCOPED_TRACE("info");
    lgr_->set_level(chucho::level::INFO_());
    log();
    expect(chucho::level::INFO_(), wrt_);
}

TEST_F(log_macro, info_lgbl)
{
    SCOPED_TRACE("info_lgbl");
    get_logger()->set_level(chucho::level::INFO_());
    log_lgbl();
    expect(chucho::level::INFO_(), lgbl_wrt_);
}

TEST_F(log_macro, info_marker)
{
    SCOPED_TRACE("info_marker");
    lgr_->set_level(chucho::level::INFO_());
    log_marker();
    expect(chucho::level::INFO_(), wrt_, MARK);
}

TEST_F(log_macro, info_lgbl_marker)
{
    SCOPED_TRACE("info_lgbl_marker");
    get_logger()->set_level(chucho::level::INFO_());
    log_lgbl_marker();
    expect(chucho::level::INFO_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, info_marker_str)
{
    SCOPED_TRACE("info_marker_str");
    lgr_->set_level(chucho::level::INFO_());
    log_marker_str();
    expect(chucho::level::INFO_(), wrt_, MARK);
}

TEST_F(log_macro, info_lgbl_marker_str)
{
    SCOPED_TRACE("info_lgbl_marker_str");
    get_logger()->set_level(chucho::level::INFO_());
    log_lgbl_marker_str();
    expect(chucho::level::INFO_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, info_str)
{
    SCOPED_TRACE("info_str");
    lgr_->set_level(chucho::level::INFO_());
    log_str();
    expect(chucho::level::INFO_(), wrt_);
}

TEST_F(log_macro, info_lgbl_str)
{
    SCOPED_TRACE("info_lgbl_str");
    get_logger()->set_level(chucho::level::INFO_());
    log_lgbl_str();
    expect(chucho::level::INFO_(), lgbl_wrt_);
}

TEST_F(log_macro, trace)
{
    SCOPED_TRACE("trace");
    lgr_->set_level(chucho::level::TRACE_());
    log();
    expect(chucho::level::TRACE_(), wrt_);
}

TEST_F(log_macro, trace_lgbl)
{
    SCOPED_TRACE("trace_lgbl");
    get_logger()->set_level(chucho::level::TRACE_());
    log_lgbl();
    expect(chucho::level::TRACE_(), lgbl_wrt_);
}

TEST_F(log_macro, trace_marker)
{
    SCOPED_TRACE("trace_marker");
    lgr_->set_level(chucho::level::TRACE_());
    log_marker();
    expect(chucho::level::TRACE_(), wrt_, MARK);
}

TEST_F(log_macro, trace_lgbl_marker)
{
    SCOPED_TRACE("trace_lgbl_marker");
    get_logger()->set_level(chucho::level::TRACE_());
    log_lgbl_marker();
    expect(chucho::level::TRACE_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, trace_marker_str)
{
    SCOPED_TRACE("trace_marker_str");
    lgr_->set_level(chucho::level::TRACE_());
    log_marker_str();
    expect(chucho::level::TRACE_(), wrt_, MARK);
}

TEST_F(log_macro, trace_lgbl_marker_str)
{
    SCOPED_TRACE("trace_lgbl_marker_str");
    get_logger()->set_level(chucho::level::TRACE_());
    log_lgbl_marker_str();
    expect(chucho::level::TRACE_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, trace_str)
{
    SCOPED_TRACE("trace_str");
    lgr_->set_level(chucho::level::TRACE_());
    log_str();
    expect(chucho::level::TRACE_(), wrt_);
}

TEST_F(log_macro, trace_lgbl_str)
{
    SCOPED_TRACE("trace_lgbl_str");
    get_logger()->set_level(chucho::level::TRACE_());
    log_lgbl_str();
    expect(chucho::level::TRACE_(), lgbl_wrt_);
}

TEST_F(log_macro, warn)
{
    SCOPED_TRACE("warn");
    lgr_->set_level(chucho::level::WARN_());
    log();
    expect(chucho::level::WARN_(), wrt_);
}

TEST_F(log_macro, warn_lgbl)
{
    SCOPED_TRACE("warn_lgbl");
    get_logger()->set_level(chucho::level::WARN_());
    log_lgbl();
    expect(chucho::level::WARN_(), lgbl_wrt_);
}

TEST_F(log_macro, warn_marker)
{
    SCOPED_TRACE("warn_marker");
    lgr_->set_level(chucho::level::WARN_());
    log_marker();
    expect(chucho::level::WARN_(), wrt_, MARK);
}

TEST_F(log_macro, warn_lgbl_marker)
{
    SCOPED_TRACE("warn_lgbl_marker");
    get_logger()->set_level(chucho::level::WARN_());
    log_lgbl_marker();
    expect(chucho::level::WARN_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, warn_marker_str)
{
    SCOPED_TRACE("warn_marker_str");
    lgr_->set_level(chucho::level::WARN_());
    log_marker_str();
    expect(chucho::level::WARN_(), wrt_, MARK);
}

TEST_F(log_macro, warn_lgbl_marker_str)
{
    SCOPED_TRACE("warn_lgbl_marker_str");
    get_logger()->set_level(chucho::level::WARN_());
    log_lgbl_marker_str();
    expect(chucho::level::WARN_(), lgbl_wrt_, MARK);
}

TEST_F(log_macro, warn_str)
{
    SCOPED_TRACE("warn_str");
    lgr_->set_level(chucho::level::WARN_());
    log_str();
    expect(chucho::level::WARN_(), wrt_);
}

TEST_F(log_macro, warn_lgbl_str)
{
    SCOPED_TRACE("warn_lgbl_str");
    get_logger()->set_level(chucho::level::WARN_());
    log_lgbl_str();
    expect(chucho::level::WARN_(), lgbl_wrt_);
}
