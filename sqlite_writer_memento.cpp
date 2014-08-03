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

#include <chucho/sqlite_writer_memento.hpp>

namespace chucho
{

sqlite_writer_memento::sqlite_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("sqlite_writer_memento");
    set_handler("file_name", [this] (const std::string& fn) { file_name_ = validate("sqlite_writer::file_name", fn); });
}

}
