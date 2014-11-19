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

#if !defined(CHUCHO_LOGGER_EMITTER_HPP__)
#define CHUCHO_LOGGER_EMITTER_HPP__

#include "writer_emitter.hpp"
#include <chucho/loggable.hpp>
#include <vector>

namespace chucho
{

namespace config_tool
{

class logger_emitter : loggable<logger_emitter>, public emitter
{
public:
    logger_emitter(const properties& props, const std::string& name);

    void add_writer_emitter(std::shared_ptr<writer_emitter> we);
    virtual void emit(std::ostream& stream, std::size_t shifts) override;
    std::shared_ptr<level> get_level() const;
    bool get_writes_to_ancestors() const;
    void remove_writer_emitter(unsigned index);
    void set_level(std::shared_ptr<level> lvl);
    void set_writes_to_ancestors(bool state);

private:
    std::string name_;
    bool writes_to_ancestors_;
    std::shared_ptr<level> level_;
    std::vector<std::shared_ptr<writer_emitter>> writer_emitters_;
};

inline void logger_emitter::add_writer_emitter(std::shared_ptr<writer_emitter> we)
{
    writer_emitters_.push_back(we);
}

inline std::shared_ptr<level> logger_emitter::get_level() const
{
    return level_;
}

inline bool logger_emitter::get_writes_to_ancestors() const
{
    return writes_to_ancestors_;
}

inline void logger_emitter::set_level(std::shared_ptr<level> lvl)
{
    level_ = lvl;
}

inline void logger_emitter::set_writes_to_ancestors(bool state)
{
    writes_to_ancestors_ = state;
}

}

}

#endif
