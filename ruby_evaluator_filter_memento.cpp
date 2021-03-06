/*
 * Copyright 2013-2021 Will Mason
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

#include <chucho/ruby_evaluator_filter_memento.hpp>
#include <chucho/ruby_evaluator_filter.hpp>

namespace chucho
{

ruby_evaluator_filter_memento::ruby_evaluator_filter_memento(configurator& cfg)
    : filter_memento(cfg)
{
    set_status_origin("ruby_evaluator_filter_memento");
    set_default_name(typeid(ruby_evaluator_filter));
    set_handler("expression", [this] (const std::string& val) { expression_ = validate("ruby_evaluator_filter::expression", val); });
}

}
