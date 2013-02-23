/*
 * Copyright 2013 Will Mason
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

#include <chucho/time_file_roller_factory.hpp>
#include <chucho/time_file_roller_memento.hpp>
#include <chucho/time_file_roller.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

time_file_roller_factory::time_file_roller_factory()
{
    set_status_origin("time_file_roller_factory");
}

std::shared_ptr<configurable> time_file_roller_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto tfrm = std::dynamic_pointer_cast<time_file_roller_memento>(mnto);
    assert(tfrm);
    if (tfrm->get_file_name_pattern().empty())
        throw exception("time_file_roller_factory: The file_name_pattern field must be set");
    if (!tfrm->get_max_history())
        throw exception("time_file_roller_factory: The max_history field must be set");
    std::shared_ptr<configurable> cnf(new time_file_roller(tfrm->get_file_name_pattern(),
                                                           *tfrm->get_max_history()));
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> time_file_roller_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new time_file_roller_memento(cfg));
    return mnto;
}

}
