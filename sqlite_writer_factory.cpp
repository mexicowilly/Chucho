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

#include <chucho/sqlite_writer_factory.hpp>
#include <chucho/sqlite_writer_memento.hpp>
#include <chucho/sqlite_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

sqlite_writer_factory::sqlite_writer_factory()
{
    set_status_origin("sqlite_writer_factory");
}

std::unique_ptr<configurable> sqlite_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto swm = dynamic_cast<sqlite_writer_memento*>(mnto.get());
    assert(swm != nullptr);
    if (swm->get_name().empty())
        throw exception("sqlite_writer_factory: The name is not set");
    auto fmt = std::move(swm->get_formatter());
    if (!fmt)
        throw exception("sqlite_writer_factory: The writer's formatter is not set");
    if (swm->get_file_name().empty()) 
        throw exception("sqlite_writer_factory: The SQLite file name must be set");
    auto sw = std::make_unique<sqlite_writer>(swm->get_name(),
                                              std::move(fmt),
                                              swm->get_file_name());
    set_filters(*sw, *swm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*sw)));
    return std::move(sw);
}

std::unique_ptr<memento> sqlite_writer_factory::create_memento(configurator& cfg)
{
    std::unique_ptr<memento> mnto = std::make_unique<sqlite_writer_memento>(cfg);
    return std::move(mnto);
}

}
