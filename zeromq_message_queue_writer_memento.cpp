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

#include <chucho/zeromq_message_queue_writer_memento.hpp>
#include <cstring>

namespace chucho
{

zeromq_message_queue_writer_memento::zeromq_message_queue_writer_memento(configurator& cfg)
    : message_queue_writer_memento(cfg)
{
    set_status_origin("zeromq_message_queue_writer_memento");
    set_handler("endpoint", [this] (const std::string& ep) { endpoint_ = validate("zeromq_message_queue_writer::endpoint", ep); });
    set_handler("prefix", [this] (const std::string& arg)
                {
                    std::string pf = validate("zeromq_message_queue_writer::prefix", arg);
                    prefix_.resize(pf.length());
                    std::memcpy(&prefix_[0], pf.data(), pf.length());
                });
}

}

