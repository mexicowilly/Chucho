/*
 * Copyright 2013-2015 Will Mason
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

#include <chucho/zeromq_writer_factory.hpp>
#include <chucho/zeromq_writer_memento.hpp>
#include <chucho/zeromq_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

zeromq_writer_factory::zeromq_writer_factory()
{
    set_status_origin("zeromq_writer_factory");
}

std::shared_ptr<configurable> zeromq_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto zm = std::dynamic_pointer_cast<zeromq_writer_memento>(mnto);
    assert(zm);
    if (!zm->get_formatter())
        throw exception("zeromq_writer_factory: The writer's formatter is not set");
    if (!zm->get_serializer())
        throw exception("zeromq_writer_factory: The writer's serializer is not set");
    if (zm->get_endpoint().empty())
        throw exception("zeromq_writer_factory: The endpoint is not set");
    auto zw = std::make_shared<zeromq_writer>(zm->get_formatter(),
                                                            zm->get_serializer(),
                                                            zm->get_endpoint(),
                                                            zm->get_prefix());
    set_filters(zw, zm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*zw)));
    return zw;
}

std::shared_ptr<memento> zeromq_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<zeromq_writer_memento>(cfg);
    return mnto;
}

}

