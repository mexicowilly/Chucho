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

#include "logger_emitter.hpp"
#include <chucho/log.hpp>

namespace chucho
{

namespace config_tool
{

logger_emitter::logger_emitter(const properties& props)
    : emitter(props)
{
    rename_logger(typeid(*this));
}

void logger_emitter::emit(std::ostream& stream, std::size_t shifts)
{
    indent(stream, shifts) << "- chucho::logger:" << std::endl;
    std::size_t new_shifts = shifts + 1;
    indent(stream, new_shifts) << "- name: " << *name_ << std::endl;
    if (level_)
        indent(stream, new_shifts) << "- level: " << level_->get_name() << std::endl; 
    indent(stream, new_shifts) << "- writes_to_ancestors: " << std::boolalpha << writes_to_ancestors_ << std::endl;
    for (auto we : writer_emitters_)
        we->emit(stream, new_shifts);
}

std::pair<bool, std::string> logger_emitter::is_valid() const
{
    return name_ ? std::make_pair(true, "") : std::make_pair(false, "The logger name is required");
}

void logger_emitter::remove_writer_emitter(unsigned index)
{
    if (index < writer_emitters_.size())
        writer_emitters_.erase(writer_emitters_.begin() + index);
    else
        CHUCHO_WARN_LGBL("Cannot erase writer at index " << index << " because it is out of range");
}

}

}
