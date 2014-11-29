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

#if !defined(CHUCHO_LEVEL_FILTER_EMITTER_HPP__)
#define CHUCHO_LEVEL_FILTER_EMITTER_HPP__

#include "emitter.hpp"
#include <chucho/filter.hpp>

namespace chucho
{

namespace config_tool
{

class level_filter_emitter : public emitter
{
public:
    level_filter_emitter(const properties& props,
                         std::shared_ptr<level> lvl,
                         filter::result match,
                         filter::result nmatch);

    virtual void emit(std::ostream& stream, std::size_t shifts) override;
    std::shared_ptr<level> get_level() const;
    filter::result get_on_match() const;
    filter::result get_on_mismatch() const;
    void set_level(std::shared_ptr<level> lvl);
    void set_on_match(filter::result match);
    void set_on_mismatch(filter::result nmatch);

private:
    std::shared_ptr<level> level_;
    filter::result on_match_;
    filter::result on_mismatch_;
};

inline std::shared_ptr<level> level_filter_emitter::get_level() const
{
    return level_;
}

inline filter::result level_filter_emitter::get_on_match() const
{
    return on_match_;
}

inline filter::result level_filter_emitter::get_on_mismatch() const
{
    return on_mismatch_;
}

inline void level_filter_emitter::set_level(std::shared_ptr<level> lvl)
{
    level_ = lvl;
}

inline void level_filter_emitter::set_on_match(filter::result match)
{
    on_match_ = match;
}

inline void level_filter_emitter::set_on_mismatch(filter::result nmatch)
{
    on_mismatch_ = nmatch;
}

}

}

#endif
