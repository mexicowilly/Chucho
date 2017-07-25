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

#if !defined(CHUCHO_LOGGER_MEMENTO_HPP__)
#define CHUCHO_LOGGER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/level.hpp>
#include <chucho/writer.hpp>
#include <chucho/optional.hpp>
#include <vector>

namespace chucho
{

class logger_memento : public memento
{
public:
    logger_memento(configurator& cfg);

    std::shared_ptr<level> get_level() const;
    const optional<std::string>& get_name() const;
    const std::vector<std::shared_ptr<writer>>& get_writers() const;
    const optional<bool>& get_writes_to_ancestors() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    optional<std::string> name_;
    std::shared_ptr<level> level_;
    std::vector<std::shared_ptr<writer>> writers_;
    optional<bool> writes_to_ancestors_;
};

inline std::shared_ptr<level> logger_memento::get_level() const
{
    return level_;
}

inline const optional<std::string>& logger_memento::get_name() const
{
    return name_;
}

inline const std::vector<std::shared_ptr<writer>>& logger_memento::get_writers() const
{
    return writers_;
}

inline const optional<bool>& logger_memento::get_writes_to_ancestors() const
{
    return writes_to_ancestors_;
}

}

#endif
