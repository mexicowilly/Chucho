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

#include <chucho/remote_writer_memento.hpp>

namespace chucho
{

remote_writer_memento::remote_writer_memento(const configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("remote_writer_memento");
    set_handler("host", [this] (const std::string& name) { host_ = name; });
    set_handler("port", [this] (const std::string& port) { port_ = std::stoul(port); });
    set_handler("unsent_cache_max", [this] (const std::string& sz) { unsent_cache_max_ = stoull(sz); });
}

}
