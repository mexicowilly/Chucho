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

#include <chucho/postgres_writer_factory.hpp>
#include <chucho/postgres_writer_memento.hpp>
#include <chucho/postgres_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

postgres_writer_factory::postgres_writer_factory()
{
    set_status_origin("postgres_writer_factory");
}

std::shared_ptr<configurable> postgres_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto pwm = std::dynamic_pointer_cast<postgres_writer_memento>(mnto);
    assert(pwm);
    if (!pwm->get_formatter())
        throw exception("postgres_writer_factory: The writer's formatter is not set");
    if (pwm->get_uri().empty()) 
        throw exception("postgres_writer_factory: The Postgres URI must be set");
    auto pw = std::make_shared<postgres_writer>(pwm->get_formatter(), pwm->get_uri());
    set_filters(pw, pwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*pw)));
    return pw;
}

std::shared_ptr<memento> postgres_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<postgres_writer_memento>(cfg);
    return mnto;
}

}
