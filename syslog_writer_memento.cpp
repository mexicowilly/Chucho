/*
 * Copyright 2013-2021 Will Mason
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
#include <chucho/syslog_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/text_util.hpp>

namespace chucho
{

syslog_writer_memento::syslog_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("syslog_writer_memento");
    set_default_name(typeid(syslog_writer));
    cfg.get_security_policy().set_integer("syslog_writer::port", static_cast<std::uint16_t>(1), static_cast<std::uint16_t>(65535));
    cfg.get_security_policy().set_text("syslog_writer::port(text)", 5);
    cfg.get_security_policy().set_text("syslog_writer::facility", 8);
    cfg.get_security_policy().set_text("syslog_writer::host_name", 253);
    set_handler("facility", std::bind(&syslog_writer_memento::set_facility, this, std::placeholders::_1));
    set_handler("host_name", [this] (const std::string& name) { host_name_ = validate("syslog_writer::host_name", name); });
    set_alias("host_name", "host");
    set_handler("port", [this] (const std::string& port) { port_ = validate("syslog_writer::port", static_cast<std::uint16_t>(std::stoul(validate("syslog_writer::port(text)", port)))); });
}

void syslog_writer_memento::set_facility(const std::string& name)
{
    auto lname = text_util::to_lower(validate("syslog_writer::facility", name));
    if (lname == "kern")
        facility_ = syslog::facility::KERN;
    else if (lname == "user")
        facility_ = syslog::facility::USER;
    else if (lname == "mail")
        facility_ = syslog::facility::MAIL;
    else if (lname == "daemon")
        facility_ = syslog::facility::DAEMON;
    else if (lname == "auth")
        facility_ = syslog::facility::AUTH;
    else if (lname == "syslog")
        facility_ = syslog::facility::SYSLOG;
    else if (lname == "lpr")
        facility_ = syslog::facility::LPR;
    else if (lname == "news")
        facility_ = syslog::facility::NEWS;
    else if (lname == "uucp")
        facility_ = syslog::facility::UUCP;
    else if (lname == "cron")
        facility_ = syslog::facility::CRON;
    else if (lname == "authpriv")
        facility_ = syslog::facility::AUTHPRIV;
    else if (lname == "ftp")
        facility_ = syslog::facility::FTP;
    else if (lname == "local0")
        facility_ = syslog::facility::LOCAL0;
    else if (lname == "local1")
        facility_ = syslog::facility::LOCAL1;
    else if (lname == "local2")
        facility_ = syslog::facility::LOCAL2;
    else if (lname == "local3")
        facility_ = syslog::facility::LOCAL3;
    else if (lname == "local4")
        facility_ = syslog::facility::LOCAL4;
    else if (lname == "local5")
        facility_ = syslog::facility::LOCAL5;
    else if (lname == "local6")
        facility_ = syslog::facility::LOCAL6;
    else if (lname == "local7")
        facility_ = syslog::facility::LOCAL7;
    else
        throw exception("facility has an invalid value of " + name);
}

}
