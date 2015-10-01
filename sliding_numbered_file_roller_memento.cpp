/*
 * Copyright 2013-2015 Will Mason
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

#include <chucho/sliding_numbered_file_roller_memento.hpp>

namespace chucho
{

sliding_numbered_file_roller_memento::sliding_numbered_file_roller_memento(configurator& cfg)
    : file_roller_memento(cfg)
{
    set_status_origin("sliding_numbered_file_roller_memento");
    cfg.get_security_policy().set_integer("sliding_numbered_file_roller::min_index", -1000, 1000);
    cfg.get_security_policy().set_text("sliding_numbered_file_roller::min_index(text)", 5);
    cfg.get_security_policy().set_integer("sliding_numbered_file_roller::max_count", static_cast<std::size_t>(1), static_cast<std::size_t>(1000));
    cfg.get_security_policy().set_text("sliding_numbered_file_roller::max_count(text)", 5);
    set_handler("min_index", [this] (const std::string& idx) { min_index_ = validate("sliding_numbered_file_roller::min_index", std::stoi(validate("sliding_numbered_file_roller::min_index(text)", idx))); });
    set_handler("max_count", [this] (const std::string& cnt) { max_count_ = validate("sliding_numbered_file_roller::max_count", std::stoul(validate("sliding_numbered_file_roller::max_count(text)", cnt))); });
}

}

