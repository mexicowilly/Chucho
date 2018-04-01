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

#include <chucho/database_writer.hpp>
#include <chucho/host.hpp>
#include <chucho/calendar.hpp>
#include <chucho/logger.hpp>
#include <chucho/process.hpp>
#include <thread>

namespace
{

constexpr const char* INSERT = "INSERT INTO chucho_event ( formatted_message, timestmp, file_name, line_number, function_name, logger, level_name, marker, thread, host_name, process_id ) VALUES ( :formatted_message, :timestmp, :file_name, :line_number, :function_name, :logger, :level_name, :marker, :thread, :host_name, :process_id )";

}

namespace chucho
{

database_writer::database_writer(const std::string& name,
                                 std::unique_ptr<formatter>&& fmt,
                                 const std::string& connection)
    : writer(name, std::move(fmt)),
      sql_(connection),
      stmt_(sql_),
      marker_ind_(soci::i_null),
      host_name_(host::get_full_name())
{
    stmt_ = (sql_.prepare << INSERT,
             soci::use(formatted_message_),
             soci::use(timestamp_),
             soci::use(file_name_),
             soci::use(line_number_),
             soci::use(function_name_),
             soci::use(logger_name_),
             soci::use(level_name_),
             soci::use(marker_, marker_ind_),
             soci::use(thread_),
             soci::use(host_name_));
}

void database_writer::write_impl(const event& evt)
{
    formatted_message_ = formatter_->format(evt);
    // slicing on purpose
    timestamp_ = calendar::get_local(event::clock_type::to_time_t(evt.get_time()));
    file_name_ = evt.get_file_name();
    line_number_ = evt.get_line_number();
    function_name_ = evt.get_function_name();
    logger_name_ = evt.get_logger()->get_name();
    level_name_ = evt.get_level()->get_name();
    if (evt.get_marker())
    {
        std::ostringstream stream;
        stream << *evt.get_marker();
        marker_ = stream.str();
        marker_ind_ = soci::i_ok;
    }
    else
    {
        marker_.clear();
        marker_ind_ = soci::i_null;
    }
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    thread_ = stream.str();
    stmt_.execute(true);
}

}
