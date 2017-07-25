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

#include <chucho/duplicate_message_filter.hpp>

namespace chucho
{

duplicate_message_filter::duplicate_message_filter()
    : count_(0)
{
}

filter::result duplicate_message_filter::evaluate(const event& evt)
{
    result res = result::NEUTRAL;
    if (count_ > 0 && evt.get_message() == message_)
    {
        count_++;
        res = result::DENY;
    }
    else
    {
        if (count_ > 1)
            report_info("The message \"" + message_ + "\" was logged " + std::to_string(count_) + " times in a row");
        message_ = evt.get_message();
        count_ = 1;
    }
    return res;
}

}
