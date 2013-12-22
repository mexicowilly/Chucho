/*
 * Copyright 2013-2014 Will Mason
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
#include "event_log.hpp"
#include <stdlib.h>

namespace
{

std::string get_dll_name()
{
    std::string result;
    std::vector<char> env(1024);
    DWORD rc = GetEnvironmentVariableA("CHUCHO_EVENT_LOG_DLL",
                                       &env[0],
                                       env.size());
    if (rc > env.size() - 1)
    {
        env.resize(rc);
        rc = GetEnvironmentVariableA("CHUCHO_EVENT_LOG_DLL",
                                     &env[0],
                                     env.size());
    }
    if (rc == 0)
    {
        result = CMAKE_INSTALL_PREFIX;
        for (auto itor = result.begin(); itor != result.end(); itor++)
        {
            if (*itor == '/')
                *itor = '\\';
        }
        result += "\\lib\\event_log.dll";
    }
    else
    {
        result = &env[0];
    }
    return result;
}

DWORD query_registry(HKEY key, const char* const name, std::vector<BYTE>& bytes)
{
    DWORD count = bytes.size();
    DWORD type;
    BYTE* bp = bytes.empty() ? nullptr : &bytes[0];
    LONG rc = RegQueryValueExA(key,
                               name,
                               nullptr,
                               &type,
                               bp,
                               &count);
    if (rc == ERROR_FILE_NOT_FOUND)
    {
        return REG_NONE;
    }
    else if (rc == ERROR_MORE_DATA || (rc == ERROR_SUCCESS && count > bytes.size()))
    {
        bytes.resize(count);
        rc = RegQueryValueExA(key,
                              name,
                              nullptr,
                              &type,
                              &bytes[0],
                              &count);
        if (rc != ERROR_SUCCESS)
        {
            throw chucho::exception("Unable to query registry: " +
                chucho::error_util::message(rc));
        }
    }
    else if (rc != ERROR_SUCCESS)
    {
        throw chucho::exception("Unable to query registry value size: " +
            chucho::error_util::message(rc));
    }
    return type;
}

}

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

windows_event_log_writer::windows_event_log_writer(std::shared_ptr<formatter> fmt,
                                                   const std::string& log,
                                                   const std::string& source,
                                                   const std::string& host)
    : writer(fmt),
      handle_(nullptr),
      user_(nullptr),
      log_(log),
      source_(source),
      host_(host)
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
    set_status_origin("windows_event_log_writer");
    look_up_user();
    try
    {
        prepare_registry();
    }
    catch (std::exception& e)
    {
        report_warning(std::string("The Registry could not be prepared to inform Event Viewer of the location of the Chucho event_log.dll: ") + e.what());
        report_warning("If the correct Registry keys are not already present in the Registry, Event Viewer will not properly display messages from Chucho.");
    }
    const char* actual_host = host_.empty() ? nullptr : host_.c_str();
    handle_ = RegisterEventSourceA(actual_host, source_.c_str());
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
    {
        DWORD err = GetLastError();
        throw exception("Unable to query the process token: " + error_util::message(err));
    }
    struct sentry
    {
        sentry(HANDLE h) : h_(h) { }
        ~sentry() { CloseHandle(h_); }
        HANDLE h_;
    } s(proc);
    DWORD len;
    if (!GetTokenInformation(proc, TokenUser, nullptr, 0, &len))
    {
        DWORD err = GetLastError();
        if (err != ERROR_INSUFFICIENT_BUFFER)
            throw exception("Unable to query user token length: " + error_util::message(err));
    }
    std::vector<std::uint8_t> raw_toke(len);
    if (!GetTokenInformation(proc, TokenUser, &raw_toke[0], len, &len))
    {
        DWORD err = GetLastError();
        throw exception("Could not retrieve user token: " + error_util::message(err));
    }
    TOKEN_USER* toke = reinterpret_cast<TOKEN_USER*>(&raw_toke[0]);
    DWORD toke_len = GetLengthSid(toke->User.Sid);
    user_ = reinterpret_cast<SID*>(new std::uint8_t[toke_len]);
    if (!CopySid(toke_len, user_, toke->User.Sid))
    {
        delete [] reinterpret_cast<std::uint8_t*>(user_);
        DWORD err = GetLastError();
        throw exception("Unable to copy user SID: " + error_util::message(err));
    }
}

void windows_event_log_writer::prepare_registry()
{
    std::string key_name = "SYSTEM\\CurrentControlSet\\Services\\EventLog\\" +
        log_ + "\\" + source_;
    HKEY key;
    LONG rc = RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                              key_name.c_str(),
                              0,
                              nullptr,
                              REG_OPTION_NON_VOLATILE,
                              KEY_READ | KEY_WRITE,
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
    std::string dll = get_dll_name();
    const char* cdll = dll.c_str();
    std::vector<BYTE> bytes;
    DWORD type = query_registry(key, "EventMessageFile", bytes);
    if ((type != REG_SZ && type != REG_EXPAND_SZ) ||
        std::strcmp(reinterpret_cast<char*>(&bytes[0]), cdll) != 0)
    {
        rc = RegSetValueExA(key,
                            "EventMessageFile",
                            0,
                            REG_SZ,
                            reinterpret_cast<const BYTE*>(cdll),
                            dll.length() + 1);
        if (rc != ERROR_SUCCESS)
        {
            throw exception("Unable to set registry value EventMessageFile for key " +
                key_name + ": " + error_util::message(rc));
        }
    }
    DWORD types = EVENTLOG_INFORMATION_TYPE |
                  EVENTLOG_WARNING_TYPE |
                  EVENTLOG_ERROR_TYPE;
    type = query_registry(key, "TypesSupported", bytes);
    if (type != REG_DWORD || *reinterpret_cast<DWORD*>(&bytes[0]) != types)
    {
        rc = RegSetValueExA(key,
                            "TypesSupported",
                            0,
                            REG_DWORD,
                            reinterpret_cast<const BYTE*>(&types),
                            sizeof(types));
        if (rc != ERROR_SUCCESS)
        {
            throw exception("Unable to set registry value TypesSupported for key " +
                key_name + ": " + error_util::message(rc));
        }
    }
}

void windows_event_log_writer::write_impl(const event& evt)
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
                      0,
                      CHUCHO_MESSAGE,
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
