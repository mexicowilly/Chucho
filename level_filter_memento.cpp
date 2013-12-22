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

#include <chucho/level_filter_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/text_util.hpp>
#include <algorithm>

namespace chucho
{

level_filter_memento::level_filter_memento(const configurator& cfg, memento_key_set ks)
    : memento(cfg)
{
    set_status_origin("level_filter_memento");
    handler lvl_hnd = [this] (const std::string& name) { level_ = level::from_text(name); };
    if (ks == memento_key_set::CHUCHO)
    {
        set_handler("level", lvl_hnd);
        set_handler("on_match", [this] (const std::string& name) { on_match_ = text_to_result(name); });
        set_handler("on_mismatch", [this] (const std::string& name) { on_mismatch_ = text_to_result(name); });
    }
    else if (ks == memento_key_set::LOG4CPLUS)
    {
        on_mismatch_ = chucho::filter::result::NEUTRAL;
        on_match_ = chucho::filter::result::DENY;
        set_handler("LogLevelToMatch", lvl_hnd);
        set_handler("AcceptOnMatch", [this] (const std::string& val) { on_match_ = boolean_value(val) ? chucho::filter::result::ACCEPT : chucho::filter::result::DENY; });
    }
}

filter::result level_filter_memento::text_to_result(const std::string& text) const
{
    std::string low = text_util::to_lower(text);
    if (low == "deny")
        return chucho::filter::result::DENY;
    if (low == "neutral")
        return chucho::filter::result::NEUTRAL;
    if (low == "accept")
        return chucho::filter::result::ACCEPT;
    throw chucho::exception("The text " + text + " does not describe a valid filter result (DENY, NEUTRAL, ACCEPT)");
}

}
