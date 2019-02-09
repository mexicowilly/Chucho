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

#include <chucho/file_writer_factory.hpp>
#include <chucho/file_writer_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

file_writer_factory::file_writer_factory()
{
    set_status_origin("file_writer_factory");
}

std::unique_ptr<configurable> file_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    std::unique_ptr<configurable> cnf;
    auto fwm = dynamic_cast<file_writer_memento*>(mnto.get());
    assert(fwm != nullptr);
    if (fwm->get_name().empty())
        throw exception("file_writer_factory: The name is not set");
    auto fmt = std::move(fwm->get_formatter());
    if (!fmt)
        throw exception("file_writer_factory: The writer's formatter is not set");
    if (fwm->get_file_name().empty())
        throw exception("file_writer_factory: The file name is not set");
    if (fwm->get_on_start() && fwm->get_flush())
    {
        cnf = std::make_unique<file_writer>(fwm->get_name(),
                                            std::move(fmt),
                                            fwm->get_file_name(),
                                            *fwm->get_on_start(),
                                            *fwm->get_flush());
    }
    else if (fwm->get_flush())
    {
        cnf = std::make_unique<file_writer>(fwm->get_name(),
                                            std::move(fmt),
                                            fwm->get_file_name(),
                                            file_writer::on_start::APPEND,
                                            *fwm->get_flush());
    }
    else if (fwm->get_on_start())
    {
        cnf = std::make_unique<file_writer>(fwm->get_name(),
                                            std::move(fmt),
                                            fwm->get_file_name(),
                                            *fwm->get_on_start());
    }
    else
    {
        cnf = std::make_unique<file_writer>(fwm->get_name(),
                                            std::move(fmt),
                                            fwm->get_file_name());
    }
    set_filters(*cnf, *fwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> file_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<file_writer_memento>(cfg, get_memento_key_set(cfg));
    return std::move(mnto);
}

}
