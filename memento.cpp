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

#include <chucho/memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <algorithm>
#include <cctype>

namespace chucho
{

memento::memento(const configurator& cfg)
    : cfg_(cfg)
{
    set_status_origin("memento");
}

bool memento::boolean_value(const std::string& value)
{
    static const std::string zero("0");
    static const std::string falso("false");

    std::string low = to_lower(value);
    return (low.empty() || low == zero || low == falso) ? false : true;
}

void memento::handle(const std::string& key, const std::string& value)
{
    auto found = handlers_.find(key);
    if (found == handlers_.end())
        report_error("The key " + key + " is not supported with " + get_status_origin());
    else
        found->second(value);
}

void memento::handle(std::shared_ptr<configurable> cnf)
{
    throw exception(demangle::get_demangled_name(typeid(*this)) + " cannot handle a type of " +
        demangle::get_demangled_name(typeid(*cnf)));
}

void memento::set_alias(const std::string& key, const std::string& alias)
{
    auto found = handlers_.find(key);
    if (found == handlers_.end())
        unconnected_aliases_.emplace(key, alias);
    else
        handlers_[key] = found->second;
}

void memento::set_handler(const std::string& key, handler hand)
{
    handlers_[key] = hand;
    auto range = unconnected_aliases_.equal_range(key);
    while (range.first != range.second)
    {
        handlers_[range.first->second] = hand;
        ++range.first;
    }
    unconnected_aliases_.erase(key);
}

std::string memento::to_lower(const std::string& value) const
{
    std::string low;
    std::transform(value.begin(),
                   value.end(),
                   std::back_inserter(low),
                   [] (char c) { return std::tolower(c); });
    return low;
}

}
