/*
 * Copyright 2013-2020 Will Mason
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

#include <chucho/kafka_writer_memento.hpp>
#include <chucho/kafka_writer.hpp>
#include <chucho/move_util.hpp>

namespace chucho
{

kafka_writer_memento::kafka_writer_memento(configurator& cfg)
    : message_queue_writer_memento(cfg)
{
    set_status_origin("kafka_writer_memento");
    set_default_name(typeid(kafka_writer));
    set_handler("brokers", [this] (const std::string& val) { brokers_ = validate("kafka_writer::brokers", val); });
    set_handler("topic", [this] (const std::string& val) { topic_ = validate("kafka_writer::topic", val); });
}

void kafka_writer_memento::handle(std::unique_ptr<configurable>&& cnf)
{
    auto kc = dynamic_move<kafka_configuration>(std::move(cnf));
    if (kc)
        kafka_conf_ = std::move(kc);
    else
        message_queue_writer_memento::handle(std::move(cnf));
}

}

