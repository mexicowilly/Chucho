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

#include <chucho/ruby_evaluator_filter_factory.hpp>
#include <chucho/ruby_evaluator_filter_memento.hpp>
#include <chucho/ruby_evaluator_filter.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

ruby_evaluator_filter_factory::ruby_evaluator_filter_factory()
{
    set_status_origin("ruby_evaluator_filter_factory");
}

std::shared_ptr<configurable> ruby_evaluator_filter_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    assert(dynamic_cast<ruby_evaluator_filter_memento*>(mnto.get()));
    auto refm = std::dynamic_pointer_cast<ruby_evaluator_filter_memento>(mnto);
    assert(refm);
    if (refm->get_expression().empty())
        throw exception("ruby_evaluator_filter_factory: The expression must be set");
    std::shared_ptr<configurable> cnf = std::make_shared<ruby_evaluator_filter>(refm->get_expression());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> ruby_evaluator_filter_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<ruby_evaluator_filter_memento>(cfg);
    return mnto;
}

}
