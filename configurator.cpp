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

#include <chucho/configurator.hpp>
#include <chucho/exception.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/environment.hpp>
#include <chucho/regex.hpp>

#include <chucho/async_writer_factory.hpp>
#include <chucho/cache_and_release_filter_factory.hpp>
#include <chucho/cerr_writer_factory.hpp>
#include <chucho/cout_writer_factory.hpp>
#include <chucho/duplicate_message_filter_factory.hpp>
#include <chucho/file_writer_factory.hpp>
#include <chucho/formatted_message_serializer_factory.hpp>
#include <chucho/interval_file_roll_trigger_factory.hpp>
#include <chucho/json_formatter_factory.hpp>
#include <chucho/level_filter_factory.hpp>
#include <chucho/level_threshold_filter_factory.hpp>
#include <chucho/logger_factory.hpp>
#include <chucho/named_pipe_writer_factory.hpp>
#include <chucho/noop_compressor_factory.hpp>
#include <chucho/numbered_file_roller_factory.hpp>
#include <chucho/on_start_file_roll_trigger_factory.hpp>
#include <chucho/pattern_formatter_factory.hpp>
#include <chucho/pipe_writer_factory.hpp>
#include <chucho/rolling_file_writer_factory.hpp>
#include <chucho/size_file_roll_trigger_factory.hpp>
#include <chucho/sliding_numbered_file_roller_factory.hpp>
#include <chucho/syslog_writer_factory.hpp>
#include <chucho/time_file_roller_factory.hpp>
#include <chucho/yaml_formatter_factory.hpp>

#if defined(CHUCHO_WINDOWS)
#include <chucho/windows_event_log_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_RUBY)
#include <chucho/ruby_evaluator_filter_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_CURL)
#include <chucho/email_writer_factory.hpp>
#include <chucho/level_threshold_email_trigger_factory.hpp>
#include <chucho/loggly_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_PROTOBUF)
#include <chucho/protobuf_serializer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_ZEROMQ)
#include <chucho/zeromq_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_ZLIB)
#include <chucho/zlib_compressor_factory.hpp>
#include <chucho/gzip_file_compressor_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_ACTIVEMQ)
#include <chucho/activemq_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_RABBITMQ)
#include <chucho/rabbitmq_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_DOORS)
#include <chucho/door_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_BZIP2)
#include <chucho/bzip2_compressor_factory.hpp>
#include <chucho/bzip2_file_compressor_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_CAPN_PROTO)
#include <chucho/capn_proto_serializer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_LZMA)
#include <chucho/lzma_compressor_factory.hpp>
#include <chucho/lzma_file_compressor_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_LIBARCHIVE)
#include <chucho/zip_file_compressor_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_LZ4)
#include <chucho/lz4_compressor_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_FLATBUFFERS)
#include <chucho/flatbuffers_serializer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_AWSSDK)
#include <chucho/cloudwatch_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_SOCI)
#include <chucho/database_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_RDKAFKA)
#include <chucho/kafka_writer_factory.hpp>
#include <chucho/kafka_configuration_factory.hpp>
#endif

#include <cstring>
#include <mutex>

namespace chucho
{

configurator::configurator(security_policy& sec_pol)
    : security_policy_(sec_pol)
{
    set_status_origin("configurator");
}

configurator::~configurator()
{
}

void configurator::add_configurable_factory(const std::string& name,
                                            std::unique_ptr<configurable_factory>&& fact)
{
    get_factories().emplace(name, std::move(fact));
}

std::map<std::string, std::unique_ptr<configurable_factory>>& configurator::get_factories()
{
    static std::once_flag once;
    // This will be cleaned at finalize()
    static std::map<std::string, std::unique_ptr<chucho::configurable_factory>>* factories;

    std::call_once(once, [&] ()
    {
        factories = new std::map<std::string, std::unique_ptr<chucho::configurable_factory>>();
        garbage_cleaner::get().add([&] () { delete factories; });
    });
    return *factories;
}

void configurator::initialize()
{
    static std::once_flag once;

    std::call_once(once, initialize_impl);
}

void configurator::initialize_impl()
{
    add_configurable_factory("chucho::cerr_writer",
                             std::make_unique<cerr_writer_factory>());
    add_configurable_factory("chucho::cout_writer",
                             std::make_unique<cout_writer_factory>());
    add_configurable_factory("chucho::duplicate_message_filter",
                             std::make_unique<duplicate_message_filter_factory>());
    add_configurable_factory("chucho::file_writer",
                             std::make_unique<file_writer_factory>());
    add_configurable_factory("chucho::level_filter",
                             std::make_unique<level_filter_factory>());
    add_configurable_factory("chucho::level_threshold_filter",
                             std::make_unique<level_threshold_filter_factory>());
    add_configurable_factory("chucho::logger",
                             std::make_unique<logger_factory>());
    add_configurable_factory("chucho::named_pipe_writer",
                             std::make_unique<named_pipe_writer_factory>());
    add_configurable_factory("chucho::noop_compressor",
                             std::make_unique<noop_compressor_factory>());
    add_configurable_factory("chucho::numbered_file_roller",
                             std::make_unique<numbered_file_roller_factory>());
    add_configurable_factory("chucho::on_start_file_roll_trigger",
                             std::make_unique<on_start_file_roll_trigger_factory>());
    add_configurable_factory("chucho::pattern_formatter",
                             std::make_unique<pattern_formatter_factory>());
    add_configurable_factory("chucho::pipe_writer",
                             std::make_unique<pipe_writer_factory>());
    add_configurable_factory("chucho::rolling_file_writer",
                             std::make_unique<rolling_file_writer_factory>());
    add_configurable_factory("chucho::size_file_roll_trigger",
                             std::make_unique<size_file_roll_trigger_factory>());
    add_configurable_factory("chucho::time_file_roller",
                             std::make_unique<time_file_roller_factory>());
    add_configurable_factory("chucho::syslog_writer",
                             std::make_unique<syslog_writer_factory>());
    add_configurable_factory("chucho::async_writer",
                             std::make_unique<async_writer_factory>());
    add_configurable_factory("chucho::sliding_numbered_file_roller",
                             std::make_unique<sliding_numbered_file_roller_factory>());
    add_configurable_factory("chucho::interval_file_roll_trigger",
                             std::make_unique<interval_file_roll_trigger_factory>());
    add_configurable_factory("chucho::formatted_message_serializer",
                             std::make_unique<formatted_message_serializer_factory>());
    add_configurable_factory("chucho::json_formatter",
                             std::make_unique<json_formatter_factory>());
    add_configurable_factory("chucho::cache_and_release_filter",
                             std::make_unique<cache_and_release_filter_factory>());
    add_configurable_factory("chucho::yaml_formatter",
                             std::make_unique<yaml_formatter_factory>());
#if defined(CHUCHO_WINDOWS)
    add_configurable_factory("chucho::windows_event_log_writer",
                             std::make_unique<windows_event_log_writer_factory>());
#endif
#if defined(CHUCHO_HAVE_RUBY)
    add_configurable_factory("chucho::ruby_evaluator_filter",
                             std::make_unique<ruby_evaluator_filter_factory>());
#endif
#if defined(CHUCHO_HAVE_CURL)
    add_configurable_factory("chucho::email_writer",
                             std::make_unique<email_writer_factory>());
    add_configurable_factory("chucho::level_threshold_email_trigger",
                             std::make_unique<level_threshold_email_trigger_factory>());
    add_configurable_factory("chucho::loggly_writer",
                             std::make_unique<loggly_writer_factory>());
#endif
#if defined(CHUCHO_HAVE_PROTOBUF)
    add_configurable_factory("chucho::protobuf_serializer",
                             std::make_unique<protobuf_serializer_factory>());
#endif
#if defined(CHUCHO_HAVE_ZEROMQ)
    add_configurable_factory("chucho::zeromq_writer",
                             std::make_unique<zeromq_writer_factory>());
#endif
#if defined(CHUCHO_HAVE_ZLIB)
    add_configurable_factory("chucho::zlib_compressor",
                             std::make_unique<zlib_compressor_factory>());
    add_configurable_factory("chucho::gzip_file_compressor",
                             std::make_unique<gzip_file_compressor_factory>());
#endif
#if defined(CHUCHO_HAVE_ACTIVEMQ)
    add_configurable_factory("chucho::activemq_writer",
                             std::make_unique<activemq_writer_factory>());
#endif
#if defined(CHUCHO_HAVE_DOORS)
    add_configurable_factory("chucho::door_writer",
                             std::make_unique<door_writer_factory>());
#endif
#if defined(CHUCHO_HAVE_RABBITMQ)
    add_configurable_factory("chucho::rabbitmq_writer",
                             std::make_unique<rabbitmq_writer_factory>());
#endif
#if defined(CHUCHO_HAVE_BZIP2)
    add_configurable_factory("chucho::bzip2_compressor",
                             std::make_unique<bzip2_compressor_factory>());
    add_configurable_factory("chucho::bzip2_file_compressor",
                             std::make_unique<bzip2_file_compressor_factory>());
#endif
#if defined(CHUCHO_HAVE_CAPN_PROTO)
    add_configurable_factory("chucho::capn_proto_serializer",
                             std::make_unique<capn_proto_serializer_factory>());
#endif
#if defined(CHUCHO_HAVE_LZMA)
    add_configurable_factory("chucho::lzma_compressor",
                             std::make_unique<lzma_compressor_factory>());
    add_configurable_factory("chucho::lzma_file_compressor",
                             std::make_unique<lzma_file_compressor_factory>());
#endif
#if defined(CHUCHO_HAVE_LZ4)
    add_configurable_factory("chucho::lz4_compressor",
                             std::make_unique<lz4_compressor_factory>());
#endif
#if defined(CHUCHO_HAVE_FLATBUFFERS)
    add_configurable_factory("chucho::flatbuffers_serializer",
                             std::make_unique<flatbuffers_serializer_factory>());
#endif
#if defined(CHUCHO_HAVE_AWSSDK)
    add_configurable_factory("chucho::cloudwatch_writer",
                             std::make_unique<cloudwatch_writer_factory>());
#endif
#if defined(CHUCHO_HAVE_SOCI)
    add_configurable_factory("chucho::database_writer",
                             std::make_unique<database_writer_factory>());
#endif
#if defined(CHUCHO_HAVE_LIBARCHIVE)
    add_configurable_factory("chucho::zip_file_compressor",
                             std::make_unique<zip_file_compressor_factory>());
#endif
#if defined(CHUCHO_HAVE_RDKAFKA)
    add_configurable_factory("kafka_configuration",
                             std::make_unique<kafka_configuration_factory>());
    add_configurable_factory("chucho::kafka_writer",
                             std::make_unique<kafka_writer_factory>());
#endif
}

std::string configurator::resolve_variables(const std::string& val)
{
    static regex::expression re("\\$([Ee][Nn][Vv])?\\{([^{]+)\\}");

    std::string result(val);
    std::size_t pos_offset = 0;
    regex::iterator itor(val, re);
    regex::iterator end;
    while (itor != end)
    {
        const regex::match& m(*itor);
        if (m[1].begin() > 0)
        {
            auto env = environment::get(val.substr(m[2].begin(), m[2].length()).c_str());
            if (env)
            {
                result.replace(m[0].begin() + pos_offset, m[0].length(), *env);
                pos_offset += env->length() - m[0].length();
            }
        }
        else
        {
            auto found = variables_.find(val.substr(m[2].begin(), m[2].length()));
            if (found != variables_.end())
            {
                result.replace(m[0].begin() + pos_offset, m[0].length(), found->second);
                pos_offset += found->second.length() - m[0].length();
            }
        }
        ++itor;
    }
    return result;
}

}
