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

#include <chucho/oracle_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/logger.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/calendar.hpp>
#include <sstream>
#include <thread>
#include <cstring>

namespace chucho
{

oracle_writer::oracle_writer(std::shared_ptr<formatter> fmt,
                             const std::string& user,
                             const std::string& password,
                             const std::string& database)
    : database_writer(fmt),
      user_(user),
      password_(password),
      database_(database),
      env_(nullptr),
      err_(nullptr),
      ctx_(nullptr),
      insert_evt_(nullptr),
      formatted_msg_(nullptr),
      timestamp_(nullptr),
      file_name_(nullptr),
      line_number_(nullptr),
      func_(nullptr),
      logger_(nullptr),
      level_(nullptr),
      marker_(nullptr),
      thread_name_(nullptr)
{
    static std::once_flag once;

    std::call_once(once, [] () { garbage_cleaner::get().add([] () { OCITerminate(OCI_DEFAULT); }); });
    set_status_origin("oracle_writer");
    auto rc = OCIEnvCreate(&env_, OCI_DEFAULT, 0, 0, 0, 0, 0, 0);
    if (rc != OCI_SUCCESS) 
        throw exception("Unable to create the Oracle environment");
    rc = OCIHandleAlloc(env_, reinterpret_cast<void**>(&err_), OCI_HTYPE_ERROR, 0, 0);
    if (rc != OCI_SUCCESS) 
    {
        finalize();
        throw exception("Unable to create the Oracle error handle");
    }
    rc = OCILogon2(env_,
                   err_,
                   &ctx_,
                   reinterpret_cast<const OraText*>(user.data()), user.length(),
                   reinterpret_cast<const OraText*>(password.data()), password.length(),
                   reinterpret_cast<const OraText*>(database.data()), database.length(),
                   OCI_DEFAULT);
    react(rc, "Unable to logon", true);
    rc = OCIHandleAlloc(env_, reinterpret_cast<void**>(&insert_evt_), OCI_HTYPE_STMT, 0, 0);
    react(rc, "Unable to create statement handle", true);
    std::string sql("INSERT INTO chucho_event ( formatted_message, timestmp, file_name, line_number, function_name, logger, level_name, marker, thread ) VALUES ( :formatted_message, :timestmp, :file_name, :line_number, :function_name, :logger, :level_name, :marker, :thread )");
    rc = OCIStmtPrepare(insert_evt_,
                        err_,
                        reinterpret_cast<const OraText*>(sql.data()),
                        sql.length(),
                        OCI_NTV_SYNTAX,
                        OCI_DEFAULT);
    react(rc, "Unable to prepare SQL insert statement", true);
}

oracle_writer::~oracle_writer()
{
    finalize();
}

void oracle_writer::finalize()
{
    if (insert_evt_ != nullptr)
        OCIHandleFree(insert_evt_, OCI_HTYPE_STMT);
    if (ctx_ != nullptr) 
        OCIHandleFree(ctx_, OCI_HTYPE_SVCCTX);
    if (err_ != nullptr) 
        OCIHandleFree(err_, OCI_HTYPE_ERROR);
    if (env_ != nullptr)
        OCIHandleFree(env_, OCI_HTYPE_ENV);
}

void oracle_writer::react(sword code, const std::string& msg, bool final)
{
    if (code == OCI_INVALID_HANDLE)
    {
        if (final) 
            finalize();
        throw exception("[oracle_writer] " + msg + ": An invalid handle was used with OCI");
    }
    else if (code == OCI_NEED_DATA) 
    {
        report_warning(msg + ": An OCI call failed due to lack of runtime data");
    }
    else if (code == OCI_ERROR || code == OCI_SUCCESS_WITH_INFO)
    {
        char buf[1024];
        buf[0] = 0;
        sb4 ecode;
        OCIErrorGet(err_, 1, 0, &ecode, reinterpret_cast<OraText*>(buf), 1024, OCI_HTYPE_ERROR);
        std::string full_msg(msg + ": " + std::string(buf));
        if (code == OCI_ERROR) 
        {
            if (final) 
                finalize();
            throw exception("[oracle_writer]" + full_msg);
        }
        report_warning(full_msg);
    }
}

void oracle_writer::write_impl(const event& evt)
{
    std::string param(":formatted_message");
    std::string msg = formatter_->format(evt);
    sword rc = OCIBindByName(insert_evt_,
                             &formatted_msg_,
                             err_,
                             reinterpret_cast<const OraText*>(param.data()),
                             param.length(),
                             reinterpret_cast<void*>(const_cast<char*>(msg.data())),
                             msg.length(),
                             SQLT_CHR,
                             0, 0, 0, 0, 0,
                             OCI_DEFAULT);
    react(rc, "Unable to bind formatted message", false);
    param = ":timestmp";
    auto pieces = calendar::get_utc(event::clock_type::to_time_t(evt.get_time()));
    OCIDateTime* dt = nullptr;
    rc = OCIDescriptorAlloc(env_,
                            reinterpret_cast<void**>(&dt),
                            OCI_DTYPE_TIMESTAMP,
                            0,
                            0);
    react(rc, "Unable to create timestamp descriptor", false);
    struct sentry
    {
        sentry(OCIDateTime* odt) : dt_(odt) { }
        ~sentry() { OCIDescriptorFree(dt_, OCI_DTYPE_TIMESTAMP); }
        OCIDateTime* dt_;
    } sent(dt);
    std::string utc("+00");
    rc = OCIDateTimeConstruct(env_,
                              err_,
                              dt,
                              pieces.tm_year + 1900,
                              pieces.tm_mon + 1,
                              pieces.tm_mday,
                              pieces.tm_hour,
                              pieces.tm_min,
                              pieces.tm_sec,
                              0,
                              reinterpret_cast<OraText*>(const_cast<char*>(utc.data())),
                              utc.length());
    react(rc, "Unable to construct timestamp", false);
    rc = OCIBindByName(insert_evt_,
                       &timestamp_,
                       err_,
                       reinterpret_cast<const OraText*>(param.data()),
                       param.length(),
                       reinterpret_cast<void*>(&dt),
                       sizeof(dt),
                       SQLT_TIMESTAMP,
                       0, 0, 0, 0, 0,
                       OCI_DEFAULT);
    react(rc, "Unable to bind timestamp", false);
    param = ":file_name";
    rc = OCIBindByName(insert_evt_,
                       &file_name_,
                       err_,
                       reinterpret_cast<const OraText*>(param.data()),
                       param.length(),
                       reinterpret_cast<void*>(const_cast<char*>(evt.get_file_name())),
                       std::strlen(evt.get_file_name()),
                       SQLT_CHR,
                       0, 0, 0, 0, 0,
                       OCI_DEFAULT);
    react(rc, "Unable to bind file name", false);
    param = ":line_number";
    unsigned ln = evt.get_line_number();
    rc = OCIBindByName(insert_evt_,
                       &line_number_,
                       err_,
                       reinterpret_cast<const OraText*>(param.data()),
                       param.length(),
                       reinterpret_cast<void*>(&ln),
                       sizeof(ln),
                       SQLT_INT,
                       0, 0, 0, 0, 0,
                       OCI_DEFAULT);
    react(rc, "Unable to bind line number", false);
    param = ":function_name";
    rc = OCIBindByName(insert_evt_,
                       &func_,
                       err_,
                       reinterpret_cast<const OraText*>(param.data()),
                       param.length(),
                       reinterpret_cast<void*>(const_cast<char*>(evt.get_function_name())),
                       std::strlen(evt.get_function_name()),
                       SQLT_CHR,
                       0, 0, 0, 0, 0,
                       OCI_DEFAULT);
    react(rc, "Unable to bind function name", false);
    param = ":logger";
    auto lgr = evt.get_logger();
    rc = OCIBindByName(insert_evt_,
                       &logger_,
                       err_,
                       reinterpret_cast<const OraText*>(param.data()),
                       param.length(),
                       reinterpret_cast<void*>(const_cast<char*>(lgr->get_name().data())),
                       lgr->get_name().length(),
                       SQLT_CHR,
                       0, 0, 0, 0, 0,
                       OCI_DEFAULT);
    react(rc, "Unable to bind logger", false);
    param = ":level_name";
    auto lvl = evt.get_level()->get_name();
    rc = OCIBindByName(insert_evt_,
                       &logger_,
                       err_,
                       reinterpret_cast<const OraText*>(param.data()),
                       param.length(),
                       reinterpret_cast<void*>(const_cast<char*>(lvl)),
                       std::strlen(lvl),
                       SQLT_CHR,
                       0, 0, 0, 0, 0,
                       OCI_DEFAULT);
    react(rc, "Unable to bind level name", false);
    param = ":marker";
    auto mrk = evt.get_marker();
    sb2 ind;
    std::string mrk_text;
    if (mrk) 
    {
        ind = 0;
        std::ostringstream stream;
        stream << *mrk;
        mrk_text = stream.str();
    }
    else
    {
        ind = -1;
    }
    rc = OCIBindByName(insert_evt_,
                       &marker_,
                       err_,
                       reinterpret_cast<const OraText*>(param.data()),
                       param.length(),
                       reinterpret_cast<void*>(const_cast<char*>(mrk_text.data())),
                       mrk_text.length(),
                       SQLT_CHR,
                       &ind, 0, 0, 0, 0,
                       OCI_DEFAULT);
    react(rc, "Unable to bind marker", false);
    param = ":thread";
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    std::string tname = stream.str();
    rc = OCIBindByName(insert_evt_,
                       &thread_name_,
                       err_,
                       reinterpret_cast<const OraText*>(param.data()),
                       param.length(),
                       reinterpret_cast<void*>(const_cast<char*>(tname.data())),
                       tname.length(),
                       SQLT_CHR,
                       0, 0, 0, 0, 0,
                       OCI_DEFAULT);
    react(rc, "Unable to bind thread name", false);
    rc = OCIStmtExecute(ctx_, insert_evt_, err_, 1, 0, 0, 0, OCI_COMMIT_ON_SUCCESS);
    react(rc, "Unable to insert into database", false);
}

}
