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

#include <chucho/level_threshold_filter.hpp>
#include <chucho/logger.hpp>
#include <stdexcept>

namespace chucho
{

level_threshold_filter::level_threshold_filter(std::shared_ptr<level> lvl)
    : level_(lvl)
{
    set_status_origin("level_threshold_filter");
    if (!level_)
        throw std::invalid_argument("The level is invalid");
}

filter::result level_threshold_filter::evaluate(const event& evt)
{
    return (!level_ || *evt.get_level() >= *level_) ?
        result::NEUTRAL : result::DENY;
}

}
