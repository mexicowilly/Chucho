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

#if !defined(CHUCHO_SQLITE_WRITER_FACTORY_HPP__)
#define CHUCHO_SQLITE_WRITER_FACTORY_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_factory.hpp>

namespace chucho
{

class sqlite_writer_factory : public writer_factory
{
public:
    sqlite_writer_factory();

    virtual std::shared_ptr<configurable> create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif