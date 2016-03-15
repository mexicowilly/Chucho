/*
 * Copyright 2013-2016 Will Mason
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
#include <chucho/noop_file_compressor.hpp>
#if defined(CHUCHO_HAVE_BZIP2)
#include <chucho/bzip2_file_compressor.hpp>
#endif
#if defined(CHUCHO_HAVE_ZLIB)
#include <chucho/gzip_file_compressor.hpp>
#endif
#if defined(CHUCHO_HAVE_MINIZIP)
#include <chucho/zip_file_compressor.hpp>
#endif
#include <chucho/async_writer.hpp>
#include <chucho/sliding_numbered_file_roller.hpp>
#if defined(CHUCHO_WINDOWS)
#include <chucho/windows_event_log_writer.hpp>
#endif
#include <chucho/exception.hpp>
#include <chucho/configuration.hpp>
#if defined(CHUCHO_HAVE_MYSQL)
#include <chucho/mysql_writer.hpp>
#endif
#if defined(CHUCHO_HAVE_ORACLE)
#include <chucho/oracle_writer.hpp>
#endif
#if defined(CHUCHO_HAVE_SQLITE)
#include <chucho/sqlite_writer.hpp>
#include <chucho/file.hpp>
#endif
#if defined(CHUCHO_HAVE_POSTGRES)
#include <chucho/postgres_writer.hpp>
#endif
#if defined(CHUCHO_HAVE_RUBY)
#include <chucho/ruby_evaluator_filter.hpp>
#endif
#if defined(CHUCHO_HAVE_EMAIL_WRITER)
#include <chucho/email_writer.hpp>
#include <chucho/level_threshold_email_trigger.hpp>
#endif
#include <chucho/pipe_writer.hpp>
#include <chucho/named_pipe_writer.hpp>
#include <sstream>
#if defined(CHUCHO_WINDOWS)
#include <windows.h>
#endif
#if defined(CHUCHO_HAVE_ZEROMQ)
#include <chucho/zeromq_writer.hpp>
#include <chucho/noop_compressor.hpp>
#endif
#include <chucho/formatted_message_serializer.hpp>
#if defined(CHUCHO_HAVE_PROTOBUF)
#include <chucho/protobuf_serializer.hpp>
#endif
#if defined(CHUCHO_HAVE_ACTIVEMQ)
#include <chucho/activemq_writer.hpp>
#endif

namespace chucho
{

namespace test
{

configurator::configurator()
{
    chucho::logger::remove_unused_loggers();
    // This ensures that all factories are registered
    chucho::logger::get("");
    chucho::status_manager::get()->clear();
}

configurator::~configurator()
{
    EXPECT_LT(chucho::status_manager::get()->get_level(), chucho::status::level::WARNING_);
}

#if defined(CHUCHO_HAVE_ACTIVEMQ)

void configurator::activemq_writer_queue_body()
{
}

void configurator::activemq_writer_topic_body()
{
}

#endif

void configurator::async_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::async_writer), typeid(*wrts[0]));
    auto awrt = std::static_pointer_cast<chucho::async_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(awrt));
    EXPECT_EQ(*chucho::level::INFO_(), *awrt->get_discard_threshold());
    EXPECT_EQ(chucho::async_writer::DEFAULT_QUEUE_CAPACITY, awrt->get_queue_capacity());
    EXPECT_EQ(typeid(chucho::file_writer), typeid(*awrt->get_writer()));
}

void configurator::async_writer_with_opts_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::async_writer), typeid(*wrts[0]));
    auto awrt = std::static_pointer_cast<chucho::async_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(awrt));
    EXPECT_EQ(*chucho::level::ERROR_(), *awrt->get_discard_threshold());
    EXPECT_EQ(700, awrt->get_queue_capacity());
    EXPECT_EQ(typeid(chucho::file_writer), typeid(*awrt->get_writer()));
    EXPECT_FALSE(awrt->get_flush_on_destruct());
}

void configurator::bzip2_file_compressor_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    auto rlr = fwrt->get_file_roller();
    ASSERT_EQ(typeid(chucho::numbered_file_roller), typeid(*rlr));
    auto nrlr = std::static_pointer_cast<chucho::numbered_file_roller>(rlr);
    ASSERT_TRUE(static_cast<bool>(nrlr));
    auto cmp = nrlr->get_file_compressor();
    ASSERT_TRUE(static_cast<bool>(cmp));
#if defined(CHUCHO_HAVE_BZIP2)
    ASSERT_EQ(typeid(chucho::bzip2_file_compressor), typeid(*cmp));
    EXPECT_EQ(1, cmp->get_min_index());
#else
    ASSERT_EQ(typeid(chucho::noop_file_compressor), typeid(*cmp));
    chucho::status_manager::get()->clear();
#endif
}

void configurator::cerr_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::cerr_writer), typeid(*wrts[0]));
}

void configurator::configure(const char* const cnf)
{
    std::istringstream in(cnf);
    get_configurator().configure(in);
}

void configurator::configure_with_error(const char* const cnf)
{
    configure(cnf);
    EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get()->get_level());
    chucho::status_manager::get()->clear();
}

void configurator::cout_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::cout_writer), typeid(*wrts[0]));
}

void configurator::duplicate_message_filter_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    auto flts = wrts[0]->get_filters();
    ASSERT_EQ(1, flts.size());
    ASSERT_EQ(typeid(chucho::duplicate_message_filter), typeid(*flts[0]));
}

#if defined(CHUCHO_HAVE_EMAIL_WRITER)

void configurator::email_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::email_writer), typeid(*wrts[0]));
    auto ewrt = std::static_pointer_cast<chucho::email_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(ewrt));
    EXPECT_EQ(7000, ewrt->get_buffer_capacity());
    EXPECT_EQ(0, ewrt->get_buffer_size());
    EXPECT_EQ(chucho::email_writer::connection_type::CLEAR, ewrt->get_connection_type());
    EXPECT_STREQ("whistler@mctweaky.com", ewrt->get_from().c_str());
    EXPECT_STREQ("mail.dummy.com", ewrt->get_host().c_str());
    ASSERT_TRUE(ewrt->get_password());
    EXPECT_STREQ("lumpy", ewrt->get_password()->c_str());
    EXPECT_EQ(123, ewrt->get_port());
    EXPECT_STREQ("%c", ewrt->get_subject().c_str());
    ASSERT_EQ(2, ewrt->get_to().size());
    EXPECT_STREQ("one@blubbery.com", ewrt->get_to()[0].c_str());
    EXPECT_STREQ("two@humid.org", ewrt->get_to()[1].c_str());
    ASSERT_EQ(typeid(chucho::level_threshold_email_trigger), typeid(*ewrt->get_trigger()));
    auto ltet = std::static_pointer_cast<chucho::level_threshold_email_trigger>(ewrt->get_trigger());
    ASSERT_TRUE(static_cast<bool>(ltet));
    EXPECT_EQ(*chucho::level::ERROR_(), *ltet->get_level());
    ASSERT_TRUE(ewrt->get_user());
    EXPECT_STREQ("scrumpy", ewrt->get_user()->c_str());
}

#endif

void configurator::file_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    EXPECT_EQ(std::string("hello.log"), fwrt->get_file_name());
    EXPECT_FALSE(fwrt->get_flush());
    EXPECT_EQ(chucho::file_writer::on_start::TRUNCATE, fwrt->get_on_start());
}

void configurator::gzip_file_compressor_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    auto rlr = fwrt->get_file_roller();
    ASSERT_EQ(typeid(chucho::time_file_roller), typeid(*rlr));
    auto trlr = std::static_pointer_cast<chucho::time_file_roller>(rlr);
    ASSERT_TRUE(static_cast<bool>(trlr));
    auto cmp = trlr->get_file_compressor();
    ASSERT_TRUE(static_cast<bool>(cmp));
#if defined(CHUCHO_HAVE_ZLIB)
    ASSERT_EQ(typeid(chucho::gzip_file_compressor), typeid(*cmp));
    EXPECT_EQ(7, cmp->get_min_index());
#else
    ASSERT_EQ(typeid(chucho::noop_file_compressor), typeid(*cmp));
    chucho::status_manager::get()->clear();
#endif
}

void configurator::interval_file_roll_trigger_body(const std::string& tmpl)
{
    std::size_t pos = tmpl.find("PERIOD");
    const char* bad[] =
    {
        "seven seconds",
        "5 years",
        "12 doggies",
        nullptr
    };
    int i = 0;
    while (bad[i] != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, bad[i++]);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get()->get_level());
    }
    chucho::status_manager::get()->clear();
    const char* good[] =
    {
        "2 MinUTeS",
        "1 HOUR",
        "4 days",
        "5 Weeks",
        "6 monTH",
        nullptr
    };
    i = 0;
    while (good[i] != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, good[i++]);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::INFO_, chucho::status_manager::get()->get_level());
    }
}

void configurator::level_filter_body(const std::string& tmpl)
{
    std::size_t pos = tmpl.find("RESULT");
    // Visual Studio 2012 does not have initializer lists
    const char* bad[] =
    {
        "",
        "nuetral",
        "willy",
        nullptr
    };
    int i = 0;
    while (bad[i] != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, bad[i++]);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get()->get_level());
    }
    chucho::status_manager::get()->clear();
    // Visual Studio 2012 does not have initializer lists
    struct { const char* first; chucho::filter::result second; } good[] =
    {
        { "ACCEPT", chucho::filter::result::ACCEPT },
        { "NEUTRAL", chucho::filter::result::NEUTRAL },
        { "DENY", chucho::filter::result::DENY },
        { "aCcEpT", chucho::filter::result::ACCEPT },
        { "NEuTrAL", chucho::filter::result::NEUTRAL },
        { "deny", chucho::filter::result::DENY },
        { nullptr, chucho::filter::result::ACCEPT }
    };
    i = 0;
    while (good[i].first != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, good[i].first);
        configure(rep.c_str());
        auto wrts = chucho::logger::get("will")->get_writers();
        ASSERT_EQ(1, wrts.size());
        auto flts = wrts[0]->get_filters();
        ASSERT_EQ(1, flts.size());
        ASSERT_EQ(typeid(chucho::level_filter), typeid(*flts[0]));
        auto lf = std::static_pointer_cast<chucho::level_filter>(flts[0]);
        EXPECT_EQ(*chucho::level::INFO_(), *lf->get_level());
        EXPECT_EQ(chucho::filter::result::NEUTRAL, lf->get_on_match());
        EXPECT_EQ(good[i++].second, lf->get_on_mismatch());
    }
}

void configurator::level_threshold_filter_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    auto flts = wrts[0]->get_filters();
    ASSERT_EQ(1, flts.size());
    ASSERT_EQ(typeid(chucho::level_threshold_filter), typeid(*flts[0]));
    auto thresh = std::static_pointer_cast<chucho::level_threshold_filter>(flts[0]);
    EXPECT_EQ(*chucho::level::FATAL_(), *thresh->get_level());
}

void configurator::logger_body()
{
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    ASSERT_TRUE(static_cast<bool>(lgr->get_level()));
    EXPECT_EQ(*chucho::level::FATAL_(), *lgr->get_level());
    EXPECT_FALSE(lgr->writes_to_ancestors());
}

void configurator::multiple_writer_body()
{
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

#if defined(CHUCHO_HAVE_MYSQL)

void configurator::mysql_writer_full_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::mysql_writer), typeid(*wrts[0]));
    auto mwrt = std::static_pointer_cast<chucho::mysql_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(mwrt));
    EXPECT_EQ(std::string("test"), mwrt->get_database());
    EXPECT_EQ(std::string("192.168.56.101"), mwrt->get_host());
    EXPECT_EQ(std::string("test_user"), mwrt->get_user());
    EXPECT_EQ(std::string("password"), mwrt->get_password());
    auto aw = mwrt->get_async_writer();
    EXPECT_EQ(*chucho::level::INFO_(), *aw->get_discard_threshold());
    EXPECT_EQ(912, aw->get_queue_capacity());
    EXPECT_EQ(false, aw->get_flush_on_destruct());
}

void configurator::mysql_writer_minimal_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::mysql_writer), typeid(*wrts[0]));
    auto mwrt = std::static_pointer_cast<chucho::mysql_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(mwrt));
    EXPECT_EQ(std::string("test"), mwrt->get_database());
    EXPECT_EQ(std::string("192.168.56.101"), mwrt->get_host());
    EXPECT_EQ(std::string("test_user"), mwrt->get_user());
    EXPECT_EQ(std::string("password"), mwrt->get_password());
}

#endif

void configurator::named_pipe_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::named_pipe_writer), typeid(*wrts[0]));
    auto npwrt = std::static_pointer_cast<chucho::named_pipe_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(npwrt));
    EXPECT_FALSE(npwrt->get_flush());
    #if defined(CHUCHO_WINDOWS)
    std::string pname("\\\\.\\pipe\\monkeyballs");
    #else
    std::string pname("monkeyballs");
    #endif
    EXPECT_EQ(pname, npwrt->get_file_name());
    chucho::status_manager::get()->clear();
}

void configurator::numbered_file_roller_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    auto rlr = fwrt->get_file_roller();
    ASSERT_EQ(typeid(chucho::numbered_file_roller), typeid(*rlr));
    auto nrlr = std::static_pointer_cast<chucho::numbered_file_roller>(rlr);
    ASSERT_TRUE(static_cast<bool>(nrlr));
    EXPECT_EQ(5, nrlr->get_max_index());
    EXPECT_EQ(-3, nrlr->get_min_index());
}

#if defined(CHUCHO_HAVE_ORACLE)

void configurator::oracle_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::oracle_writer), typeid(*wrts[0]));
    auto owrt = std::static_pointer_cast<chucho::oracle_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(owrt));
    EXPECT_EQ(std::string("192.168.56.102/pdb1"), owrt->get_database());
    EXPECT_EQ(std::string("test_user"), owrt->get_user());
    EXPECT_EQ(std::string("password"), owrt->get_password());
}

#endif

void configurator::pipe_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::pipe_writer), typeid(*wrts[0]));
    auto pwrt = std::static_pointer_cast<chucho::pipe_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(pwrt));
    EXPECT_FALSE(pwrt->get_flush());
}

#if defined(CHUCHO_HAVE_POSTGRES)

void configurator::postgres_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::postgres_writer), typeid(*wrts[0]));
    auto pwrt = std::static_pointer_cast<chucho::postgres_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(pwrt));
    EXPECT_EQ(std::string("postgres://test_user:password@192.168.56.101/postgres"), pwrt->get_uri());
}

#endif

void configurator::remote_writer_body()
{
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

void configurator::rolling_file_writer_body()
{
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

#if defined(CHUCHO_HAVE_RUBY)

void configurator::ruby_evaluator_filter_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    auto flts = wrts[0]->get_filters();
    ASSERT_EQ(1, flts.size());
    ASSERT_EQ(typeid(chucho::ruby_evaluator_filter), typeid(*flts[0]));
    auto rb = std::static_pointer_cast<chucho::ruby_evaluator_filter>(flts[0]);
    EXPECT_EQ(std::string("$logger == \"will\""), rb->get_expression());
}

#endif

void configurator::size_file_roll_trigger_body(const std::string& tmpl)
{
    std::size_t pos = tmpl.find("SIZE");
    // Visual Studio 2012 does not have initializer lists
    const char* bad[] =
    {
        "",
        "Willy",
        "5000x",
        "5000gx",
        "5000gbx",
        nullptr
    };
    int i = 0;
    while (bad[i] != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 4, bad[i++]);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get()->get_level());
    }
    chucho::status_manager::get()->clear();
    // Visual Studio 2012 does not have initializer lists
    struct { const char* first; std::uintmax_t second; } good[] =
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
        { "1g", 1024ULL * 1024ULL * 1024ULL },
        { "1Gb", 1024ULL * 1024ULL * 1024ULL },
        { "1GB", 1024ULL * 1024ULL * 1024ULL },
        { "1gB", 1024ULL * 1024ULL * 1024ULL },
        { nullptr, 0 }
    };
    i = 0;
    while (good[i].first != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        std::string rep = tmpl;
        rep.replace(pos, 4, good[i].first);
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
        EXPECT_EQ(good[i++].second, strg->get_max_size());
    }
}

void configurator::sliding_numbered_file_roller_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    auto rlr = fwrt->get_file_roller();
    ASSERT_EQ(typeid(chucho::sliding_numbered_file_roller), typeid(*rlr));
    auto srlr = std::static_pointer_cast<chucho::sliding_numbered_file_roller>(rlr);
    ASSERT_TRUE(static_cast<bool>(srlr));
    EXPECT_EQ(5, srlr->get_max_count());
    EXPECT_EQ(-3, srlr->get_min_index());
}

#if defined(CHUCHO_HAVE_SQLITE)

void configurator::sqlite_writer_body()
{
    auto will = chucho::logger::get("will");
    auto wrts = will->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::sqlite_writer), typeid(*wrts[0]));
    auto wrt = std::static_pointer_cast<chucho::sqlite_writer>(wrts[0]);
    EXPECT_EQ(std::string("database.sqlite"), wrt->get_file_name());
    will->remove_writer(wrt);
    wrt.reset();
    chucho::file::remove("database.sqlite");
}

#endif

void configurator::syslog_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::syslog_writer), typeid(*wrts[0]));
    auto wrt = std::static_pointer_cast<chucho::syslog_writer>(wrts[0]);
    EXPECT_EQ(chucho::syslog::facility::LOCAL0, wrt->get_facility());
    EXPECT_EQ(std::string("localhost"), wrt->get_host_name());
    ASSERT_TRUE(wrt->get_port());
    EXPECT_EQ(514, *wrt->get_port());
}

void configurator::syslog_writer_facility_body(const std::string& tmpl)
{
    std::size_t pos = tmpl.find("FCL");
    // Visual Studio 2012 does not have initializer lists
    const char* bad[] =
    {
        "kernel",
        "my dog has fleas",
        "",
        nullptr
    };
    int i = 0;
    while (bad[i] != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 3, bad[i++]);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get()->get_level());
    }
    // Visual Studio 2012 does not have initializer lists
    struct { const char* first; chucho::syslog::facility second; } good[] =
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
        { "LoCal7", chucho::syslog::facility::LOCAL7 },
        { nullptr, chucho::syslog::facility::LOCAL7 }
    };
    i = 0;
    while (good[i].first != nullptr)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get()->clear();
        std::string rep = tmpl;
        rep.replace(pos, 3, good[i].first);
        configure(rep.c_str());
        auto wrts = chucho::logger::get("will")->get_writers();
        ASSERT_EQ(1, wrts.size());
        EXPECT_EQ(typeid(chucho::syslog_writer), typeid(*wrts[0]));
        auto wrt = std::static_pointer_cast<chucho::syslog_writer>(wrts[0]);
        EXPECT_EQ(good[i++].second, wrt->get_facility());
    }
}

void configurator::syslog_writer_port_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::syslog_writer), typeid(*wrts[0]));
    auto wrt = std::static_pointer_cast<chucho::syslog_writer>(wrts[0]);
    EXPECT_EQ(chucho::syslog::facility::LOCAL0, wrt->get_facility());
    EXPECT_EQ(std::string("localhost"), wrt->get_host_name());
    ASSERT_TRUE(wrt->get_port());
    EXPECT_EQ(19567, *wrt->get_port());
}

void configurator::time_file_roller_body()
{
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

#if defined(CHUCHO_WINDOWS)

void configurator::windows_event_log_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::windows_event_log_writer), typeid(*wrts[0]));
    auto welw = std::static_pointer_cast<chucho::windows_event_log_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(welw));
    EXPECT_TRUE(welw->get_host().empty());
    EXPECT_EQ(std::string("what"), welw->get_source());
    EXPECT_EQ(std::string("hello"), welw->get_log());
    chucho::status_manager::get()->clear();
}

void configurator::windows_event_log_writer_no_log_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::windows_event_log_writer), typeid(*wrts[0]));
    auto welw = std::static_pointer_cast<chucho::windows_event_log_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(welw));
    EXPECT_TRUE(welw->get_host().empty());
    EXPECT_EQ(std::string("what"), welw->get_source());
    EXPECT_EQ(std::string("Application"), welw->get_log());
    chucho::status_manager::get()->clear();
}

#endif

#if defined(CHUCHO_HAVE_ZEROMQ)

void configurator::zeromq_writer_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::zeromq_writer), typeid(*wrts[0]));
    auto zw = std::static_pointer_cast<chucho::zeromq_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(zw));
    auto ser = zw->get_serializer();
    ASSERT_TRUE(static_cast<bool>(ser));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(*ser));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7777"), zw->get_endpoint());
    std::string pfx_str("Hi");
    std::vector<std::uint8_t> pfx(pfx_str.begin(), pfx_str.end());
    EXPECT_EQ(pfx, zw->get_prefix());
}

void configurator::zeromq_writer_with_compressor_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::zeromq_writer), typeid(*wrts[0]));
    auto zw = std::static_pointer_cast<chucho::zeromq_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(zw));
    auto ser = zw->get_serializer();
    ASSERT_TRUE(static_cast<bool>(ser));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(*ser));
    auto cmp = zw->get_compressor();
    ASSERT_TRUE(static_cast<bool>(cmp));
    EXPECT_EQ(typeid(chucho::noop_compressor), typeid(*cmp));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7776"), zw->get_endpoint());
    std::string pfx_str("Hi");
    std::vector<std::uint8_t> pfx(pfx_str.begin(), pfx_str.end());
    EXPECT_EQ(pfx, zw->get_prefix());
}

void configurator::zeromq_writer_no_prefix_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::zeromq_writer), typeid(*wrts[0]));
    auto zw = std::static_pointer_cast<chucho::zeromq_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(zw));
    auto ser = zw->get_serializer();
    ASSERT_TRUE(static_cast<bool>(ser));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(*ser));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7778"), zw->get_endpoint());
    EXPECT_TRUE(zw->get_prefix().empty());
}

#if defined(CHUCHO_HAVE_PROTOBUF)

void configurator::zeromq_writer_protobuf_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::zeromq_writer), typeid(*wrts[0]));
    auto zw = std::static_pointer_cast<chucho::zeromq_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(zw));
    auto ser = zw->get_serializer();
    ASSERT_TRUE(static_cast<bool>(ser));
    EXPECT_EQ(typeid(chucho::protobuf_serializer), typeid(*ser));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7779"), zw->get_endpoint());
    std::string pfx_str("Hi");
    std::vector<std::uint8_t> pfx(pfx_str.begin(), pfx_str.end());
    EXPECT_EQ(pfx, zw->get_prefix());
}

#endif

#endif

void configurator::zip_file_compressor_body()
{
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    auto rlr = fwrt->get_file_roller();
    ASSERT_EQ(typeid(chucho::time_file_roller), typeid(*rlr));
    auto trlr = std::static_pointer_cast<chucho::time_file_roller>(rlr);
    ASSERT_TRUE(static_cast<bool>(trlr));
    auto cmp = trlr->get_file_compressor();
    ASSERT_TRUE(static_cast<bool>(cmp));
#if defined(CHUCHO_HAVE_MINIZIP)
    ASSERT_EQ(typeid(chucho::zip_file_compressor), typeid(*cmp));
    EXPECT_EQ(700, cmp->get_min_index());
#else
    ASSERT_EQ(typeid(chucho::noop_file_compressor), typeid(*cmp));
    chucho::status_manager::get()->clear();
#endif
}

}

}
