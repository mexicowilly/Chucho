#include <gtest/gtest.h>
#include <chucho/log.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/marker.hpp>
#include <sstream>
#include <vector>
#include <array>
#include <iostream>

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
            chucho::TRACE_LEVEL,
            chucho::DEBUG_LEVEL,
            chucho::INFO_LEVEL,
            chucho::WARN_LEVEL,
            chucho::ERROR_LEVEL,
            chucho::FATAL_LEVEL
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
    lgr_->set_level(chucho::DEBUG_LEVEL);
    log();
    expect(chucho::DEBUG_LEVEL);
}

TEST_F(log_macro_test, debug_marker)
{
    SCOPED_TRACE("debug_marker");
    lgr_->set_level(chucho::DEBUG_LEVEL);
    log_marker();
    expect(chucho::DEBUG_LEVEL, MARK);
}

TEST_F(log_macro_test, debug_marker_str)
{
    SCOPED_TRACE("debug_str");
    lgr_->set_level(chucho::DEBUG_LEVEL);
    log_marker_str();
    expect(chucho::DEBUG_LEVEL, MARK);
}

TEST_F(log_macro_test, debug_str)
{
    SCOPED_TRACE("debug_str");
    lgr_->set_level(chucho::DEBUG_LEVEL);
    log_str();
    expect(chucho::DEBUG_LEVEL);
}

TEST_F(log_macro_test, error)
{
    SCOPED_TRACE("error");
    lgr_->set_level(chucho::ERROR_LEVEL);
    log();
    expect(chucho::ERROR_LEVEL);
}

TEST_F(log_macro_test, error_marker)
{
    SCOPED_TRACE("error_marker");
    lgr_->set_level(chucho::ERROR_LEVEL);
    log_marker();
    expect(chucho::ERROR_LEVEL, MARK);
}

TEST_F(log_macro_test, error_marker_str)
{
    SCOPED_TRACE("error_str");
    lgr_->set_level(chucho::ERROR_LEVEL);
    log_marker_str();
    expect(chucho::ERROR_LEVEL, MARK);
}

TEST_F(log_macro_test, error_str)
{
    SCOPED_TRACE("error_str");
    lgr_->set_level(chucho::ERROR_LEVEL);
    log_str();
    expect(chucho::ERROR_LEVEL);
}

TEST_F(log_macro_test, fatal)
{
    SCOPED_TRACE("fatal");
    lgr_->set_level(chucho::FATAL_LEVEL);
    log();
    expect(chucho::FATAL_LEVEL);
}

TEST_F(log_macro_test, fatal_marker)
{
    SCOPED_TRACE("fatal_marker");
    lgr_->set_level(chucho::FATAL_LEVEL);
    log_marker();
    expect(chucho::FATAL_LEVEL, MARK);
}

TEST_F(log_macro_test, fatal_marker_str)
{
    SCOPED_TRACE("fatal_str");
    lgr_->set_level(chucho::FATAL_LEVEL);
    log_marker_str();
    expect(chucho::FATAL_LEVEL, MARK);
}

TEST_F(log_macro_test, fatal_str)
{
    SCOPED_TRACE("fatal_str");
    lgr_->set_level(chucho::FATAL_LEVEL);
    log_str();
    expect(chucho::FATAL_LEVEL);
}

TEST_F(log_macro_test, info)
{
    SCOPED_TRACE("info");
    lgr_->set_level(chucho::INFO_LEVEL);
    log();
    expect(chucho::INFO_LEVEL);
}

TEST_F(log_macro_test, info_marker)
{
    SCOPED_TRACE("info_marker");
    lgr_->set_level(chucho::INFO_LEVEL);
    log_marker();
    expect(chucho::INFO_LEVEL, MARK);
}

TEST_F(log_macro_test, info_marker_str)
{
    SCOPED_TRACE("info_str");
    lgr_->set_level(chucho::INFO_LEVEL);
    log_marker_str();
    expect(chucho::INFO_LEVEL, MARK);
}

TEST_F(log_macro_test, info_str)
{
    SCOPED_TRACE("info_str");
    lgr_->set_level(chucho::INFO_LEVEL);
    log_str();
    expect(chucho::INFO_LEVEL);
}

TEST_F(log_macro_test, trace)
{
    SCOPED_TRACE("trace");
    lgr_->set_level(chucho::TRACE_LEVEL);
    log();
    expect(chucho::TRACE_LEVEL);
}

TEST_F(log_macro_test, trace_marker)
{
    SCOPED_TRACE("trace_marker");
    lgr_->set_level(chucho::TRACE_LEVEL);
    log_marker();
    expect(chucho::TRACE_LEVEL, MARK);
}

TEST_F(log_macro_test, trace_marker_str)
{
    SCOPED_TRACE("trace_str");
    lgr_->set_level(chucho::TRACE_LEVEL);
    log_marker_str();
    expect(chucho::TRACE_LEVEL, MARK);
}

TEST_F(log_macro_test, trace_str)
{
    SCOPED_TRACE("trace_str");
    lgr_->set_level(chucho::TRACE_LEVEL);
    log_str();
    expect(chucho::TRACE_LEVEL);
}

TEST_F(log_macro_test, warn)
{
    SCOPED_TRACE("warn");
    lgr_->set_level(chucho::WARN_LEVEL);
    log();
    expect(chucho::WARN_LEVEL);
}

TEST_F(log_macro_test, warn_marker)
{
    SCOPED_TRACE("warn_marker");
    lgr_->set_level(chucho::TRACE_LEVEL);
    log_marker();
    expect(chucho::TRACE_LEVEL, MARK);
}

TEST_F(log_macro_test, warn_marker_str)
{
    SCOPED_TRACE("warn_str");
    lgr_->set_level(chucho::TRACE_LEVEL);
    log_marker_str();
    expect(chucho::TRACE_LEVEL, MARK);
}

TEST_F(log_macro_test, warn_str)
{
    SCOPED_TRACE("warn_str");
    lgr_->set_level(chucho::TRACE_LEVEL);
    log_str();
    expect(chucho::TRACE_LEVEL);
}
