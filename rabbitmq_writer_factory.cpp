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

#include <chucho/rabbitmq_writer_factory.hpp>
#include <chucho/rabbitmq_writer_memento.hpp>
#include <chucho/rabbitmq_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

rabbitmq_writer_factory::rabbitmq_writer_factory()
{
    set_status_origin("rabbitmq_writer_factory");
}

std::unique_ptr<configurable> rabbitmq_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto am = dynamic_cast<rabbitmq_writer_memento*>(mnto.get());
    assert(am != nullptr);
    if (am->get_name().empty())
        throw exception("rabbitmq_writer_factory: The name is not set");
    auto fmt = std::move(am->get_formatter());
    if (!fmt)
        throw exception("rabbitmq_writer_factory: The writer's formatter is not set");
    auto ser = std::move(am->get_serializer());
    if (!ser)
        throw exception("rabbitmq_writer_factory: The writer's serializer is not set");
    if (am->get_url().empty())
        throw exception("rabbitmq_writer_factory: The URL is not set");
    optional<std::string> rk;
    if (!am->get_routing_key().empty())
        rk = am->get_routing_key();
    auto aw = std::make_unique<rabbitmq_writer>(am->get_name(),
                                                std::move(fmt),
                                                std::move(ser),
                                                am->get_coalesce_max(),
                                                std::move(am->get_compressor()),
                                                am->get_url(),
                                                am->get_exchange(),
                                                rk);
    set_filters(*aw, *am);
    report_info("Created a " + demangle::get_demangled_name(typeid(*aw)));
    return std::move(aw);
}

std::unique_ptr<memento> rabbitmq_writer_factory::create_memento(configurator& cfg)
{
    std::unique_ptr<memento> mnto = std::make_unique<rabbitmq_writer_memento>(cfg);
    return std::move(mnto);
}

}
