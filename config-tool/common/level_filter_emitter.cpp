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

#include "level_filter_emitter.hpp"

namespace
{

const char* result_to_text(chucho::filter::result res)
{
    if (res == chucho::filter::result::DENY)
        return "deny";
    if (res == chucho::filter::result::NEUTRAL)
        return "neutral";
    return "accept";
}

}

namespace chucho
{

namespace config_tool
{

level_filter_emitter::level_filter_emitter(const properties& props,
                                           std::shared_ptr<level> lvl,
                                           filter::result match,
                                           filter::result nmatch)
    : emitter(props),
      level_(lvl),
      on_match_(match),
      on_mismatch_(nmatch)
{
}

void level_filter_emitter::emit(std::ostream& stream, std::size_t shifts)
{
    indent(stream, shifts) << "- chucho::level_filter:" << std::endl;
    std::size_t new_shifts = shifts + 1;
    indent(stream, new_shifts) << "- level: " << level_->get_name() << std::endl;
    indent(stream, new_shifts) << "- on_match: " << result_to_text(on_match_) << std::endl;
    indent(stream, new_shifts) << "- on_mismatch: " << result_to_text(on_mismatch_) << std::endl;
}

}

}
