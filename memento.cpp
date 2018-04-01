/*
 * Copyright 2013-2018 Will Mason
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
#include <chucho/text_util.hpp>

namespace chucho
{

memento::memento(configurator& cfg)
    : cfg_(cfg)
{
    set_status_origin("memento");
}

memento::~memento()
{
}

bool memento::boolean_value(const std::string& value)
{
    static const std::string zero("0");
    static const std::string falso("false");

    std::string low = text_util::to_lower(value);
    return (low.empty() || low == zero || low == falso) ? false : true;
}

void memento::handle(const std::string& key, const std::string& value)
{
    auto found = handlers_.find(key);
    if (found == handlers_.end())
        report_error("The key " + key + " is not supported with " + demangle::get_demangled_name(typeid(*this)));
    else
        found->second(value);
}

void memento::handle(std::unique_ptr<configurable>&& cnf)
{
    report_error(demangle::get_demangled_name(typeid(*this)) + " cannot handle a type of " +
        demangle::get_demangled_name(typeid(*cnf)));
}

void memento::set_alias(const std::string& key, const std::string& alias)
{
    auto found = handlers_.find(key);
    if (found == handlers_.end())
        unconnected_aliases_.insert(std::make_pair(key, alias));
    else
        handlers_[alias] = found->second;
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

}
