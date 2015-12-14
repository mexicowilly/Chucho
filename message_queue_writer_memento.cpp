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

#include <chucho/message_queue_writer_memento.hpp>
#include <chucho/demangle.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

message_queue_writer_memento::message_queue_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("message_queue_writer_memento");
}

void message_queue_writer_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto ser = std::dynamic_pointer_cast<serializer>(cnf);
    if (ser)
        serializer_ = ser;
    else
        writer_memento::handle(cnf);
}

}
