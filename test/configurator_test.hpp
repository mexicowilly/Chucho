/*
 * Copyright 2013-2018 Will Mason
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
#include <chucho/configurator.hpp>

namespace chucho
{

namespace test
{

class configurator : public ::testing::Test
{
public:
    configurator();
    ~configurator();

protected:
#if defined(CHUCHO_HAVE_ACTIVEMQ)
    void activemq_writer_queue_body();
    void activemq_writer_topic_body();
    void activemq_writer_topic_coalesce_body();
#endif
    void async_writer_body();
    void async_writer_with_opts_body();
    void bzip2_file_compressor_body();
    void cerr_writer_body();
#if defined(CHUCHO_HAVE_AWSSDK)
    void cloudwatch_writer_body();
#endif
    void configure(const char* const cnf);
    void configure_with_error(const char* const cnf);
    void cout_writer_body();
#if defined(CHUCHO_HAVE_DOORS)
    void door_writer_body();
#endif
    void duplicate_message_filter_body();
#if defined(CHUCHO_HAVE_CURL)
    void email_writer_body();
    void loggly_writer_body();
#endif
    void file_writer_body();
    virtual chucho::configurator& get_configurator() = 0;
    void gzip_file_compressor_body();
    void interval_file_roll_trigger_body(const std::string& tmpl);
    void level_filter_body(const std::string& tmpl);
    void level_threshold_filter_body();
    void logger_body();
    void lzma_file_compressor_body();
    void multiple_writer_body();
    void named_pipe_writer_body();
    void numbered_file_roller_body();
    void pipe_writer_body();
#if defined(CHUCHO_HAVE_RABBITMQ)
    void rabbitmq_writer_body();
    void rabbitmq_writer_coalesce_body();
#if defined(CHUCHO_HAVE_CAPN_PROTO)
    void rabbitmq_writer_capn_proto_body();
#endif
#endif
#if defined(CHUCHO_HAVE_RUBY)
    void ruby_evaluator_filter_body();
#endif
    void rolling_file_writer_body();
    void root_alias_body();
    void size_file_roll_trigger_body(const std::string& tmpl);
    void sliding_numbered_file_roller_body();
    void syslog_writer_body();
    void syslog_writer_facility_body(const std::string& tmpl);
    void syslog_writer_port_body();
    void time_file_roller_body();
#if defined(CHUCHO_WINDOWS)
    void windows_event_log_writer_body();
    void windows_event_log_writer_no_log_body();
#endif
#if defined(CHUCHO_HAVE_ZEROMQ)
    void zeromq_writer_body();
    void zeromq_writer_coalesce_body();
    void zeromq_writer_with_compressor_body();
    void zeromq_writer_no_prefix_body();
#if defined(CHUCHO_HAVE_PROTOBUF)
    void zeromq_writer_protobuf_body();
#endif
#if defined(CHUCHO_HAVE_FLATBUFFERS)
    void zeromq_writer_flatbuffers_body();
#endif
#endif
    void zip_file_compressor_body();
};

}

}
