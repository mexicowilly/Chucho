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

#include <chucho/level_filter_memento.hpp>
#include <chucho/exception.hpp>
#include <algorithm>

namespace
{

chucho::filter::result text_to_result(const std::string& text)
{
    std::string up;
    std::transform(text.begin(),
                   text.end(),
                   std::back_inserter(up),
                   (int(*)(int))std::toupper);
    if (up == "DENY")
        return chucho::filter::result::DENY;
    if (up == "NEUTRAL")
        return chucho::filter::result::NEUTRAL;
    if (up == "ACCEPT")
        return chucho::filter::result::ACCEPT;
    throw chucho::exception("The text " + text + " does not describe a valid filter result (DENY, NEUTRAL, ACCEPT)");
}

}

namespace chucho
{

level_filter_memento::level_filter_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("level_filter_memento");
    set_handler("level", [this] (const std::string& name) { level_ = level::from_text(name); });
    set_handler("on_match", [this] (const std::string& name) { on_match_ = text_to_result(name); });
    set_handler("on_mismatch", [this] (const std::string& name) { on_mismatch_ = text_to_result(name); });
}

}
