/*
 * Copyright 2013-2017 Will Mason
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

#include <chucho/db2_writer_memento.hpp>

namespace chucho
{

db2_writer_memento::db2_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("db2_writer_memento");
    cfg.get_security_policy().set_text("db2_writer::database", 128);
    cfg.get_security_policy().set_text("db2_writer::user", 256);
    cfg.get_security_policy().set_text("db2_writer::password", 256);
    set_handler("user", [this] (const std::string& user) { user_ = validate("db2_writer::user", user); });
    set_handler("password", [this] (const std::string& password) { password_ = validate("db2_writer::password", password); });
    set_handler("database", [this] (const std::string& database) { database_ = validate("db2_writer::database", database); });
}

}
