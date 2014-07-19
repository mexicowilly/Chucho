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

#include <chucho/sliding_numbered_file_roller_memento.hpp>

namespace chucho
{

sliding_numbered_file_roller_memento::sliding_numbered_file_roller_memento(const configurator& cfg)
    : file_roller_memento(cfg)
{
    set_status_origin("sliding_numbered_file_roller_memento");
    set_handler("min_index", [this] (const std::string& idx) { min_index_ = std::stoi(idx); });
    set_handler("max_count", [this] (const std::string& cnt) { max_count_ = std::stoul(cnt); });
}

}
