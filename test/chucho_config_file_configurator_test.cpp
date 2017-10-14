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
              "chucho.writer.as.discard_threshold = error\n"
              "chucho.writer.as.queue_capacity = 700\n"
              "chucho.writer.as.flush_on_destruct = false\n"
              "chucho.writer.fw = chucho::file_writer\n"
              "chucho.writer.fw.file_name = hello.log\n"
              "chucho.writer.fw.formatter = fmt\n"
              "chucho.formatter.fmt = chucho::pattern_formatter\n"
              "chucho.formatter.fmt.pattern = %m%n\n");
    async_writer_with_opts_body();
}

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

#if defined(CHUCHO_HAVE_DB2)

TEST_F(chucho_config_file_configurator, db2_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = or\n"
              "chucho.writer.or = chucho::db2_writer\n"
              "chucho.writer.or.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.or.user = db2inst1\n"
              "chucho.writer.or.password = db2inst1\n"
              "chucho.writer.or.database = chucho");
    db2_writer_body();
}

#endif

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

#if defined(CHUCHO_HAVE_EMAIL_WRITER)

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
              "chucho.writer.co.filter = a\n"
              "chucho.filter.a = chucho::level_threshold_filter\n"
              "chucho.filter.a.level = debug\n"
              "chucho.writer.co.filter = m\n"
              "chucho.filter.m = chucho::level_threshold_filter\n"
              "chucho.filter.m.level = warn");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    auto flts = wrts[0]->get_filters();
    ASSERT_EQ(3, flts.size());
    ASSERT_EQ(typeid(chucho::level_threshold_filter), typeid(*flts[0]));
    auto thresh = std::static_pointer_cast<chucho::level_threshold_filter>(flts[0]);
    EXPECT_EQ(*chucho::level::FATAL_(), *thresh->get_level());
    ASSERT_EQ(typeid(chucho::level_threshold_filter), typeid(*flts[1]));
    thresh = std::static_pointer_cast<chucho::level_threshold_filter>(flts[1]);
    EXPECT_EQ(*chucho::level::DEBUG_(), *thresh->get_level());
    ASSERT_EQ(typeid(chucho::level_threshold_filter), typeid(*flts[2]));
    thresh = std::static_pointer_cast<chucho::level_threshold_filter>(flts[2]);
    EXPECT_EQ(*chucho::level::WARN_(), *thresh->get_level());
}

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

#if defined(CHUCHO_HAVE_MYSQL)

TEST_F(chucho_config_file_configurator, mysql_writer_full)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = my\n"
              "chucho.writer.my = chucho::mysql_writer\n"
              "chucho.writer.my.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.my.host = 192.168.56.101\n"
              "chucho.writer.my.user = test_user\n"
              "chucho.writer.my.password = password\n"
              "chucho.writer.my.database = test\n"
              "chucho.writer.my.port = 3306\n"
              "chucho.writer.my.queue_capacity = 912\n"
              "chucho.writer.my.discard_threshold = info\n"
              "chucho.writer.my.flush_on_destruct = false");
    mysql_writer_full_body();
}

TEST_F(chucho_config_file_configurator, mysql_writer_minimal)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = my\n"
              "chucho.writer.my = chucho::mysql_writer\n"
              "chucho.writer.my.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.my.host = 192.168.56.101\n"
              "chucho.writer.my.user = test_user\n"
              "chucho.writer.my.password = password\n"
              "chucho.writer.my.database = test");
    mysql_writer_minimal_body();
}

#endif

TEST_F(chucho_config_file_configurator, multiple_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = fw1\n"
              "chucho.writer.fw1 = chucho::file_writer\n"
              "chucho.writer.fw1.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.fw1.file_name = one.log\n"
              "chucho.logger.will.writer = fw2\n"
              "chucho.writer.fw2 = chucho::file_writer\n"
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
              "chucho.writer.npw.name = monkeyballs\n"
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

#if defined(CHUCHO_HAVE_ORACLE)

TEST_F(chucho_config_file_configurator, oracle_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = or\n"
              "chucho.writer.or = chucho::oracle_writer\n"
              "chucho.writer.or.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.or.user = test_user\n"
              "chucho.writer.or.password = password\n"
              "chucho.writer.or.database = 192.168.56.102/pdb1");
    oracle_writer_body();
}

#endif

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

#if defined(CHUCHO_HAVE_POSTGRES)

TEST_F(chucho_config_file_configurator, postgres_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = pg\n"
              "chucho.writer.pg = chucho::postgres_writer\n"
              "chucho.writer.pg.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m\n"
              "chucho.writer.pg.uri = postgres://test_user:password@192.168.56.101/postgres");
    postgres_writer_body();
}

#endif

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

TEST_F(chucho_config_file_configurator, remote_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = rw1\n"
              "chucho.writer.rw1 = chucho::remote_writer\n"
              "chucho.writer.rw1.host = motherboy\n"
              "chucho.logger = will2\n"
              "chucho.logger.will2.writer = rw2\n"
              "chucho.writer.rw2 = chucho::remote_writer\n"
              "chucho.writer.rw2.host = motherboy\n"
              "chucho.writer.rw2.port = 19567\n"
              "chucho.logger = will3\n"
              "chucho.logger.will3.writer = rw3\n"
              "chucho.writer.rw3 = chucho::remote_writer\n"
              "chucho.writer.rw3.host = motherboy\n"
              "chucho.writer.rw3.unsent_cache_max = 750");
    remote_writer_body();
}

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

#if defined(CHUCHO_HAVE_SQLITE)

TEST_F(chucho_config_file_configurator, sqlite_writer)
{
    configure("chucho.logger = will\n"
              "chucho.logger.will.writer = sw\n"
              "chucho.writer.sw = chucho::sqlite_writer\n"
              "chucho.writer.sw.formatter = pf\n"
              "chucho.formatter.pf = chucho::pattern_formatter\n"
              "chucho.formatter.pf.pattern = %m%n\n"
              "chucho.writer.sw.file_name = database.sqlite");
    sqlite_writer_body();
}

#endif

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

#endif

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
