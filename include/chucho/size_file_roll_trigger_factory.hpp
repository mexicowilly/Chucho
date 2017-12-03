/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_SIZE_FILE_ROLL_TRIGGER_FACTORY_HPP__)
#define CHUCHO_SIZE_FILE_ROLL_TRIGGER_FACTORY_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/configurable_factory.hpp>

namespace chucho
{

class size_file_roll_trigger_factory : public configurable_factory
{
public:
    size_file_roll_trigger_factory();

    virtual std::unique_ptr<configurable> create_configurable(const memento& mnto) override;
    virtual std::unique_ptr<memento> create_memento(configurator& cfg) override;
};

}

#endif
