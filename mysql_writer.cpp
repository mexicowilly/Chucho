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

#include <chucho/mysql_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/calendar.hpp>
#include <chucho/logger.hpp>
#include <mysql.h>
#include <sstream>
#include <cstring>

namespace
{

class real_mysql_writer : public chucho::writer
{
public:
    real_mysql_writer(std::shared_ptr<chucho::formatter> fmt,
                      const std::string& host,
                      std::uint16_t port,
                      const std::string& user,
                      const std::string& password,
                      const std::string& database);
    ~real_mysql_writer();

protected:
    virtual void write_impl(const chucho::event& evt) override;

private:
    MYSQL* mysql_;
    MYSQL_STMT* insert_;
};

real_mysql_writer::real_mysql_writer(std::shared_ptr<chucho::formatter> fmt,
                                     const std::string& host,
                                     std::uint16_t port,
                                     const std::string& user,
                                     const std::string& password,
                                     const std::string& database)
    : chucho::writer(fmt),
      mysql_(mysql_init(nullptr)),
      insert_(nullptr)
{
    set_status_origin("real_mysql_writer");
    if (mysql_ == nullptr)
        throw std::bad_alloc();
    if (mysql_real_connect(mysql_,
                           host.c_str(),
                           user.c_str(),
                           password.c_str(),
                           database.c_str(),
                           port,
                           nullptr,
                           0) == nullptr)
    {
        throw chucho::exception(std::string("[real_mysql_writer] Unable to connect to database: ") + mysql_error(mysql_));
    }
    insert_ = mysql_stmt_init(mysql_);
    if (insert_ == nullptr) 
        throw std::bad_alloc();
    std::string sql("INSERT INTO chucho_event ( formatted_message, timestmp, file_name, line_number, function_name, logger, level_name, marker, thread ) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ? );");
    if (mysql_stmt_prepare(insert_, sql.c_str(), sql.length()) != 0) 
    {
        std::string msg = mysql_error(mysql_);
        mysql_close(mysql_);
        throw chucho::exception(std::string("[real_mysql_writer] Error preparing insert statement: ") + msg);
    }
    mysql_autocommit(mysql_, 1);
}

real_mysql_writer::~real_mysql_writer()
{
    mysql_stmt_close(insert_);
    mysql_close(mysql_);
}

void real_mysql_writer::write_impl(const chucho::event& evt)
{
    MYSQL_BIND params[9];
    std::memset(params, 0, sizeof(params));

    std::string msg = formatter_->format(evt);
    unsigned long p0_len = msg.length();
    my_bool p0_null = 0;
    params[0].buffer = reinterpret_cast<void*>(const_cast<char*>(msg.data()));
    params[0].buffer_type = MYSQL_TYPE_STRING;
    params[0].buffer_length = p0_len;
    params[0].length = &p0_len;
    params[0].is_null = &p0_null;

    auto pieces = chucho::calendar::get_utc(chucho::event::clock_type::to_time_t(evt.get_time()));
    MYSQL_TIME ts;
    std::memset(&ts, 0, sizeof(ts));
    ts.year = pieces.tm_year + 1900;
    ts.month = pieces.tm_mon + 1;
    ts.day = pieces.tm_mday;
    ts.hour = pieces.tm_hour;
    ts.minute = pieces.tm_min;
    ts.second = pieces.tm_sec;
    unsigned long p1_len = sizeof(ts);
    my_bool p1_null = 0;
    params[1].buffer = &ts;
    params[1].buffer_type = MYSQL_TYPE_TIMESTAMP;
    params[1].buffer_length = p1_len;
    params[1].length = &p1_len;
    params[1].is_null = &p1_null;

    unsigned long p2_len = std::strlen(evt.get_file_name());
    my_bool p2_null = 0;
    params[2].buffer = reinterpret_cast<void*>(const_cast<char*>(evt.get_file_name()));
    params[2].buffer_type = MYSQL_TYPE_STRING;
    params[2].buffer_length = p2_len;
    params[2].length = &p2_len;
    params[2].is_null = &p2_null;

    unsigned ln = evt.get_line_number();
    unsigned long p3_len = sizeof(ln);
    my_bool p3_null = 0;
    params[3].buffer = &ln;
    params[3].buffer_type = MYSQL_TYPE_LONG;
    params[3].buffer_length = p3_len;
    params[3].length = &p3_len;
    params[3].is_null = &p3_null;
    params[3].is_unsigned = 1;

    unsigned long p4_len = std::strlen(evt.get_function_name());
    my_bool p4_null = 0;
    params[4].buffer = reinterpret_cast<void*>(const_cast<char*>(evt.get_function_name()));
    params[4].buffer_type = MYSQL_TYPE_STRING;
    params[4].buffer_length = p4_len;
    params[4].length = &p4_len;
    params[4].is_null = &p4_null;

    auto lgr = evt.get_logger();
    unsigned long p5_len = lgr->get_name().length();
    my_bool p5_null = 0;
    params[5].buffer = reinterpret_cast<void*>(const_cast<char*>(lgr->get_name().data()));
    params[5].buffer_type = MYSQL_TYPE_STRING;
    params[5].buffer_length = p5_len;
    params[5].length = &p5_len;
    params[5].is_null = &p5_null;

    auto lvl = evt.get_level()->get_name();
    unsigned long p6_len = std::strlen(lvl);
    my_bool p6_null = 0;
    params[6].buffer = reinterpret_cast<void*>(const_cast<char*>(lvl));
    params[6].buffer_type = MYSQL_TYPE_STRING;
    params[6].buffer_length = p6_len;
    params[6].length = &p6_len;
    params[6].is_null = &p6_null;

    auto mrk = evt.get_marker();
    unsigned long p7_len;
    my_bool p7_null;
    std::string mrk_text;
    if (mrk) 
    {
        std::ostringstream stream;
        stream << *mrk;
        mrk_text = stream.str();
        p7_len = mrk_text.length();
        p7_null = 0;
    }
    else
    {
        p7_len = 0;
        p7_null = 1;
    }
    params[7].buffer = reinterpret_cast<void*>(const_cast<char*>(mrk_text.data()));
    params[7].buffer_type = MYSQL_TYPE_STRING;
    params[7].buffer_length = p7_len;
    params[7].length = &p7_len;
    params[7].is_null = &p7_null;

    std::ostringstream stream;
    stream << std::this_thread::get_id();
    std::string tname = stream.str();
    unsigned long p8_len = tname.length();
    my_bool p8_null = 0;
    params[8].buffer = reinterpret_cast<void*>(const_cast<char*>(tname.data()));
    params[8].buffer_type = MYSQL_TYPE_STRING;
    params[8].buffer_length = p8_len;
    params[8].length = &p8_len;
    params[8].is_null = &p8_null;

    if (mysql_stmt_bind_param(insert_, params) != 0) 
        throw chucho::exception(std::string("Unable to bind insert statement: ") + mysql_error(mysql_));
    if (mysql_stmt_execute(insert_) != 0) 
        throw chucho::exception(std::string("Unable to execute insert statement: ") + mysql_error(mysql_));
}

}

namespace chucho
{

const std::uint16_t mysql_writer::DEFAULT_PORT = 3306;

mysql_writer::mysql_writer(std::shared_ptr<formatter> fmt,
                           const std::string& host,
                           const std::string& user,
                           const std::string& password,
                           const std::string& database,
                           std::uint16_t port,
                           std::size_t capacity,
                           std::shared_ptr<level> discard_threshold,
                           bool flush_on_destruct)
    : database_writer(fmt),
      host_(host),
      user_(user),
      password_(password),
      database_(database),
      port_(port)
{
    set_status_origin("mysql_writer");
    std::shared_ptr<writer> real = std::make_shared<real_mysql_writer>(fmt, host, port, user, password, database);
    // std::function won't accept the bare C functions, so wrap them in lambdas
    std::function<void()> ent([] () { if (mysql_thread_init() != 0) throw exception("Unable to initialize thread for MySQL"); });
    std::function<void()> lv([] () { mysql_thread_end(); });
    async_.reset(new async_writer(real, capacity, discard_threshold, flush_on_destruct, ent, lv));
}

void mysql_writer::write_impl(const event& evt)
{
    async_->write(evt);
}

}
