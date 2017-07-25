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

#include <chucho/level_threshold_email_trigger_factory.hpp>
#include <chucho/level_threshold_email_trigger_memento.hpp>
#include <chucho/level_threshold_email_trigger.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

level_threshold_email_trigger_factory::level_threshold_email_trigger_factory()
{
    set_status_origin("level_threshold_email_trigger_factory");
}

std::shared_ptr<configurable> level_threshold_email_trigger_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    assert(dynamic_cast<level_threshold_email_trigger_memento*>(mnto.get()));
    auto ltetm = std::dynamic_pointer_cast<level_threshold_email_trigger_memento>(mnto);
    assert(ltetm);
    if (!ltetm->get_level())
        throw exception("level_threshold_email_trigger_factory: The level must be set");
    std::shared_ptr<configurable> cnf(new level_threshold_email_trigger(ltetm->get_level()));
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> level_threshold_email_trigger_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<level_threshold_email_trigger_memento>(cfg);
    return mnto;
}

}
