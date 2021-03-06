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

#include <chucho/named_pipe_writer_memento.hpp>
#include <chucho/named_pipe_writer.hpp>

namespace chucho
{

named_pipe_writer_memento::named_pipe_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("named_pipe_writer_memento");
    set_default_name(typeid(named_pipe_writer));
    cfg.get_security_policy().set_text("named_pipe_writer::flush", 5);
    set_handler("flush", [this] (const std::string& val) { flush_ = boolean_value(validate("named_pipe_writer::flush", val)); });
    set_handler("pipe_name", [this] (const std::string& val) { pipe_name_ = validate("named_pipe_writer::pipe_name", val); });
}

}
