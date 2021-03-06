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

#include <chucho/kafka_configuration_factory.hpp>
#include <chucho/kafka_configuration_memento.hpp>
#include <chucho/kafka_configuration.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

kafka_configuration_factory::kafka_configuration_factory()
{
    set_status_origin("kafka_configuration_factory");
}

std::unique_ptr<configurable> kafka_configuration_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto kcm = dynamic_cast<kafka_configuration_memento*>(mnto.get());
    assert(kcm != nullptr);
    auto cnf = std::make_unique<kafka_configuration>(kcm->get_key_values());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> kafka_configuration_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<kafka_configuration_memento>(cfg);
    return std::move(mnto);
}

}
