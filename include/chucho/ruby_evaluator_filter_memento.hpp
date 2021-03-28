/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_RUBY_EVALUATOR_FILTER_MEMENTO_HPP_)
#define CHUCHO_RUBY_EVALUATOR_FILTER_MEMENTO_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/filter_memento.hpp>

namespace chucho
{

class ruby_evaluator_filter_memento : public filter_memento
{
public:
    ruby_evaluator_filter_memento(configurator& cfg);

    const std::string& get_expression() const;

private:
    std::string expression_;
};

inline const std::string& ruby_evaluator_filter_memento::get_expression() const
{
    return expression_;
}

}

#endif
