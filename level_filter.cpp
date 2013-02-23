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

#include <chucho/level_filter.hpp>

namespace chucho
{

level_filter::level_filter(std::shared_ptr<level>lvl,
                           result on_match,
                           result on_mismatch)
    : level_(lvl),
      on_match_(on_match),
      on_mismatch_(on_mismatch)
{
}

filter::result level_filter::evaluate(const event& evt)
{
    return (*evt.get_level() == *level_) ? on_match_ : on_mismatch_;
}

}
