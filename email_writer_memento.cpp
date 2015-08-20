/*
 * Copyright 2013-2015 Will Mason
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

#include <chucho/email_writer_memento.hpp>
#include <chucho/text_util.hpp>

namespace chucho
{

email_writer_memento::email_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("email_writer_memento");
    cfg.get_security_policy().set_text("email_writer::from", 320);
    cfg.get_security_policy().set_text("email_writer::host", 253);
    cfg.get_security_policy().set_integer("email_writer::port", static_cast<std::uint16_t>(1), static_cast<uint16_t>(65535));
    cfg.get_security_policy().set_text("email_writer::port(text)", 5);
    cfg.get_security_policy().set_text("email_writer::subject", 989);
    cfg.get_security_policy().set_text("email_writer::connection_type", 8);
    cfg.get_security_policy().set_integer("email_writer::buffer_size", 1U, 65536U);
    cfg.get_security_policy().set_text("email_writer::buffer_size(text)", 5);
    cfg.get_security_policy().set_text("email_writer::user", 320);
    cfg.get_security_policy().set_integer("email_writer::to(count)", 1U, 100U);
    cfg.get_security_policy().set_text("email_writer::to(text)", 320 * 100 + 100);
    cfg.get_security_policy().set_text("email_writer::to(address)", 320);
    set_handler("from", [this] (const std::string& from) { from_ = validate("email_writer::from", from); });
    set_handler("to", std::bind(&email_writer_memento::set_to, this, std::placeholders::_1));
    set_handler("host", [this] (const std::string& host) { host_ = validate("email_writer::host", host); });
    set_handler("port", [this] (const std::string& port) { port_ = validate("email_writer::port", static_cast<std::uint16_t>(std::stoul(validate("email_writer::port(text)", port)))); });
    set_handler("subject", [this] (const std::string& sub) { subject_ = validate("email_writer::subject", sub); });
    set_handler("user", [this] (const std::string& user) { user_ = validate("email_writer::user", user); });
    set_handler("password", [this] (const std::string& pass) { password_ = validate("email_writer::password", pass); });
    set_handler("connection_type", std::bind(&email_writer_memento::set_connection_type, this, std::placeholders::_1));
    set_handler("buffer_size", [this] (const std::string& bs) { buffer_size_ = validate("email_writer::buffer_size", std::stoul(validate("email_writer::buffer_size(text)", bs))); });
}

void email_writer_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto trg = std::dynamic_pointer_cast<email_trigger>(cnf);
    if (trg)
        trigger_ = trg;
    else
        writer_memento::handle(cnf);
}

void email_writer_memento::set_connection_type(const std::string& connect)
{
    std::string low = text_util::to_lower(validate("email_writer::connection_type", connect));
    if (low == "clear")
        connection_type_ = email_writer::connection_type::CLEAR;
    else if (low == "ssl")
        connection_type_ = email_writer::connection_type::SSL;
    else if (low == "starttls")
        connection_type_ = email_writer::connection_type::STARTTLS;
    else
        throw exception("connection_type has an invalid value of " + connect);
}

void email_writer_memento::set_to(const std::string& to)
{
    auto rcpts = text_util::tokenize(validate("email_writer::to(text)", to), ',');
    validate("email_writer::to(count)", rcpts.size());
    for (const auto& one : rcpts)
        validate("email_writer::to(address)", one);
    to_ = rcpts;
}

}