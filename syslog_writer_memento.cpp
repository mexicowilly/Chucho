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

#include <chucho/syslog_writer_memento.hpp>
#include <chucho/exception.hpp>
#include <map>

namespace
{

// If you make this static to the set_facility method, then
// you get SIGABRT when an exception is thrown.
const std::map<std::string, chucho::syslog::facility> facilities =
{
    { "kern", chucho::syslog::facility::KERN },
    { "user", chucho::syslog::facility::USER },
    { "mail", chucho::syslog::facility::MAIL },
    { "daemon", chucho::syslog::facility::DAEMON },
    { "auth", chucho::syslog::facility::AUTH },
    { "syslog", chucho::syslog::facility::SYSLOG },
    { "lpr", chucho::syslog::facility::LPR },
    { "news", chucho::syslog::facility::NEWS },
    { "uucp", chucho::syslog::facility::UUCP },
    { "cron", chucho::syslog::facility::CRON },
    { "authpriv", chucho::syslog::facility::AUTHPRIV },
    { "ftp", chucho::syslog::facility::FTP },
    { "local0", chucho::syslog::facility::LOCAL0 },
    { "local1", chucho::syslog::facility::LOCAL1 },
    { "local2", chucho::syslog::facility::LOCAL2 },
    { "local3", chucho::syslog::facility::LOCAL3 },
    { "local4", chucho::syslog::facility::LOCAL4 },
    { "local5", chucho::syslog::facility::LOCAL5 },
    { "local6", chucho::syslog::facility::LOCAL6 },
    { "local7", chucho::syslog::facility::LOCAL7 }
};

}

namespace chucho
{


syslog_writer_memento::syslog_writer_memento(const configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("syslog_writer_memento");
    set_handler("facility", std::bind(&syslog_writer_memento::set_facility, this, std::placeholders::_1));
    set_handler("host_name", [this] (const std::string& name) { host_name_ = name; });
}

void syslog_writer_memento::set_facility(const std::string& name)
{
    auto found = facilities.find(to_lower(name));
    if (found == facilities.end())
        throw exception("facility has an invalid value of " + name);
    facility_ = found->second;
}

}
