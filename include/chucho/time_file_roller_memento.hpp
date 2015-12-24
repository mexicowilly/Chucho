/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_TIME_FILE_ROLLER_MEMENTO_HPP__)
#define CHUCHO_TIME_FILE_ROLLER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/file_roller_memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class time_file_roller_memento : public file_roller_memento
{
public:
    time_file_roller_memento(configurator& cfg);

    const std::string& get_file_name_pattern() const;
    const optional<std::size_t>& get_max_history() const;

private:
    std::string file_name_pattern_;
    optional<std::size_t> max_history_;
};

inline const std::string& time_file_roller_memento::get_file_name_pattern() const
{
    return file_name_pattern_;
}

inline const optional<std::size_t>& time_file_roller_memento::get_max_history() const
{
    return max_history_;
}

}

#endif
