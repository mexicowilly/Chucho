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

#include <chucho/duplicate_message_filter_factory.hpp>
#include <chucho/duplicate_message_filter.hpp>
#include <chucho/filter_memento.hpp>
#include <chucho/demangle.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

duplicate_message_filter_factory::duplicate_message_filter_factory()
{
    set_status_origin("duplicate_message_filter_factory");
}

std::unique_ptr<configurable> duplicate_message_filter_factory::create_configurable(const memento& mnto)
{
    auto m = dynamic_cast<const filter_memento&>(mnto);
    if (m.get_name().empty())
        throw exception("duplicate_message_filter_factory: The filter's name is not set");
    auto cnf = std::make_unique<duplicate_message_filter>(m.get_name());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> duplicate_message_filter_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<memento>(cfg);
    return std::move(mnto);
}

}
