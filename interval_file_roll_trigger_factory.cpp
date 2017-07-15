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

#include <chucho/interval_file_roll_trigger_factory.hpp>
#include <chucho/interval_file_roll_trigger_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

interval_file_roll_trigger_factory::interval_file_roll_trigger_factory()
{
    set_status_origin("interval_file_roll_trigger_factory");
}

std::shared_ptr<configurable> interval_file_roll_trigger_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto ifrtm = std::dynamic_pointer_cast<interval_file_roll_trigger_memento>(mnto);
    assert(ifrtm);
    if (!ifrtm->get_period() || ifrtm->get_count())
        throw exception(get_status_origin() + ": The \"every\" field of the configuration must be set");
    std::shared_ptr<configurable> cnf = std::make_shared<interval_file_roll_trigger>(*ifrtm->get_period(),
                                                                                     *ifrtm->get_count());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> interval_file_roll_trigger_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<interval_file_roll_trigger_memento>(cfg);
    return mnto;
}

}
