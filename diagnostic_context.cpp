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

#include <chucho/diagnostic_context.hpp>

namespace chucho
{

std::string& diagnostic_context::at(const std::string& key)
{
    return get_map()[key];
}

void diagnostic_context::clear()
{
    get_map().clear();
}

bool diagnostic_context::empty()
{
    return get_map().empty();
}

void diagnostic_context::erase(const std::string& key)
{
    get_map().erase(key);
}

std::map<std::string, std::string> diagnostic_context::get()
{
    return get_map();
}

void diagnostic_context::set(const std::map<std::string, std::string>& ctx)
{
    get_map() = ctx;
}

}

