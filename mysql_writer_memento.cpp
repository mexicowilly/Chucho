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

#include <chucho/mysql_writer_memento.hpp>

namespace chucho
{

mysql_writer_memento::mysql_writer_memento(const configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("mysql_writer_memento");
    set_handler("host", [this] (const std::string& host) { host_ = host; });
    set_handler("user", [this] (const std::string& user) { user_ = user; });
    set_handler("password", [this] (const std::string& password) { password_ = password; });
    set_handler("database", [this] (const std::string& database) { database_ = database; });
    set_handler("port", [this] (const std::string& port) { port_ = static_cast<std::uint16_t>(std::stoul(port)); });
    set_handler("queue_capacity", [this] (const std::string& cap) { queue_capacity_ = std::stoul(cap); });
    set_handler("discard_threshold", [this] (const std::string& name) { discard_threshold_ = level::from_text(name); });
    set_handler("flush_on_destruct", [this] (const std::string& name) { flush_on_destruct_ = boolean_value(name); });
}

}
