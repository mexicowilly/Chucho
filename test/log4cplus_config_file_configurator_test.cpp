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

#include "configurator_test.hpp"
#include <chucho/config_file_configurator.hpp>
#include <chucho/logger.hpp>
#include <chucho/level_filter.hpp>
#include <chucho/rolling_file_writer.hpp>
#include <chucho/numbered_file_roller.hpp>
#include <chucho/remote_writer.hpp>
#include <chucho/status_manager.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/event.hpp>
#include <chucho/line_ending.hpp>
#include <chucho/calendar.hpp>
#include <chucho/time_file_roller.hpp>
#include <chucho/configuration.hpp>
#include <thread>

namespace
{

class log4cplus_config_file_configurator : public chucho::test::configurator
{
protected:
    log4cplus_config_file_configurator();

    virtual chucho::configurator& get_configurator() override;

private:
    chucho::config_file_configurator cnf_;
};

log4cplus_config_file_configurator::log4cplus_config_file_configurator()
    : cnf_(chucho::configuration::get_security_policy())
{
}

chucho::configurator& log4cplus_config_file_configurator::get_configurator()
{
    return cnf_;
}

}

TEST_F(log4cplus_config_file_configurator, async_writer)
{
    configure("log4cplus.logger.will = info, as\n"
              "log4cplus.appender.as = log4cplus::AsyncAppender\n"
              "log4cplus.appender.as.Appender = fw\n"
              "log4cplus.appender.fw = log4cplus::FileAppender\n"
              "log4cplus.appender.fw.File = hello.log\n"
              "log4cplus.appender.fw.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.fw.layout.ConversionPattern = %m%n\n");
    async_writer_body();
}

TEST_F(log4cplus_config_file_configurator, cerr_writer)
{
    configure("log4cplus.logger.will = info, cerr\n"
              "log4cplus.appender.cerr = log4cplus::ConsoleAppender\n"
              "log4cplus.appender.cerr.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.cerr.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.cerr.logToStdErr = true");
    cerr_writer_body();
}

TEST_F(log4cplus_config_file_configurator, cout_writer)
{
    configure("log4cplus.logger.will = info, cerr\n"
              "log4cplus.appender.cerr = log4cplus::ConsoleAppender\n"
              "log4cplus.appender.cerr.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.cerr.layout.ConversionPattern = %m%n");
    cout_writer_body();
}

TEST_F(log4cplus_config_file_configurator, daily_rolling_file_appender)
{
    std::pair<const char*, const char*> exp[] =
    {
        std::make_pair("monthly", "%Y-%m"),
        std::make_pair("WEEKLY", "%Y-%U"),
        std::make_pair("HoUrLy", "%Y-%m-%d %H"),
        std::make_pair("HoUrLy", "%Y-%m-%d %H"),
        std::make_pair("mInUtElY", "%Y-%m-%d %H:%M"),
        std::make_pair("daily", "%Y-%m-%d"),
        std::make_pair("never", "%Y-%m-%d"),
        std::make_pair(nullptr, nullptr)
    };
    std::string tmpl("log4cplus.logger.will = info, drf\n"
                     "log4cplus.appender.drf = log4cplus::DailyRollingFileAppender\n"
                     "log4cplus.appender.drf.layout = log4cplus::PatternLayout\n"
                     "log4cplus.appender.drf.layout.ConversionPattern = %m%n\n"
                     "log4cplus.appender.drf.Schedule = SCHEDULE");
    std::size_t rep_loc = tmpl.find("SCHEDULE");
    int i = 0;
    while (exp[i].first != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(rep_loc, 8, exp[i].first);
        struct std::tm now = chucho::calendar::get_utc(time(nullptr));
        configure(rep.c_str());
        auto& rfw = dynamic_cast<chucho::rolling_file_writer&>(
            chucho::logger::get("will")->get_writer("chucho::rolling_file_writer"));
        auto& tfr = dynamic_cast<chucho::time_file_roller&>(rfw.get_file_roller());
        std::string fn = tfr.get_active_file_name();
        EXPECT_EQ("." + chucho::calendar::format(now, exp[i].second), fn);
        i++;
    }
    // clear the status because we generated some warnings
    chucho::status_manager::get()->clear();
}

TEST_F(log4cplus_config_file_configurator, file_writer)
{
    configure("log4cplus.logger.will = info, fw\n"
              "log4cplus.appender.fw = log4cplus::FileAppender\n"
              "log4cplus.appender.fw.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.fw.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.fw.File = hello.log\n"
              "log4cplus.appender.fw.ImmediateFlush = false");
    file_writer_body();
}

TEST_F(log4cplus_config_file_configurator, level_filter_accept)
{
    configure("log4cplus.logger.will = info, fl\n"
              "log4cplus.appender.fl = log4cplus::FileAppender\n"
              "log4cplus.appender.fl.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.fl.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.fl.filters.1 = log4cplus::spi::LogLevelMatchFilter\n"
              "log4cplus.appender.fl.filters.1.LogLevelToMatch = error\n"
              "log4cplus.appender.fl.filters.1.AcceptOnMatch = true\n"
              "log4cplus.appender.fl.File = hello.log");
    auto lgr = chucho::logger::get("will");
    auto wrts = lgr->get_writer_names();
    ASSERT_EQ(1, wrts.size());
    auto& wrt = lgr->get_writer(wrts[0]);
    auto flts = wrt.get_filter_names();
    ASSERT_EQ(1, flts.size());
    auto& lf = dynamic_cast<chucho::level_filter&>(wrt.get_filter(flts[0]));
    EXPECT_EQ(*chucho::level::ERROR_(), *lf.get_level());
    EXPECT_EQ(chucho::filter::result::ACCEPT, lf.get_on_match());
    EXPECT_EQ(chucho::filter::result::NEUTRAL, lf.get_on_mismatch());
}

TEST_F(log4cplus_config_file_configurator, level_filter_deny)
{
    configure("log4cplus.logger.will = info, cout\n"
              "log4cplus.appender.cout = log4cplus::ConsoleAppender\n"
              "log4cplus.appender.cout.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.cout.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.cout.filters.1 = log4cplus::spi::LogLevelMatchFilter\n"
              "log4cplus.appender.cout.filters.1.LogLevelToMatch = error");
    auto lgr = chucho::logger::get("will");
    auto wrts = lgr->get_writer_names();
    ASSERT_EQ(1, wrts.size());
    auto& wrt = lgr->get_writer(wrts[0]);
    auto flts = wrt.get_filter_names();
    ASSERT_EQ(1, flts.size());
    auto& lf = dynamic_cast<chucho::level_filter&>(wrt.get_filter(flts[0]));
    EXPECT_EQ(*chucho::level::ERROR_(), *lf.get_level());
    EXPECT_EQ(chucho::filter::result::DENY, lf.get_on_match());
    EXPECT_EQ(chucho::filter::result::NEUTRAL, lf.get_on_mismatch());
}

TEST_F(log4cplus_config_file_configurator, level_threshold_filter)
{
    configure("log4cplus.logger.will = info, cout\n"
              "log4cplus.appender.cout = log4cplus::ConsoleAppender\n"
              "log4cplus.appender.cout.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.cout.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.cout.filters.1 = log4cplus::spi::LogLevelRangeFilter\n"
              "log4cplus.appender.cout.filters.1.LogLevelMin = fatal");
    level_threshold_filter_body();
}

TEST_F(log4cplus_config_file_configurator, logger)
{
    configure("log4cplus.logger.will = fatal\n");
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    ASSERT_TRUE(static_cast<bool>(lgr->get_level()));
    EXPECT_EQ(*chucho::level::FATAL_(), *lgr->get_level());
}

/*
TEST_F(log4cplus_config_file_configurator, multiple_writer)
{
    configure("log4cplus.logger.will = info, fw1, fw2\n"
              "log4cplus.appender.fw1 = log4cplus::FileAppender\n"
              "log4cplus.appender.fw1.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.fw1.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.fw1.File = one.log\n"
              "log4cplus.appender.fw2 = log4cplus::ConsoleAppender\n"
              "log4cplus.appender.fw2.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.fw2.layout.ConversionPattern = %m%n\");
    multiple_writer_body();
}
*/

TEST_F(log4cplus_config_file_configurator, null_appender)
{
    configure("log4cplus.logger.will = info, na\n"
              "log4cplus.appender.na = log4cplus::NullAppender");
    auto wrts = chucho::logger::get("will")->get_writer_names();
    ASSERT_EQ(0, wrts.size());
}

TEST_F(log4cplus_config_file_configurator, numbered_file_roller)
{
    configure("log4cplus.logger.will = info, nfr\n"
              "log4cplus.appender.nfr = log4cplus::RollingFileAppender\n"
              "log4cplus.appender.nfr.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.nfr.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.nfr.File = hello.log");
    auto& rfw = dynamic_cast<chucho::rolling_file_writer&>(
        chucho::logger::get("will")->get_writer("chucho::rolling_file_writer"));
    auto& nrlr = dynamic_cast<chucho::numbered_file_roller&>(rfw.get_file_roller());
    EXPECT_EQ(1, nrlr.get_max_index());
    EXPECT_EQ(1, nrlr.get_min_index());
}

TEST_F(log4cplus_config_file_configurator, numbered_file_roller_with_opts)
{
    configure("log4cplus.logger.will = info, nfr\n"
              "log4cplus.appender.nfr = log4cplus::RollingFileAppender\n"
              "log4cplus.appender.nfr.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.nfr.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.nfr.File = hello.log\n"
              "log4cplus.appender.nfr.MaxBackupIndex = 72");
    auto& rfw = dynamic_cast<chucho::rolling_file_writer&>(
        chucho::logger::get("will")->get_writer("chucho::rolling_file_writer"));
    auto& nrlr = dynamic_cast<chucho::numbered_file_roller&>(rfw.get_file_roller());
    EXPECT_EQ(72, nrlr.get_max_index());
    EXPECT_EQ(1, nrlr.get_min_index());
}

/*
TEST_F(log4cplus_config_file_configurator, remote_writer)
{
    configure("log4cplus.logger.will = info, rw1\n"
              "log4cplus.appender.rw1 = log4cplus::SocketAppender\n"
              "log4cplus.appender.rw1.host = motherboy\n"
              "log4cplus.logger.will2 = info, rw2\n"
              "log4cplus.appender.rw2 = log4cplus::SocketAppender\n"
              "log4cplus.appender.rw2.host = motherboy\n"
              "log4cplus.appender.rw2.port = 19567");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::remote_writer), typeid(*wrts[0]));
    auto rw = std::static_pointer_cast<chucho::remote_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(rw));
    EXPECT_EQ(std::string("motherboy"), rw->get_host());
    EXPECT_EQ(chucho::remote_writer::DEFAULT_PORT, rw->get_port());
    EXPECT_EQ(chucho::remote_writer::DEFAULT_UNSENT_CACHE_MAX, rw->get_unsent_cache_max());
    wrts = chucho::logger::get("will2")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::remote_writer), typeid(*wrts[0]));
    rw = std::static_pointer_cast<chucho::remote_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(rw));
    EXPECT_EQ(std::string("motherboy"), rw->get_host());
    EXPECT_EQ(19567, rw->get_port());
    EXPECT_EQ(chucho::remote_writer::DEFAULT_UNSENT_CACHE_MAX, rw->get_unsent_cache_max());
    // clear the status because we generated some warnings
    chucho::status_manager::get()->clear();
}
*/

TEST_F(log4cplus_config_file_configurator, rolling_file_writer)
{
    configure("log4cplus.logger.will = info, rfr\n"
              "log4cplus.appender.rfr = log4cplus::RollingFileAppender\n"
              "log4cplus.appender.rfr.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.rfr.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.rfr.File = hello.log\n"
              "log4cplus.appender.rfr.Append = false\n"
              "log4cplus.appender.rfr.ImmediateFlush = false");
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(
        chucho::logger::get("will")->get_writer("chucho::rolling_file_writer"));
    EXPECT_EQ(std::string("hello.log"), fwrt.get_file_name());
    EXPECT_EQ(chucho::file_writer::on_start::TRUNCATE, fwrt.get_on_start());
    EXPECT_FALSE(fwrt.get_flush());
}

TEST_F(log4cplus_config_file_configurator, rolling_file_writer_invalid_1)
{
    configure_with_error("log4cplus.logger.will = info, rfr");
}

TEST_F(log4cplus_config_file_configurator, rolling_file_writer_invalid_2)
{
    configure_with_error("log4cplus.logger.will = info, rfr\n"
                         "log4cplus.appender.rfr = log4cplus::RollingFileAppende\n"
                         "log4cplus.appender.rfr.layout = log4cplus::PatternLayout\n"
                         "log4cplus.appender.rfr.layout.ConversionPattern = %m%n\n"
                         "log4cplus.appender.rfr.File = hello.log\n"
                         "log4cplus.appender.rfr.Append = false\n"
                         "log4cplus.appender.rfr.ImmediateFlush = false");
}

TEST_F(log4cplus_config_file_configurator, rolling_file_writer_invalid_3)
{
    configure_with_error("log4cplus.logger.will = info, rfr\n"
                         "log4cplus.appender.rfr = log4cplus::RollingFileAppender\n"
                         "log4cplus.appender.rfr.layout = log4cplus::PatternLayout\n"
                         "log4cplus.appender.rfr.layout.ConversionPatter = %m%n\n"
                         "log4cplus.appender.rfr.File = hello.log\n"
                         "log4cplus.appender.rfr.Append = false\n"
                         "log4cplus.appender.rfr.ImmediateFlush = false");
}

TEST_F(log4cplus_config_file_configurator, rolling_file_writer_invalid_4)
{
    configure_with_error("log4cplus.logger.will = info, rfr\n"
                         "log4cplus.appender.rfr = log4cplus::RollingFileAppender\n"
                         "log4cplus.appender.rfr.File = hello.log\n"
                         "log4cplus.appender.rfr.Append = false\n"
                         "log4cplus.appender.rfr.ImmediateFlush = false");
}

TEST_F(log4cplus_config_file_configurator, rolling_file_writer_invalid_5)
{
    configure_with_error("log4cplus.logger.will = info, rfr\n"
                         "log4cplus.appender.rfr = log4cplus::RollingFileAppender\n"
                         "log4cplus.appender.rfr.layout = log4cplus::PatternLayout\n"
                         "log4cplus.appender.rfr.layout.ConversionPattern = %m%n\n"
                         "log4cplus.appender.rfr.Fil = hello.log\n"
                         "log4cplus.appender.rfr.Append = false\n"
                         "log4cplus.appender.rfr.ImmediateFlush = false");
}

TEST_F(log4cplus_config_file_configurator, simple_layout)
{
    configure("log4cplus.logger.will = info, sl\n"
              "log4cplus.appender.sl = log4cplus::ConsoleAppender\n"
              "log4cplus.appender.sl.layout = log4cplus::SimpleLayout");
    auto lgr = chucho::logger::get("will");
    auto wrts = lgr->get_writer_names();
    ASSERT_EQ(1, wrts.size());
    auto& pf = dynamic_cast<chucho::pattern_formatter&>(lgr->get_writer(wrts[0]).get_formatter());
    chucho::event evt(chucho::logger::get("pattern logger"),
                      chucho::level::INFO_(),
                      "hi",
                      __FILE__,
                      __LINE__,
                      "dowdy",
                      "chucho");
    std::string f = pf.format(evt);
    std::string exp(chucho::level::INFO_()->get_name());
    exp += " - hi";
    exp += chucho::line_ending::EOL;
    EXPECT_EQ(exp, f);
}

TEST_F(log4cplus_config_file_configurator, size_file_roll_trigger)
{
    std::string tmpl("log4cplus.logger.will = info, rfr\n"
                     "log4cplus.appender.rfr = log4cplus::RollingFileAppender\n"
                     "log4cplus.appender.rfr.layout = log4cplus::PatternLayout\n"
                     "log4cplus.appender.rfr.layout.ConversionPattern = %m%n\n"
                     "log4cplus.appender.rfr.MaxFileSize = SIZE\n"
                     "log4cplus.appender.rfr.File = what.log");
    size_file_roll_trigger_body(tmpl);
}

TEST_F(log4cplus_config_file_configurator, syslog_writer)
{
    configure("log4cplus.logger.will = info, sl\n"
              "log4cplus.appender.sl = log4cplus::SysLogAppender\n"
              "log4cplus.appender.sl.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.sl.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.sl.host = localhost\n"
              "log4cplus.appender.sl.facility = LOCAL0");
    syslog_writer_body();
}

TEST_F(log4cplus_config_file_configurator, syslog_writer_facility)
{
    std::string tmpl("log4cplus.logger.will = info, sl\n"
                     "log4cplus.appender.sl = log4cplus::SysLogAppender\n"
                     "log4cplus.appender.sl.layout = log4cplus::PatternLayout\n"
                     "log4cplus.appender.sl.layout.ConversionPattern = %m%n\n"
                     "log4cplus.appender.sl.facility = FCL");
    syslog_writer_facility_body(tmpl);
}

#if !defined(CHUCHO_SOLARIS)
TEST_F(log4cplus_config_file_configurator, syslog_writer_port)
{
    configure("log4cplus.logger.will = info, sl\n"
              "log4cplus.appender.sl = log4cplus::SysLogAppender\n"
              "log4cplus.appender.sl.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.sl.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.sl.host = localhost\n"
              "log4cplus.appender.sl.facility = LOCAL0\n"
              "log4cplus.appender.sl.port = 19567");
    syslog_writer_port_body();
}
#endif

TEST_F(log4cplus_config_file_configurator, ttcc_layout)
{
    configure("log4cplus.logger.will = info, sl\n"
              "log4cplus.appender.sl = log4cplus::ConsoleAppender\n"
              "log4cplus.appender.sl.layout = log4cplus::TTCCLayout");
    auto lgr = chucho::logger::get("will");
    auto wrts = lgr->get_writer_names();
    ASSERT_EQ(1, wrts.size());
    auto& pf = dynamic_cast<chucho::pattern_formatter&>(lgr->get_writer(wrts[0]).get_formatter());
    chucho::event evt(chucho::logger::get("will"),
                      chucho::level::INFO_(),
                      "hi",
                      __FILE__,
                      __LINE__,
                      "dowdy",
                      "chucho");
    std::string f = pf.format(evt);
    // Erase the milliseconds, since we can't guess it
    f.erase(0, f.find(' ') + 1);
    std::ostringstream stream;
    stream << std::this_thread::get_id() << ' ' << *chucho::level::INFO_() <<
        " will - hi" << chucho::line_ending::EOL;
    EXPECT_EQ(stream.str(), f);
}

#if defined(CHUCHO_WINDOWS)
TEST_F(log4cplus_config_file_configurator, windows_event_log_writer)
{
    configure("log4cplus.logger.will = info, nt\n"
              "log4cplus.appender.nt = log4cplus::NTEventLogAppender\n"
              "log4cplus.appender.nt.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.nt.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.nt.source = what\n"
              "log4cplus.appender.nt.log = hello");
    windows_event_log_writer_body();
}

TEST_F(log4cplus_config_file_configurator, windows_event_log_writer_no_log)
{
    configure("log4cplus.logger.will = info, nt\n"
              "log4cplus.appender.nt = log4cplus::NTEventLogAppender\n"
              "log4cplus.appender.nt.layout = log4cplus::PatternLayout\n"
              "log4cplus.appender.nt.layout.ConversionPattern = %m%n\n"
              "log4cplus.appender.nt.source = what");
    windows_event_log_writer_no_log_body();
}
#endif
