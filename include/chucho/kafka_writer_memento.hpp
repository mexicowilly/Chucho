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

#if !defined(CHUCHO_KAFKA_WRITER_MEMENTO_HPP_)
#define CHUCHO_KAFKA_WRITER_MEMENTO_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/message_queue_writer_memento.hpp>
#include <chucho/kafka_configuration.hpp>

namespace chucho
{

class kafka_writer_memento : public message_queue_writer_memento
{
public:
    kafka_writer_memento(configurator& cfg);
    virtual void handle(std::unique_ptr<configurable>&& cnf) override;

    const std::string& get_brokers() const;
    std::unique_ptr<kafka_configuration> get_kafka_config();
    const std::string& get_topic() const;

private:
    std::unique_ptr<kafka_configuration> kafka_conf_;
    std::string brokers_;
    std::string topic_;
};

inline const std::string& kafka_writer_memento::get_brokers() const
{
    return brokers_;
}

inline std::unique_ptr<kafka_configuration> kafka_writer_memento::get_kafka_config()
{
    return std::move(kafka_conf_);
}

inline const std::string& kafka_writer_memento::get_topic() const
{
    return topic_;
}

}

#endif
