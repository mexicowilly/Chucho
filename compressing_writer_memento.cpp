/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/compressing_writer_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

compressing_writer_memento::compressing_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("compressing_writer_memento");
    cfg.get_security_policy().set_text("compressing_writer::max_cached_kb(text)", 7);
    cfg.get_security_policy().set_integer("compressing_writer::max_cached_kb", 1, 1000000);
    cfg.get_security_policy().set_text("compressing_writer::max_cached_events(text)", 7);
    cfg.get_security_policy().set_integer("compressing_writer::max_cached_events", 1, 1000000);
    set_handler("max_cached_kb", [this] (const std::string& txt) { max_kb_ = validate("compressing_writer::max_cached_kb", std::stoul(validate("compressing_writer::max_cached_kb(text)", txt))); });
    set_handler("max_cached_events", [this] (const std::string& txt) { max_events_ = validate("compressing_writer::max_cached_events", std::stoul(validate("compressing_writer::max_cached_events(text)", txt))); });
}

void compressing_writer_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto cmp = std::dynamic_pointer_cast<compressor>(cnf);
    if (cmp)
    {
        compressor_ = cmp;
    }
    else
    {
        auto ser = std::dynamic_pointer_cast<serializer>(cnf);
        if (ser)
            serializer_ = ser;
        else
            writer_memento::handle(cnf);
    }
}

}