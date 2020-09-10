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

#if !defined(CHUCHO_KAFKA_CONFIGURATION_HPP__)
#define CHUCHO_KAFKA_CONFIGURATION_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/configurable.hpp>
#include <librdkafka/rdkafka.h>
#include <map>
#include <string>

namespace chucho
{

class kafka_configuration : public configurable
{
public:
    kafka_configuration(const std::map<std::string, std::string>& key_values);
    ~kafka_configuration();

    rd_kafka_conf_t* get_conf();

private:
    rd_kafka_conf_t* conf_;
};

}

#endif
