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

#include <chucho/mysql_writer_memento.hpp>

namespace chucho
{

mysql_writer_memento::mysql_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("mysql_writer_memento");
    cfg.get_security_policy().set_text("mysql_writer::user", 16);
    cfg.get_security_policy().set_integer("mysql_writer::port", static_cast<std::uint16_t>(1), static_cast<uint16_t>(65535));
    cfg.get_security_policy().set_text("mysql_writer::port(text)", 5);
    cfg.get_security_policy().set_text("mysql_writer::database", 64);
    cfg.get_security_policy().set_integer("mysql_writer::queue_capacity", 10, 32 * 1024);
    cfg.get_security_policy().set_text("mysql_writer::queue_capacity(text)", 5);
    cfg.get_security_policy().set_text("mysql_writer::host", 253);
    cfg.get_security_policy().set_text("mysql_writer::flush_on_destruct", 5);
    set_handler("host", [this] (const std::string& host) { host_ = validate("mysql_writer::host", host); });
    set_handler("user", [this] (const std::string& user) { user_ = validate("mysql_writer::user", user); });
    set_handler("password", [this] (const std::string& password) { password_ = validate("mysql_writer::password", password); });
    set_handler("database", [this] (const std::string& database) { database_ = validate("mysql_writer::database", database); });
    set_handler("port", [this] (const std::string& port) { port_ = validate("mysql_writer::port", static_cast<std::uint16_t>(std::stoul(validate("mysql_writer::port(text)", port)))); });
    set_handler("queue_capacity", [this] (const std::string& cap) { queue_capacity_ = validate("mysql_writer::queue_capacity", std::stoul(validate("mysql_writer::queue_capacity(text)", cap))); });
    set_handler("discard_threshold", [this] (const std::string& name) { discard_threshold_ = level::from_text(validate("mysql_writer::discard_threshold", name)); });
    set_handler("flush_on_destruct", [this] (const std::string& name) { flush_on_destruct_ = boolean_value(validate("mysql_writer::flush_on_destruct", name)); });
}

}
