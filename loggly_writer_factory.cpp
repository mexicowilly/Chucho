/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/loggly_writer_factory.hpp>
#include <chucho/loggly_writer_memento.hpp>
#include <chucho/loggly_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

std::unique_ptr<configurable> loggly_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto lwm = dynamic_cast<loggly_writer_memento*>(mnto.get());
    assert(lwm != nullptr);
    if (lwm->get_name().empty())
        throw exception("loggly_writer_factory: The name is not set");
    auto fmt = std::move(lwm->get_formatter());
    if (!fmt)
        throw exception("loggly_writer_factory: The writer's formatter is not set");
    if (lwm->get_token().empty())
        throw exception("loggly_writer_factory: The writer's token is not set");
    auto lw = std::make_unique<loggly_writer>(lwm->get_name(), std::move(fmt), lwm->get_token());
    set_filters(*lw, *lwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*lw)));
    return std::move(lw);
}

std::unique_ptr<memento> loggly_writer_factory::create_memento(configurator& cfg)
{
    std::unique_ptr<memento> mnto = std::make_unique<loggly_writer_memento>(cfg);
    return std::move(mnto);
}

}
