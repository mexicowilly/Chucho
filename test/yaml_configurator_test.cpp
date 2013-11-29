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

#include "configurator_test.hpp"
#include <chucho/yaml_configurator.hpp>
#include <chucho/configuration.hpp>
#include <chucho/logger.hpp>

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
    virtual chucho::configurator& get_configurator() override;

private:
    chucho::yaml_configurator cnf_;
};

chucho::configurator& yaml_configurator::get_configurator()
{
    return cnf_;
}

}

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

TEST_F(yaml_configurator, async_writer_with_opts)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::async_writer:\n"
              "        chucho::file_writer:\n"
              "            chucho::pattern_formatter:\n"
              "                pattern: '%m%n'\n"
              "            file_name: hello.log\n"
              "        discard_threshold: error\n"
              "        queue_capacity: 700");
    async_writer_with_opts_body();
}

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

TEST_F(yaml_configurator, cerr_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::cerr_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m%n'");
    cerr_writer_body();
}

TEST_F(yaml_configurator, cout_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'");
    cout_writer_body();
}

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
    multiple_writer_body();
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
    remote_writer_body();
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
        rolling_file_writer_body();
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
        std::bind([&] (std::map<std::string, std::string> u, const std::string& k, const std::string& v) { unknowns[k] = v; return true; },
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
