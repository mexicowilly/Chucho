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

#include <chucho/ruby_evaluator_filter_factory.hpp>
#include <chucho/ruby_evaluator_filter_memento.hpp>
#include <chucho/ruby_evaluator_filter.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

ruby_evaluator_filter_factory::ruby_evaluator_filter_factory()
{
    set_status_origin("ruby_evaluator_filter_factory");
}

std::unique_ptr<configurable> ruby_evaluator_filter_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto refm = dynamic_cast<const ruby_evaluator_filter&>(mnto);
    if (refm.get_name().empty())
        throw exception("ruby_evaluator_filter_factory: The name must be set");
    if (refm.get_expression().empty())
        throw exception("ruby_evaluator_filter_factory: The expression must be set");
    auto cnf = std::make_unique<ruby_evaluator_filter>(refm.get_name(), refm.get_expression());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> ruby_evaluator_filter_factory::create_memento(configurator& cfg)
{
    auto  mnto = std::make_unique<ruby_evaluator_filter_memento>(cfg);
    return std::move(mnto);
}

}
