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

#include <chucho/sqlite_writer.hpp>
#include <chucho/logger.hpp>
#include <chucho/exception.hpp>
#include <sstream>
#include <thread>
#include <cstring>

namespace chucho
{

sqlite_writer::sqlite_writer(const std::string& name, std::shared_ptr<formatter> fmt, const std::string& file_name)
    : database_writer(name, fmt),
      file_name_(file_name),
      db_(nullptr),
      insert_(nullptr)
{
    if (sqlite3_open(file_name_.c_str(), &db_) != SQLITE_OK)
        throw exception("[sqlite_writer] Unable to open database " + file_name_);
    sqlite3_extended_result_codes(db_, 1);
    if (sqlite3_exec(db_,
                     "CREATE TABLE IF NOT EXISTS 'chucho_event' (event_id INTEGER PRIMARY KEY AUTOINCREMENT, formatted_message TEXT NOT NULL, timestamp INTEGER NOT NULL, file_name TEXT NOT NULL, line_number INTEGER NOT NULL, function_name TEXT NOT NULL, logger TEXT NOT NULL, level_name TEXT NOT NULL, marker TEXT, thread TEXT NOT NULL);",
                     nullptr,
                     nullptr,
                     nullptr) != SQLITE_OK)
    {
        std::string msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw exception("[sqlite_writer] Unable to execute SQL: " + msg);
    }
    if (sqlite3_prepare_v2(db_,
                           "INSERT INTO 'chucho_event' (formatted_message, timestamp, file_name, line_number, function_name, logger, level_name, marker, thread) VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9);",
                           -1,
                           &insert_,
                           nullptr) != SQLITE_OK)
    {
        std::string msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw exception("[sqlite_writer] Unable to prepare statement: " + msg);
    }
}

sqlite_writer::~sqlite_writer()
{
    sqlite3_finalize(insert_);
    sqlite3_close(db_);
}

void sqlite_writer::write_impl(const event& evt)
{
    sqlite3_reset(insert_);
    std::string msg = formatter_->format(evt);
    sqlite3_bind_text(insert_, 1, msg.data(), msg.length(), SQLITE_STATIC);
    sqlite3_bind_int64(insert_, 2, chucho::event::clock_type::to_time_t(evt.get_time()));
    sqlite3_bind_text(insert_, 3, evt.get_file_name(), std::strlen(evt.get_file_name()), SQLITE_STATIC);
    sqlite3_bind_int(insert_, 4, evt.get_line_number());
    sqlite3_bind_text(insert_, 5, evt.get_function_name(), std::strlen(evt.get_function_name()), SQLITE_STATIC);
    sqlite3_bind_text(insert_, 6, evt.get_logger()->get_name().data(), evt.get_logger()->get_name().length(), SQLITE_STATIC);
    sqlite3_bind_text(insert_, 7, evt.get_level()->get_name(), std::strlen(evt.get_level()->get_name()), SQLITE_STATIC);
    auto mrk = evt.get_marker();
    std::string mrk_text;
    if (mrk) 
    {
        std::ostringstream stream;
        stream << *mrk;
        mrk_text = stream.str();
        sqlite3_bind_text(insert_, 8, mrk_text.data(), mrk_text.length(), SQLITE_STATIC);
    }
    else
    {
        sqlite3_bind_null(insert_, 8);
    }
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    std::string thr = stream.str();
    sqlite3_bind_text(insert_, 9, thr.data(), thr.length(), SQLITE_STATIC);
    if (sqlite3_step(insert_) != SQLITE_DONE) 
        throw exception(std::string("[sqlite_writer] Error writing to database: ") + sqlite3_errmsg(db_));
}

}
