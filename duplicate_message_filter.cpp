/*
 * Copyright 2013-2021 Will Mason
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

duplicate_message_filter::duplicate_message_filter(const std::string& name)
    : writeable_filter(name),
      count_(0)
{
}

duplicate_message_filter::duplicate_message_filter(const std::string& name, writer& wrt)
    : writeable_filter(name, wrt),
      count_(0)
{
}

filter::result duplicate_message_filter::evaluate(const event& evt)
{
    result res = result::NEUTRAL;
    if (!has_writer())
        throw std::runtime_error("No writer has been set in the duplicate_message_filter '" + get_name() + "'");
    auto fmsg = get_writer().get_formatter().format(evt);
    if (count_ > 0 && fmsg == message_)
    {
        count_++;
        res = result::DENY;
    }
    else
    {
        if (count_ > 1)
        {
            event levt(evt.get_logger(),
                       evt.get_level(),
                       "The last message was logged " + std::to_string(count_) + " times in a row",
                       evt.get_file_name(),
                       evt.get_line_number(),
                       evt.get_function_name(),
                       evt.get_marker());
            write(levt);
        }
        message_ = fmsg;
        count_ = 1;
    }
    return res;
}

}
