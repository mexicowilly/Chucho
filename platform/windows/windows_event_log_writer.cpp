/*
 * Copyright 2013 Will Mason
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

#include <chucho/windows_event_log_writer.hpp>
#include <chucho/exception.hpp>
#include "error_util.hpp"

namespace chucho
{

windows_event_log_writer::windows_event_log_writer(std::shared_ptr<formatter> fmt,
                                                   const std::string& source)
    : writer(fmt),
      handle_(nullptr),
      user_(nullptr),
      log_("Application"),
      source_(source)
{
    init();
}

windows_event_log_writer::windows_event_log_writer(std::shared_ptr<formatter> fmt,
                                                   const std::string& log,
                                                   const std::string& source)
    : writer(fmt),
      handle_(nullptr),
      user_(nullptr),
      log_(log),
      source_(source)
{
    init();
}

windows_event_log_writer::~windows_event_log_writer()
{
    delete [] reinterpret_cast<std::uint8_t*>(user_);
    DeregisterEventSource(handle_);
}

void windows_event_log_writer::init()
{
    if (source_.empty())
        throw exception("Source must be set for windows_event_log_writer");
    look_up_user();
    prepare_registry();
    handle_ = RegisterEventSourceA(nullptr, source_.c_str());
    if (handle_ == nullptr)
    {
        DWORD err = GetLastError();
        throw exception("Could not register event source: " +
            error_util::message(err));
    }
}

void windows_event_log_writer::look_up_user()
{
    HANDLE proc;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &proc))
        throw exception("Unable to query the process token");
    struct sentry
    {
        sentry(HANDLE h) : h_(h) { }
        ~sentry() { CloseHandle(h_); }
        HANDLE h_;
    } s(proc);
    DWORD len;
    if (!GetTokenInformation(proc, TokenUser, nullptr, 0, &len))
        throw exception("Unable to query user token length");
    std::vector<std::uint8_t> raw_toke(len);
    if (!GetTokenInformation(proc, TokenUser, &raw_toke[0], len, &len))
        throw exception("Could not retrieve user token");
    TOKEN_USER* toke = reinterpret_cast<TOKEN_USER*>(&raw_toke[0]);
    DWORD toke_len = GetLengthSid(toke->User.Sid);
    user_ = reinterpret_cast<SID*>(new std::uint8_t[toke_len]);
    if (!CopySid(toke_len, user_, toke->User.Sid))
    {
        delete [] reinterpret_cast<std::uint8_t*>(user_);
        throw exception("Unable to copy user SID");
    }
}

void windows_event_log_writer::prepare_registry()
{
    std::string key_name = "SYSTEM\\CurrentControlSet\\Services\\EventLog" +
        log_ + "\\" + source_;
    HKEY key;
    LONG rc = RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                              key_name.c_str(),
                              0,
                              nullptr,
                              REG_OPTION_NON_VOLATILE,
                              KEY_WRITE,
                              nullptr,
                              &key,
                              nullptr);
    if (rc != ERROR_SUCCESS)
    {
        throw exception("Unable to open registry key " +
            key_name + ": " + error_util::message(rc));
    }
    struct sentry
    {
        sentry(HKEY k) : k_(k) { }
        ~sentry() { RegCloseKey(k_); }
        HKEY k_;
    } s(key);
    const char* mf = "ChuchoPlaceholderMessages.dll";
    rc = RegSetValueExA(key,
                        "EventMessageFile",
                        0,
                        REG_SZ,
                        reinterpret_cast<const BYTE*>(mf),
                        std::strlen(mf) + 1);
    if (rc != ERROR_SUCCESS)
    {
        throw exception("Unable to set registry value EventMessageFile for key " +
            key_name + ": " + error_util::message(rc));
    }
}

void windows_event_log_writer::write_impl(const event& evt)
{
    if (handle_ != nullptr)
    {
        std::string msg = formatter_->format(evt);
        if (msg.length() > 31839)
            msg.resize(31839);
        WORD type;
        if (*evt.get_level() >= *level::ERROR_())
            type = EVENTLOG_ERROR_TYPE;
        else if (*evt.get_level() >= *level::WARN_())
            type = EVENTLOG_WARNING_TYPE;
        else
            type = EVENTLOG_INFORMATION_TYPE;
        const char* cmsg = msg.c_str();
        if (!ReportEventA(handle_,
                          type,
                          evt.get_level()->get_value(),
                          1,
                          user_,
                          1,
                          0,
                          &cmsg,
                          nullptr))
        {
            DWORD err = GetLastError();
            throw exception("Unable to report a Windows Event Log event: " +
                error_util::message(err));
        }
    }
}

}
