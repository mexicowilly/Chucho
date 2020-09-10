/*
 * Copyright 2013-2020 Will Mason
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

std::unique_ptr<configurable> async_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto awm = dynamic_cast<async_writer_memento*>(mnto.get());
    assert(awm != nullptr);
    if (awm->get_name().empty())
        throw exception("async_writer_factory: The name is not set");
    auto wrt = std::move(awm->get_writer());
    if (!wrt)
        throw exception("async_writer: The async writer's writer must be set");
    std::size_t chunk_sz = awm->get_chunk_size() ?
        *awm->get_chunk_size() : async_writer::DEFAULT_CHUNK_SIZE;
    std::size_t max_ch = awm->get_max_chunks() ?
        *awm->get_max_chunks() : async_writer::DEFAULT_MAX_CHUNKS;
    bool flsh = awm->get_flush_on_destruct() ?
        *awm->get_flush_on_destruct() : true;
    auto aw = std::make_unique<async_writer>(awm->get_name(), std::move(wrt), chunk_sz, max_ch, flsh);
    report_info("Created a " + demangle::get_demangled_name(typeid(*aw)));
    return std::move(aw);
}

std::unique_ptr<memento> async_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<async_writer_memento>(cfg);
    return std::move(mnto);
}

}
