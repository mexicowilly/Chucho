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

#if !defined(CHUCHO_KAFKA_CONFIGURATION_MEMENTO_HPP__)
#define CHUCHO_KAFKA_CONFIGURATION_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <map>
#include <string>

namespace chucho
{

class kafka_configuration_memento : public memento
{
public:
    kafka_configuration_memento(configurator& cfg);

    const std::map<std::string, std::string>& get_key_values() const;

protected:
    virtual void default_handler(const std::string& key, const std::string& value) override;

private:
    std::map <std::string, std::string> key_values_;
};

inline const std::map<std::string, std::string>& kafka_configuration_memento::get_key_values() const
{
    return key_values_;
}

}

#endif
