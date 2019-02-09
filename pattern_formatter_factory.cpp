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

#include <chucho/pattern_formatter_factory.hpp>
#include <chucho/pattern_formatter_memento.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

pattern_formatter_factory::pattern_formatter_factory()
{
    set_status_origin("pattern_formatter_factory");
}

std::unique_ptr<configurable> pattern_formatter_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto pfm = dynamic_cast<pattern_formatter_memento*>(mnto.get());
    assert(pfm != nullptr);
    if (pfm->get_pattern().empty())
        throw exception("pattern_formatter_factory: The pattern must be set");
    auto cnf = std::make_unique<pattern_formatter>(pfm->get_pattern());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> pattern_formatter_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<pattern_formatter_memento>(cfg);
    return std::move(mnto);
}

}
