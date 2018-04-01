/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_ACTIVEMQ_WRITER_MEMENTO_HPP__)
#define CHUCHO_ACTIVEMQ_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/message_queue_writer_memento.hpp>
#include <chucho/activemq_writer.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class activemq_writer_memento : public message_queue_writer_memento
{
public:
    activemq_writer_memento(configurator& cfg);

    const std::string& get_broker() const;
    const optional<activemq_writer::consumer_type>& get_consumer_type() const;
    const std::string& get_topic_or_queue() const;

private:
    void set_topic_or_queue(const std::string& val);

    std::string broker_;
    optional<activemq_writer::consumer_type> type_;
    std::string topic_or_queue_;
};

inline const std::string& activemq_writer_memento::get_broker() const
{
    return broker_;
}

inline const optional<activemq_writer::consumer_type>& activemq_writer_memento::get_consumer_type() const
{
    return type_;
}

inline const std::string& activemq_writer_memento::get_topic_or_queue() const
{
    return topic_or_queue_;
}

}

#endif
