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

#include <chucho/compressing_writer_factory.hpp>
#include <chucho/compressing_writer_memento.hpp>
#include <chucho/compressing_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

compressing_writer_factory::compressing_writer_factory()
{
    set_status_origin("compressing_writer_factory");
}

std::shared_ptr<configurable> compressing_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto cwm = std::dynamic_pointer_cast<compressing_writer_memento>(mnto);
    assert(cwm);
    if (!cwm->get_formatter())
        throw exception("compressing_writer_factory: The writer's formatter is not set");
    if (!cwm->get_compressor())
        throw exception("compressing_writer_factory: The writer's compressor is not set");
    if (!cwm->get_serializer())
        throw exception("compressing_writer_factory: The writer's serializer is not set");
    auto cw = std::make_shared<compressing_writer>(cwm->get_formatter(),
                                                   std::shared_ptr<std::ostream>(),
                                                   cwm->get_compressor(),
                                                   cwm->get_serializer(),
                                                   cwm->get_max_cached_kb(),
                                                   cwm->get_max_cached_events());
    set_filters(cw, cwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cw)));
    return cw;
}

std::shared_ptr<memento> compressing_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<compressing_writer_memento>(cfg);
    return mnto;
}

}