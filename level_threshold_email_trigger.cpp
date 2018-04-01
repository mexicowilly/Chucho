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

#include <chucho/level_threshold_email_trigger.hpp>

namespace chucho
{

level_threshold_email_trigger::level_threshold_email_trigger(std::shared_ptr<level> lvl)
    : level_(lvl)
{
    set_status_origin("level_threshold_email_trigger");
    if (!level_)
        throw std::invalid_argument("The level is unset");
}

bool level_threshold_email_trigger::is_triggered(const event& evt)
{
    return level_ && *evt.get_level() >= *level_;
}

}

