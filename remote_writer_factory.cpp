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

#include <chucho/remote_writer_factory.hpp>
#include <chucho/remote_writer_memento.hpp>
#include <chucho/remote_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

remote_writer_factory::remote_writer_factory()
{
    set_status_origin("remote_writer_factory");
}

std::unique_ptr<configurable> remote_writer_factory::create_configurable(const memento& mnto)
{
    auto rwm = dynamic_cast<const remote_writer_memento&>(mnto);
    if (rwm.get_name().empty())
        throw exception("remote_writer_factory: The name is not set");
    if (rwm.get_host().empty())
        throw exception("remote_writer_factory: The writer's host is not set");
    std::size_t uns = rwm.get_unsent_cache_max() ?
        *rwm.get_unsent_cache_max() : remote_writer::DEFAULT_UNSENT_CACHE_MAX;
    std::uint16_t prt = rwm.get_port() ?
        *rwm.get_port() : remote_writer::DEFAULT_PORT;
    auto cnf = std::make_unique<remote_writer>(rwm.get_name(), rwm.get_host(), prt, uns);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> remote_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<remote_writer_memento>(cfg, get_memento_key_set(cfg));
    return std::move(mnto);
}

}
