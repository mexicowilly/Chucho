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

#if !defined(CHUCHO_SIZE_FILE_ROLL_TRIGGER_MEMENTO_HPP__)
#define CHUCHO_SIZE_FILE_ROLL_TRIGGER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>
#include <cstdint>

namespace chucho
{

class size_file_roll_trigger_memento : public memento
{
public:
    size_file_roll_trigger_memento(configurator& cfg);

    const optional<std::uintmax_t>& get_max_size() const;

private:
    optional<std::uintmax_t> max_size_;
};

inline const optional<std::uintmax_t>& size_file_roll_trigger_memento::get_max_size() const
{
    return max_size_;
}

}

#endif
