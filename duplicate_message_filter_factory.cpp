/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/duplicate_message_filter_factory.hpp>
#include <chucho/duplicate_message_filter_memento.hpp>
#include <chucho/duplicate_message_filter.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

duplicate_message_filter_factory::duplicate_message_filter_factory()
{
    set_status_origin("duplicate_message_filter_factory");
}

std::shared_ptr<configurable> duplicate_message_filter_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    std::shared_ptr<configurable> cnf(new duplicate_message_filter());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> duplicate_message_filter_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto(new duplicate_message_filter_memento(cfg));
    return mnto;
}

}
