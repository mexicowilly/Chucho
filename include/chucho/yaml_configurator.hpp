/*
 * Copyright 2013-2014 Will Mason
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

#if !defined(CHUCHO_YAML_CONFIGURATOR_HPP__)
#define CHUCHO_YAML_CONFIGURATOR_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/configurator.hpp>
#include <yaml.h>

namespace chucho
{

class memento;

class CHUCHO_PRIV_EXPORT yaml_configurator : public configurator
{
public:
    yaml_configurator(const security_policy& sec_pol);

    virtual void configure(std::istream& in) override;

private:
    void extract_variables(yaml_document_t& doc, const yaml_node_t& node);
    void handle(yaml_document_t& doc,
                const yaml_node_t& node,
                int level,
                const std::string& key,
                std::shared_ptr<memento> mnto);
};

}

#endif
