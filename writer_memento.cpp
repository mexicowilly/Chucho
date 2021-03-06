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

#include <chucho/writer_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/move_util.hpp>

namespace chucho
{

writer_memento::writer_memento(configurator& cfg)
    : nameable_memento(cfg)
{
    set_status_origin("writer_memento");
}

void writer_memento::handle(std::unique_ptr<configurable>&& cnf)
{
    auto fmt = dynamic_move<formatter>(std::move(cnf));
    if (fmt)
    {
        fmt_ = std::move(fmt);
    }
    else
    {
        auto flt = dynamic_move<filter>(std::move(cnf));
        if (flt)
            filters_.push_back(std::move(flt));
        else
            memento::handle(std::move(cnf));
    }
}

}
