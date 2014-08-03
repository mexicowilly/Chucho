/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/size_file_roll_trigger_factory.hpp>
#include <chucho/size_file_roll_trigger_memento.hpp>
#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

size_file_roll_trigger_factory::size_file_roll_trigger_factory()
{
    set_status_origin("size_file_roll_trigger_factory");
}

std::shared_ptr<configurable> size_file_roll_trigger_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto sfrtm = std::dynamic_pointer_cast<size_file_roll_trigger_memento>(mnto);
    assert(sfrtm);
    if (!sfrtm->get_max_size())
        throw exception("size_file_roll_trigger_factory: The max_size field must be set to create a size_file_roll_trigger");
    std::shared_ptr<configurable> cnf(new size_file_roll_trigger(*sfrtm->get_max_size()));
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> size_file_roll_trigger_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto(new size_file_roll_trigger_memento(cfg));
    return mnto;
}

}
