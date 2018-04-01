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

#include <chucho/door_writer_factory.hpp>
#include <chucho/door_writer_memento.hpp>
#include <chucho/door_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

door_writer_factory::door_writer_factory()
{
    set_status_origin("door_writer_factory");
}

std::unique_ptr<configurable> door_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto dwm = dynamic_cast<door_writer_memento*>(mnto.get());
    assert(dwm != nullptr);
    if (dwm->get_name().empty())
        throw exception("door_writer_factory: The name is not set");
    auto fmt = std::move(dwm->get_formatter());
    if (!fmt)
        throw exception("door_writer_factory: The writer's formatter is not set");
    if (dwm->get_file_name().empty())
        throw exception("door_writer_factory: The door's file name must be set");
    auto cnf = std::make_unique<door_writer>(dwm->get_name(),
                                             std::move(fmt),
                                             dwm->get_file_name());
    set_filters(*cnf, *dwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> door_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<door_writer_memento>(cfg);
    return std::move(mnto);
}
}

