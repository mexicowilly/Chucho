/*
 * Copyright 2013 Will Mason
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

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/configurator.hpp>
#include <yaml-cpp/yaml.h>

namespace chucho
{

class memento;

class CHUCHO_EXPORT yaml_configurator : public configurator
{
public:
    yaml_configurator();

    virtual void configure(std::istream& in) override;

private:
    std::map<std::string, std::string> extract_variables(const YAML::Node& n);
    void handle(const std::string& key, const YAML::Node& n, std::shared_ptr<memento> mnto);
};

}

#endif
