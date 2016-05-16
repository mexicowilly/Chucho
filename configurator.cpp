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

#include <chucho/configurator.hpp>
#include <chucho/exception.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/environment.hpp>
#include <chucho/regex.hpp>

#include <chucho/async_writer_factory.hpp>
#include <chucho/bzip2_file_compressor_factory.hpp>
#include <chucho/cerr_writer_factory.hpp>
#include <chucho/cout_writer_factory.hpp>
#include <chucho/duplicate_message_filter_factory.hpp>
#include <chucho/file_writer_factory.hpp>
#include <chucho/formatted_message_serializer_factory.hpp>
#include <chucho/gzip_file_compressor_factory.hpp>
#include <chucho/interval_file_roll_trigger_factory.hpp>
#include <chucho/level_filter_factory.hpp>
#include <chucho/level_threshold_filter_factory.hpp>
#include <chucho/logger_factory.hpp>
#include <chucho/named_pipe_writer_factory.hpp>
#include <chucho/noop_compressor_factory.hpp>
#include <chucho/numbered_file_roller_factory.hpp>
#include <chucho/pattern_formatter_factory.hpp>
#include <chucho/pipe_writer_factory.hpp>
#include <chucho/remote_writer_factory.hpp>
#include <chucho/rolling_file_writer_factory.hpp>
#include <chucho/size_file_roll_trigger_factory.hpp>
#include <chucho/sliding_numbered_file_roller_factory.hpp>
#include <chucho/syslog_writer_factory.hpp>
#include <chucho/time_file_roller_factory.hpp>
#include <chucho/zip_file_compressor_factory.hpp>

#if defined(CHUCHO_WINDOWS)
#include <chucho/windows_event_log_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_MYSQL)
#include <chucho/mysql_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_ORACLE)
#include <chucho/oracle_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_SQLITE)
#include <chucho/sqlite_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_POSTGRES)
#include <chucho/postgres_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_DB2)
#include <chucho/db2_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_RUBY)
#include <chucho/ruby_evaluator_filter_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_EMAIL_WRITER)
#include <chucho/email_writer_factory.hpp>
#include <chucho/level_threshold_email_trigger_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_PROTOBUF)
#include <chucho/protobuf_serializer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_ZEROMQ)
#include <chucho/zeromq_writer_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_ZLIB)
#include <chucho/zlib_compressor_factory.hpp>
#endif
#if defined(CHUCHO_HAVE_ACTIVEMQ)
#include <chucho/activemq_writer_factory.hpp>
#endif

#include <cstring>
#include <mutex>

namespace chucho
{

configurator::configurator(const security_policy& sec_pol)
    : security_policy_(sec_pol)
{
    set_status_origin("configurator");
}

configurator::~configurator()
{
}

std::map<std::string, std::shared_ptr<configurable_factory>>& configurator::get_factories()
{
    static std::once_flag once;
    // This will be cleaned at finalize()
    static std::map<std::string, std::shared_ptr<chucho::configurable_factory>>* factories;

    std::call_once(once, [&] ()
    {
        factories = new std::map<std::string, std::shared_ptr<chucho::configurable_factory>>();
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
    std::shared_ptr<configurable_factory> fact(new cerr_writer_factory());
    add_configurable_factory("chucho::cerr_writer", fact);
    fact.reset(new cout_writer_factory());
    add_configurable_factory("chucho::cout_writer", fact);
    fact.reset(new duplicate_message_filter_factory());
    add_configurable_factory("chucho::duplicate_message_filter", fact);
    fact.reset(new file_writer_factory());
    add_configurable_factory("chucho::file_writer", fact);
    fact.reset(new level_filter_factory());
    add_configurable_factory("chucho::level_filter", fact);
    fact.reset(new level_threshold_filter_factory());
    add_configurable_factory("chucho::level_threshold_filter", fact);
    fact.reset(new logger_factory());
    add_configurable_factory("chucho::logger", fact);
    fact.reset(new named_pipe_writer_factory());
    add_configurable_factory("chucho::named_pipe_writer", fact);
    fact.reset(new noop_compressor_factory());
    add_configurable_factory("chucho::noop_compressor", fact);
    fact.reset(new numbered_file_roller_factory());
    add_configurable_factory("chucho::numbered_file_roller", fact);
    fact.reset(new pattern_formatter_factory());
    add_configurable_factory("chucho::pattern_formatter", fact);
    fact.reset(new pipe_writer_factory());
    add_configurable_factory("chucho::pipe_writer", fact);
    fact.reset(new rolling_file_writer_factory());
    add_configurable_factory("chucho::rolling_file_writer", fact);
    fact.reset(new size_file_roll_trigger_factory());
    add_configurable_factory("chucho::size_file_roll_trigger", fact);
    fact.reset(new time_file_roller_factory());
    add_configurable_factory("chucho::time_file_roller", fact);
    fact.reset(new syslog_writer_factory());
    add_configurable_factory("chucho::syslog_writer", fact);
    fact.reset(new remote_writer_factory());
    add_configurable_factory("chucho::remote_writer", fact);
    fact.reset(new bzip2_file_compressor_factory());
    add_configurable_factory("chucho::bzip2_file_compressor", fact);
    fact.reset(new gzip_file_compressor_factory());
    add_configurable_factory("chucho::gzip_file_compressor", fact);
    fact.reset(new zip_file_compressor_factory());
    add_configurable_factory("chucho::zip_file_compressor", fact);
    fact.reset(new async_writer_factory());
    add_configurable_factory("chucho::async_writer", fact);
    fact.reset(new sliding_numbered_file_roller_factory());
    add_configurable_factory("chucho::sliding_numbered_file_roller", fact);
    fact.reset(new interval_file_roll_trigger_factory());
    add_configurable_factory("chucho::interval_file_roll_trigger", fact);
    fact.reset(new formatted_message_serializer_factory());
    add_configurable_factory("chucho::formatted_message_serializer", fact);
#if defined(CHUCHO_WINDOWS)
    fact.reset(new windows_event_log_writer_factory());
    add_configurable_factory("chucho::windows_event_log_writer", fact);
#endif
#if defined(CHUCHO_HAVE_MYSQL)
    fact.reset(new mysql_writer_factory());
    add_configurable_factory("chucho::mysql_writer", fact);
#endif
#if defined(CHUCHO_HAVE_ORACLE)
    fact.reset(new oracle_writer_factory());
    add_configurable_factory("chucho::oracle_writer", fact);
#endif
#if defined(CHUCHO_HAVE_SQLITE)
    fact.reset(new sqlite_writer_factory());
    add_configurable_factory("chucho::sqlite_writer", fact);
#endif
#if defined(CHUCHO_HAVE_POSTGRES)
    fact.reset(new postgres_writer_factory());
    add_configurable_factory("chucho::postgres_writer", fact);
#endif
#if defined(CHUCHO_HAVE_DB2)
    fact.reset(new db2_writer_factory());
    add_configurable_factory("chucho::db2_writer", fact);
#endif
#if defined(CHUCHO_HAVE_RUBY)
    fact.reset(new ruby_evaluator_filter_factory());
    add_configurable_factory("chucho::ruby_evaluator_filter", fact);
#endif
#if defined(CHUCHO_HAVE_EMAIL_WRITER)
    fact.reset(new email_writer_factory());
    add_configurable_factory("chucho::email_writer", fact);
    fact.reset(new level_threshold_email_trigger_factory());
    add_configurable_factory("chucho::level_threshold_email_trigger", fact);
#endif
#if defined(CHUCHO_HAVE_PROTOBUF)
    fact.reset(new protobuf_serializer_factory());
    add_configurable_factory("chucho::protobuf_serializer", fact);
#endif
#if defined(CHUCHO_HAVE_ZEROMQ)
    fact.reset(new zeromq_writer_factory());
    add_configurable_factory("chucho::zeromq_writer", fact);
#endif
#if defined(CHUCHO_HAVE_ZLIB)
    fact.reset(new zlib_compressor_factory());
    add_configurable_factory("chucho::zlib_compressor", fact);
#endif
#if defined(CHUCHO_HAVE_ACTIVEMQ)
    fact.reset(new activemq_writer_factory());
    add_configurable_factory("chucho::activemq_writer", fact);
#endif
}

std::string configurator::resolve_variables(const std::string& val)
{
    static regex::expression re("\\$([Ee][Nn][Vv])?\\{([^{]+)\\}");

    std::string result(val);
    int pos_offset = 0;
    regex::iterator itor(val, re);
    regex::iterator end;
    while (itor != end)
    {
        const regex::match& m(*itor);
        if (m[1].begin() > 0)
        {
            optional<std::string> env = environment::get(
                val.substr(m[2].begin(), m[2].length()).c_str());
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
