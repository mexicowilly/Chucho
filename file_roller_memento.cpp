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

#include <chucho/file_roller_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

file_roller_memento::file_roller_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("file_roller_memento");
}

void file_roller_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto comp = std::dynamic_pointer_cast<file_compressor>(cnf);
    if (comp)
    {
        compressor_ = comp;
    }
    else
    {
        report_error("A file_roller cannot make use of type " +
            demangle::get_demangled_name(typeid(*cnf)) + ". Only file_compressors can be embedded in the configuration.");
    }
}

}
