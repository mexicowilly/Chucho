/*
 * Copyright 2013-2017 Will Mason
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

#include <chucho/async_writer_factory.hpp>
#include <chucho/async_writer_memento.hpp>
#include <chucho/async_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

async_writer_factory::async_writer_factory()
{
    set_status_origin("async_writer_factory");
}

std::shared_ptr<configurable> async_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto awm = std::dynamic_pointer_cast<async_writer_memento>(mnto);
    assert(awm);
    if (!awm->get_writer())
        throw exception("async_writer: The async writer's writer must be set");
    std::size_t queue_cap = awm->get_queue_capacity() ?
        *awm->get_queue_capacity() : async_writer::DEFAULT_QUEUE_CAPACITY;
    std::shared_ptr<level> dis = awm->get_discard_threshold() ?
        awm->get_discard_threshold() : level::INFO_();
    bool flsh = awm->get_flush_on_destruct() ?
        *awm->get_flush_on_destruct() : true;
    auto aw = std::make_shared<async_writer>(awm->get_writer(), queue_cap, dis, flsh);
    report_info("Created a " + demangle::get_demangled_name(typeid(*aw)));
    return aw;
}

std::shared_ptr<memento> async_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_shared<async_writer_memento>(cfg);
    return mnto;
}

}
