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

#include "emitter.hpp"

namespace chucho
{

namespace config_tool
{

emitter::emitter(const properties& props)
    : props_(props),
      spaces_(props_.get_shift_width() * 10, ' ')
{
}

std::ostream& emitter::indent(std::ostream& stream, std::size_t shifts)
{
    std::size_t space_count = props_.get_shift_width() * shifts;
    if (space_count > 0)
    {
        if (spaces_.size() < space_count) 
            spaces_.resize(space_count, ' ');
        stream.write(&spaces_[0], space_count);
    }
    return stream;
}

}

}
