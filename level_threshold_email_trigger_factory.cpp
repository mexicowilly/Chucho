/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/level_threshold_email_trigger_factory.hpp>
#include <chucho/level_threshold_email_trigger_memento.hpp>
#include <chucho/level_threshold_email_trigger.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

level_threshold_email_trigger_factory::level_threshold_email_trigger_factory()
{
    set_status_origin("level_threshold_email_trigger_factory");
}

std::unique_ptr<configurable> level_threshold_email_trigger_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto ltetm = dynamic_cast<level_threshold_email_trigger_memento*>(mnto.get());
    if (!ltetm->get_level())
        throw exception("level_threshold_email_trigger_factory: The level must be set");
    auto cnf = std::make_unique<level_threshold_email_trigger>(ltetm->get_level());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> level_threshold_email_trigger_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<level_threshold_email_trigger_memento>(cfg);
    return std::move(mnto);
}

}
