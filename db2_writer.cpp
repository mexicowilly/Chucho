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

#include <chucho/db2_writer.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/exception.hpp>
#include <chucho/calendar.hpp>
#include <chucho/logger.hpp>
#include <chucho/host.hpp>
#include <cstring>
#include <sstream>
#include <thread>

namespace
{

SQLHENV db2_env = SQL_NULL_HENV;

SQLCHAR* get_sqlchar(const std::string& str)
{
    return reinterpret_cast<SQLCHAR*>(const_cast<std::string::pointer>(str.c_str()));
}

}

namespace chucho
{

db2_writer::db2_writer(const std::string& name,
                       std::shared_ptr<formatter> fmt,
                       const std::string& database,
                       const std::string& user,
                       const std::string& password)
    : database_writer(name, fmt),
      database_(database),
      user_(user),
      password_(password)
{
    set_status_origin("db2_writer");

    static std::once_flag once;
    std::call_once(once, std::bind(&db2_writer::allocate_environment, this));

    SQLRETURN rc = SQLAllocHandle(SQL_HANDLE_DBC, db2_env, &dbc_);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to allocate database handle: " + get_error_message(SQL_HANDLE_DBC));
    rc = SQLConnect(dbc_,
                    get_sqlchar(database_.c_str()),
                    database_.length(),
                    get_sqlchar(user_.c_str()),
                    user_.length(),
                    get_sqlchar(password_.c_str()),
                    password_.length());
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to connect to database " + database_ + ": " + get_error_message(SQL_HANDLE_DBC));
    rc = SQLAllocHandle(SQL_HANDLE_STMT, dbc_, &stmt_);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to allocate statement handle: " + get_error_message(SQL_HANDLE_DBC));
    std::string sql("INSERT INTO chucho_event ( formatted_message, timestmp, file_name, line_number, function_name, logger, level_name, marker, thread, host_name ) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ? );");
    rc = SQLPrepare(stmt_, get_sqlchar(sql.c_str()), sql.length());
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to prepare SQL statement: " + get_error_message(SQL_HANDLE_STMT));
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
            report_warning("Error disconnecting from database " + database_ +
                ": " + get_error_message(SQL_HANDLE_DBC));
        }
        SQLFreeHandle(SQL_HANDLE_DBC, dbc_);
    }
}

void db2_writer::allocate_environment() const
{
    SQLRETURN rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &db2_env);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to allocate DB2 environment handle: " + get_error_message(SQL_HANDLE_ENV));
    rc = SQLSetEnvAttr(db2_env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER*>(SQL_OV_ODBC3), 0);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to set DB2 environment attribute: " + get_error_message(SQL_HANDLE_ENV));
    chucho::garbage_cleaner::get().add([&] { if (db2_env != SQL_NULL_HENV) { SQLFreeHandle(SQL_HANDLE_ENV, db2_env); } });
}

std::string db2_writer::get_error_message(SQLSMALLINT handle_type) const
{
    SQLHANDLE hand;
    if (handle_type == SQL_HANDLE_ENV)
        hand = db2_env;
    else if (handle_type == SQL_HANDLE_DBC)
        hand = dbc_;
    else if (handle_type == SQL_HANDLE_STMT)
        hand = stmt_;
    SQLCHAR state[6];
    SQLSMALLINT msg_len;
    std::vector<SQLCHAR> text(1024);
    SQLINTEGER native;
    int i = 1;
    std::ostringstream stream;
    while (true)
    {
        SQLRETURN rc = SQLGetDiagRec(handle_type,
                                     hand,
                                     i,
                                     state,
                                     &native,
                                     &text[0],
                                     text.size(),
                                     &msg_len);
        if (rc == SQL_SUCCESS_WITH_INFO)
        {
            text.resize(msg_len + 1);
            rc = SQLGetDiagRec(handle_type,
                               hand,
                               i,
                               state,
                               &native,
                               &text[0],
                               text.size(),
                               &msg_len);
        }
        if (rc != SQL_SUCCESS)
            break;
        stream << "{ " << i << ": " << &text[0] << " (state " << state << ") (native " << native << ')' << "} ";
        i++;
    }
    return stream.str();
}

void db2_writer::write_impl(const event& evt)
{
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
                                    NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind formatted message parameter: " + get_error_message(SQL_HANDLE_STMT));
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
                          NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind timestamp parameter: " + get_error_message(SQL_HANDLE_STMT));
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
                          NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind file name parameter: " + get_error_message(SQL_HANDLE_STMT));
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
                          NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind line number parameter: " + get_error_message(SQL_HANDLE_STMT));
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
                          NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind function name parameter: " + get_error_message(SQL_HANDLE_STMT));
    rc = SQLBindParameter(stmt_,
                          6,
                          SQL_PARAM_INPUT,
                          SQL_C_CHAR,
                          SQL_VARCHAR,
                          evt.get_logger()->get_name().length(),
                          0,
                          const_cast<char*>(evt.get_logger()->get_name().data()),
                          evt.get_logger()->get_name().length(),
                          NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind logger parameter: " + get_error_message(SQL_HANDLE_STMT));
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
                          NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind level parameter: " + get_error_message(SQL_HANDLE_STMT));
    // We need this variable in this scope
    SQLLEN nd = SQL_NULL_DATA;
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
                              NULL);
    }
    else
    {
        rc = SQLBindParameter(stmt_,
                              8,
                              SQL_PARAM_INPUT,
                              SQL_C_CHAR,
                              SQL_VARCHAR,
                              0,
                              0,
                              NULL,
                              0,
                              &nd);

    }
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind marker parameter: " + get_error_message(SQL_HANDLE_STMT));
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
                          NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind thread parameter: " + get_error_message(SQL_HANDLE_STMT));
    std::string hname = host::get_full_name();
    rc = SQLBindParameter(stmt_,
                          10,
                          SQL_PARAM_INPUT,
                          SQL_C_CHAR,
                          SQL_VARCHAR,
                          hname.length(),
                          0,
                          const_cast<char*>(hname.data()),
                          hname.length(),
                          NULL);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to bind host_name parameter: " + get_error_message(SQL_HANDLE_STMT));
    rc = SQLExecute(stmt_);
    if (!SQL_SUCCEEDED(rc))
        throw exception("Unable to execute prepared statement (INSERT): " + get_error_message(SQL_HANDLE_STMT));
}

}
