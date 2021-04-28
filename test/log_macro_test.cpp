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
    string_writer(std::unique_ptr<chucho::formatter>&& fmt)
        : writer("string", std::move(fmt)) { }

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
    enum class expected_logger
    {
        INTRINSIC,
        EXPLICIT
    };

    log_macro()
        : lgr_(chucho::logger::get("log_macro"))
    {
        auto wrt = std::make_unique<string_writer>(std::move(std::make_unique<chucho::pattern_formatter>("%p %m %k%n")));
        lgr_->add_writer(std::move(wrt));
        wrt = std::make_unique<string_writer>(std::move(std::make_unique<chucho::pattern_formatter>("%p %m %k%n")));
        get_logger()->add_writer(std::move(wrt));
    }

    ~log_macro()
    {
        lgr_.reset();
        get_logger()->clear_writers();
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

    void log_l()
    {
        CHUCHO_TRACE_L("my dog " << "has fleas");
        CHUCHO_DEBUG_L("my dog " << "has fleas");
        CHUCHO_INFO_L("my dog " << "has fleas");
        CHUCHO_WARN_L("my dog " << "has fleas");
        CHUCHO_ERROR_L("my dog " << "has fleas");
        CHUCHO_FATAL_L("my dog " << "has fleas");
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

    void log_l_marker()
    {
        CHUCHO_TRACE_L_M(MARK, "my dog " << "has fleas");
        CHUCHO_DEBUG_L_M(MARK, "my dog " << "has fleas");
        CHUCHO_INFO_L_M(MARK, "my dog " << "has fleas");
        CHUCHO_WARN_L_M(MARK, "my dog " << "has fleas");
        CHUCHO_ERROR_L_M(MARK, "my dog " << "has fleas");
        CHUCHO_FATAL_L_M(MARK, "my dog " << "has fleas");
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

    void log_l_marker_str()
    {
        CHUCHO_TRACE_L_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_DEBUG_L_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_INFO_L_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_WARN_L_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_ERROR_L_STR_M(MARK->get_name(), "my dog has fleas");
        CHUCHO_FATAL_L_STR_M(MARK->get_name(), "my dog has fleas");
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

    void log_l_str()
    {
        CHUCHO_TRACE_L_STR("my dog has fleas");
        CHUCHO_DEBUG_L_STR("my dog has fleas");
        CHUCHO_INFO_L_STR("my dog has fleas");
        CHUCHO_WARN_L_STR("my dog has fleas");
        CHUCHO_ERROR_L_STR("my dog has fleas");
        CHUCHO_FATAL_L_STR("my dog has fleas");
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
                expected_logger exp,
                const chucho::optional<chucho::marker>& mark = chucho::optional<chucho::marker>(),
                std::size_t lines_per_lvl = 1)
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
        chucho::writer& wrt = exp == expected_logger::EXPLICIT ?
            lgr_->get_writer("string") : get_logger()->get_writer("string");
        std::vector<std::string> lines(dynamic_cast<string_writer&>(wrt).get_lines());
        auto i = std::distance(lvls.begin(), std::find(lvls.begin(), lvls.end(), lvl));
        ASSERT_EQ((lvls.size() - i) * lines_per_lvl, lines.size());
        for (std::size_t cur = 0; cur < lines.size(); cur++)
        {
            if (cur > 0 && (cur % lines_per_lvl) == 0)
                ++i;
//            std::cout << "cur " << cur << " '" << lines[cur] << "'" << std::endl;
//            std::cout << "lvl " << i << " " << lvls[i]->get_name() << std::endl;
            EXPECT_STREQ(lvls[i]->get_name(), lines[cur].substr(0, std::strlen(lvls[i]->get_name())).c_str());
            if (mark)
                EXPECT_NE(std::string::npos, lines[cur].find(mark->get_name()));
        }
    }

    std::shared_ptr<chucho::logger> lgr_;
};

}

TEST_F(log_macro, debug)
{
    SCOPED_TRACE("debug");
    lgr_->set_level(chucho::level::DEBUG_());
    log();
    expect(chucho::level::DEBUG_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, debug_l)
{
    SCOPED_TRACE("debug_l");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_l();
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, debug_lgbl)
{
    SCOPED_TRACE("debug_lgbl");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_lgbl();
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, debug_marker)
{
    SCOPED_TRACE("debug_marker");
    lgr_->set_level(chucho::level::DEBUG_());
    log_marker();
    expect(chucho::level::DEBUG_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, debug_l_marker)
{
    SCOPED_TRACE("debug_l_marker");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_l_marker();
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, debug_lgbl_marker)
{
    SCOPED_TRACE("debug_lgbl_marker");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_lgbl_marker();
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, debug_marker_str)
{
    SCOPED_TRACE("debug_marker_str");
    lgr_->set_level(chucho::level::DEBUG_());
    log_marker_str();
    expect(chucho::level::DEBUG_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, debug_l_marker_str)
{
    SCOPED_TRACE("debug_l_marker_str");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_l_marker_str();
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, debug_lgbl_marker_str)
{
    SCOPED_TRACE("debug_lgbl_marker_str");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_lgbl_marker_str();
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, debug_str)
{
    SCOPED_TRACE("debug_str");
    lgr_->set_level(chucho::level::DEBUG_());
    log_str();
    expect(chucho::level::DEBUG_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, debug_l_str)
{
    SCOPED_TRACE("debug_l_str");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_l_str();
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, debug_lgbl_str)
{
    SCOPED_TRACE("debug_lgbl_str");
    get_logger()->set_level(chucho::level::DEBUG_());
    log_lgbl_str();
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, debug_every_n)
{
    SCOPED_TRACE("debug_every_n");
    lgr_->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(TRACE, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(DEBUG, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(INFO, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(WARN, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(ERROR, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(FATAL, 10, lgr_, "my dog " << "has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, debug_every_n_lgbl)
{
    SCOPED_TRACE("debug_every_n_lgbl");
    get_logger()->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, debug_every_n_l)
{
    SCOPED_TRACE("debug_every_n_l");
    get_logger()->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::DEBUG_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, debug_every_n_str)
{
    SCOPED_TRACE("debug_every_n_str");
    lgr_->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(TRACE, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(DEBUG, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(INFO, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(WARN, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(ERROR, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(FATAL, 1000, lgr_, "my dog has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, debug_every_n_lgbl_str)
{
    SCOPED_TRACE("debug_every_n_lgbl_str");
    get_logger()->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, debug_every_n_l_str)
{
    SCOPED_TRACE("debug_every_n_l_str");
    get_logger()->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, debug_every_n_m)
{
    SCOPED_TRACE("debug_every_n_m");
    lgr_->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(TRACE, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(DEBUG, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(INFO, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(WARN, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(ERROR, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(FATAL, 10, MARK, lgr_, "my dog " << "has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, debug_every_n_lgbl_m)
{
    SCOPED_TRACE("debug_every_n_lgbl_m");
    get_logger()->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, debug_every_n_l_m)
{
    SCOPED_TRACE("debug_every_n_l_m");
    get_logger()->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, debug_every_n_str_m)
{
    SCOPED_TRACE("debug_every_n_str_m");
    lgr_->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(TRACE, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(DEBUG, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(INFO, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(WARN, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(ERROR, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(FATAL, 1000, MARK, lgr_, "my dog has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, debug_every_n_lgbl_str_m)
{
    SCOPED_TRACE("debug_every_n_lgbl_str_m");
    get_logger()->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, debug_every_n_l_str_m)
{
    SCOPED_TRACE("debug_every_n_l_str_m");
    get_logger()->set_level(chucho::level::DEBUG_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::DEBUG_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, error)
{
    SCOPED_TRACE("error");
    lgr_->set_level(chucho::level::ERROR_());
    log();
    expect(chucho::level::ERROR_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, error_l)
{
    SCOPED_TRACE("error_l");
    get_logger()->set_level(chucho::level::ERROR_());
    log_l();
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, error_lgbl)
{
    SCOPED_TRACE("error_lgbl");
    get_logger()->set_level(chucho::level::ERROR_());
    log_lgbl();
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, error_marker)
{
    SCOPED_TRACE("error_marker");
    lgr_->set_level(chucho::level::ERROR_());
    log_marker();
    expect(chucho::level::ERROR_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, error_l_marker)
{
    SCOPED_TRACE("error_l_marker");
    get_logger()->set_level(chucho::level::ERROR_());
    log_l_marker();
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, error_lgbl_marker)
{
    SCOPED_TRACE("error_lgbl_marker");
    get_logger()->set_level(chucho::level::ERROR_());
    log_lgbl_marker();
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, error_marker_str)
{
    SCOPED_TRACE("error_marker_str");
    lgr_->set_level(chucho::level::ERROR_());
    log_marker_str();
    expect(chucho::level::ERROR_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, error_l_marker_str)
{
    SCOPED_TRACE("error_l_marker_str");
    get_logger()->set_level(chucho::level::ERROR_());
    log_l_marker_str();
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, error_lgbl_marker_str)
{
    SCOPED_TRACE("error_lgbl_marker_str");
    get_logger()->set_level(chucho::level::ERROR_());
    log_lgbl_marker_str();
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, error_str)
{
    SCOPED_TRACE("error_str");
    lgr_->set_level(chucho::level::ERROR_());
    log_str();
    expect(chucho::level::ERROR_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, error_l_str)
{
    SCOPED_TRACE("error_l_str");
    get_logger()->set_level(chucho::level::ERROR_());
    log_l_str();
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, error_lgbl_str)
{
    SCOPED_TRACE("error_lgbl_str");
    get_logger()->set_level(chucho::level::ERROR_());
    log_lgbl_str();
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, error_every_n)
{
    SCOPED_TRACE("error_every_n");
    lgr_->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(TRACE, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(DEBUG, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(INFO, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(WARN, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(ERROR, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(FATAL, 10, lgr_, "my dog " << "has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, error_every_n_lgbl)
{
    SCOPED_TRACE("error_every_n_lgbl");
    get_logger()->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, error_every_n_l)
{
    SCOPED_TRACE("error_every_n_l");
    get_logger()->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::ERROR_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, error_every_n_str)
{
    SCOPED_TRACE("error_every_n_str");
    lgr_->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(TRACE, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(DEBUG, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(INFO, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(WARN, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(ERROR, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(FATAL, 1000, lgr_, "my dog has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, error_every_n_lgbl_str)
{
    SCOPED_TRACE("error_every_n_lgbl_str");
    get_logger()->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, error_every_n_l_str)
{
    SCOPED_TRACE("error_every_n_l_str");
    get_logger()->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, error_every_n_m)
{
    SCOPED_TRACE("error_every_n_m");
    lgr_->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(TRACE, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(DEBUG, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(INFO, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(WARN, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(ERROR, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(FATAL, 10, MARK, lgr_, "my dog " << "has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, error_every_n_lgbl_m)
{
    SCOPED_TRACE("error_every_n_lgbl_m");
    get_logger()->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, error_every_n_l_m)
{
    SCOPED_TRACE("error_every_n_l_m");
    get_logger()->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, error_every_n_str_m)
{
    SCOPED_TRACE("error_every_n_str_m");
    lgr_->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(TRACE, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(DEBUG, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(INFO, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(WARN, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(ERROR, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(FATAL, 1000, MARK, lgr_, "my dog has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, error_every_n_lgbl_str_m)
{
    SCOPED_TRACE("error_every_n_lgbl_str_m");
    get_logger()->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, error_every_n_l_str_m)
{
    SCOPED_TRACE("error_every_n_l_str_m");
    get_logger()->set_level(chucho::level::ERROR_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::ERROR_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, fatal)
{
    SCOPED_TRACE("fatal");
    lgr_->set_level(chucho::level::FATAL_());
    log();
    expect(chucho::level::FATAL_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, fatal_l)
{
    SCOPED_TRACE("fatal_l");
    get_logger()->set_level(chucho::level::FATAL_());
    log_l();
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, fatal_lgbl)
{
    SCOPED_TRACE("fatal_lgbl");
    get_logger()->set_level(chucho::level::FATAL_());
    log_lgbl();
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, fatal_marker)
{
    SCOPED_TRACE("fatal_marker");
    lgr_->set_level(chucho::level::FATAL_());
    log_marker();
    expect(chucho::level::FATAL_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, fatal_l_marker)
{
    SCOPED_TRACE("fatal_l_marker");
    get_logger()->set_level(chucho::level::FATAL_());
    log_l_marker();
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, fatal_lgbl_marker)
{
    SCOPED_TRACE("fatal_lgbl_marker");
    get_logger()->set_level(chucho::level::FATAL_());
    log_lgbl_marker();
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, fatal_marker_str)
{
    SCOPED_TRACE("fatal_marker_str");
    lgr_->set_level(chucho::level::FATAL_());
    log_marker_str();
    expect(chucho::level::FATAL_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, fatal_l_marker_str)
{
    SCOPED_TRACE("fatal_l_marker_str");
    get_logger()->set_level(chucho::level::FATAL_());
    log_l_marker_str();
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, fatal_lgbl_marker_str)
{
    SCOPED_TRACE("fatal_lgbl_marker_str");
    get_logger()->set_level(chucho::level::FATAL_());
    log_lgbl_marker_str();
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, fatal_str)
{
    SCOPED_TRACE("fatal_str");
    lgr_->set_level(chucho::level::FATAL_());
    log_str();
    expect(chucho::level::FATAL_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, fatal_l_str)
{
    SCOPED_TRACE("fatal_l_str");
    get_logger()->set_level(chucho::level::FATAL_());
    log_l_str();
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, fatal_lgbl_str)
{
    SCOPED_TRACE("fatal_lgbl_str");
    get_logger()->set_level(chucho::level::FATAL_());
    log_lgbl_str();
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, fatal_every_n)
{
    SCOPED_TRACE("fatal_every_n");
    lgr_->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(TRACE, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(DEBUG, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(INFO, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(WARN, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(ERROR, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(FATAL, 10, lgr_, "my dog " << "has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, fatal_every_n_lgbl)
{
    SCOPED_TRACE("fatal_every_n_lgbl");
    get_logger()->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, fatal_every_n_l)
{
    SCOPED_TRACE("fatal_every_n_l");
    get_logger()->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::FATAL_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, fatal_every_n_str)
{
    SCOPED_TRACE("fatal_every_n_str");
    lgr_->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(TRACE, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(DEBUG, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(INFO, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(WARN, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(ERROR, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(FATAL, 1000, lgr_, "my dog has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, fatal_every_n_lgbl_str)
{
    SCOPED_TRACE("fatal_every_n_lgbl_str");
    get_logger()->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, fatal_every_n_l_str)
{
    SCOPED_TRACE("fatal_every_n_l_str");
    get_logger()->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, fatal_every_n_m)
{
    SCOPED_TRACE("fatal_every_n_m");
    lgr_->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(TRACE, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(DEBUG, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(INFO, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(WARN, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(ERROR, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(FATAL, 10, MARK, lgr_, "my dog " << "has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, fatal_every_n_lgbl_m)
{
    SCOPED_TRACE("fatal_every_n_lgbl_m");
    get_logger()->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, fatal_every_n_l_m)
{
    SCOPED_TRACE("fatal_every_n_l_m");
    get_logger()->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, fatal_every_n_str_m)
{
    SCOPED_TRACE("fatal_every_n_str_m");
    lgr_->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(TRACE, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(DEBUG, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(INFO, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(WARN, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(ERROR, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(FATAL, 1000, MARK, lgr_, "my dog has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, fatal_every_n_lgbl_str_m)
{
    SCOPED_TRACE("fatal_every_n_lgbl_str_m");
    get_logger()->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, fatal_every_n_l_str_m)
{
    SCOPED_TRACE("fatal_every_n_l_str_m");
    get_logger()->set_level(chucho::level::FATAL_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::FATAL_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, info)
{
    SCOPED_TRACE("info");
    lgr_->set_level(chucho::level::INFO_());
    log();
    expect(chucho::level::INFO_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, info_l)
{
    SCOPED_TRACE("info_l");
    get_logger()->set_level(chucho::level::INFO_());
    log_l();
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, info_lgbl)
{
    SCOPED_TRACE("info_lgbl");
    get_logger()->set_level(chucho::level::INFO_());
    log_lgbl();
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, info_marker)
{
    SCOPED_TRACE("info_marker");
    lgr_->set_level(chucho::level::INFO_());
    log_marker();
    expect(chucho::level::INFO_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, info_l_marker)
{
    SCOPED_TRACE("info_l_marker");
    get_logger()->set_level(chucho::level::INFO_());
    log_l_marker();
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, info_lgbl_marker)
{
    SCOPED_TRACE("info_lgbl_marker");
    get_logger()->set_level(chucho::level::INFO_());
    log_lgbl_marker();
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, info_marker_str)
{
    SCOPED_TRACE("info_marker_str");
    lgr_->set_level(chucho::level::INFO_());
    log_marker_str();
    expect(chucho::level::INFO_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, info_l_marker_str)
{
    SCOPED_TRACE("info_l_marker_str");
    get_logger()->set_level(chucho::level::INFO_());
    log_l_marker_str();
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, info_lgbl_marker_str)
{
    SCOPED_TRACE("info_lgbl_marker_str");
    get_logger()->set_level(chucho::level::INFO_());
    log_lgbl_marker_str();
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, info_str)
{
    SCOPED_TRACE("info_str");
    lgr_->set_level(chucho::level::INFO_());
    log_str();
    expect(chucho::level::INFO_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, info_l_str)
{
    SCOPED_TRACE("info_l_str");
    get_logger()->set_level(chucho::level::INFO_());
    log_l_str();
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, info_lgbl_str)
{
    SCOPED_TRACE("info_lgbl_str");
    get_logger()->set_level(chucho::level::INFO_());
    log_lgbl_str();
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, info_every_n)
{
    SCOPED_TRACE("info_every_n");
    lgr_->set_level(chucho::level::INFO_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(TRACE, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(DEBUG, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(INFO, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(WARN, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(ERROR, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(FATAL, 10, lgr_, "my dog " << "has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, info_every_n_lgbl)
{
    SCOPED_TRACE("info_every_n_lgbl");
    get_logger()->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, info_every_n_l)
{
    SCOPED_TRACE("info_every_n_l");
    get_logger()->set_level(chucho::level::INFO_());
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::INFO_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, info_every_n_str)
{
    SCOPED_TRACE("info_every_n_str");
    lgr_->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(TRACE, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(DEBUG, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(INFO, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(WARN, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(ERROR, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(FATAL, 1000, lgr_, "my dog has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, info_every_n_lgbl_str)
{
    SCOPED_TRACE("info_every_n_lgbl_str");
    get_logger()->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, info_every_n_l_str)
{
    SCOPED_TRACE("info_every_n_l_str");
    get_logger()->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, info_every_n_m)
{
    SCOPED_TRACE("info_every_n_m");
    lgr_->set_level(chucho::level::INFO_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(TRACE, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(DEBUG, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(INFO, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(WARN, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(ERROR, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(FATAL, 10, MARK, lgr_, "my dog " << "has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, info_every_n_lgbl_m)
{
    SCOPED_TRACE("info_every_n_lgbl_m");
    get_logger()->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, info_every_n_l_m)
{
    SCOPED_TRACE("info_every_n_l_m");
    get_logger()->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, info_every_n_str_m)
{
    SCOPED_TRACE("info_every_n_str_m");
    lgr_->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(TRACE, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(DEBUG, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(INFO, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(WARN, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(ERROR, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(FATAL, 1000, MARK, lgr_, "my dog has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, info_every_n_lgbl_str_m)
{
    SCOPED_TRACE("info_every_n_lgbl_str_m");
    get_logger()->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, info_every_n_l_str_m)
{
    SCOPED_TRACE("info_every_n_l_str_m");
    get_logger()->set_level(chucho::level::INFO_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::INFO_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, trace)
{
    SCOPED_TRACE("trace");
    lgr_->set_level(chucho::level::TRACE_());
    log();
    expect(chucho::level::TRACE_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, trace_l)
{
    SCOPED_TRACE("trace_l");
    get_logger()->set_level(chucho::level::TRACE_());
    log_l();
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, trace_lgbl)
{
    SCOPED_TRACE("trace_lgbl");
    get_logger()->set_level(chucho::level::TRACE_());
    log_lgbl();
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, trace_marker)
{
    SCOPED_TRACE("trace_marker");
    lgr_->set_level(chucho::level::TRACE_());
    log_marker();
    expect(chucho::level::TRACE_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, trace_l_marker)
{
    SCOPED_TRACE("trace_l_marker");
    get_logger()->set_level(chucho::level::TRACE_());
    log_l_marker();
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, trace_lgbl_marker)
{
    SCOPED_TRACE("trace_lgbl_marker");
    get_logger()->set_level(chucho::level::TRACE_());
    log_lgbl_marker();
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, trace_marker_str)
{
    SCOPED_TRACE("trace_marker_str");
    lgr_->set_level(chucho::level::TRACE_());
    log_marker_str();
    expect(chucho::level::TRACE_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, trace_l_marker_str)
{
    SCOPED_TRACE("trace_l_marker_str");
    get_logger()->set_level(chucho::level::TRACE_());
    log_l_marker_str();
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, trace_lgbl_marker_str)
{
    SCOPED_TRACE("trace_lgbl_marker_str");
    get_logger()->set_level(chucho::level::TRACE_());
    log_lgbl_marker_str();
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, trace_str)
{
    SCOPED_TRACE("trace_str");
    lgr_->set_level(chucho::level::TRACE_());
    log_str();
    expect(chucho::level::TRACE_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, trace_l_str)
{
    SCOPED_TRACE("trace_l_str");
    get_logger()->set_level(chucho::level::TRACE_());
    log_l_str();
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, trace_lgbl_str)
{
    SCOPED_TRACE("trace_lgbl_str");
    get_logger()->set_level(chucho::level::TRACE_());
    log_lgbl_str();
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, trace_every_n)
{
    SCOPED_TRACE("trace_every_n");
    lgr_->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(TRACE, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(DEBUG, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(INFO, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(WARN, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(ERROR, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(FATAL, 10, lgr_, "my dog " << "has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, trace_every_n_lgbl)
{
    SCOPED_TRACE("trace_every_n_lgbl");
    get_logger()->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, trace_every_n_l)
{
    SCOPED_TRACE("trace_every_n_l");
    get_logger()->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::TRACE_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, trace_every_n_str)
{
    SCOPED_TRACE("trace_every_n_str");
    lgr_->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(TRACE, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(DEBUG, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(INFO, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(WARN, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(ERROR, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(FATAL, 1000, lgr_, "my dog has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, trace_every_n_lgbl_str)
{
    SCOPED_TRACE("trace_every_n_lgbl_str");
    get_logger()->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, trace_every_n_l_str)
{
    SCOPED_TRACE("trace_every_n_l_str");
    get_logger()->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, trace_every_n_m)
{
    SCOPED_TRACE("trace_every_n_m");
    lgr_->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(TRACE, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(DEBUG, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(INFO, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(WARN, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(ERROR, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(FATAL, 10, MARK, lgr_, "my dog " << "has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, trace_every_n_lgbl_m)
{
    SCOPED_TRACE("trace_every_n_lgbl_m");
    get_logger()->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, trace_every_n_l_m)
{
    SCOPED_TRACE("trace_every_n_l_m");
    get_logger()->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, trace_every_n_str_m)
{
    SCOPED_TRACE("trace_every_n_str_m");
    lgr_->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(TRACE, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(DEBUG, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(INFO, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(WARN, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(ERROR, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(FATAL, 1000, MARK, lgr_, "my dog has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, trace_every_n_lgbl_str_m)
{
    SCOPED_TRACE("trace_every_n_lgbl_str_m");
    get_logger()->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, trace_every_n_l_str_m)
{
    SCOPED_TRACE("trace_every_n_l_str_m");
    get_logger()->set_level(chucho::level::TRACE_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::TRACE_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, warn)
{
    SCOPED_TRACE("warn");
    lgr_->set_level(chucho::level::WARN_());
    log();
    expect(chucho::level::WARN_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, warn_l)
{
    SCOPED_TRACE("warn_l");
    get_logger()->set_level(chucho::level::WARN_());
    log_l();
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, warn_lgbl)
{
    SCOPED_TRACE("warn_lgbl");
    get_logger()->set_level(chucho::level::WARN_());
    log_lgbl();
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, warn_marker)
{
    SCOPED_TRACE("warn_marker");
    lgr_->set_level(chucho::level::WARN_());
    log_marker();
    expect(chucho::level::WARN_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, warn_l_marker)
{
    SCOPED_TRACE("warn_l_marker");
    get_logger()->set_level(chucho::level::WARN_());
    log_l_marker();
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, warn_lgbl_marker)
{
    SCOPED_TRACE("warn_lgbl_marker");
    get_logger()->set_level(chucho::level::WARN_());
    log_lgbl_marker();
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, warn_marker_str)
{
    SCOPED_TRACE("warn_marker_str");
    lgr_->set_level(chucho::level::WARN_());
    log_marker_str();
    expect(chucho::level::WARN_(), expected_logger::EXPLICIT, MARK);
}

TEST_F(log_macro, warn_l_marker_str)
{
    SCOPED_TRACE("warn_l_marker_str");
    get_logger()->set_level(chucho::level::WARN_());
    log_l_marker_str();
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, warn_lgbl_marker_str)
{
    SCOPED_TRACE("warn_lgbl_marker_str");
    get_logger()->set_level(chucho::level::WARN_());
    log_lgbl_marker_str();
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC, MARK);
}

TEST_F(log_macro, warn_str)
{
    SCOPED_TRACE("warn_str");
    lgr_->set_level(chucho::level::WARN_());
    log_str();
    expect(chucho::level::WARN_(), expected_logger::EXPLICIT);
}

TEST_F(log_macro, warn_l_str)
{
    SCOPED_TRACE("warn_l_str");
    get_logger()->set_level(chucho::level::WARN_());
    log_l_str();
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, warn_lgbl_str)
{
    SCOPED_TRACE("warn_lgbl_str");
    get_logger()->set_level(chucho::level::WARN_());
    log_lgbl_str();
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, warn_every_n)
{
    SCOPED_TRACE("warn_every_n");
    lgr_->set_level(chucho::level::WARN_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(TRACE, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(DEBUG, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(INFO, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(WARN, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(ERROR, 10, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N(FATAL, 10, lgr_, "my dog " << "has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, warn_every_n_lgbl)
{
    SCOPED_TRACE("warn_every_n_lgbl");
    get_logger()->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, warn_every_n_l)
{
    SCOPED_TRACE("warn_every_n_l");
    get_logger()->set_level(chucho::level::WARN_());
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(TRACE, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(DEBUG, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(INFO, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(WARN, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(ERROR, 1000, "my dog " << "has fleas");
    for (int i = 0; i < 800; i++)
        CHUCHO_EVERY_N_L(FATAL, 1000, "my dog " << "has fleas");
    expect(chucho::level::WARN_(), expected_logger::INTRINSIC);
}

TEST_F(log_macro, warn_every_n_str)
{
    SCOPED_TRACE("warn_every_n_str");
    lgr_->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(TRACE, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(DEBUG, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(INFO, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(WARN, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(ERROR, 1000, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR(FATAL, 1000, lgr_, "my dog has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::EXPLICIT,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, warn_every_n_lgbl_str)
{
    SCOPED_TRACE("warn_every_n_lgbl_str");
    get_logger()->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, warn_every_n_l_str)
{
    SCOPED_TRACE("warn_every_n_l_str");
    get_logger()->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(TRACE, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(DEBUG, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(INFO, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(WARN, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(ERROR, 1000, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR(FATAL, 1000, "my dog has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::INTRINSIC,
           chucho::optional<chucho::marker>(),
           3);
}

TEST_F(log_macro, warn_every_n_m)
{
    SCOPED_TRACE("warn_every_n_m");
    lgr_->set_level(chucho::level::WARN_());
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(TRACE, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(DEBUG, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(INFO, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(WARN, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(ERROR, 10, MARK, lgr_, "my dog " << "has fleas");
    for (int i = 0; i < 25; i++)
        CHUCHO_EVERY_N_M(FATAL, 10, MARK, lgr_, "my dog " << "has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, warn_every_n_lgbl_m)
{
    SCOPED_TRACE("warn_every_n_lgbl_m");
    get_logger()->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, warn_every_n_l_m)
{
    SCOPED_TRACE("warn_every_n_l_m");
    get_logger()->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(TRACE, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(DEBUG, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(INFO, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(WARN, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(ERROR, 1000, MARK, "my dog " << "has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_M(FATAL, 1000, MARK, "my dog " << "has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, warn_every_n_str_m)
{
    SCOPED_TRACE("warn_every_n_str_m");
    lgr_->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(TRACE, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(DEBUG, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(INFO, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(WARN, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(ERROR, 1000, MARK, lgr_, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_STR_M(FATAL, 1000, MARK, lgr_, "my dog has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::EXPLICIT,
           MARK,
           3);
}

TEST_F(log_macro, warn_every_n_lgbl_str_m)
{
    SCOPED_TRACE("warn_every_n_lgbl_str_m");
    get_logger()->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_LGBL_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

TEST_F(log_macro, warn_every_n_l_str_m)
{
    SCOPED_TRACE("warn_every_n_l_str_m");
    get_logger()->set_level(chucho::level::WARN_());
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(TRACE, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(DEBUG, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(INFO, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(WARN, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(ERROR, 1000, MARK, "my dog has fleas");
    for (int i = 0; i < 2500; i++)
        CHUCHO_EVERY_N_L_STR_M(FATAL, 1000, MARK, "my dog has fleas");
    expect(chucho::level::WARN_(),
           expected_logger::INTRINSIC,
           MARK,
           3);
}

