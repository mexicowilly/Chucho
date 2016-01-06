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

#include <chucho/postgres_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/calendar.hpp>
#include <chucho/logger.hpp>
#include <sstream>
#include <thread>

namespace chucho
{

postgres_writer::postgres_writer(std::shared_ptr<formatter> fmt, const std::string& uri)
    : database_writer(fmt),
      uri_(uri),
      connection_(PQconnectdb(uri.c_str()))
{
    if (PQstatus(connection_) == CONNECTION_BAD)
    {
        std::string msg("Unknown error");
        if (connection_ != nullptr)
        {
            msg = PQerrorMessage(connection_);
            PQfinish(connection_);
        }
        throw exception("Unable to connect to " + uri + ": " + msg);
    }
    PGresult* res = PQprepare(connection_,
                              "chucho_insert_event",
                              "INSERT INTO chucho_event ( formatted_message, timestamp, file_name, line_number, function_name, logger, level_name, marker, thread ) VALUES ( $1, $2, $3, $4, $5, $6, $7, $8, $9 )",
                              9,
                              nullptr); 
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string msg = PQerrorMessage(connection_);
        PQclear(res);
        PQfinish(connection_);
        throw exception("Unable to create prepared statement for inserting Chucho data: " + msg);
    }
    PQclear(res);
}

postgres_writer::~postgres_writer()
{
    if (connection_ != nullptr)
        PQfinish(connection_); 
}

void postgres_writer::write_impl(const event& evt)
{
    calendar::pieces cal = calendar::get_utc(event::clock_type::to_time_t(evt.get_time()));
    std::string msg = "'" + evt.get_message() + "'";
    std::string timestamp = calendar::format(cal, "'%Y-%m-%d %H:%M:%S UTC'");
    std::string fn = std::string("'") + evt.get_file_name() + "'";
    std::string line_number = std::to_string(evt.get_line_number());
    std::string func = std::string("'") + evt.get_function_name() + "'";
    std::string lg = "'" + evt.get_logger()->get_name() + "'";
    std::string lvl = std::string("'") + evt.get_level()->get_name() + "'";
    std::string marker;
    const char* c_marker = nullptr;
    if (evt.get_marker())
    {
        std::ostringstream stream;
        stream << "'" << *evt.get_marker() << "'";
        marker = stream.str();
        c_marker = marker.c_str();
    }
    std::ostringstream stream;
    stream << '\'' << std::this_thread::get_id() << '\'';
    std::string tid = stream.str();
    const char* params[9] =
    {
        msg.c_str(),
        timestamp.c_str(),
        fn.c_str(),
        line_number.c_str(),
        func.c_str(),
        lg.c_str(),
        lvl.c_str(),
        c_marker,
        tid.c_str()
    };
    PGresult* res = PQexecPrepared(connection_,
                                   "chucho_insert_event",
                                   9,
                                   params,
                                   nullptr,
                                   nullptr,
                                   1);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string msg = PQerrorMessage(connection_);
        PQclear(res);
        throw exception("Unable to insert event into database: " + msg);
    }
    PQclear(res);
}

}
