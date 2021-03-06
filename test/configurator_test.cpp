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

#include "configurator_test.hpp"
#include <chucho/logger.hpp>
#include <chucho/status_manager.hpp>
#include <chucho/cache_and_release_filter.hpp>
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
#include <chucho/json_formatter.hpp>
#include <chucho/on_start_file_roll_trigger.hpp>
#if defined(CHUCHO_HAVE_BZIP2)
#include <chucho/bzip2_file_compressor.hpp>
#endif
#if defined(CHUCHO_HAVE_ZLIB)
#include <chucho/gzip_file_compressor.hpp>
#endif
#if defined(CHUCHO_HAVE_LIBARCHIVE)
#include <chucho/zip_file_compressor.hpp>
#endif
#if defined(CHUCHO_HAVE_LZMA)
#include <chucho/lzma_file_compressor.hpp>
#endif
#include <chucho/async_writer.hpp>
#include <chucho/sliding_numbered_file_roller.hpp>
#if defined(CHUCHO_WINDOWS)
#include <chucho/windows_event_log_writer.hpp>
#endif
#include <chucho/exception.hpp>
#include <chucho/configuration.hpp>
#if defined(CHUCHO_HAVE_RUBY)
#include <chucho/ruby_evaluator_filter.hpp>
#endif
#if defined(CHUCHO_HAVE_CURL)
#include <chucho/email_writer.hpp>
#include <chucho/level_threshold_email_trigger.hpp>
#include <chucho/loggly_writer.hpp>
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
#if defined(CHUCHO_HAVE_DOORS)
#include <chucho/door_writer.hpp>
#endif
#if defined(CHUCHO_HAVE_RABBITMQ)
#include <chucho/rabbitmq_writer.hpp>
#endif
#if defined(CHUCHO_HAVE_CAPN_PROTO)
#include <chucho/capn_proto_serializer.hpp>
#endif
#if defined(CHUCHO_HAVE_FLATBUFFERS)
#include <chucho/flatbuffers_serializer.hpp>
#endif
#if defined(CHUCHO_HAVE_AWSSDK)
#include <chucho/cloudwatch_writer.hpp>
#endif
#if defined(CHUCHO_HAVE_RDKAFKA)
#include <chucho/kafka_writer.hpp>
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
    chucho::status_manager::get().clear();
}

configurator::~configurator()
{
    EXPECT_LT(chucho::status_manager::get().get_level(), chucho::status::level::WARNING_);
}

#if defined(CHUCHO_HAVE_ACTIVEMQ)

void configurator::activemq_writer_queue_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& aw = dynamic_cast<chucho::activemq_writer&>(lgr->get_writer("chucho::activemq_writer"));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(aw.get_serializer()));
    EXPECT_EQ(std::string("tcp://127.0.0.1:61616"), aw.get_broker());
    EXPECT_EQ(activemq_writer::consumer_type::QUEUE, aw.get_consumer_type());
    EXPECT_EQ(std::string("MonkeyBalls"), aw.get_topic_or_queue());
}

void configurator::activemq_writer_topic_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& aw = dynamic_cast<chucho::activemq_writer&>(lgr->get_writer("chucho::activemq_writer"));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(aw.get_serializer()));
    EXPECT_EQ(std::string("tcp://127.0.0.1:61616"), aw.get_broker());
    EXPECT_EQ(activemq_writer::consumer_type::TOPIC, aw.get_consumer_type());
    EXPECT_EQ(std::string("MonkeyBalls"), aw.get_topic_or_queue());
}

void configurator::activemq_writer_topic_coalesce_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& aw = dynamic_cast<chucho::activemq_writer&>(lgr->get_writer("chucho::activemq_writer"));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(aw.get_serializer()));
    EXPECT_EQ(std::string("tcp://127.0.0.1:61616"), aw.get_broker());
    EXPECT_EQ(activemq_writer::consumer_type::TOPIC, aw.get_consumer_type());
    EXPECT_EQ(std::string("MonkeyBalls"), aw.get_topic_or_queue());
    EXPECT_EQ(301, aw.get_coalesce_max());
}

#endif

void configurator::async_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& awrt = dynamic_cast<chucho::async_writer&>(lgr->get_writer("chucho::async_writer"));
    auto stats = awrt.get_cache_stats();
    EXPECT_EQ(chucho::async_writer::DEFAULT_CHUNK_SIZE, stats.get_chunk_size());
    EXPECT_EQ(chucho::async_writer::DEFAULT_MAX_CHUNKS, stats.get_max_size() / stats.get_chunk_size());
    EXPECT_EQ(typeid(chucho::file_writer), typeid(awrt.get_writer()));
}

void configurator::async_writer_with_opts_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& awrt = dynamic_cast<chucho::async_writer&>(lgr->get_writer("chucho::async_writer"));
    auto stats = awrt.get_cache_stats();
    EXPECT_EQ(7000, stats.get_chunk_size());
    EXPECT_EQ(10, stats.get_max_size() / stats.get_chunk_size());
    EXPECT_EQ(typeid(chucho::file_writer), typeid(awrt.get_writer()));
    EXPECT_FALSE(awrt.get_flush_on_destruct());
}

#if defined(CHUCHO_HAVE_BZIP2)

void configurator::bzip2_file_compressor_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    auto& rlr = dynamic_cast<chucho::numbered_file_roller&>(fwrt.get_file_roller());
    auto cmp = rlr.get_file_compressor();
    ASSERT_TRUE(cmp != nullptr);
    ASSERT_EQ(typeid(chucho::bzip2_file_compressor), typeid(*cmp));
    EXPECT_EQ(1, cmp->get_min_index());
}

#endif

void configurator::cache_and_release_filter_body()
{
    auto& wrt = chucho::logger::get("will")->get_writer("chucho::cout_writer");
    ASSERT_EQ(1, wrt.get_filter_names().size());
    auto& flt = dynamic_cast<chucho::cache_and_release_filter&>(wrt.get_filter("chucho::cache_and_release_filter"));
    EXPECT_STREQ("chucho::cache_and_release_filter", flt.get_name().c_str());
    EXPECT_EQ(chucho::level::DEBUG_(), flt.get_cache_threshold());
    EXPECT_EQ(chucho::level::ERROR_(), flt.get_release_threshold());
    auto stats = flt.get_cache_stats();
    EXPECT_EQ(256 * 1024, stats.get_chunk_size());
    EXPECT_EQ(256 * 1024 * 7, stats.get_max_size());
}

void configurator::cerr_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& wrt = dynamic_cast<chucho::cerr_writer&>(lgr->get_writer("chucho::cerr_writer"));
    EXPECT_STREQ("chucho::cerr_writer", wrt.get_name().c_str());
}

#if defined(CHUCHO_HAVE_AWSSDK)

void configurator::cloudwatch_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& wrt = dynamic_cast<chucho::cloudwatch_writer&>(lgr->get_writer("chucho::cloudwatch_writer"));
    EXPECT_STREQ("chucho::cloudwatch_writer", wrt.get_name().c_str());
    EXPECT_STREQ("monkeyballs", wrt.get_log_group().c_str());
    EXPECT_STREQ("streamer", wrt.get_log_stream().c_str());
    EXPECT_STREQ("us-west-1", wrt.get_region().c_str());
    EXPECT_EQ(chucho::cloudwatch_writer::DEFAULT_BATCH_SIZE, wrt.get_batch_size());
    EXPECT_EQ(0, wrt.get_current_batch_size());
}

#endif

void configurator::configure(const char* const cnf)
{
    std::istringstream in(cnf);
    get_configurator().configure(in);
}

void configurator::configure_with_error(const char* const cnf)
{
    configure(cnf);
    EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get().get_level());
    chucho::status_manager::get().clear();
}

void configurator::cout_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& wrt = dynamic_cast<chucho::cout_writer&>(lgr->get_writer("chucho::cout_writer"));
    EXPECT_STREQ("chucho::cout_writer", wrt.get_name().c_str());
}

#if defined(CHUCHO_HAVE_DOORS)

void configurator::door_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& dwrt = dynamic_cast<chucho::door_writer&>(lgr->get_writer("chucho::door_writer"));
    EXPECT_STREQ("gargle", dwrt.get_file_name().c_str());
    chucho::status_manager::get().clear();
}

#endif

void configurator::duplicate_message_filter_body()
{
    auto& wrt = chucho::logger::get("will")->get_writer("chucho::cout_writer");
    ASSERT_EQ(1, wrt.get_filter_names().size());
    auto& flt = dynamic_cast<chucho::duplicate_message_filter&>(wrt.get_filter("chucho::duplicate_message_filter"));
    EXPECT_STREQ("chucho::duplicate_message_filter", flt.get_name().c_str());
}

#if defined(CHUCHO_HAVE_CURL)

void configurator::email_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& ewrt = dynamic_cast<chucho::email_writer&>(lgr->get_writer("chucho::email_writer"));
    EXPECT_EQ(7000, ewrt.get_buffer_capacity());
    EXPECT_EQ(0, ewrt.get_buffer_size());
    EXPECT_EQ(chucho::email_writer::connection_type::CLEAR, ewrt.get_connection_type());
    EXPECT_STREQ("whistler@mctweaky.com", ewrt.get_from().c_str());
    EXPECT_STREQ("mail.dummy.com", ewrt.get_host().c_str());
    ASSERT_TRUE(ewrt.get_password());
    EXPECT_STREQ("lumpy", ewrt.get_password()->c_str());
    EXPECT_EQ(123, ewrt.get_port());
    EXPECT_STREQ("%c", ewrt.get_subject().c_str());
    ASSERT_EQ(2, ewrt.get_to().size());
    EXPECT_STREQ("one@blubbery.com", ewrt.get_to()[0].c_str());
    EXPECT_STREQ("two@humid.org", ewrt.get_to()[1].c_str());
    auto& ltet = dynamic_cast<chucho::level_threshold_email_trigger&>(ewrt.get_trigger());
    EXPECT_EQ(*chucho::level::ERROR_(), *ltet.get_level());
    ASSERT_TRUE(ewrt.get_user());
    EXPECT_STREQ("scrumpy", ewrt.get_user()->c_str());
}

void configurator::loggly_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& lwrt = dynamic_cast<chucho::loggly_writer&>(lgr->get_writer("chucho::loggly_writer"));
    EXPECT_STREQ("monkey-balls", lwrt.get_token().c_str());
}

#endif

void configurator::file_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::file_writer&>(lgr->get_writer("chucho::file_writer"));
    EXPECT_EQ(std::string("hello.log"), fwrt.get_file_name());
    EXPECT_FALSE(fwrt.get_flush());
    EXPECT_EQ(chucho::file_writer::on_start::TRUNCATE, fwrt.get_on_start());
}

#if defined(CHUCHO_HAVE_ZLIB)

void configurator::gzip_file_compressor_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    auto& rlr = fwrt.get_file_roller();
    auto cmp = rlr.get_file_compressor();
    ASSERT_TRUE(cmp != nullptr);
    ASSERT_EQ(typeid(chucho::gzip_file_compressor), typeid(*cmp));
    EXPECT_EQ(7, cmp->get_min_index());
}

#endif

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
        chucho::status_manager::get().clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, bad[i++]);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get().get_level());
    }
    chucho::status_manager::get().clear();
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
        chucho::status_manager::get().clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, good[i++]);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::INFO_, chucho::status_manager::get().get_level());
    }
}

void configurator::json_formatter_body(const std::string& tmpl)
{
    auto dpos = tmpl.find("DS");
    auto fpos = tmpl.find("FIELDS");
    std::array<const char*, 2> dispositions = { "in", "ex" };
    for (const auto& dis : dispositions)
    {
        std::array<const char*, 3> bad =
        {
            "message, file_name, doggies, line_number",
            "dirty stuff",
            "message file_name"
        };
        for (const auto& b : bad)
        {
            chucho::logger::remove_unused_loggers();
            chucho::status_manager::get().clear();
            std::string rep = tmpl;
            rep.replace(dpos, 2, dis);
            rep.replace(fpos, 6, b);
            configure(rep.c_str());
            EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get().get_level());
        }
        chucho::status_manager::get().clear();
        std::array<const char*, 3> good =
        {
            "message",
            "message, file_name, line_number",
            "message,file_name,line_number,logger,host_name,diagnostic_context,function_name,level,marker,timestamp,process_id,thread"
        };
        for (const auto& g : good)
        {
            chucho::logger::remove_unused_loggers();
            chucho::status_manager::get().clear();
            std::string rep = tmpl;
            rep.replace(dpos, 2, dis);
            rep.replace(fpos, 6, g);
            configure(rep.c_str());
            EXPECT_EQ(chucho::status::level::INFO_, chucho::status_manager::get().get_level());
        }
    }
}

#if defined(CHUCHO_HAVE_RDKAFKA)

void configurator::kafka_writer_brokers_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& kwrt = dynamic_cast<chucho::kafka_writer&>(lgr->get_writer("chucho::kafka_writer"));
    EXPECT_STREQ("monkeyballs", kwrt.get_topic().c_str());
    EXPECT_STREQ("192.168.56.101", kwrt.get_config_value("bootstrap.servers").c_str());
}

void configurator::kafka_writer_config_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& kwrt = dynamic_cast<chucho::kafka_writer&>(lgr->get_writer("chucho::kafka_writer"));
    EXPECT_STREQ("chunkymonkey", kwrt.get_topic().c_str());
    EXPECT_STREQ("192.168.56.101", kwrt.get_config_value("bootstrap.servers").c_str());
}

#endif

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
        chucho::status_manager::get().clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, bad[i++]);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get().get_level());
    }
    chucho::status_manager::get().clear();
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
        chucho::status_manager::get().clear();
        std::string rep = tmpl;
        rep.replace(pos, 6, good[i].first);
        configure(rep.c_str());
        auto lgr = chucho::logger::get("will");
        ASSERT_EQ(1, lgr->get_writer_names().size());
        auto& wrt = lgr->get_writer("chucho::cout_writer");
        auto& flt = dynamic_cast<chucho::level_filter&>(wrt.get_filter("chucho::level_filter"));
        EXPECT_EQ(*chucho::level::INFO_(), *flt.get_level());
        EXPECT_EQ(chucho::filter::result::NEUTRAL, flt.get_on_match());
        EXPECT_EQ(good[i++].second, flt.get_on_mismatch());
    }
}

void configurator::level_threshold_filter_body()
{
    auto lgr = chucho::logger::get("will");
    auto names = lgr->get_writer_names();
    ASSERT_EQ(1, names.size());
    auto& wrt = lgr->get_writer(names[0]);
    auto fnames = wrt.get_filter_names();
    ASSERT_EQ(1, fnames.size());
    auto& flt = dynamic_cast<chucho::level_threshold_filter&>(wrt.get_filter(fnames[0]));
    EXPECT_EQ(*chucho::level::FATAL_(), *flt.get_level());
}

void configurator::logger_body()
{
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    ASSERT_TRUE(static_cast<bool>(lgr->get_level()));
    EXPECT_EQ(*chucho::level::FATAL_(), *lgr->get_level());
    EXPECT_FALSE(lgr->writes_to_ancestors());
}

#if defined(CHUCHO_HAVE_LZMA)

void configurator::lzma_file_compressor_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    auto& rlr = fwrt.get_file_roller();
    auto cmp = rlr.get_file_compressor();
    ASSERT_TRUE(cmp != nullptr);
    ASSERT_EQ(typeid(chucho::lzma_file_compressor), typeid(*cmp));
    EXPECT_EQ(1, cmp->get_min_index());
}

#endif

void configurator::multiple_writer_body()
{
    auto lgr = chucho::logger::get("will");
    auto wrts = lgr->get_writer_names();
    ASSERT_EQ(2, wrts.size());
    auto& wrt1 = dynamic_cast<chucho::file_writer&>(lgr->get_writer(wrts[0]));
    EXPECT_EQ(std::string("one.log"), wrt1.get_file_name());
    auto& wrt2 = dynamic_cast<chucho::file_writer&>(lgr->get_writer(wrts[1]));
    EXPECT_EQ(std::string("two.log"), wrt2.get_file_name());
}

void configurator::named_pipe_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& npwrt = dynamic_cast<chucho::named_pipe_writer&>(lgr->get_writer("chucho::named_pipe_writer"));
    EXPECT_FALSE(npwrt.get_flush());
    #if defined(CHUCHO_WINDOWS)
    std::string pname("\\\\.\\pipe\\monkeyballs");
    #else
    std::string pname("monkeyballs");
    #endif
    EXPECT_EQ(pname, npwrt.get_file_name());
    chucho::status_manager::get().clear();
}

void configurator::numbered_file_roller_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    auto& nrlr = dynamic_cast<chucho::numbered_file_roller&>(fwrt.get_file_roller());
    EXPECT_EQ(5, nrlr.get_max_index());
    EXPECT_EQ(-3, nrlr.get_min_index());
}

void configurator::on_start_file_roll_trigger_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    EXPECT_NO_THROW(auto& ons = dynamic_cast<chucho::on_start_file_roll_trigger&>(fwrt.get_file_roll_trigger()));
}

void configurator::pipe_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& pwrt = dynamic_cast<chucho::pipe_writer&>(lgr->get_writer("chucho::pipe_writer"));
    EXPECT_FALSE(pwrt.get_flush());
}

#if defined(CHUCHO_HAVE_RABBITMQ)

void configurator::rabbitmq_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& pwrt = dynamic_cast<chucho::rabbitmq_writer&>(lgr->get_writer("chucho::rabbitmq_writer"));
    EXPECT_EQ(std::string("amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc"), pwrt.get_url());
    EXPECT_EQ(std::string("logs"), pwrt.get_exchange());
    EXPECT_TRUE(pwrt.get_routing_key().empty());
}

void configurator::rabbitmq_writer_coalesce_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& pwrt = dynamic_cast<chucho::rabbitmq_writer&>(lgr->get_writer("chucho::rabbitmq_writer"));
    EXPECT_EQ(std::string("amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc"), pwrt.get_url());
    EXPECT_EQ(std::string("logs"), pwrt.get_exchange());
    EXPECT_TRUE(pwrt.get_routing_key().empty());
    EXPECT_EQ(302, pwrt.get_coalesce_max());
}

#if defined(CHUCHO_HAVE_CAPN_PROTO)

void configurator::rabbitmq_writer_capn_proto_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& pwrt = dynamic_cast<chucho::rabbitmq_writer&>(lgr->get_writer("chucho::rabbitmq_writer"));
    EXPECT_EQ(typeid(chucho::capn_proto_serializer), typeid(pwrt.get_serializer()));
    EXPECT_EQ(std::string("amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc"), pwrt.get_url());
    EXPECT_EQ(std::string("logs"), pwrt.get_exchange());
    EXPECT_TRUE(pwrt.get_routing_key().empty());
}

#endif

#endif

void configurator::rolling_file_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    EXPECT_EQ(std::string("what.log"), fwrt.get_file_name());
    EXPECT_EQ(chucho::file_writer::on_start::TRUNCATE, fwrt.get_on_start());
    EXPECT_FALSE(fwrt.get_flush());
    auto& nrlr = dynamic_cast<chucho::numbered_file_roller&>(fwrt.get_file_roller());
    EXPECT_EQ(3, nrlr.get_min_index());
    EXPECT_EQ(5, nrlr.get_max_index());
    auto& strg = dynamic_cast<chucho::size_file_roll_trigger&>(fwrt.get_file_roll_trigger());
    EXPECT_EQ(5000, strg.get_max_size());
}

void configurator::root_alias_body()
{
    auto lgr = chucho::logger::get("");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    EXPECT_EQ(typeid(chucho::cout_writer), typeid(lgr->get_writer("chucho::cout_writer")));

}

#if defined(CHUCHO_HAVE_RUBY)

void configurator::ruby_evaluator_filter_body()
{
    auto lgr = chucho::logger::get("will");
    auto names = lgr->get_writer_names();
    ASSERT_EQ(1, names.size());
    auto& wrt = lgr->get_writer(names[0]);
    auto fnames = wrt.get_filter_names();
    ASSERT_EQ(1, fnames.size());
    auto& flt = dynamic_cast<chucho::ruby_evaluator_filter&>(wrt.get_filter(fnames[0]));
    EXPECT_EQ(std::string("$logger == \"will\""), flt.get_expression());
}

#endif

void configurator::size_file_roll_trigger_body(const std::string& tmpl)
{
    std::size_t pos = tmpl.find("SIZE");
    std::vector<const char*> bads{ "", "Willy", "5000x", "5000gx", "5000gbx" };
    for (auto bad : bads)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get().clear();
        std::string rep = tmpl;
        rep.replace(pos, 4, bad);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get().get_level());
    }
    chucho::status_manager::get().clear();
    std::vector<std::pair<const char*, std::uintmax_t>> goods{
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
        { "1gB", 1024ULL * 1024ULL * 1024ULL }
    };
    for (const auto& good : goods)
    {
        chucho::logger::remove_unused_loggers();
        std::string rep = tmpl;
        rep.replace(pos, 4, good.first);
        configure(rep.c_str());
        auto lgr = chucho::logger::get("will");
        ASSERT_EQ(1, lgr->get_writer_names().size());
        auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
        auto& strg = dynamic_cast<chucho::size_file_roll_trigger&>(fwrt.get_file_roll_trigger());
        EXPECT_EQ(good.second, strg.get_max_size());
    }
}

void configurator::sliding_numbered_file_roller_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    auto& srlr = dynamic_cast<chucho::sliding_numbered_file_roller&>(fwrt.get_file_roller());
    EXPECT_EQ(5, srlr.get_max_count());
    EXPECT_EQ(-3, srlr.get_min_index());
}

void configurator::syslog_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& wrt = dynamic_cast<chucho::syslog_writer&>(lgr->get_writer("chucho::syslog_writer"));
    EXPECT_EQ(chucho::syslog::facility::LOCAL0, wrt.get_facility());
    EXPECT_EQ(std::string("localhost"), wrt.get_host_name());
    ASSERT_TRUE(wrt.get_port());
    EXPECT_EQ(514, *wrt.get_port());
}

void configurator::syslog_writer_facility_body(const std::string& tmpl)
{
    std::size_t pos = tmpl.find("FCL");
    std::vector<const char*> bads{ "kernel", "my dog has fleas", "" };
    for (auto bad : bads)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get().clear();
        std::string rep = tmpl;
        rep.replace(pos, 3, bad);
        configure(rep.c_str());
        EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get().get_level());
    }
    // Visual Studio 2012 does not have initializer lists
    std::vector<std::pair<const char*, chucho::syslog::facility>> goods{
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
    for (const auto& good : goods)
    {
        chucho::logger::remove_unused_loggers();
        chucho::status_manager::get().clear();
        std::string rep = tmpl;
        rep.replace(pos, 3, good.first);
        configure(rep.c_str());
        auto lgr = chucho::logger::get("will");
        ASSERT_EQ(1, lgr->get_writer_names().size());
        auto& wrt = dynamic_cast<chucho::syslog_writer&>(lgr->get_writer("chucho::syslog_writer"));
        EXPECT_EQ(good.second, wrt.get_facility());
    }
}

void configurator::syslog_writer_port_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& wrt = dynamic_cast<chucho::syslog_writer&>(lgr->get_writer("chucho::syslog_writer"));
    EXPECT_EQ(chucho::syslog::facility::LOCAL0, wrt.get_facility());
    EXPECT_EQ(std::string("localhost"), wrt.get_host_name());
    ASSERT_TRUE(wrt.get_port());
    EXPECT_EQ(19567, *wrt.get_port());
}

void configurator::time_file_roller_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    auto& trlr = dynamic_cast<chucho::time_file_roller&>(fwrt.get_file_roller());
    EXPECT_EQ(std::string("%d{%d}"), trlr.get_file_name_pattern());
    EXPECT_EQ(5, trlr.get_max_history());
    EXPECT_EQ(&trlr, &fwrt.get_file_roll_trigger());
}

#if defined(CHUCHO_WINDOWS)

void configurator::windows_event_log_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& welw = dynamic_cast<chucho::windows_event_log_writer&>(lgr->get_writer("chucho::windows_event_log_writer"));
    EXPECT_TRUE(welw.get_host().empty());
    EXPECT_EQ(std::string("what"), welw.get_source());
    EXPECT_EQ(std::string("hello"), welw.get_log());
    chucho::status_manager::get().clear();
}

void configurator::windows_event_log_writer_no_log_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& welw = dynamic_cast<chucho::windows_event_log_writer&>(lgr->get_writer("chucho::windows_event_log_writer"));
    EXPECT_TRUE(welw.get_host().empty());
    EXPECT_EQ(std::string("what"), welw.get_source());
    EXPECT_EQ(std::string("Application"), welw.get_log());
    chucho::status_manager::get().clear();
}

#endif

void configurator::yaml_formatter_body(const std::string& tmpl)
{
    auto dpos = tmpl.find("DS");
    auto fpos = tmpl.find("FIELDS");
    std::array<const char*, 2> dispositions = { "in", "ex" };
    for (const auto& dis : dispositions)
    {
        std::array<const char*, 3> bad =
        {
            "message, file_name, doggies, line_number",
            "dirty stuff",
            "message file_name"
        };
        for (const auto& b : bad)
        {
            chucho::logger::remove_unused_loggers();
            chucho::status_manager::get().clear();
            std::string rep = tmpl;
            rep.replace(dpos, 2, dis);
            rep.replace(fpos, 6, b);
            configure(rep.c_str());
            EXPECT_EQ(chucho::status::level::ERROR_, chucho::status_manager::get().get_level());
        }
        chucho::status_manager::get().clear();
        std::array<const char*, 3> good =
        {
            "message",
            "message, file_name, line_number",
            "message,file_name,line_number,logger,host_name,diagnostic_context,function_name,level,marker,timestamp,process_id,thread"
        };
        for (const auto& g : good)
        {
            chucho::logger::remove_unused_loggers();
            chucho::status_manager::get().clear();
            std::string rep = tmpl;
            rep.replace(dpos, 2, dis);
            rep.replace(fpos, 6, g);
            configure(rep.c_str());
            EXPECT_EQ(chucho::status::level::INFO_, chucho::status_manager::get().get_level());
        }
    }
}

#if defined(CHUCHO_HAVE_ZEROMQ)

void configurator::zeromq_writer_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& zw = dynamic_cast<chucho::zeromq_writer&>(lgr->get_writer("chucho::zeromq_writer"));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(zw.get_serializer()));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7777"), zw.get_endpoint());
    std::string pfx_str("Hi");
    std::vector<std::uint8_t> pfx(pfx_str.begin(), pfx_str.end());
    EXPECT_EQ(pfx, zw.get_prefix());
}

void configurator::zeromq_writer_coalesce_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& zw = dynamic_cast<chucho::zeromq_writer&>(lgr->get_writer("chucho::zeromq_writer"));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(zw.get_serializer()));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7780"), zw.get_endpoint());
    std::string pfx_str("Hi");
    std::vector<std::uint8_t> pfx(pfx_str.begin(), pfx_str.end());
    EXPECT_EQ(pfx, zw.get_prefix());
    EXPECT_EQ(zw.get_coalesce_max(), 300);
}

void configurator::zeromq_writer_with_compressor_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& zw = dynamic_cast<chucho::zeromq_writer&>(lgr->get_writer("chucho::zeromq_writer"));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(zw.get_serializer()));
    auto& cmp = zw.get_compressor();
    ASSERT_TRUE(static_cast<bool>(cmp));
    EXPECT_EQ(typeid(chucho::noop_compressor), typeid(*cmp));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7776"), zw.get_endpoint());
    std::string pfx_str("Hi");
    std::vector<std::uint8_t> pfx(pfx_str.begin(), pfx_str.end());
    EXPECT_EQ(pfx, zw.get_prefix());
}

void configurator::zeromq_writer_no_prefix_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& zw = dynamic_cast<chucho::zeromq_writer&>(lgr->get_writer("chucho::zeromq_writer"));
    EXPECT_EQ(typeid(chucho::formatted_message_serializer), typeid(zw.get_serializer()));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7778"), zw.get_endpoint());
    EXPECT_TRUE(zw.get_prefix().empty());
}

#if defined(CHUCHO_HAVE_PROTOBUF)

void configurator::zeromq_writer_protobuf_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& zw = dynamic_cast<chucho::zeromq_writer&>(lgr->get_writer("chucho::zeromq_writer"));
    EXPECT_EQ(typeid(chucho::protobuf_serializer), typeid(zw.get_serializer()));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7779"), zw.get_endpoint());
    std::string pfx_str("Hi");
    std::vector<std::uint8_t> pfx(pfx_str.begin(), pfx_str.end());
    EXPECT_EQ(pfx, zw.get_prefix());
}

#endif

#if defined(CHUCHO_HAVE_FLATBUFFERS)

void configurator::zeromq_writer_flatbuffers_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& zw = dynamic_cast<chucho::zeromq_writer&>(lgr->get_writer("chucho::zeromq_writer"));
    EXPECT_EQ(typeid(chucho::flatbuffers_serializer), typeid(zw.get_serializer()));
    EXPECT_EQ(std::string("tcp://127.0.0.1:7781"), zw.get_endpoint());
    std::string pfx_str("Hi");
    std::vector<std::uint8_t> pfx(pfx_str.begin(), pfx_str.end());
    EXPECT_EQ(pfx, zw.get_prefix());
}

#endif

#endif

#if defined(CHUCHO_HAVE_LIBARCHIVE)

void configurator::zip_file_compressor_body()
{
    auto lgr = chucho::logger::get("will");
    ASSERT_EQ(1, lgr->get_writer_names().size());
    auto& fwrt = dynamic_cast<chucho::rolling_file_writer&>(lgr->get_writer("chucho::rolling_file_writer"));
    auto& trlr = dynamic_cast<chucho::time_file_roller&>(fwrt.get_file_roller());
    auto cmp = trlr.get_file_compressor();
    ASSERT_TRUE(static_cast<bool>(cmp));
    ASSERT_EQ(typeid(chucho::zip_file_compressor), typeid(*cmp));
    EXPECT_EQ(700, cmp->get_min_index());
}

#endif

}

}
