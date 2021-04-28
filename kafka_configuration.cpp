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

#include <chucho/kafka_configuration.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

kafka_configuration::kafka_configuration(const std::map<std::string, std::string>& key_values)
    : conf_(rd_kafka_conf_new())
{
    char msg[1024];
    for (auto& p : key_values)
    {
        auto rc = rd_kafka_conf_set(conf_, p.first.c_str(), p.second.c_str(), msg, sizeof(msg));
        if (rc != RD_KAFKA_CONF_OK)
            throw exception("kafka_writer_factory: Error setting Kafka configuration (" + p.first + ", " + p.second + "): " + msg);
    }
}

kafka_configuration::~kafka_configuration()
{
    if (conf_ != nullptr)
        rd_kafka_conf_destroy(conf_);
}

rd_kafka_conf_t* kafka_configuration::get_conf()
{
    auto local = conf_;
    conf_ = nullptr;
    return local;
}

}
