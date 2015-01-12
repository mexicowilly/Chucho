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

#include "scrollable.hpp"
#include "logger_emitter.hpp"

namespace chucho
{

namespace config_tool
{

class loggers_win : public scrollable
{
public:
    loggers_win(unsigned x,
                unsigned y,
                std::size_t width,
                std::size_t height);

protected:
    virtual exit_status selected() override;
    virtual exit_status unknown(chtype ch) override;

private:
    std::vector<logger_emitter> lgr_ems_;
};
}

}
