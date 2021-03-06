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
#include <chucho/yaml_configurator.hpp>
#include <chucho/configuration.hpp>
#include <chucho/logger.hpp>
#if defined(CHUCHO_WINDOWS)
#include <windows.h>
#endif

namespace
{

void setenv(const char* var, const char* val)
{
#if defined(CHUCHO_WINDOWS)
    if (!SetEnvironmentVariableA(var, val))
        std::cout << "Failed environment: " << GetLastError() << std::endl;
#else
    ::setenv(var, val, 1);
#endif
}

class yaml_configurator : public chucho::test::configurator
{
protected:
    yaml_configurator();

    virtual chucho::configurator& get_configurator() override;

private:
    chucho::yaml_configurator cnf_;
};

chucho::configurator& yaml_configurator::get_configurator()
{
    return cnf_;
}

yaml_configurator::yaml_configurator()
    : cnf_(chucho::configuration::get_security_policy())
{
}

}

#if defined(CHUCHO_HAVE_ACTIVEMQ)

TEST_F(yaml_configurator, activemq_writer_bad)
{
    configure_with_error("- chucho::logger:\n"
                         "    - name: will\n"
                         "    - chucho::activemq_writer:\n"
                         "        - chucho::pattern_formatter:\n"
                         "            - pattern: '%m'\n"
                         "        - chucho::formatted_message_serializer\n"
                         "        - broker: 'tcp://127.0.0.1:61616'\n"
                         "        - consumer_type: junk\n"
                         "        - topic_or_queue: MonkeyBalls");
}

TEST_F(yaml_configurator, activemq_writer_queue)
{
    configure("- chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::activemq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::formatted_message_serializer\n"
              "        - broker: 'tcp://127.0.0.1:61616'\n"
              "        - consumer_type: queue\n"
              "        - topic_or_queue: MonkeyBalls");
    activemq_writer_queue_body();
}

TEST_F(yaml_configurator, activemq_writer_topic)
{
    configure("- chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::activemq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::formatted_message_serializer\n"
              "        - broker: 'tcp://127.0.0.1:61616'\n"
              "        - consumer_type: topic\n"
              "        - topic_or_queue: MonkeyBalls");
    activemq_writer_topic_body();
}

TEST_F(yaml_configurator, activemq_writer_topic_coalesce)
{
    configure("- chucho::logger:\n"
                  "    - name: will\n"
                  "    - chucho::activemq_writer:\n"
                  "        - chucho::pattern_formatter:\n"
                  "            - pattern: '%m'\n"
                  "        - chucho::formatted_message_serializer\n"
                  "        - broker: 'tcp://127.0.0.1:61616'\n"
                  "        - consumer_type: topic\n"
                  "        - coalesce_max: 301\n"
                  "        - topic_or_queue: MonkeyBalls");
    activemq_writer_topic_coalesce_body();
}

#endif

TEST_F(yaml_configurator, async_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::async_writer:\n"
              "        chucho::file_writer:\n"
              "            chucho::pattern_formatter:\n"
              "                pattern: '%m%n'\n"
              "            file_name: hello.log");
    async_writer_body();
}

TEST_F(yaml_configurator, async_writer_invalid_1)
{
    configure_with_error("chucho::logger:\n"
                         "    name: will\n"
                         "    chucho::async_writer:");
}

TEST_F(yaml_configurator, async_writer_invalid_2)
{
    configure_with_error("chucho::logger:\n"
                         "    - name: will\n"
                         "    - chucho::async_writer");
}

TEST_F(yaml_configurator, async_writer_with_opts)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::async_writer:\n"
              "        chucho::file_writer:\n"
              "            chucho::pattern_formatter:\n"
              "                pattern: '%m%n'\n"
              "            file_name: hello.log\n"
              "        chunk_size: 7000\n"
              "        max_chunks: 10\n"
              "        flush_on_destruct: false");
    async_writer_with_opts_body();
}

#if defined(CHUCHO_HAVE_BZIP2)

TEST_F(yaml_configurator, bzip2_file_compressor)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::rolling_file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::numbered_file_roller:\n"
              "            min_index: 3\n"
              "            max_index: 5\n"
              "            chucho::bzip2_file_compressor:\n"
              "                min_index: 1\n"
              "        chucho::size_file_roll_trigger:\n"
              "            max_size: 5000\n"
              "        file_name: what.log");
    bzip2_file_compressor_body();
}

#endif

TEST_F(yaml_configurator, cache_and_release_filter)
{
    configure(R"cnf(
chucho::logger:
    name: will
    chucho::cout_writer:
        - chucho::pattern_formatter:
            pattern: '%m%n'
        - chucho::cache_and_release_filter:
            cache_threshold: debug
            release_threshold: ErRoR
            chunk_size: 256k
            max_chunks: 7
)cnf");
    cache_and_release_filter_body();
}

TEST_F(yaml_configurator, cerr_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::cerr_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m%n'");
    cerr_writer_body();
}

TEST_F(yaml_configurator, cerr_writer_invalid_1)
{
    configure_with_error("chucho::logger:\n"
                         "    - name: will\n"
                         "    - chucho::cerr_writer:");
}

TEST_F(yaml_configurator, cerr_writer_invalid_2)
{
    configure_with_error("chucho::logger:\n"
                         "    - name: will\n"
                         "    - chucho::cerr_writer");
}

#if defined(CHUCHO_HAVE_AWSSDK)

TEST_F(yaml_configurator, cloudwatch_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::cloudwatch_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            pattern: '%m'\n"
              "        - log_group: monkeyballs\n"
              "        - log_stream: streamer\n"
              "        - region: us-west-1\n");
    cloudwatch_writer_body();
}

#endif

TEST_F(yaml_configurator, cout_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'");
    cout_writer_body();
}

TEST_F(yaml_configurator, cout_writer_invalid_1)
{
    configure_with_error("chucho::logger:\n"
                         "    - name: will\n"
                         "    - chucho::cout_writer:");
}

TEST_F(yaml_configurator, cout_writer_invalid_2)
{
    configure_with_error("chucho::logger:\n"
                         "    - name: will\n"
                         "    - chucho::cout_writer");
}

#if defined(CHUCHO_HAVE_DOORS)

TEST_F(yaml_configurator, door_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::door_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m'\n"
              "        file_name: gargle");
    door_writer_body();
}

#endif

TEST_F(yaml_configurator, duplicate_message_filter)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        - chucho::duplicate_message_filter");
    duplicate_message_filter_body();
}

#if defined(CHUCHO_HAVE_CURL)

TEST_F(yaml_configurator, email_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::email_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            pattern: '%m'\n"
              "        - chucho::level_threshold_email_trigger:\n"
              "            level: error\n"
              "        - host: mail.dummy.com\n"
              "        - port: 123\n"
              "        - from: whistler@mctweaky.com\n"
              "        - to: one@blubbery.com,two@humid.org\n"
              "        - subject: '%c'\n"
              "        - connection_type: clear\n"
              "        - user: scrumpy\n"
              "        - password: lumpy\n"
              "        - buffer_size: 7000\n");
    email_writer_body();
}

TEST_F(yaml_configurator, loggly_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::loggly_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            pattern: '%m'\n"
              "        - token: monkey-balls\n");
    loggly_writer_body();
}

#endif

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
    file_writer_body();
}

TEST_F(yaml_configurator, file_writer_invalid_1)
{
    configure_with_error("chucho::logger:\n"
                         "    name: will\n"
                         "    chucho::file_writer:\n"
                         "        chucho::pattern_formatte:\n"
                         "            pattern: '%m%n'\n"
                         "        file_name: hello.log\n"
                         "        on_start: truncate\n"
                         "        flush: false");
}

TEST_F(yaml_configurator, file_writer_invalid_2)
{
    configure_with_error("chucho::logger:\n"
                         "    name: will\n"
                         "    chucho::file_writer:\n"
                         "        chucho::pattern_formatter:\n"
                         "            pattern: '%m%n'\n"
                         "        file_nam: hello.log\n"
                         "        on_star: truncate\n"
                         "        flush:");
}

#if defined(CHUCHO_HAVE_ZLIB)

TEST_F(yaml_configurator, gzip_file_compressor)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::rolling_file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::time_file_roller:\n"
              "            file_name_pattern: '%d{%d}'\n"
              "            max_history: 5\n"
              "            chucho::gzip_file_compressor:\n"
              "                min_index: 7");
    gzip_file_compressor_body();
}

#endif

TEST_F(yaml_configurator, interval_file_roll_trigger)
{
    std::string tmpl("chucho::logger:\n"
                     "    name: will\n"
                     "    chucho::rolling_file_writer:\n"
                     "        chucho::pattern_formatter:\n"
                     "            pattern: '%m%n'\n"
                     "        chucho::numbered_file_roller:\n"
                     "            max_index: 1\n"
                     "        chucho::interval_file_roll_trigger:\n"
                     "            every: PERIOD\n"
                     "        file_name: what.log\n");
    interval_file_roll_trigger_body(tmpl);
}

TEST_F(yaml_configurator, invalid_utf8)
{
    EXPECT_ANY_THROW(configure("chucho::logger:\n"
                               "    name: \x81\x82\x83"));
}

TEST_F(yaml_configurator, json_formatter)
{
    std::string tmpl(R"tmpl(
chucho::logger:
    name: will
    chucho::cout_writer:
        chucho::json_formatter:
            style: PrEtTy
            time_zone: uTc
            DScluded_fields: FIELDS
)tmpl");
    json_formatter_body(tmpl);
}

#if defined(CHUCHO_HAVE_RDKAFKA)

TEST_F(yaml_configurator, kafka_writer_brokers)
{
    configure(R"cnf(
chucho::logger:
    name: will
    chucho::kafka_writer:
        - chucho::pattern_formatter:
            - pattern: '%m'
        - chucho::formatted_message_serializer
        - brokers: 192.168.56.101
        - topic: monkeyballs
)cnf");
    kafka_writer_brokers_body();
}

TEST_F(yaml_configurator, kafka_writer_config)
{
    configure(R"cnf(
chucho::logger:
    name: will
    chucho::kafka_writer:
        - chucho::pattern_formatter:
            - pattern: '%m'
        - chucho::formatted_message_serializer
        - kafka_configuration:
            bootstrap.servers: 192.168.56.101
        - topic: chunkymonkey
)cnf");
    kafka_writer_config_body();
}

#endif

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
    level_filter_body(tmpl);
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
    level_threshold_filter_body();
}

TEST_F(yaml_configurator, logger)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    level: fatal\n"
              "    writes_to_ancestors: false");
    logger_body();
}

#if defined(CHUCHO_HAVE_LZMA)

TEST_F(yaml_configurator, lzma_file_compressor)
{
    configure("chucho::logger:\n"
                  "    name: will\n"
                  "    chucho::rolling_file_writer:\n"
                  "        chucho::pattern_formatter:\n"
                  "            pattern: '%m%n'\n"
                  "        chucho::numbered_file_roller:\n"
                  "            min_index: 3\n"
                  "            max_index: 5\n"
                  "            chucho::lzma_file_compressor:\n"
                  "                min_index: 1\n"
                  "        chucho::size_file_roll_trigger:\n"
                  "            max_size: 5000\n"
                  "        file_name: what.log");
    lzma_file_compressor_body();
}

#endif

TEST_F(yaml_configurator, multiple_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::file_writer:\n"
              "        name: one\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        file_name: one.log\n"
              "    - chucho::file_writer:\n"
              "        name: two\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        file_name: two.log\n");
    multiple_writer_body();
}

TEST_F(yaml_configurator, named_pipe_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::named_pipe_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        pipe_name: monkeyballs\n"
              "        flush: false");
    named_pipe_writer_body();
}

TEST_F(yaml_configurator, numbered_file_roller)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::rolling_file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::numbered_file_roller:\n"
              "            max_index: 5\n"
              "            min_index: -3\n"
              "        chucho::size_file_roll_trigger:\n"
              "            max_size: 5000\n"
              "        file_name: hello");
    numbered_file_roller_body();
}

TEST_F(yaml_configurator, on_start_file_roll_trigger)
{
    configure(R"cnf(
chucho::logger:
    name: will
    chucho::rolling_file_writer:
        - chucho::pattern_formatter:
            pattern: '%m%n'
        - chucho::numbered_file_roller:
            max_index: 1
        - chucho::on_start_file_roll_trigger
        - file_name: what.log)cnf");
    on_start_file_roll_trigger_body();
}

TEST_F(yaml_configurator, pipe_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::pipe_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        flush: false");
    pipe_writer_body();
}

#if defined(CHUCHO_HAVE_RABBITMQ)

TEST_F(yaml_configurator, rabbitmq_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::rabbitmq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::formatted_message_serializer\n"
              "        - url: 'amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc'\n"
              "        - exchange: logs");
    rabbitmq_writer_body();
}

TEST_F(yaml_configurator, rabbitmq_writer_coalesce)
{
    configure("chucho::logger:\n"
                  "    - name: will\n"
                  "    - chucho::rabbitmq_writer:\n"
                  "        - coalesce_max: 302\n"
                  "        - chucho::pattern_formatter:\n"
                  "            - pattern: '%m'\n"
                  "        - chucho::formatted_message_serializer\n"
                  "        - url: 'amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc'\n"
                  "        - exchange: logs");
    rabbitmq_writer_coalesce_body();
}

#if defined(CHUCHO_HAVE_CAPN_PROTO)

TEST_F(yaml_configurator, rabbitmq_writer_capn_proto)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::rabbitmq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::capn_proto_serializer\n"
              "        - url: 'amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc'\n"
              "        - exchange: logs");
    rabbitmq_writer_capn_proto_body();
}

#endif

#endif

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
    rolling_file_writer_body();
}

TEST_F(yaml_configurator, root_alias)
{
    chucho::logger::get("")->clear_writers();
    configure("chucho::logger:\n"
                  "    name: <root>\n"
                  "    chucho::cout_writer:\n"
                  "        chucho::pattern_formatter:\n"
                  "            pattern: '%m%n'");
    root_alias_body();
}

#if defined(CHUCHO_HAVE_RUBY)

TEST_F(yaml_configurator, ruby_evaluator_filter)
{
    configure("- chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::ruby_evaluator_filter:\n"
              "            expression: '$logger == \"will\"'");
    ruby_evaluator_filter_body();
}

#endif

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
    size_file_roll_trigger_body(tmpl);
}

TEST_F(yaml_configurator, sliding_numbered_file_roller)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::rolling_file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::sliding_numbered_file_roller:\n"
              "            max_count: 5\n"
              "            min_index: -3\n"
              "        chucho::size_file_roll_trigger:\n"
              "            max_size: 5000\n"
              "        file_name: hello");
    sliding_numbered_file_roller_body();
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
    syslog_writer_body();
}

TEST_F(yaml_configurator, syslog_writer_facility)
{
    std::string tmpl("chucho::logger:\n"
                     "    - name: will\n"
                     "    - chucho::syslog_writer:\n"
                     "        - chucho::pattern_formatter:\n"
                     "            - pattern: '%m%n'\n"
                     "        - facility: FCL");
    syslog_writer_facility_body(tmpl);
}

#if !defined(CHUCHO_SOLARIS)
TEST_F(yaml_configurator, syslog_writer_port)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::syslog_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m%n'\n"
              "        - facility: LOCAL0\n"
              "        - host_name: localhost\n"
              "        - port: 19567");
    syslog_writer_port_body();
}
#endif

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
    time_file_roller_body();
}

TEST_F(yaml_configurator, unknown)
{
    std::map<std::string, std::string> unknowns;
    chucho::configuration::set_unknown_handler(
        std::bind([&] (std::map<std::string, std::string> u, const std::string& k, const std::string& v) { unknowns[k] = v; },
        std::ref(unknowns),
        std::placeholders::_1,
        std::placeholders::_2));
    configure("- my_key: my_value\n"
              "- chucho::logger:\n"
              "    name: will\n"
              "- my_second_key: my_second_value");
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    EXPECT_EQ(2, unknowns.size());
    EXPECT_EQ(std::string("my_value"), unknowns["my_key"]);
    EXPECT_EQ(std::string("my_second_value"), unknowns["my_second_key"]);
}

TEST_F(yaml_configurator, variables)
{
    setenv("CHUCHO_WRITES_KEY", "writes_to_ancestors");
    setenv("CHUCHO_WRITES_VALUE", "false");
    configure("- variables:\n"
              "    MY_NAME_IS: will\n"
              "    MY_TYPE_IS: logger\n"
              "- variables:\n"
              "    - MY_KEY_IS: level\n"
              "    - MY_VALUE_IS: fatal\n"
              "- 'chucho::${MY_TYPE_IS}':\n"
              "    name: '${MY_NAME_IS}'\n"
              "    '${MY_KEY_IS}': '${MY_VALUE_IS}'\n"
              "    '$env{CHUCHO_WRITES_KEY}': '$ENV{CHUCHO_WRITES_VALUE}'");
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    ASSERT_NE(nullptr, lgr->get_level().get());
    EXPECT_EQ(*chucho::level::FATAL_(), *lgr->get_level());
    EXPECT_FALSE(lgr->writes_to_ancestors());
}

TEST_F(yaml_configurator, yaml_formatter)
{
    std::string tmpl(R"tmpl(
chucho::logger:
    name: will
    chucho::cout_writer:
        chucho::yaml_formatter:
            style: PrEtTy
            time_zone: uTc
            DScluded_fields: FIELDS
)tmpl");
    yaml_formatter_body(tmpl);
}

#if defined(CHUCHO_WINDOWS)
TEST_F(yaml_configurator, windows_event_log_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::windows_event_log_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        source: what\n"
              "        log: hello");
    windows_event_log_writer_body();
}

TEST_F(yaml_configurator, windows_event_log_writer_no_log)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::windows_event_log_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        source: what");
    windows_event_log_writer_no_log_body();
}
#endif

#if defined(CHUCHO_HAVE_ZEROMQ)

TEST_F(yaml_configurator, zeromq_writer)
{
    configure("- chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::zeromq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::formatted_message_serializer\n"
              "        - endpoint: 'tcp://127.0.0.1:7777'\n"
              "        - prefix: Hi");
    zeromq_writer_body();
}

TEST_F(yaml_configurator, zeromq_writer_coalesce)
{
    configure("- chucho::logger:\n"
                  "    - name: will\n"
                  "    - chucho::zeromq_writer:\n"
                  "        - chucho::pattern_formatter:\n"
                  "            - pattern: '%m'\n"
                  "        - chucho::formatted_message_serializer\n"
                  "        - endpoint: 'tcp://127.0.0.1:7780'\n"
                  "        - coalesce_max: 300\n"
                  "        - prefix: Hi");
    zeromq_writer_coalesce_body();
}

TEST_F(yaml_configurator, zeromq_writer_no_prefix)
{
    configure("- chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::zeromq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::formatted_message_serializer\n"
              "        - endpoint: 'tcp://127.0.0.1:7778'\n");
    zeromq_writer_no_prefix_body();
}

TEST_F(yaml_configurator, zeromq_writer_with_compressor)
{
    configure("- chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::zeromq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::formatted_message_serializer\n"
              "        - endpoint: 'tcp://127.0.0.1:7776'\n"
              "        - chucho::noop_compressor\n"
              "        - prefix: Hi\n");
    zeromq_writer_with_compressor_body();
}

#if defined(CHUCHO_HAVE_PROTOBUF)

TEST_F(yaml_configurator, zeromq_writer_protobuf)
{
    configure("- chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::zeromq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::protobuf_serializer\n"
              "        - endpoint: 'tcp://127.0.0.1:7779'\n"
              "        - prefix: Hi");
    zeromq_writer_protobuf_body();
}

#endif

#if defined(CHUCHO_HAVE_FLATBUFFERS)

TEST_F(yaml_configurator, zeromq_writer_flatbuffers)
{
    configure("- chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::zeromq_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m'\n"
              "        - chucho::flatbuffers_serializer\n"
              "        - endpoint: 'tcp://127.0.0.1:7781'\n"
              "        - prefix: Hi");
    zeromq_writer_flatbuffers_body();
}

#endif

#endif

#if defined(CHUCHO_HAVE_LIBARCHIVE)

TEST_F(yaml_configurator, zip_file_compressor)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::rolling_file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::time_file_roller:\n"
              "            file_name_pattern: '%d{%d}'\n"
              "            max_history: 5\n"
              "            chucho::zip_file_compressor:\n"
              "                min_index: 700");
    zip_file_compressor_body();
}

#endif
