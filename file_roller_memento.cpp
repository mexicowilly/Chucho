/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/file_roller_memento.hpp>
#include <chucho/demangle.hpp>
#include <chucho/move_util.hpp>

namespace chucho
{

file_roller_memento::file_roller_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("file_roller_memento");
}

void file_roller_memento::handle(std::unique_ptr<configurable>&& cnf)
{
    auto comp = dynamic_move<file_compressor>(std::move(cnf));
    if (comp)
        compressor_ = std::move(comp);
    else
        memento::handle(std::move(cnf));
}

}
