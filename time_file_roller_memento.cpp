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

#include <chucho/time_file_roller_memento.hpp>

namespace chucho
{

time_file_roller_memento::time_file_roller_memento(configurator& cfg)
    : file_roller_memento(cfg)
{
    set_status_origin("time_file_roller_memento");
    cfg.get_security_policy().set_integer("time_file_roller::max_history", static_cast<size_t>(1), static_cast<size_t>(1000));
    set_handler("file_name_pattern", [this] (const std::string& val) { file_name_pattern_ = validate("time_file_roller::file_name_patter", val); });
    set_handler("max_history", [this] (const std::string& val) { max_history_ = validate("time_file_roller::max_history", std::stoul(validate("time_file_roller::max_history(text)", val))); });
}

}
