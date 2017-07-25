/*
 * Copyright 2013-2017 Will Mason
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

#include <chucho/activemq_writer_memento.hpp>
#include <chucho/text_util.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

activemq_writer_memento::activemq_writer_memento(configurator& cfg)
    : message_queue_writer_memento(cfg)
{
    set_status_origin("activemq_writer_memento");
    set_handler("broker", [this] (const std::string& val) { broker_ = validate("activemq_writer::broker", val); });
    set_handler("topic_or_queue", [this] (const std::string& val) { topic_or_queue_ = validate("activemq_writer::topic_or_queue", val); });
    set_handler("consumer_type", std::bind(&activemq_writer_memento::set_topic_or_queue, this, std::placeholders::_1));
}

void activemq_writer_memento::set_topic_or_queue(const std::string& val)
{
    auto low = text_util::to_lower(validate("activemq_writer::consumer_type", val));
    if (low == "topic")
        type_ = activemq_writer::consumer_type::TOPIC;
    else if (low == "queue")
        type_ = activemq_writer::consumer_type::QUEUE;
    else
        throw exception("Only \"topic\" and \"queue\" are valid ActiveMQ consumer types. Found " + val);
}

}
