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

#include <chucho/level_threshold_filter_memento.hpp>
#include <chucho/level_threshold_filter.hpp>

namespace chucho
{

level_threshold_filter_memento::level_threshold_filter_memento(configurator& cfg, memento_key_set ks)
    : filter_memento(cfg)
{
    set_status_origin("level_threshold_filter_memento");
    set_default_name(typeid(level_threshold_filter));
    handler lvl_hnd = [this](const std::string& name) { level_ = level::from_text(validate("level_threshold_filter::level", name)); };
    if (ks == memento_key_set::CHUCHO)
        set_handler("level", lvl_hnd);
    else if (ks == memento_key_set::LOG4CPLUS)
        set_handler("LogLevelMin", lvl_hnd);
}

}
