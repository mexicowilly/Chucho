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

#include <chucho/async_writer_memento.hpp>
#include <chucho/move_util.hpp>
#include <chucho/text_util.hpp>

namespace chucho
{

async_writer_memento::async_writer_memento(configurator& cfg)
    : memento(cfg),
      name_("chucho::async_writer")
{
    set_status_origin("async_writer_memento");
    cfg.get_security_policy().set_integer("async_writer::chunk_size", 1024, 100 * 1024 * 1024);
    cfg.get_security_policy().set_integer("async_writer::max_chunks", 2, 1000000);
    cfg.get_security_policy().set_text("async_writer::chunk_size(text)", 9);
    cfg.get_security_policy().set_text("async_writer::max_chunks(text)", 7);
    cfg.get_security_policy().set_text("async_writer::flush_on_destruct", 5);
    cfg.get_security_policy().set_text("async_writer::name", 256);
    set_handler("chunk_size", [this] (const std::string& s) { chunk_size_ = validate("async_writer::chunk_size",
         text_util::parse_byte_size(validate("async_writer::chunk_size(text)", s))); });
    set_handler("max_chunks", [this] (const std::string& cap) { max_chunks_ = validate("async_writer::max_chunks", std::stoul(validate("async_writer::max_chunks(text)", cap))); });
    set_handler("flush_on_destruct", [this] (const std::string& val) { flush_on_destruct_ = boolean_value(validate("async_writer::flush_on_destruct", val)); });
    set_handler("name", [this] (const std::string& name) { name_ = validate("async_writer::name", name); });
}

void async_writer_memento::handle(std::unique_ptr<configurable>&& cnf)
{
    auto wrt = dynamic_move<writer>(std::move(cnf));
    if (wrt)
        writer_ = std::move(wrt);
    else
        memento::handle(std::move(cnf));
}

}
