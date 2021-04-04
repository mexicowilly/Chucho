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

#include <chucho/pipe_writer_factory.hpp>
#include <chucho/pipe_writer_memento.hpp>
#include <chucho/pipe_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

pipe_writer_factory::pipe_writer_factory()
{
    set_status_origin("pipe_writer_factory");
}

std::unique_ptr<configurable> pipe_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto pwm = dynamic_cast<pipe_writer_memento*>(mnto.get());
    assert(pwm != nullptr);
    if (pwm->get_name().empty())
        throw exception("pipe_writer_factory: The name is not set");
    auto fmt = std::move(pwm->get_formatter());
    if (!fmt)
        throw exception("pipe_writer_factory: The writer's formatter is not set");
    std::unique_ptr<configurable> cnf;
    if (pwm->get_flush())
    {
        cnf = std::make_unique<pipe_writer>(pwm->get_name(),
                                            std::move(fmt),
                                            *pwm->get_flush());
    }
    else
    {
        cnf = std::make_unique<pipe_writer>(pwm->get_name(), std::move(fmt));
    }
    set_filters(*cnf, *pwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> pipe_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<pipe_writer_memento>(cfg);
    return std::move(mnto);
}

}
