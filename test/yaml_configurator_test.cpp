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
#include <chucho/yaml_configurator.hpp>
#include <chucho/logger.hpp>
#include <chucho/status_manager.hpp>
#include <chucho/cerr_writer.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/file_writer.hpp>
#include <chucho/level_filter.hpp>
#include <chucho/level_threshold_filter.hpp>
#include <chucho/rolling_file_writer.hpp>
#include <chucho/numbered_file_roller.hpp>
#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/time_file_roller.hpp>
#include <chucho/duplicate_message_filter.hpp>
#include <chucho/syslog_writer.hpp>
#include <chucho/remote_writer.hpp>
#include <chucho/exception.hpp>
#include <sstream>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace
{

void setenv(const char* var, const char* val)
{
#if defined(_WIN32)
    SetEnvironmentVariableA(var, val);
#else
    ::setenv(var, val, 1);
#endif
}

}

class yaml_configurator : public ::testing::Test
{
public:
    yaml_configurator()
    {
        chucho::logger::remove_unused_loggers();
        // This ensures that all factories are registered
        chucho::logger::get("");
        chucho::status_manager::get()->clear();
    }

    ~yaml_configurator()
    {
        EXPECT_LT(chucho::status_manager::get()->get_level(), chucho::status::level::WARNING);
    }

protected:
    void configure(const char* const yaml)
    {
        std::istringstream stream(yaml);
        cnf_.configure(stream);
    }

private:
    chucho::yaml_configurator cnf_;
};

TEST_F(yaml_configurator, cerr_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::cerr_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m%n'");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::cerr_writer), typeid(*wrts[0]));
}

TEST_F(yaml_configurator, cout_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::cout_writer), typeid(*wrts[0]));
}

TEST_F(yaml_configurator, duplicate_message_filter)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::duplicate_message_filter");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    auto flts = wrts[0]->get_filters();
    ASSERT_EQ(1, flts.size());
    ASSERT_EQ(typeid(chucho::duplicate_message_filter), typeid(*flts[0]));
}

TEST_F(yaml_configurator, file_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        file_name: hello.log\n"
              "        on_start: truncate\n"
              "        flush: false");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    EXPECT_EQ(std::string("hello.log"), fwrt->get_file_name());
    EXPECT_FALSE(fwrt->get_flush());
    EXPECT_EQ(chucho::file_writer::on_start::TRUNCATE, fwrt->get_on_start());
}

TEST_F(yaml_configurator, level_filter)
{
    std::string tmpl("- chucho::logger:\n"
                     "    name: will\n"
                     "    chucho::cout_writer:\n"
                     "        chucho::pattern_formatter:\n"
                     "            pattern: '%m%n'\n"
                     "        chucho::level_filter:\n"
                     "            level: info\n"
                     "            on_match: NEUTRAL\n"
                     "            on_mismatch: RESULT");
    std::size_t pos = tmpl.find("RESULT");
    std::vector<std::string> bad =
    {
        "",
        "nuetral",
        "willy"
    };
    for (auto item : bad)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, item);
        EXPECT_ANY_THROW(configure(rep.c_str()));
    }
    chucho::status_manager::get()->clear();
    std::map<std::string, chucho::filter::result> good =
    {
        { "ACCEPT", chucho::filter::result::ACCEPT },
        { "NEUTRAL", chucho::filter::result::NEUTRAL },
        { "DENY", chucho::filter::result::DENY },
        { "aCcEpT", chucho::filter::result::ACCEPT },
        { "NEuTrAL", chucho::filter::result::NEUTRAL },
        { "deny", chucho::filter::result::DENY }
    };
    for (auto item : good)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, item.first);
        configure(rep.c_str());
        auto wrts = chucho::logger::get("will")->get_writers();
        ASSERT_EQ(1, wrts.size());
        auto flts = wrts[0]->get_filters();
        ASSERT_EQ(1, flts.size());
        ASSERT_EQ(typeid(chucho::level_filter), typeid(*flts[0]));
        auto lf = std::static_pointer_cast<chucho::level_filter>(flts[0]);
        EXPECT_EQ(*chucho::level::INFO(), *lf->get_level());
        EXPECT_EQ(chucho::filter::result::NEUTRAL, lf->get_on_match());
        EXPECT_EQ(item.second, lf->get_on_mismatch());
    }
}

TEST_F(yaml_configurator, level_threshold_filter)
{
    configure("- chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::level_threshold_filter:\n"
              "            level: fatal");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    auto flts = wrts[0]->get_filters();
    ASSERT_EQ(1, flts.size());
    ASSERT_EQ(typeid(chucho::level_threshold_filter), typeid(*flts[0]));
    auto thresh = std::static_pointer_cast<chucho::level_threshold_filter>(flts[0]);
    EXPECT_EQ(*chucho::level::FATAL(), *thresh->get_level());
}

TEST_F(yaml_configurator, logger)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    level: fatal\n"
              "    writes_to_ancestors: false");
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    ASSERT_TRUE(static_cast<bool>(lgr->get_level()));
    EXPECT_EQ(*chucho::level::FATAL(), *lgr->get_level());
    EXPECT_FALSE(lgr->writes_to_ancestors());
}

TEST_F(yaml_configurator, multiple_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        file_name: one.log\n"
              "    - chucho::file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        file_name: two.log\n");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(2, wrts.size());
    EXPECT_EQ(typeid(chucho::file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    EXPECT_EQ(std::string("one.log"), fwrt->get_file_name());
    EXPECT_EQ(typeid(chucho::file_writer), typeid(*wrts[1]));
    fwrt = std::static_pointer_cast<chucho::file_writer>(wrts[1]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    EXPECT_EQ(std::string("two.log"), fwrt->get_file_name());
}

TEST_F(yaml_configurator, remote_writer)
{
    configure("- chucho::logger:\n"
              "    name: will\n"
              "    chucho::remote_writer:\n"
              "        host: motherboy\n"
              "- chucho::logger:\n"
              "    - name: will2\n"
              "    - chucho::remote_writer:\n"
              "        - host: motherboy\n"
              "        - port: 19567\n"
              "- chucho::logger:\n"
              "    - name: will3\n"
              "    - chucho::remote_writer:\n"
              "        - host: motherboy\n"
              "        - unsent_cache_max: 750");
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
    wrts = chucho::logger::get("will3")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::remote_writer), typeid(*wrts[0]));
    rw = std::static_pointer_cast<chucho::remote_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(rw));
    EXPECT_EQ(std::string("motherboy"), rw->get_host());
    EXPECT_EQ(chucho::remote_writer::DEFAULT_PORT, rw->get_port());
    EXPECT_EQ(750, rw->get_unsent_cache_max());
    // clear the status because we generated some warnings
    chucho::status_manager::get()->clear();
}

TEST_F(yaml_configurator, rolling_file_writer)
{
        configure("chucho::logger:\n"
                  "    name: will\n"
                  "    chucho::rolling_file_writer:\n"
                  "        chucho::pattern_formatter:\n"
                  "            pattern: '%m%n'\n"
                  "        chucho::numbered_file_roller:\n"
                  "            min_index: 3\n"
                  "            max_index: 5\n"
                  "        chucho::size_file_roll_trigger:\n"
                  "            max_size: 5000\n"
                  "        file_name: what.log\n"
                  "        on_start: TruNCAte\n"
                  "        flush: false");
        auto wrts = chucho::logger::get("will")->get_writers();
        ASSERT_EQ(1, wrts.size());
        ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
        auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
        ASSERT_TRUE(static_cast<bool>(fwrt));
        EXPECT_EQ(std::string("what.log"), fwrt->get_file_name());
        EXPECT_EQ(chucho::file_writer::on_start::TRUNCATE, fwrt->get_on_start());
        EXPECT_FALSE(fwrt->get_flush());
        auto rlr = fwrt->get_file_roller();
        ASSERT_EQ(typeid(chucho::numbered_file_roller), typeid(*rlr));
        auto nrlr = std::static_pointer_cast<chucho::numbered_file_roller>(rlr);
        ASSERT_TRUE(static_cast<bool>(nrlr));
        EXPECT_EQ(3, nrlr->get_min_index());
        EXPECT_EQ(5, nrlr->get_max_index());
        auto trg = fwrt->get_file_roll_trigger();
        ASSERT_EQ(typeid(chucho::size_file_roll_trigger), typeid(*trg));
        auto strg = std::static_pointer_cast<chucho::size_file_roll_trigger>(trg);
        ASSERT_TRUE(static_cast<bool>(strg));
        EXPECT_EQ(5000, strg->get_max_size());
}

TEST_F(yaml_configurator, size_file_roll_trigger)
{
    std::string tmpl("chucho::logger:\n"
                     "    name: will\n"
                     "    chucho::rolling_file_writer:\n"
                     "        chucho::pattern_formatter:\n"
                     "            pattern: '%m%n'\n"
                     "        chucho::numbered_file_roller:\n"
                     "            max_index: 1\n"
                     "        chucho::size_file_roll_trigger:\n"
                     "            max_size: SIZE\n"
                     "        file_name: what.log\n");
    std::size_t pos = tmpl.find("SIZE");
    std::vector<std::string> bad =
    {
        "",
        "Willy",
        "5000x",
        "5000gx",
        "5000gbx"
    };
    for (auto item : bad)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 4, item);
        EXPECT_ANY_THROW(configure(rep.c_str()));
    }
    chucho::status_manager::get()->clear();
    std::map<std::string, std::uintmax_t> good =
    {
        { "5000", 5000 },
        { "5001b", 5001 },
        { "2k", 1024 * 2 },
        { "3kb", 1024 * 3 },
        { "4K", 1024 * 4 },
        { "5Kb", 1024 * 5 },
        { "6kB", 1024 * 6 },
        { "7KB", 1024 * 7 },
        { "2m", 1024 * 1024 * 2 },
        { "3mb", 1024 * 1024 * 3 },
        { "4M", 1024 * 1024 * 4 },
        { "5Mb", 1024 * 1024 * 5 },
        { "6mB", 1024 * 1024 * 6 },
        { "7MB", 1024 * 1024 * 7 },
        { "2g", 1024ULL * 1024ULL * 1024ULL * 2ULL },
        { "3gb", 1024ULL * 1024ULL * 1024ULL * 3ULL },
        { "4G", 1024ULL * 1024ULL * 1024ULL * 4ULL },
        { "5Gb", 1024ULL * 1024ULL * 1024ULL * 5ULL },
        { "6gB", 1024ULL * 1024ULL * 1024ULL * 6ULL },
        { "7GB", 1024ULL * 1024ULL * 1024ULL * 7ULL }
    };
    for (auto item : good)
    {
        chucho::logger::remove_unused_loggers();
        std::string rep = tmpl;
        rep.replace(pos, 4, item.first);
        configure(rep.c_str());
        auto wrts = chucho::logger::get("will")->get_writers();
        ASSERT_EQ(1, wrts.size());
        ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
        auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
        ASSERT_TRUE(static_cast<bool>(fwrt));
        auto trg = fwrt->get_file_roll_trigger();
        ASSERT_EQ(typeid(chucho::size_file_roll_trigger), typeid(*trg));
        auto strg = std::static_pointer_cast<chucho::size_file_roll_trigger>(trg);
        ASSERT_TRUE(static_cast<bool>(strg));
        EXPECT_EQ(item.second, strg->get_max_size());
    }
}

TEST_F(yaml_configurator, syslog_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::syslog_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m%n'\n"
              "        - facility: LOCAL0\n"
              "        - host_name: localhost");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::syslog_writer), typeid(*wrts[0]));
    auto wrt = std::static_pointer_cast<chucho::syslog_writer>(wrts[0]);
    EXPECT_EQ(chucho::syslog::facility::LOCAL0, wrt->get_facility());
    EXPECT_EQ(std::string("localhost"), wrt->get_host_name());
}

TEST_F(yaml_configurator, syslog_writer_facility)
{
    std::string tmpl("chucho::logger:\n"
                     "    - name: will\n"
                     "    - chucho::syslog_writer:\n"
                     "        - chucho::pattern_formatter:\n"
                     "            - pattern: '%m%n'\n"
                     "        - facility: FCL");
    std::size_t pos = tmpl.find("FCL");
    std::vector<std::string> bad =
    {
        "kernel",
        "my dog has fleas",
        ""
    };
    for (auto item : bad)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 3, item);
        EXPECT_ANY_THROW(configure(rep.c_str()));
    }
    std::map<std::string, chucho::syslog::facility> good =
    {
        { "kern", chucho::syslog::facility::KERN },
        { "USER", chucho::syslog::facility::USER },
        { "mAiL", chucho::syslog::facility::MAIL },
        { "Daemon", chucho::syslog::facility::DAEMON },
        { "autH", chucho::syslog::facility::AUTH },
        { "SYsLog", chucho::syslog::facility::SYSLOG },
        { "LPR", chucho::syslog::facility::LPR },
        { "nEws", chucho::syslog::facility::NEWS },
        { "uucp", chucho::syslog::facility::UUCP },
        { "CrOn", chucho::syslog::facility::CRON },
        { "AuTHpriv", chucho::syslog::facility::AUTHPRIV },
        { "fTp", chucho::syslog::facility::FTP },
        { "local0", chucho::syslog::facility::LOCAL0 },
        { "LoCal1", chucho::syslog::facility::LOCAL1 },
        { "LOCAL2", chucho::syslog::facility::LOCAL2 },
        { "lOcal3", chucho::syslog::facility::LOCAL3 },
        { "Local4", chucho::syslog::facility::LOCAL4 },
        { "loCAl5", chucho::syslog::facility::LOCAL5 },
        { "loCal6", chucho::syslog::facility::LOCAL6 },
        { "LoCal7", chucho::syslog::facility::LOCAL7 }
    };
    for (auto item : good)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 3, item.first);
        configure(rep.c_str());
        auto wrts = chucho::logger::get("will")->get_writers();
        ASSERT_EQ(1, wrts.size());
        EXPECT_EQ(typeid(chucho::syslog_writer), typeid(*wrts[0]));
        auto wrt = std::static_pointer_cast<chucho::syslog_writer>(wrts[0]);
        EXPECT_EQ(item.second, wrt->get_facility());
    }
}

TEST_F(yaml_configurator, time_file_roller)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::rolling_file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::time_file_roller:\n"
              "            file_name_pattern: '%d{%d}'\n"
              "            max_history: 5\n");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    auto rlr = fwrt->get_file_roller();
    ASSERT_EQ(typeid(chucho::time_file_roller), typeid(*rlr));
    auto trlr = std::static_pointer_cast<chucho::time_file_roller>(rlr);
    ASSERT_TRUE(static_cast<bool>(trlr));
    EXPECT_EQ(std::string("%d{%d}"), trlr->get_file_name_pattern());
    EXPECT_EQ(5, trlr->get_max_history());
    EXPECT_EQ(trlr, fwrt->get_file_roll_trigger());
}

TEST_F(yaml_configurator, variables)
{
    setenv("CHUCHO_WRITES_KEY", "writes_to_ancestors", 1);
    setenv("CHUCHO_WRITES_VALUE", "false", 1);
    configure("- variables:\n"
              "    MY_NAME_IS: will\n"
              "    MY_TYPE_IS: logger\n"
              "    MY_KEY_IS: level\n"
              "    MY_VALUE_IS: fatal\n"
              "- 'chucho::${MY_TYPE_IS}':\n"
              "    name: '${MY_NAME_IS}'\n"
              "    '${MY_KEY_IS}': '${MY_VALUE_IS}'\n"
              "    '$env{CHUCHO_WRITES_KEY}': '$ENV{CHUCHO_WRITES_VALUE}'");
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    ASSERT_NE(nullptr, lgr->get_level().get());
    EXPECT_EQ(*chucho::level::FATAL(), *lgr->get_level());
    EXPECT_FALSE(lgr->writes_to_ancestors());
}
