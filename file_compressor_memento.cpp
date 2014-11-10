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

#include <chucho/file_compressor_memento.hpp>

namespace chucho
{

file_compressor_memento::file_compressor_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("file_compressor_memento");
    cfg.get_security_policy().set_integer("file_compressor::min_index", 1, 1000);
    set_handler("min_index", [this] (const std::string& idx) { min_index_ = validate("file_compressor::min_index", std::stoul(validate("file_compressor::min_index(text)", idx))); });
}

}

