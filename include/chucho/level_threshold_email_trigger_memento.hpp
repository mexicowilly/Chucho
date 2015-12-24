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

#if !defined(CHUCHO_LEVEL_THRESHOLD_EMAIL_TRIGGER_MEMENTO_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_EMAIL_TRIGGER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/level.hpp>

namespace chucho
{

class level_threshold_email_trigger_memento : public memento
{
public:
    level_threshold_email_trigger_memento(configurator& cfg);

    std::shared_ptr<level> get_level() const;

private:
    std::shared_ptr<level> level_;
};

inline std::shared_ptr<level> level_threshold_email_trigger_memento::get_level() const
{
    return level_;
}

}

#endif
