/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/kafka_writer_factory.hpp>
#include <chucho/kafka_writer_memento.hpp>
#include <chucho/kafka_writer.hpp>
#include <chucho/demangle.hpp>
#include <librdkafka/rdkafka.h>
#include <cassert>

namespace chucho
{

kafka_writer_factory::kafka_writer_factory()
{
    set_status_origin("kafka_writer_factory");
}

std::unique_ptr<configurable> kafka_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto kwm = dynamic_cast<kafka_writer_memento*>(mnto.get());
    assert(kwm != nullptr);
    if (kwm->get_name().empty())
        throw exception("kafka_writer_factory: The name is not set");
    auto fmt = std::move(kwm->get_formatter());
    if (!fmt)
        throw exception("kafka_writer_factory: The writer's formatter is not set");
    auto ser = std::move(kwm->get_serializer());
    if (!ser)
        throw exception("kafka_writer_factory: The writer's serializer is not set");
    if (kwm->get_topic().empty())
        throw exception("kafka_writer_factory: The writer's topic is not set");
    auto conf = kwm->get_kafka_config();
    rd_kafka_conf_t* raw_conf = conf ? conf->get_conf() : rd_kafka_conf_new();
    char msg[1024];
    if (!kwm->get_brokers().empty())
    {
        auto rc = rd_kafka_conf_set(raw_conf, "bootstrap.servers", kwm->get_brokers().c_str(), msg, sizeof(msg));
        if (rc != RD_KAFKA_CONF_OK)
            throw exception("kafka_writer_factory: Error setting Kafka configuration (bootstrap_servers, " + kwm->get_brokers() + "): " + msg);
    }
    std::size_t len;
    if (rd_kafka_conf_get(raw_conf, "bootstrap.servers", nullptr, &len) != RD_KAFKA_CONF_OK)
        throw exception("kafka_writer_factory: The writer's brokers are not set");
    auto kw = std::make_unique<kafka_writer>(kwm->get_name(),
                                             std::move(fmt),
                                             std::move(ser),
                                             kwm->get_topic(),
                                             raw_conf);
    set_filters(*kw, *kwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*kw)));
    return std::move(kw);
}

std::unique_ptr<memento> kafka_writer_factory::create_memento(configurator& cfg)
{
    std::unique_ptr<memento> mnto = std::make_unique<kafka_writer_memento>(cfg);
    return std::move(mnto);
}

}
