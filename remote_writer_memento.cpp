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

#include <chucho/remote_writer_memento.hpp>

namespace chucho
{

remote_writer_memento::remote_writer_memento(configurator& cfg, memento_key_set ks)
    : writer_memento(cfg)
{
    set_status_origin("remote_writer_memento");
    cfg.get_security_policy().set_integer("remote_writer::port", 1, 65535);
    cfg.get_security_policy().set_text("remote_writer::port(text)", 5);
    set_handler("host", [this] (const std::string& name) { host_ = name; });
    set_handler("port", [this] (const std::string& port) { port_ = validate("remote_writer::port", static_cast<std::uint16_t>(std::stoul(validate("remote_writer::port(text)", port)))); });
    if (ks == memento_key_set::CHUCHO)
        set_handler("unsent_cache_max", [this](const std::string& sz) {unsent_cache_max_ = static_cast<std::size_t>(stoull(sz));}); 
}

}
