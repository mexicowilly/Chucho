/*
 * Copyright 2013-2021 Will Mason
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

#if !defined(CHUCHO_RABBITMQ_WRITER_MEMENTO_HPP_)
#define CHUCHO_RABBITMQ_WRITER_MEMENTO_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/message_queue_writer_memento.hpp>

namespace chucho
{

class rabbitmq_writer_memento : public message_queue_writer_memento
{
public:
    rabbitmq_writer_memento(configurator& cfg);

    const std::string& get_exchange() const;
    const std::string& get_routing_key() const;
    const std::string& get_url() const;

private:
    std::string url_;
    std::string exchange_;
    std::string routing_key_;
};

inline const std::string& rabbitmq_writer_memento::get_exchange() const
{
    return exchange_;
}

inline const std::string& rabbitmq_writer_memento::get_routing_key() const
{
    return routing_key_;
}

inline const std::string& rabbitmq_writer_memento::get_url() const
{
    return url_;
}

}

#endif
