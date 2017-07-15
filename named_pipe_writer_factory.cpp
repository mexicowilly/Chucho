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

#include <chucho/named_pipe_writer_factory.hpp>
#include <chucho/named_pipe_writer_memento.hpp>
#include <chucho/named_pipe_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

named_pipe_writer_factory::named_pipe_writer_factory()
{
    set_status_origin("named_pipe_writer_factory");
}

std::shared_ptr<configurable> named_pipe_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    std::shared_ptr<configurable> cnf;
    auto npwm = std::dynamic_pointer_cast<named_pipe_writer_memento>(mnto);
    assert(npwm);
    if (!npwm->get_formatter())
        throw exception("named_pipe_writer_factory: The writer's formatter is not set");
    if (npwm->get_name().empty())
        throw exception("named_pipe_writer_factory: The pipe's name must be set");
    if (npwm->get_flush())
    {
        cnf.reset(new named_pipe_writer(npwm->get_formatter(),
                                        npwm->get_name(),
                                        *npwm->get_flush()));
    }
    else
    {
        cnf.reset(new named_pipe_writer(npwm->get_formatter(),
                                        npwm->get_name()));
    }
    set_filters(cnf, npwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> named_pipe_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<named_pipe_writer_memento>(cfg);
    return mnto;
}
}
