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

#if !defined(CHUCHO_WINDOWS_EVENT_LOG_WRITER__)
#define CHUCHO_WINDOWS_EVENT_LOG_WRITER__

#include <chucho/writer.hpp>
#include <windows.h>

namespace chucho
{

class CHUCHO_EXPORT windows_event_log_writer : public writer
{
public:
    windows_event_log_writer(std::shared_ptr<formatter> fmt,
                             const std::string& source,
                             const std::string& host = std::string());
    windows_event_log_writer(std::shared_ptr<formatter> fmt,
                             const std::string& log,
                             const std::string& source,
                             const std::string& host = std::string());
    ~windows_event_log_writer();

    const std::string& get_host() const;
    const std::string& get_log() const;
    const std::string& get_source() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    void init();
    void look_up_user();
    void prepare_registry();

    HANDLE handle_;
    SID* user_;
    std::string log_;
    std::string source_;
    std::string host_;
};

inline const std::string& windows_event_log_writer::get_host() const
{
    return host_;
}

inline const std::string& windows_event_log_writer::get_log() const
{
    return log_;
}

inline const std::string& windows_event_log_writer::get_source() const
{
    return source_;
}

}

#endif
