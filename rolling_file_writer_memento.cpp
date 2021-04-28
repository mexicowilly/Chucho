/*
 * Copyright 2013-2021 Will Mason
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

#include <chucho/rolling_file_writer_memento.hpp>
#include <chucho/rolling_file_writer.hpp>
#include <chucho/demangle.hpp>
#include <chucho/exception.hpp>
#include <chucho/move_util.hpp>

namespace chucho
{

rolling_file_writer_memento::rolling_file_writer_memento(configurator& cfg)
    : file_writer_memento(cfg, get_memento_key_set(cfg))
{
    set_status_origin("rolling_file_writer_memento");
    set_default_name(typeid(rolling_file_writer));
}

void rolling_file_writer_memento::handle(std::unique_ptr<configurable>&& cnf)
{
    auto roller = dynamic_move<file_roller>(std::move(cnf));
    if (roller)
    {
        roller_ = std::move(roller);
    }
    else
    {
        auto trigger = dynamic_move<file_roll_trigger>(std::move(cnf));
        if (trigger)
            trigger_ = std::move(trigger);
        else
            file_writer_memento::handle(std::move(cnf));
    }
}

}
