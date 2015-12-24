/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/numbered_file_roller_memento.hpp>

namespace chucho
{

numbered_file_roller_memento::numbered_file_roller_memento(configurator& cfg)
    : file_roller_memento(cfg)
{
    set_status_origin("numbered_file_roller_memento");
    cfg.get_security_policy().set_integer("numbered_file_roller::min_index", -1000, 1000);
    cfg.get_security_policy().set_text("numbered_file_roller::min_index(text)", 5);
    cfg.get_security_policy().set_integer("numbered_file_roller::max_index", -1000, 1000);
    cfg.get_security_policy().set_text("numbered_file_roller::max_index(text)", 5);
    set_handler("min_index", [this](const std::string& idx) {min_index_ = validate("numbered_file_roller::min_index", std::stoi(validate("numbered_file_roller::min_index(text)", idx))); });
    set_handler("max_index", [this](const std::string& idx) {max_index_ = validate("numbered_file_roller::max_index", std::stoi(validate("numbered_file_roller::max_index(text)", idx))); }); 
}

}
