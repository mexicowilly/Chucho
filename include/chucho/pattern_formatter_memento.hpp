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

#if !defined(CHUCHO_PATTERN_FORMATTER_MEMENTO_HPP__)
#define CHUCHO_PATTERN_FORMATTER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>

namespace chucho
{

class pattern_formatter_memento : public memento
{
public:
    pattern_formatter_memento(const configurator& cfg);

    const std::string& get_pattern() const;

private:
    std::string pattern_;
};

inline const std::string& pattern_formatter_memento::get_pattern() const
{
    return pattern_;
}

}

#endif
