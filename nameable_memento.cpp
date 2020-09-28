/*
 * Copyright 2013-2020 Will Mason
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

#include <chucho/nameable_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

nameable_memento::nameable_memento(configurator& cfg)
    : memento(cfg)
{
    cfg.get_security_policy().set_text("nameable::name", 256);
    set_handler("name", [this] (const std::string& name) { name_  = validate("nameable::name", name); });
}

void nameable_memento::set_default_name(const std::type_info& ti)
{
    name_ = demangle::get_demangled_name(ti);
}

}
