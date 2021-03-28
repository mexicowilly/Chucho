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

#if !defined(CHUCHO_JSON_CONFIGURATOR_HPP_)
#define CHUCHO_JSON_CONFIGURATOR_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/configurator.hpp>
#include <chucho/configurable.hpp>
#include <chucho/configurable_factory.hpp>
#include <cJSON.h>
#include <vector>

namespace chucho
{

class CHUCHO_PRIV_EXPORT json_configurator : public configurator
{
public:
    json_configurator(security_policy& sec_pol);

    virtual void configure(std::istream &in) override;

private:
    std::unique_ptr<configurable> create_subobject(const cJSON* json,
                                                   std::unique_ptr<configurable_factory>& fact);
    std::unique_ptr<configurable_factory>& get_factory(const char* const str);
    std::string value_to_text(const cJSON* json);
};

}

#endif
