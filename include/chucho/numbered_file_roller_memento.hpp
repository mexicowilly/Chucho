/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_NUMBERED_FILE_ROLLER_MEMENTO_HPP__)
#define CHUCHO_NUMBERED_FILE_ROLLER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/file_roller_memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class numbered_file_roller_memento : public file_roller_memento
{
public:
    numbered_file_roller_memento(configurator& cfg);

    const optional<int>& get_max_index() const;
    const optional<int>& get_min_index() const;

private:
    optional<int> min_index_;
    optional<int> max_index_;
};

inline const optional<int>& numbered_file_roller_memento::get_max_index() const
{
    return max_index_;
}

inline const optional<int>& numbered_file_roller_memento::get_min_index() const
{
    return min_index_;
}

}

#endif
