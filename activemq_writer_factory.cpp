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

#include <chucho/activemq_writer_factory.hpp>
#include <chucho/activemq_writer_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

activemq_writer_factory::activemq_writer_factory()
{
    set_status_origin("activemq_writer_factory");
}

std::shared_ptr<configurable> activemq_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto am = std::dynamic_pointer_cast<activemq_writer_memento>(mnto);
    assert(am);
    if (!am->get_formatter())
        throw exception("activemq_writer_factory: The writer's formatter is not set");
    if (!am->get_serializer())
        throw exception("activemq_writer_factory: The writer's serializer is not set");
    if (am->get_broker().empty())
        throw exception("activemq_writer_factory: The broker is not set");
    if (!am->get_consumer_type())
        throw exception("activemq_writer_factory: The consumer type is not set");
    if (am->get_topic_or_queue().empty())
        throw exception("activemq_writer_factory: The topic or queue name is not set");
    auto aw = std::make_shared<activemq_writer>(am->get_formatter(),
                                                am->get_serializer(),
                                                am->get_compressor(),
                                                am->get_broker(),
                                                *am->get_consumer_type(),
                                                am->get_topic_or_queue());
    set_filters(aw, am);
    report_info("Created a " + demangle::get_demangled_name(typeid(*aw)));
    return aw;
}

std::shared_ptr<memento> activemq_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<activemq_writer_memento>(cfg);
    return mnto;
}

}


