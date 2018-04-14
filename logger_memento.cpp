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

#include <chucho/logger_memento.hpp>
#include <chucho/move_util.hpp>

namespace chucho
{

logger_memento::logger_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("logger_memento");
    cfg.get_security_policy().set_text("logger::writes_to_ancestors", 5);
    set_handler("level", [this] (const std::string& name) { level_ = level::from_text(validate("logger::level", name)); });
    set_handler("name", [this] (const std::string& name) {  name_ = validate("logger::name", name); });
    set_handler("writes_to_ancestors", [this] (const std::string& name) { writes_to_ancestors_ = boolean_value(validate("logger::writer_to_ancestors", name)); });
}

void logger_memento::handle(std::unique_ptr<configurable>&& cnf)
{
    auto wrt = dynamic_move<writer>(std::move(cnf));
    if (wrt)
        writers_.push_back(std::move(wrt));
    else
        memento::handle(std::move(cnf));
}

}
