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

#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_MEMENTO_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/level.hpp>
#include <chucho/memento_key_set.hpp>

namespace chucho
{

class level_threshold_filter_memento : public memento
{
public:
    level_threshold_filter_memento(configurator& cfg, memento_key_set ks);

    std::shared_ptr<level> get_level() const;

private:
    std::shared_ptr<level> level_;
};

inline std::shared_ptr<level> level_threshold_filter_memento::get_level() const
{
    return level_;
}

}

#endif
