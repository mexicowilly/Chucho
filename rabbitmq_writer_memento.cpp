/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/rabbitmq_writer_memento.hpp>
#include <chucho/text_util.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

rabbitmq_writer_memento::rabbitmq_writer_memento(configurator& cfg)
    : message_queue_writer_memento(cfg)
{
    set_status_origin("rabbitmq_writer_memento");
    set_handler("url", [this] (const std::string& val) { url_ = validate("rabbitmq_writer::url", val); });
    set_handler("exchange", [this] (const std::string& val) { exchange_ = validate("rabbitmq_writer::exchange", val); });
    set_handler("routing_key", [this] (const std::string& val) { routing_key_ = validate("rabbitmq_writer::routing_key", val); });
}

}