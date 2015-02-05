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

#if !defined(CHUCHO_LOGGER_WIN_HPP__)
#define CHUCHO_LOGGER_WIN_HPP__

#include "cancellable.hpp"
#include "logger_emitter.hpp"
#include <chucho/optional.hpp>

namespace chucho
{

namespace config_tool
{

class logger_win : public cancellable
{
public:
    logger_win(const logger_emitter& emitter,
               unsigned x,
               unsigned y,
               std::size_t width,
               std::size_t height);

    const optional<logger_emitter>& get_logger_emitter() const;

protected:
    virtual exit_status selected() override;
    virtual exit_status unknown(chtype ch) override;

private:
    optional<logger_emitter> emitter_;
};

inline const optional<logger_emitter>& logger_win::get_logger_emitter() const
{
    return emitter_;
}

}

}

#endif
