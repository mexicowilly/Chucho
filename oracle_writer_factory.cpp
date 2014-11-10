/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/oracle_writer_factory.hpp>
#include <chucho/oracle_writer_memento.hpp>
#include <chucho/oracle_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

oracle_writer_factory::oracle_writer_factory()
{
    set_status_origin("oracle_writer_factory");
}

std::shared_ptr<configurable> oracle_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto owm = std::dynamic_pointer_cast<oracle_writer_memento>(mnto);
    assert(owm);
    if (!owm->get_formatter())
        throw exception("oracle_writer_factory: The writer's formatter is not set");
    if (owm->get_user().empty()) 
        throw exception("oracle_writer_factory: The Oracle user name must be set");
    if (owm->get_password().empty()) 
        throw exception("oracle_writer_factory: The Oracle user password must be set");
    if (owm->get_database().empty()) 
        throw exception("oracle_writer_factory: The Oracle database name must be set");
    auto ow = std::make_shared<oracle_writer>(owm->get_formatter(),
                                              owm->get_user(),
                                              owm->get_password(),
                                              owm->get_database());
    set_filters(ow, owm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*ow)));
    return ow;
}

std::shared_ptr<memento> oracle_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<oracle_writer_memento>(cfg);
    return mnto;
}

}
