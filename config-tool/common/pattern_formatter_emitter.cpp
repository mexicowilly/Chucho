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

#include "pattern_formatter_emitter.hpp"

namespace chucho
{

namespace config_tool
{

pattern_formatter_emitter::pattern_formatter_emitter(const properties& props)
    : formatter_emitter(props)
{
}

void pattern_formatter_emitter::emit(std::ostream& stream, std::size_t shifts)
{
    indent(stream, shifts) << "- chucho::pattern_formatter:" << std::endl;
    indent(stream, shifts + 1) << "- pattern: '" << pattern_ << '\'' << std::endl;
}

}

}
