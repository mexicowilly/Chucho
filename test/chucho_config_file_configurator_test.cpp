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
#include <chucho/config_file_configurator.hpp>
#include <chucho/logger.hpp>
#include <chucho/level_threshold_filter.hpp>
#include <chucho/configuration.hpp>

namespace
{

class chucho_config_file_configurator : public chucho::test::configurator
{
protected:
    chucho_config_file_configurator();

    virtual chucho::configurator& get_configurator() override;

private:
    chucho::config_file_configurator cnf_;
};

chucho_config_file_configurator::chucho_config_file_configurator()
    : cnf_(chucho::configuration::get_security_policy())
{
}

chucho::configurator& chucho_config_file_configurator::get_configurator()
{
    return cnf_;
}

}

#if defined(CHUCHO_HAVE_ACTIVEMQ)

TEST_F(chucho_config_file_configurator, activemq_writer_bad)
{
    configure_with_error("chucho.logger = will\n"
                         "chucho.logger.will.writer = aw\n"
                         "chucho.writer.aw = chucho::activemq_writer\n"
                         "chucho.writer.aw.formatter = pf\n"
                         "chucho.formatter.pf = chucho::pattern_formatter\n"
                         "chucho.formatter.pf.pattern = %m\n"
                         "chucho.writer.aw.serializer = fms\n"
                         "chucho.serializer.fms = chucho::formatted_message_serializer\n"
                         "chucho.writer.aw.broker = tcp://127.0.0.1:61616\n"
                         "chucho.writer.aw.consumer_type = junk\n"
                         "chucho.writer.aw.topic_or_queue = MonkeyBalls");
}

TEST_F(chucho_config_file_configurator, activemq_writer_queue)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = aw\n"
              "chucho.writer.aw = chucho::activemq_writer\n"
              "chucho.writer.aw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.aw.serializer = fms\n"
              "chucho.serializer.fms = chucho::formatted_message_serializer\n"
              "chucho.writer.aw.broker = tcp://127.0.0.1:61616\n"
              "chucho.writer.aw.consumer_type = QUEUE\n"
              "chucho.writer.aw.topic_or_queue = MonkeyBalls");
    activemq_writer_queue_body();
}

TEST_F(chucho_config_file_configurator, activemq_writer_topic)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = aw\n"
              "chucho.writer.aw = chucho::activemq_writer\n"
              "chucho.writer.aw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.aw.serializer = fms\n"
              "chucho.serializer.fms = chucho::formatted_message_serializer\n"
              "chucho.writer.aw.broker = tcp://127.0.0.1:61616\n"
              "chucho.writer.aw.consumer_type = ToPiC\n"
              "chucho.writer.aw.topic_or_queue = MonkeyBalls");
    activemq_writer_topic_body();
}

TEST_F(chucho_config_file_configurator, activemq_writer_topic_coalesce)
{
    configure("chucho.logger = will\n"
                  "chucho.logger.will.writer = aw\n"
                  "chucho.writer.aw = chucho::activemq_writer\n"
                  "chucho.writer.aw.formatter = pf\n"
                  "chucho.formatter.pf = chucho::pattern_formatter\n"
                  "chucho.formatter.pf.pattern = %m\n"
                  "chucho.writer.aw.serializer = fms\n"
                  "chucho.serializer.fms = chucho::formatted_message_serializer\n"
                  "chucho.writer.aw.broker = tcp://127.0.0.1:61616\n"
                  "chucho.writer.aw.consumer_type = ToPiC\n"
                  "chucho.writer.aw.coalesce_max = 301\n"
                  "chucho.writer.aw.topic_or_queue = MonkeyBalls");
    activemq_writer_topic_coalesce_body();
}

#endif

TEST_F(chucho_config_file_configurator, async_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = as\n"
              "chucho.writer.as = chucho::async_writer\n"
              "chucho.writer.as.writer = fw\n"
              "chucho.writer.fw = chucho::file_writer\n"
              "chucho.writer.fw.file_name = hello.log\n"
              "chucho.writer.fw.formatter = fmt\n"
              "chucho.formatter.fmt = chucho::pattern_formatter\n"
              "chucho.formatter.fmt.pattern = %m%n\n");
    async_writer_body();
}

TEST_F(chucho_config_file_configurator, async_writer_with_opts)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = as\n"
              "chucho.writer.as = chucho::async_writer\n"
              "chucho.writer.as.writer = fw\n"
              "chucho.writer.as.chunk_size = 7000\n"
              "chucho.writer.as.max_chunks = 10\n"
              "chucho.writer.as.flush_on_destruct = false\n"
              "chucho.writer.fw = chucho::file_writer\n"
              "chucho.writer.fw.file_name = hello.log\n"
              "chucho.writer.fw.formatter = fmt\n"
              "chucho.formatter.fmt = chucho::pattern_formatter\n"
              "chucho.formatter.fmt.pattern = %m%n\n");
    async_writer_with_opts_body();
}

#if defined(CHUCHO_HAVE_BZIP2)

TEST_F(chucho_config_file_configurator, bzip2_file_compressor)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = rfw\n"
              "chucho.writer.rfw = chucho::rolling_file_writer\n"
              "chucho.writer.rfw.formatter = pf\n"
              "chucho.writer.rfw.file_roller = nfr\n"
              "chucho.writer.rfw.file_roll_trigger = sfrt\n"
              "chucho.writer.rfw.file_name = what.log\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.file_roller.nfr = chucho::numbered_file_roller\n"
              "chucho.file_roller.nfr.min_index = 3\n"
              "chucho.file_roller.nfr.max_index = 5\n"
              "chucho.file_roller.nfr.file_compressor = bz\n"
              "chucho.file_compressor.bz = chucho::bzip2_file_compressor\n"
              "chucho.file_compressor.bz.min_index = 1\n"
              "chucho.file_roll_trigger.sfrt = chucho::size_file_roll_trigger\n"
              "chucho.file_roll_trigger.sfrt.max_size = 5000");
    bzip2_file_compressor_body();
}

#endif

TEST_F(chucho_config_file_configurator, cache_and_release_filter)
{
    configure(R"cnf(
chucho.logger = will
chucho.logger.will.writer = co
chucho.writer.co = chucho::cout_writer
chucho.writer.co.filter = crf
chucho.filter.crf = chucho::cache_and_release_filter
chucho.filter.crf.cache_threshold = DeBuG
chucho.filter.crf.release_threshold = ERROR
chucho.filter.crf.chunk_size = 256k
chucho.filter.crf.max_chunks = 7
chucho.writer.co.formatter = pf
chucho.formatter.pf = chucho::pattern_formatter
chucho.formatter.pf.pattern = %m%n
)cnf");
    cache_and_release_filter_body();
}

TEST_F(chucho_config_file_configurator, cerr_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = ce\n"
              "chucho.writer.ce = chucho::cerr_writer\n"
              "chucho.writer.ce.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n");
    cerr_writer_body();
}

#if defined(CHUCHO_HAVE_AWSSDK)

TEST_F(chucho_config_file_configurator, cloudwatch_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = ce\n"
              "chucho.writer.ce = chucho::cloudwatch_writer\n"
              "chucho.writer.ce.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.ce.log_group=monkeyballs\n"
              "chucho.writer.ce.log_stream=streamer\n"
              "chucho.writer.ce.region=us-west-1");
    cloudwatch_writer_body();
}

#endif

TEST_F(chucho_config_file_configurator, cout_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = ce\n"
              "chucho.writer.ce = chucho::cout_writer\n"
              "chucho.writer.ce.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n");
    cout_writer_body();
}

#if defined(CHUCHO_HAVE_DOORS)

TEST_F(chucho_config_file_configurator, door_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = dw\n"
              "chucho.writer.dw = chucho::door_writer\n"
              "chucho.writer.dw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.dw.file_name = gargle\n");
    door_writer_body();
}

#endif

TEST_F(chucho_config_file_configurator, duplicate_message_filter)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = co\n"
              "chucho.writer.co = chucho::cout_writer\n"
              "chucho.writer.co.filter = dmf\n"
              "chucho.filter.dmf = chucho::duplicate_message_filter\n"
              "chucho.writer.co.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n");
    duplicate_message_filter_body();
}

#if defined(CHUCHO_HAVE_CURL)

TEST_F(chucho_config_file_configurator, email_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = em\n"
              "chucho.writer.em = chucho::email_writer\n"
              "chucho.writer.em.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.em.email_trigger = trg\n"
              "chucho.email_trigger.trg = chucho::level_threshold_email_trigger\n"
              "chucho.email_trigger.trg.level = error\n"
              "chucho.writer.em.host = mail.dummy.com\n"
              "chucho.writer.em.port = 123\n"
              "chucho.writer.em.from = whistler@mctweaky.com\n"
              "chucho.writer.em.to = one@blubbery.com,two@humid.org\n"
              "chucho.writer.em.subject = %c\n"
              "chucho.writer.em.connection_type = clear\n"
              "chucho.writer.em.user = scrumpy\n"
              "chucho.writer.em.password = lumpy\n"
              "chucho.writer.em.buffer_size = 7000");
    email_writer_body();
}

TEST_F(chucho_config_file_configurator, loggly_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = lw\n"
              "chucho.writer.lw = chucho::loggly_writer\n"
              "chucho.writer.lw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.lw.token = monkey-balls");
    loggly_writer_body();
}

#endif

TEST_F(chucho_config_file_configurator, file_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = fw\n"
              "chucho.writer.fw = chucho::file_writer\n"
              "chucho.writer.fw.file_name = hello.log\n"
              "chucho.writer.fw.on_start = truncate\n"
              "chucho.writer.fw.flush = false\n"
              "chucho.writer.fw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n");
    file_writer_body();
}

TEST_F(chucho_config_file_configurator, file_writer_invalid_1)
{
    configure_with_error("chucho.logger = will\n"
                         "chucho.logger.will.writer = fw\n"
                         "chucho.writer.fw = chucho::file_write\n"
                         "chucho.writer.fw.file_name = hello.log\n"
                         "chucho.writer.fw.on_start = truncate\n"
                         "chucho.writer.fw.flush = false\n"
                         "chucho.writer.fw.formatter = pf\n"
                         "chucho.formatter.pf = chucho::pattern_formatter\n"
                         "chucho.formatter.pf.pattern = %m%n");
}

TEST_F(chucho_config_file_configurator, file_writer_invalid_2)
{
    configure_with_error("chucho.logger = will\n"
                         "chucho.logger.will.writer = fw\n"
                         "chucho.writer.fw = chucho::file_writer");
}

TEST_F(chucho_config_file_configurator, file_writer_invalid_3)
{
    configure_with_error("chucho.logger = will\n"
                         "chucho.logger.will.writer = fw\n"
                         "chucho.writer.fw = chucho::file_writer\n"
                         "chucho.writer.fw.file_nam = hello.log\n"
                         "chucho.writer.fw.on_star = truncate\n"
                         "chucho.writer.fw.flus = false\n"
                         "chucho.writer.fw.formatter = pf\n"
                         "chucho.formatter.pf = chucho::pattern_formatter\n"
                         "chucho.formatter.pf.pattern = %m%n");
}

TEST_F(chucho_config_file_configurator, file_writer_invalid_4)
{
    configure_with_error("chucho.logger = will\n"
                         "chucho.logger.will.writer = fw\n"
                         "chucho.writer.fw = chucho::file_writer\n"
                         "chucho.writer.fw.file_name = hello.log\n"
                         "chucho.writer.fw.on_start = truncate\n"
                         "chucho.writer.fw.flush = false\n"
                         "chucho.writer.fw.formatter = pf\n"
                         "chucho.formatter.pf = chucho::cout_writer\n"
                         "chucho.formatter.pf.pattern = %m%n");
}

TEST_F(chucho_config_file_configurator, file_writer_invalid_5)
{
    configure_with_error("chucho.logger = will\n"
                         "chucho.logger.will.writer = fw\n"
                         "chucho.writer.fw = chucho::file_writer\n"
                         "chucho.writer.fw.file_name = hello.log\n"
                         "chucho.writer.fw.on_start = truncate\n"
                         "chucho.writer.fw.flush = false\n"
                         "chucho.writer.fw.formatter = pf\n"
                         "chucho.formatter.pf = chucho::cout_writer\n"
                         "chucho.formatter.pf.formatter = pf2\n"
                         "chucho.formatter.pf2 = chucho::pattern_formatter\n"
                         "chucho.formatter.pf2.patter = %m%n");
}

TEST_F(chucho_config_file_configurator, file_writer_invalid_6)
{
    configure_with_error("chucho.logger = will\n"
                         "chucho.logger.will.writer = fw");
}

TEST_F(chucho_config_file_configurator, filter_order)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = co\n"
              "chucho.writer.co = chucho::cout_writer\n"
              "chucho.writer.co.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.co.filter = z\n"
              "chucho.filter.z = chucho::level_threshold_filter\n"
              "chucho.filter.z.level = fatal\n"
              "chucho.filter.z.name = one\n"
              "chucho.writer.co.filter = a\n"
              "chucho.filter.a = chucho::level_threshold_filter\n"
              "chucho.filter.a.level = debug\n"
              "chucho.filter.a.name = two\n"
              "chucho.writer.co.filter = m\n"
              "chucho.filter.m = chucho::level_threshold_filter\n"
              "chucho.filter.m.level = warn\n"
              "chucho.filter.m.name = three");
    auto lgr = chucho::logger::get("will");
    auto wrts = lgr->get_writer_names();
    ASSERT_EQ(1, wrts.size());
    auto& wrt = lgr->get_writer(wrts[0]);
    auto flts = wrt.get_filter_names();
    ASSERT_EQ(3, flts.size());
    EXPECT_EQ(*chucho::level::FATAL_(),
              *dynamic_cast<chucho::level_threshold_filter&>(wrt.get_filter("one")).get_level());
    EXPECT_EQ(*chucho::level::DEBUG_(),
              *dynamic_cast<chucho::level_threshold_filter&>(wrt.get_filter("two")).get_level());
    EXPECT_EQ(*chucho::level::WARN_(),
              *dynamic_cast<chucho::level_threshold_filter&>(wrt.get_filter("three")).get_level());
}

#if defined(CHUCHO_HAVE_ZLIB)

TEST_F(chucho_config_file_configurator, gzip_file_compressor)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = rfw\n"
              "chucho.writer.rfw = chucho::rolling_file_writer\n"
              "chucho.writer.rfw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.rfw.file_roller = tfr\n"
              "chucho.file_roller.tfr = chucho::time_file_roller\n"
              "chucho.file_roller.tfr.file_name_pattern = %d{%d}\n"
              "chucho.file_roller.tfr.max_history = 5\n"
              "chucho.file_roller.tfr.file_compressor = gz\n"
              "chucho.file_compressor.gz = chucho::gzip_file_compressor\n"
              "chucho.file_compressor.gz.min_index = 7");
    gzip_file_compressor_body();
}

#endif

TEST_F(chucho_config_file_configurator, interval_file_roll_trigger)
{
    std::string tmpl("chucho.logger = will\n"
                     "chucho.logger.will.writer = rfw\n"
                     "chucho.writer.rfw = chucho::rolling_file_writer\n"
                     "chucho.writer.rfw.formatter = pf\n"
                     "chucho.formatter.pf = chucho::pattern_formatter\n"
                     "chucho.formatter.pf.pattern = %m%n\n"
                     "chucho.writer.rfw.file_roller = nfr\n"
                     "chucho.file_roller.nfr = chucho::numbered_file_roller\n"
                     "chucho.file_roller.nfr.max_index = 1\n"
                     "chucho.writer.rfw.file_roll_trigger = ifrt\n"
                     "chucho.file_roll_trigger.ifrt = chucho::interval_file_roll_trigger\n"
                     "chucho.file_roll_trigger.ifrt.every = PERIOD\n"
                     "chucho.writer.rfw.file_name = what.log");
    interval_file_roll_trigger_body(tmpl);
}

#if defined(CHUCHO_HAVE_RDKAFKA)

TEST_F(chucho_config_file_configurator, kafka_writer_brokers)
{
    configure(R"cnf(
chucho.logger = will
chucho.logger.will.writer = kw
chucho.writer.kw = chucho::kafka_writer
chucho.writer.kw.formatter = pf
chucho.formatter.pf = chucho::pattern_formatter
chucho.formatter.pf.pattern = %m
chucho.writer.kw.serializer = fms
chucho.serializer.fms = chucho::formatted_message_serializer
chucho.writer.kw.brokers = 192.168.56.101
chucho.writer.kw.topic = monkeyballs
)cnf");
    kafka_writer_brokers_body();
}

TEST_F(chucho_config_file_configurator, kafka_writer_config)
{
    configure(R"cnf(
chucho.logger = will
chucho.logger.will.writer = kw
chucho.writer.kw = chucho::kafka_writer
chucho.writer.kw.formatter = pf
chucho.formatter.pf = chucho::pattern_formatter
chucho.formatter.pf.pattern = %m
chucho.writer.kw.serializer = fms
chucho.serializer.fms = chucho::formatted_message_serializer
chucho.writer.kw.topic = chunkymonkey
chucho.writer.kw.kafka = kc
chucho.kafka.kc = kafka_configuration
chucho.kafka.kc.bootstrap.servers = 192.168.56.101
)cnf");
    kafka_writer_config_body();
}

#endif

TEST_F(chucho_config_file_configurator, level_filter)
{
    std::string tmpl("chucho.logger = will\n"
                     "chucho.logger.will.writer = co\n"
                     "chucho.writer.co = chucho::cout_writer\n"
                     "chucho.writer.co.formatter = pf\n"
                     "chucho.formatter.pf = chucho::pattern_formatter\n"
                     "chucho.formatter.pf.pattern = %m%n\n"
                     "chucho.writer.co.filter = lf\n"
                     "chucho.filter.lf = chucho::level_filter\n"
                     "chucho.filter.lf.level = info\n"
                     "chucho.filter.lf.on_match = NEUTRAL\n"
                     "chucho.filter.lf.on_mismatch = RESULT");
    level_filter_body(tmpl);
}

TEST_F(chucho_config_file_configurator, level_threshold_filter)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = co\n"
              "chucho.writer.co = chucho::cout_writer\n"
              "chucho.writer.co.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.co.filter = ltf\n"
              "chucho.filter.ltf = chucho::level_threshold_filter\n"
              "chucho.filter.ltf.level = fatal");
    level_threshold_filter_body();
}

TEST_F(chucho_config_file_configurator, logger)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.level = fatal\n"
              "chucho.logger.will.writes_to_ancestors = false");
    logger_body();
}

#if defined(CHUCHO_HAVE_LZMA)

TEST_F(chucho_config_file_configurator, lzma_file_compressor)
{
    configure("chucho.logger = will\n"
                  "chucho.logger.will.writer = rfw\n"
                  "chucho.writer.rfw = chucho::rolling_file_writer\n"
                  "chucho.writer.rfw.formatter = pf\n"
                  "chucho.writer.rfw.file_roller = nfr\n"
                  "chucho.writer.rfw.file_roll_trigger = sfrt\n"
                  "chucho.writer.rfw.file_name = what.log\n"
                  "chucho.formatter.pf = chucho::pattern_formatter\n"
                  "chucho.formatter.pf.pattern = %m%n\n"
                  "chucho.file_roller.nfr = chucho::numbered_file_roller\n"
                  "chucho.file_roller.nfr.min_index = 3\n"
                  "chucho.file_roller.nfr.max_index = 5\n"
                  "chucho.file_roller.nfr.file_compressor = lz\n"
                  "chucho.file_compressor.lz = chucho::lzma_file_compressor\n"
                  "chucho.file_compressor.lz.min_index = 1\n"
                  "chucho.file_roll_trigger.sfrt = chucho::size_file_roll_trigger\n"
                  "chucho.file_roll_trigger.sfrt.max_size = 5000");
    lzma_file_compressor_body();
}

#endif

TEST_F(chucho_config_file_configurator, multiple_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = fw1\n"
              "chucho.writer.fw1 = chucho::file_writer\n"
              "chucho.writer.fw1.name = one\n"
              "chucho.writer.fw1.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.fw1.file_name = one.log\n"
              "chucho.logger.will.writer = fw2\n"
              "chucho.writer.fw2 = chucho::file_writer\n"
              "chucho.writer.fw2.name = two\n"
              "chucho.writer.fw2.formatter = pf\n"
              "chucho.writer.fw2.file_name = two.log");
    multiple_writer_body();
}

TEST_F(chucho_config_file_configurator, named_pipe_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = npw\n"
              "chucho.writer.npw = chucho::named_pipe_writer\n"
              "chucho.writer.npw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.npw.pipe_name = monkeyballs\n"
              "chucho.writer.npw.flush = false");
    named_pipe_writer_body();
}

TEST_F(chucho_config_file_configurator, numbered_file_roller)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = rfw\n"
              "chucho.writer.rfw = chucho::rolling_file_writer\n"
              "chucho.writer.rfw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.rfw.file_roller = nfr\n"
              "chucho.file_roller.nfr = chucho::numbered_file_roller\n"
              "chucho.file_roller.nfr.max_index = 5\n"
              "chucho.file_roller.nfr.min_index = -3\n"
              "chucho.writer.rfw.file_roll_trigger = sfrt\n"
              "chucho.file_roll_trigger.sfrt = chucho::size_file_roll_trigger\n"
              "chucho.file_roll_trigger.sfrt.max_size = 5000\n"
              "chucho.writer.rfw.file_name = hello");
    numbered_file_roller_body();
}

TEST_F(chucho_config_file_configurator, pipe_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = pw\n"
              "chucho.writer.pw = chucho::pipe_writer\n"
              "chucho.writer.pw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.pw.flush = false");
    pipe_writer_body();
}

#if defined(CHUCHO_HAVE_RABBITMQ)

TEST_F(chucho_config_file_configurator, rabbitmq_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = pg\n"
              "chucho.writer.pg = chucho::rabbitmq_writer\n"
              "chucho.writer.pg.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.pg.serializer = fms\n"
              "chucho.serializer.fms = chucho::formatted_message_serializer\n"
              "chucho.writer.pg.url = amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc\n"
              "chucho.writer.pg.exchange = logs");
    rabbitmq_writer_body();
}

TEST_F(chucho_config_file_configurator, rabbitmq_writer_coalesce)
{
    configure("chucho.logger = will\n"
                  "chucho.logger.will.writer = pg\n"
                  "chucho.writer.pg = chucho::rabbitmq_writer\n"
                  "chucho.writer.pg.coalesce_max = 302\n"
                  "chucho.writer.pg.formatter = pf\n"
                  "chucho.formatter.pf = chucho::pattern_formatter\n"
                  "chucho.formatter.pf.pattern = %m\n"
                  "chucho.writer.pg.serializer = fms\n"
                  "chucho.serializer.fms = chucho::formatted_message_serializer\n"
                  "chucho.writer.pg.url = amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc\n"
                  "chucho.writer.pg.exchange = logs");
    rabbitmq_writer_coalesce_body();
}

#if defined(CHUCHO_HAVE_RABBITMQ)

TEST_F(chucho_config_file_configurator, rabbitmq_writer_capn_proto)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = pg\n"
              "chucho.writer.pg = chucho::rabbitmq_writer\n"
              "chucho.writer.pg.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.pg.serializer = fms\n"
              "chucho.serializer.fms = chucho::capn_proto_serializer\n"
              "chucho.writer.pg.url = amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc\n"
              "chucho.writer.pg.exchange = logs");
    rabbitmq_writer_body();
}

#endif

#endif

TEST_F(chucho_config_file_configurator, rolling_file_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = rfw\n"
              "chucho.writer.rfw = chucho::rolling_file_writer\n"
              "chucho.writer.rfw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.rfw.file_roller = nfr\n"
              "chucho.file_roller.nfr = chucho::numbered_file_roller\n"
              "chucho.file_roller.nfr.min_index = 3\n"
              "chucho.file_roller.nfr.max_index = 5\n"
              "chucho.writer.rfw.file_roll_trigger = sfrt\n"
              "chucho.file_roll_trigger.sfrt = chucho::size_file_roll_trigger\n"
              "chucho.file_roll_trigger.sfrt.max_size = 5000\n"
              "chucho.writer.rfw.file_name = what.log\n"
              "chucho.writer.rfw.on_start = TruNCAte\n"
              "chucho.writer.rfw.flush = false");
    rolling_file_writer_body();
}

TEST_F(chucho_config_file_configurator, root_alias)
{
    chucho::logger::get("")->clear_writers();
    configure("chucho.logger = <root>\n"
              "chucho.logger.<root>.writer = ce\n"
              "chucho.writer.ce = chucho::cout_writer\n"
              "chucho.writer.ce.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n");
    root_alias_body();
}

#if defined(CHUCHO_HAVE_RUBY)

TEST_F(chucho_config_file_configurator, ruby_evaluator_filter)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = co\n"
              "chucho.writer.co = chucho::cout_writer\n"
              "chucho.writer.co.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.co.filter = ref\n"
              "chucho.filter.ref = chucho::ruby_evaluator_filter\n"
              "chucho.filter.ref.expression = $logger == \"will\"");
    ruby_evaluator_filter_body();
}

#endif

TEST_F(chucho_config_file_configurator, size_file_roll_trigger)
{
    std::string tmpl("chucho.logger = will\n"
                     "chucho.logger.will.writer = rfw\n"
                     "chucho.writer.rfw = chucho::rolling_file_writer\n"
                     "chucho.writer.rfw.formatter = pf\n"
                     "chucho.formatter.pf = chucho::pattern_formatter\n"
                     "chucho.formatter.pf.pattern = %m%n\n"
                     "chucho.writer.rfw.file_roller = nfr\n"
                     "chucho.file_roller.nfr = chucho::numbered_file_roller\n"
                     "chucho.file_roller.nfr.max_index = 1\n"
                     "chucho.writer.rfw.file_roll_trigger = sfrt\n"
                     "chucho.file_roll_trigger.sfrt = chucho::size_file_roll_trigger\n"
                     "chucho.file_roll_trigger.sfrt.max_size = SIZE\n"
                     "chucho.writer.rfw.file_name = what.log");
    size_file_roll_trigger_body(tmpl);
}

TEST_F(chucho_config_file_configurator, sliding_numbered_file_roller)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = rfw\n"
              "chucho.writer.rfw = chucho::rolling_file_writer\n"
              "chucho.writer.rfw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.rfw.file_roller = snfr\n"
              "chucho.file_roller.snfr = chucho::sliding_numbered_file_roller\n"
              "chucho.file_roller.snfr.max_count = 5\n"
              "chucho.file_roller.snfr.min_index = -3\n"
              "chucho.writer.rfw.file_roll_trigger = sfrt\n"
              "chucho.file_roll_trigger.sfrt = chucho::size_file_roll_trigger\n"
              "chucho.file_roll_trigger.sfrt.max_size = 5000\n"
              "chucho.writer.rfw.file_name = hello");
    sliding_numbered_file_roller_body();
}

TEST_F(chucho_config_file_configurator, syslog_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = sw\n"
              "chucho.writer.sw = chucho::syslog_writer\n"
              "chucho.writer.sw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.sw.facility = LOCAL0\n"
              "chucho.writer.sw.host_name = localhost");
    syslog_writer_body();
}

TEST_F(chucho_config_file_configurator, syslog_writer_facility)
{
    std::string tmpl("chucho.logger = will\n"
                     "chucho.logger.will.writer = sw\n"
                     "chucho.writer.sw = chucho::syslog_writer\n"
                     "chucho.writer.sw.formatter = pf\n"
                     "chucho.formatter.pf = chucho::pattern_formatter\n"
                     "chucho.formatter.pf.pattern = %m%n\n"
                     "chucho.writer.sw.facility = FCL");
    syslog_writer_facility_body(tmpl);
}

#if !defined(CHUCHO_SOLARIS)
TEST_F(chucho_config_file_configurator, syslog_writer_port)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = sw\n"
              "chucho.writer.sw = chucho::syslog_writer\n"
              "chucho.writer.sw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.sw.facility = LOCAL0\n"
              "chucho.writer.sw.host_name = localhost\n"
              "chucho.writer.sw.port = 19567");
    syslog_writer_port_body();
}
#endif

TEST_F(chucho_config_file_configurator, time_file_roller)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = rfw\n"
              "chucho.writer.rfw = chucho::rolling_file_writer\n"
              "chucho.writer.rfw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.rfw.file_roller = tfr\n"
              "chucho.file_roller.tfr = chucho::time_file_roller\n"
              "chucho.file_roller.tfr.file_name_pattern = %d{%d}\n"
              "chucho.file_roller.tfr.max_history = 5");
    time_file_roller_body();
}

#if defined(CHUCHO_WINDOWS)
TEST_F(chucho_config_file_configurator, windows_event_log_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = welw\n"
              "chucho.writer.welw = chucho::windows_event_log_writer\n"
              "chucho.writer.welw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.welw.source = what\n"
              "chucho.writer.welw.log = hello");
    windows_event_log_writer_body();
}

TEST_F(chucho_config_file_configurator, windows_event_log_writer_no_log)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = welw\n"
              "chucho.writer.welw = chucho::windows_event_log_writer\n"
              "chucho.writer.welw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.welw.source = what");
    windows_event_log_writer_no_log_body();
}
#endif

#if defined(CHUCHO_HAVE_ZEROMQ)

TEST_F(chucho_config_file_configurator, zeromq_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = zw\n"
              "chucho.writer.zw = chucho::zeromq_writer\n"
              "chucho.writer.zw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.zw.serializer = fms\n"
              "chucho.serializer.fms = chucho::formatted_message_serializer\n"
              "chucho.writer.zw.endpoint = tcp://127.0.0.1:7777\n"
              "chucho.writer.zw.prefix = Hi");
    zeromq_writer_body();
}

TEST_F(chucho_config_file_configurator, zeromq_writer_coalesce)
{
    configure("chucho.logger = will\n"
                  "chucho.logger.will.writer = zw\n"
                  "chucho.writer.zw = chucho::zeromq_writer\n"
                  "chucho.writer.zw.formatter = pf\n"
                  "chucho.formatter.pf = chucho::pattern_formatter\n"
                  "chucho.formatter.pf.pattern = %m\n"
                  "chucho.writer.zw.serializer = fms\n"
                  "chucho.writer.zw.coalesce_max = 300\n"
                  "chucho.serializer.fms = chucho::formatted_message_serializer\n"
                  "chucho.writer.zw.endpoint = tcp://127.0.0.1:7780\n"
                  "chucho.writer.zw.prefix = Hi");
    zeromq_writer_coalesce_body();
}

TEST_F(chucho_config_file_configurator, zeromq_writer_no_prefix)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = zw\n"
              "chucho.writer.zw = chucho::zeromq_writer\n"
              "chucho.writer.zw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.zw.serializer = fms\n"
              "chucho.serializer.fms = chucho::formatted_message_serializer\n"
              "chucho.writer.zw.endpoint = tcp://127.0.0.1:7778\n");
    zeromq_writer_no_prefix_body();
}

TEST_F(chucho_config_file_configurator, zeromq_writer_with_compressor)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = zw\n"
              "chucho.writer.zw = chucho::zeromq_writer\n"
              "chucho.writer.zw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.zw.serializer = fms\n"
              "chucho.serializer.fms = chucho::formatted_message_serializer\n"
              "chucho.writer.zw.endpoint = tcp://127.0.0.1:7776\n"
              "chucho.writer.zw.prefix = Hi\n"
              "chucho.writer.zw.compressor = cmp\n"
              "chucho.compressor.cmp = chucho::noop_compressor");
    zeromq_writer_with_compressor_body();
}

#if defined(CHUCHO_HAVE_PROTOBUF)

TEST_F(chucho_config_file_configurator, zeromq_writer_protobuf)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = zw\n"
              "chucho.writer.zw = chucho::zeromq_writer\n"
              "chucho.writer.zw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.zw.serializer = ps\n"
              "chucho.serializer.ps = chucho::protobuf_serializer\n"
              "chucho.writer.zw.endpoint = tcp://127.0.0.1:7779\n"
              "chucho.writer.zw.prefix = Hi");
    zeromq_writer_protobuf_body();
}

#endif

#if defined(CHUCHO_HAVE_FLATBUFFERS)

TEST_F(chucho_config_file_configurator, zeromq_writer_flatbuffers)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = zw\n"
              "chucho.writer.zw = chucho::zeromq_writer\n"
              "chucho.writer.zw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.zw.serializer = ps\n"
              "chucho.serializer.ps = chucho::flatbuffers_serializer\n"
              "chucho.writer.zw.endpoint = tcp://127.0.0.1:7781\n"
              "chucho.writer.zw.prefix = Hi");
    zeromq_writer_flatbuffers_body();
}

#endif

#endif

#if defined(CHUCHO_HAVE_LIBARCHIVE)

TEST_F(chucho_config_file_configurator, zip_file_compressor)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = rfw\n"
              "chucho.writer.rfw = chucho::rolling_file_writer\n"
              "chucho.writer.rfw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.rfw.file_roller = tfr\n"
              "chucho.file_roller.tfr = chucho::time_file_roller\n"
              "chucho.file_roller.tfr.file_name_pattern = %d{%d}\n"
              "chucho.file_roller.tfr.max_history = 5\n"
              "chucho.file_roller.tfr.file_compressor = zfc\n"
              "chucho.file_compressor.zfc = chucho::zip_file_compressor\n"
              "chucho.file_compressor.zfc.min_index = 700");
    zip_file_compressor_body();
}

#endif
