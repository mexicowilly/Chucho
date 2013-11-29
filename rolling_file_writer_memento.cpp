/*
 * Copyright 2013 Will Mason
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
#include <chucho/demangle.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

rolling_file_writer_memento::rolling_file_writer_memento(const configurator& cfg)
    : file_writer_memento(cfg, get_memento_key_set(cfg))
{
    set_status_origin("rolling_file_writer_memento");
}

void rolling_file_writer_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto roller = std::dynamic_pointer_cast<file_roller>(cnf);
    if (roller)
    {
        roller_ = roller;
    }
    else
    {
        auto trigger = std::dynamic_pointer_cast<file_roll_trigger>(cnf);
        if (trigger)
            trigger_ = trigger;
        else
            file_writer_memento::handle(cnf);
    }
}

}
