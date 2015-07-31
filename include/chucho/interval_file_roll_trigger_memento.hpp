
/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_INTERVAL_FILE_ROLL_TRIGGER_MEMENTO_HPP__)
#define CHUCHO_INTERVAL_FILE_ROLL_TRIGGER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>
#include <chucho/interval_file_roll_trigger.hpp>

namespace chucho
{

class interval_file_roll_trigger_memento : public memento
{
public:
    interval_file_roll_trigger_memento(configurator& cfg);

    const optional<std::size_t>& get_count() const;
    const optional<interval_file_roll_trigger::period>& get_period() const;

private:
    void parse_period(const std::string& spec);

    optional<interval_file_roll_trigger::period> period_;
    optional<std::size_t> count_;
};

inline const optional<std::size_t>& interval_file_roll_trigger_memento::get_count() const
{
    return count_;
}

inline const optional<interval_file_roll_trigger::period>& interval_file_roll_trigger_memento::get_period() const
{
    return period_;
}

}

#endif
