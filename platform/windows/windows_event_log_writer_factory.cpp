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

#include <chucho/windows_event_log_writer_factory.hpp>
#include <chucho/windows_event_log_writer.hpp>
#include <chucho/windows_event_log_writer_memento.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

windows_event_log_writer_factory::windows_event_log_writer_factory()
{
    set_status_origin("windows_event_log_writer_factory");
}

std::unique_ptr<configurable> windows_event_log_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    std::unique_ptr<configurable> cnf;
    auto welwm = dynamic_cast<windows_event_log_writer_memento*>(mnto.get());
    assert(welwm != nullptr);
    if (welwm->get_name().empty())
        throw exception("windows_event_log_writer_factory: The name is not set");
    if (!welwm->get_formatter())
        throw exception("windows_event_log_writer_factory: The writer's formatter is not set");
    if (welwm->get_log().empty() && welwm->get_host().empty())
    {
        cnf = std::make_unique<windows_event_log_writer>(welwm->get_name(),
                                                         std::move(welwm->get_formatter()),
                                                         welwm->get_source());
    }
    else if (welwm->get_host().empty())
    {
        cnf = std::make_unique<windows_event_log_writer>(welwm->get_name(),
                                                         std::move(welwm->get_formatter()),
                                                         welwm->get_log(),
                                                         welwm->get_source());
    }
    else
    {
        cnf = std::make_unique<windows_event_log_writer>(welwm->get_name(),
                                                         std::move(welwm->get_formatter()),
                                                         welwm->get_log(),
                                                         welwm->get_source(),
                                                         welwm->get_host());
    }
    set_filters(*cnf, *welwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> windows_event_log_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<windows_event_log_writer_memento>(cfg, get_memento_key_set(cfg));
    return std::move(mnto);
}

}
