/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/pipe_writer_memento.hpp>
#include <chucho/pipe_writer.hpp>

namespace chucho
{

pipe_writer_memento::pipe_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("pipe_writer_memento");
    set_default_name(typeid(pipe_writer));
    cfg.get_security_policy().set_text("pipe_writer::flush", 5);
    set_handler("flush", [this] (const std::string& val) { flush_ = boolean_value(validate("pipe_writer::flush", val)); });
}

}
