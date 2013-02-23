/*
 * Copyright 2013 Will Mason
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

#include <chucho/logger_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

logger_memento::logger_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("logger_memento");
    set_handler("level", [this] (const std::string& name) { level_ = level::from_text(name); });
    set_handler("name", [this] (const std::string& name) {  name_ = name; });
    set_handler("writes_to_ancestors", [this] (const std::string& name) { writes_to_ancestors_ = boolean_value(name); });
}

void logger_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto wrt = std::dynamic_pointer_cast<writer>(cnf);
    if (wrt)
    {
        writers_.push_back(wrt);
    }
    else
    {
        report_error("A logger cannot make use of type " +
            demangle::get_demangled_name(typeid(*cnf)) + ". Only writers can be embedded in the configuration.");
    }
}

}
