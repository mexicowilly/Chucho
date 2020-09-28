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

#include <chucho/size_file_roll_trigger_memento.hpp>
#include <chucho/text_util.hpp>

namespace chucho
{

size_file_roll_trigger_memento::size_file_roll_trigger_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("size_file_roll_trigger_memento");
    cfg.get_security_policy().set_integer("size_file_roll_trigger::max_size", static_cast<std::uintmax_t>(1), static_cast<std::uintmax_t>(1024 * 1024 * 1024));
    cfg.get_security_policy().set_text("size_file_roll_trigger::max_size(text)", 50);
    set_handler("max_size", [this] (const std::string& s) { max_size_ = validate("size_file_roll_trigger::max_size",
         text_util::parse_byte_size(validate("size_file_roll_trigger::max_size(text)", s))); });
}

}
