/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_WINDOWS_EVENT_LOG_WRITER_MEMENTO_HPP__)
#define CHUCHO_WINDOWS_EVENT_LOG_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/memento_key_set.hpp>

namespace chucho
{

class windows_event_log_writer_memento : public writer_memento
{
public:
    windows_event_log_writer_memento(configurator& cfg, memento_key_set ks);

    const std::string& get_host() const;
    const std::string& get_log() const;
    const std::string& get_source() const;

private:
    std::string host_;
    std::string log_;
    std::string source_;
};

inline const std::string& windows_event_log_writer_memento::get_host() const
{
    return host_;
}

inline const std::string& windows_event_log_writer_memento::get_log() const
{
    return log_;
}

inline const std::string& windows_event_log_writer_memento::get_source() const
{
    return source_;
}

}

#endif
