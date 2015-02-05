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

#include "cout_writer_emitter.hpp"

namespace chucho
{

namespace config_tool
{

cout_writer_emitter::cout_writer_emitter(const properties& props,
                                         std::shared_ptr<formatter_emitter> fmt,
                                         const std::vector<std::shared_ptr<filter_emitter>>& flts)
    : writer_emitter(props, fmt, flts)
{
}

void cout_writer_emitter::emit(std::ostream& stream, std::size_t shifts)
{
    indent(stream, shifts) << "- chucho::cout_writer:" << std::endl;
    writer_emitter::emit(stream, shifts);
}

const char* cout_writer_emitter::get_name() const
{
    return "Stdout Writer";
}

}

}
