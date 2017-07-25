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

#include <chucho/postgres_writer_memento.hpp>

namespace chucho
{

postgres_writer_memento::postgres_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("postgres_writer_memento");
    cfg.get_security_policy().set_text("postgres_writer::uri", 8000);
    set_handler("uri", [this] (const std::string& uri) { uri_ = validate("postgres_writer::uri", uri); });
}

}
