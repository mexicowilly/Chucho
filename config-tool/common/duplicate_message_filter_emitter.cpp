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

#include "duplicate_message_filter_emitter.hpp"

namespace chucho
{

namespace config_tool
{

duplicate_message_filter_emitter::duplicate_message_filter_emitter(const properties& props)
    : filter_emitter(props)
{
}

void duplicate_message_filter_emitter::emit(std::ostream& stream, std::size_t shifts)
{
    indent(stream, shifts) << "- chucho::duplicate_message_filter" << std::endl;
}

}

}