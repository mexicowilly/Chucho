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

std::shared_ptr<configurable> door_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    std::shared_ptr<configurable> cnf;
    auto dwm = std::dynamic_pointer_cast<door_writer_memento>(mnto);
    assert(dwm);
    if (!dwm->get_formatter())
        throw exception("door_writer_factory: The writer's formatter is not set");
    if (dwm->get_file_name().empty())
        throw exception("door_writer_factory: The door's file name must be set");
    cnf.reset(new door_writer(dwm->get_formatter(),
                              dwm->get_file_name()));
    set_filters(cnf, dwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> door_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<door_writer_memento>(cfg);
    return mnto;
}
}

