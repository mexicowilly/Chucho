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

#include <chucho/formatted_message_serializer.hpp>
#include <chucho/line_ending.hpp>

namespace chucho
{

std::vector<std::uint8_t> formatted_message_serializer::finish_blob()
{
    auto result(std::vector<std::uint8_t>(events_.begin(), events_.end()));
    events_.clear();
    return result;
}

void formatted_message_serializer::serialize(const event& evt, formatter& fmt)
{
    if (!events_.empty())
        events_ += line_ending::EOL;
    events_ += fmt.format(evt);
}

}
