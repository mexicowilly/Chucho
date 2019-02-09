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

#include <chucho/cerr_writer_factory.hpp>
#include <chucho/cerr_writer.hpp>
#include <chucho/demangle.hpp>
#include <chucho/exception.hpp>
#include <assert.h>

namespace chucho
{

cerr_writer_factory::cerr_writer_factory()
{
    set_status_origin("cerr_writer_factory");
}

std::unique_ptr<configurable> cerr_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto cwm = dynamic_cast<writer_memento*>(mnto.get());
    assert(cwm != nullptr);
    if (cwm->get_name().empty())
        throw exception("cerr_writer_factory: The name is not set");
    auto fmt = std::move(cwm->get_formatter());
    if (!fmt)
        throw exception("cerr_writer_factory: The writer's formatter is not set");
    auto cnf = std::make_unique<cerr_writer>(cwm->get_name(), std::move(fmt));
    set_filters(*cnf, *cwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> cerr_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<writer_memento>(cfg);
    mnto->set_default_name(typeid(cerr_writer));
    return std::move(mnto);
}

}
