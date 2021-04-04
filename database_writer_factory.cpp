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

#include <chucho/database_writer_factory.hpp>
#include <chucho/database_writer_memento.hpp>
#include <chucho/database_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

std::unique_ptr<configurable> database_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto dwm = dynamic_cast<database_writer_memento*>(mnto.get());
    assert(dwm != nullptr);
    if (dwm->get_name().empty())
        throw exception("database_writer_factory: The name is not set");
    auto fmt = std::move(dwm->get_formatter());
    if (!fmt)
        throw exception("database_writer_factory: The writer's formatter is not set");
    if (dwm->get_connection().empty())
        throw exception("database_writer_factory: The writer's connection is not set");
    auto dw = std::make_unique<database_writer>(dwm->get_name(), std::move(fmt), dwm->get_connection());
    set_filters(*dw, *dwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*dw)));
    return std::move(dw);
}

std::unique_ptr<memento> database_writer_factory::create_memento(configurator& cfg)
{
    std::unique_ptr<memento> mnto = std::make_unique<database_writer_memento>(cfg);
    return std::move(mnto);
}

}
