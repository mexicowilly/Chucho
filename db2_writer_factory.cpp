/*
 * Copyright 2013-2017 Will Mason
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

#include <chucho/db2_writer_factory.hpp>
#include <chucho/db2_writer_memento.hpp>
#include <chucho/db2_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

db2_writer_factory::db2_writer_factory()
{
    set_status_origin("db2_writer_factory");
}

std::unique_ptr<configurable> db2_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto dwm = dynamic_cast<db2_writer_memento*>(mnto.get());
    assert(dwm != nullptr);
    if (dwm->get_name().empty())
        throw exception("db2_writer_factory: The name is not set");
    auto fmt = std::move(dwm->get_formatter());
    if (!fmt)
        throw exception("db2_writer_factory: The writer's formatter is not set");
    if (dwm->get_user().empty()) 
        throw exception("db2_writer_factory: The DB2 user name must be set");
    if (dwm->get_password().empty()) 
        throw exception("db2_writer_factory: The DB2 user password must be set");
    if (dwm->get_database().empty()) 
        throw exception("db2_writer_factory: The DB2 database name must be set");
    auto dw = std::make_unique<db2_writer>(dwm->get_name(),
                                           std::move(fmt),
                                           dwm->get_database(),
                                           dwm->get_user(),
                                           dwm->get_password());
    set_filters(*dw, *dwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*dw)));
    return std::move(dw);
}

std::unique_ptr<memento> db2_writer_factory::create_memento(configurator& cfg)
{
    std::unique_ptr<memento> mnto = std::make_unique<db2_writer_memento>(cfg);
    return std::move(mnto);
}

}
