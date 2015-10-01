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

#include <chucho/async_writer_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

async_writer_memento::async_writer_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("async_writer_memento");
    cfg.get_security_policy().set_integer("async_writer::queue_capacity", 10, 32 * 1024);
    cfg.get_security_policy().set_text("async_writer::queue_capacity(text)", 5);
    cfg.get_security_policy().set_text("async_writer::flush_on_destruct", 5);
    set_handler("discard_threshold", [this] (const std::string& name) { discard_threshold_ = level::from_text(validate("async_writer::discard_threshold", name)); });
    set_handler("queue_capacity", [this] (const std::string& cap) { queue_capacity_ = validate("async_writer::queue_capacity", std::stoul(validate("async_writer::queue_capacity(text)", cap))); });
    set_handler("flush_on_destruct", [this] (const std::string& val) { flush_on_destruct_ = boolean_value(validate("async_writer::flush_on_destruct", val)); });
}

void async_writer_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto wrt = std::dynamic_pointer_cast<writer>(cnf);
    if (wrt)
        writer_ = wrt;
    else
        memento::handle(cnf);
}

}
