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

#include <chucho/db2_writer.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/exception.hpp>
#include <chucho/calendar.hpp>
#include <chucho/logger.hpp>
#include <cstring>
#include <sstream>
#include <thread>

namespace
{

SQLHENV db2_env = SQL_NULL_HENV;

void allocate_environment()
{
    SQLRETURN rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &db2_env);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    rc = SQLSetEnvAttr(db2_env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER*>(SQL_OV_ODBC3), 0);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    chucho::garbage_cleaner::get().add([&] { if (db2_env != SQL_NULL_HENV) { SQLFreeHandle(SQL_HANDLE_ENV, db2_env); } });
}

SQLCHAR* get_sqlchar(const std::string& str)
{
    return reinterpret_cast<SQLCHAR*>(const_cast<std::string::pointer>(str.c_str()));
}

}

namespace chucho
{

db2_writer::db2_writer(std::shared_ptr<formatter> fmt,
                       const std::string& server,
                       const std::string& user,
                       const std::string& password)
    : database_writer(fmt),
      server_(server),
      user_(user),
      password_(password)
{
    static std::once_flag once;
    std::call_once(once, allocate_environment);
    SQLRETURN rc = SQLAllocHandle(SQL_HANDLE_DBC, db2_env, &dbc_);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    rc = SQLConnect(dbc_,
                    get_sqlchar(server_.c_str()),
                    server_.length(),
                    get_sqlchar(user_.c_str()),
                    user_.length(),
                    get_sqlchar(password_.c_str()),
                    password_.length());
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    rc = SQLAllocHandle(SQL_HANDLE_STMT, dbc_, &stmt_);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    rc = SQLSetConnectAttr(dbc_,
                           SQL_ATTR_AUTOCOMMIT,
                           SQL_AUTOCOMMIT_OFF,
                           SQL_NTS);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    std::string sql("INSERT INTO chucho_event ( formatted_message, timestmp, file_name, line_number, function_name, logger, level_name, marker, thread ) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ? );");
    rc = SQLPrepare(stmt_, get_sqlchar(sql.c_str()), sql.length());
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
}

db2_writer::~db2_writer()
{
    if (stmt_ != SQL_NULL_HSTMT)
        SQLFreeHandle(SQL_HANDLE_STMT, stmt_);
    if (dbc_ != SQL_NULL_HDBC)
    {
        SQLRETURN rc = SQLDisconnect(dbc_);
        if (!SQL_SUCCEEDED(rc))
        {
            // error
        }
        SQLFreeHandle(SQL_HANDLE_DBC, dbc_);
    }
}

void db2_writer::write_impl(const event& evt)
{
    SQLLEN ret_len;
    std::string msg = formatter_->format(evt);
    SQLRETURN rc = SQLBindParameter(stmt_,
                                    1,
                                    SQL_PARAM_INPUT,
                                    SQL_C_CHAR,
                                    SQL_VARCHAR,
                                    msg.length(),
                                    0,
                                    const_cast<std::string::pointer>(msg.data()),
                                    msg.length(),
                                    &ret_len);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    auto pieces = calendar::get_utc(event::clock_type::to_time_t(evt.get_time()));
    struct TIMESTAMP_STRUCT ts;
    ts.year = pieces.tm_year + 1900;
    ts.month = pieces.tm_mon + 1;
    ts.day = pieces.tm_mday;
    ts.hour = pieces.tm_hour;
    ts.minute = pieces.tm_min;
    ts.second = pieces.tm_sec;
    ts.fraction = 0;
    rc = SQLBindParameter(stmt_,
                          2,
                          SQL_PARAM_INPUT,
                          SQL_C_TYPE_TIMESTAMP,
                          SQL_TYPE_TIMESTAMP,
                          SQL_TIMESTAMP_LEN,
                          0,
                          &ts,
                          sizeof(ts),
                          &ret_len);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    auto slen = std::strlen(evt.get_file_name());
    rc = SQLBindParameter(stmt_,
                          3,
                          SQL_PARAM_INPUT,
                          SQL_C_CHAR,
                          SQL_VARCHAR,
                          slen,
                          0,
                          const_cast<char*>(evt.get_file_name()),
                          slen,
                          &ret_len);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    long ln = evt.get_line_number();
    rc = SQLBindParameter(stmt_,
                          4,
                          SQL_PARAM_INPUT,
                          SQL_C_LONG,
                          SQL_INTEGER,
                          10,
                          0,
                          &ln,
                          sizeof(ln),
                          &ret_len);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    slen = std::strlen(evt.get_function_name());
    rc = SQLBindParameter(stmt_,
                          5,
                          SQL_PARAM_INPUT,
                          SQL_C_CHAR,
                          SQL_VARCHAR,
                          slen,
                          0,
                          const_cast<char*>(evt.get_function_name()),
                          slen,
                          &ret_len);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    rc = SQLBindParameter(stmt_,
                          6,
                          SQL_PARAM_INPUT,
                          SQL_C_CHAR,
                          SQL_VARCHAR,
                          evt.get_logger()->get_name().length(),
                          0,
                          const_cast<char*>(evt.get_logger()->get_name().data()),
                          evt.get_logger()->get_name().length(),
                          &ret_len);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    slen = std::strlen(evt.get_level()->get_name());
    rc = SQLBindParameter(stmt_,
                          7,
                          SQL_PARAM_INPUT,
                          SQL_C_CHAR,
                          SQL_VARCHAR,
                          slen,
                          0,
                          const_cast<char*>(evt.get_level()->get_name()),
                          slen,
                          &ret_len);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    if (evt.get_marker())
    {
        std::ostringstream stream;
        stream << *evt.get_marker();
        auto mrk_text = stream.str();
        rc = SQLBindParameter(stmt_,
                              8,
                              SQL_PARAM_INPUT,
                              SQL_C_CHAR,
                              SQL_VARCHAR,
                              mrk_text.length(),
                              0,
                              const_cast<char*>(mrk_text.data()),
                              mrk_text.length(),
                              &ret_len);
    }
    else
    {
        rc = SQLBindParameter(stmt_,
                              8,
                              SQL_PARAM_INPUT,
                              SQL_C_PTR,
                              SQL_NULL_DATA,
                              0,
                              0,
                              nullptr,
                              0,
                              &ret_len);
    }
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    std::string tname = stream.str();
    rc = SQLBindParameter(stmt_,
                          9,
                          SQL_PARAM_INPUT,
                          SQL_C_CHAR,
                          SQL_VARCHAR,
                          tname.length(),
                          0,
                          const_cast<char*>(tname.data()),
                          tname.length(),
                          &ret_len);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
    SQLSMALLINT disposition = SQL_COMMIT;
    rc = SQLExecute(stmt_);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
        disposition = SQL_ROLLBACK;
    }
    rc = SQLEndTran(SQL_HANDLE_DBC, dbc_, disposition);
    if (!SQL_SUCCEEDED(rc))
    {
        // error
    }
}

}