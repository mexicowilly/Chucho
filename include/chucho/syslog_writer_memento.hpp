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

#if !defined(CHUCHO_SYSLOG_WRITER_MEMENTO_HPP__)
#define CHUCHO_SYSLOG_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/syslog_constants.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class syslog_writer_memento : public writer_memento
{
public:
    syslog_writer_memento(configurator& cfg);

    const optional<syslog::facility>& get_facility() const;
    const std::string& get_host_name() const;
    const optional<std::uint16_t> get_port() const;

private:
    void set_facility(const std::string& name);

    std::string host_name_;
    optional<syslog::facility> facility_;
    optional<std::uint16_t> port_;
};

inline const optional<syslog::facility>& syslog_writer_memento::get_facility() const
{
    return facility_;
}

inline const std::string& syslog_writer_memento::get_host_name() const
{
    return host_name_;
}

inline const optional<std::uint16_t> syslog_writer_memento::get_port() const
{
    return port_;
}

}

#endif
