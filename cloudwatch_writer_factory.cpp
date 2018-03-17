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

#include <chucho/cloudwatch_writer_factory.hpp>
#include <chucho/cloudwatch_writer_memento.hpp>
#include <chucho/cloudwatch_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

std::unique_ptr<configurable> cloudwatch_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto cwm = dynamic_cast<cloudwatch_writer_memento*>(mnto.get());
    assert(cwm != nullptr);
    if (cwm->get_name().empty())
        throw exception("cloudwatch_writer_factory: The name is not set");
    auto fmt = std::move(cwm->get_formatter());
    if (!fmt)
        throw exception("cloudwatch_writer_factory: The writer's formatter is not set");
    if (cwm->get_log_group().empty())
        throw exception("cloudwatch_writer_factory: The writer's log group is not set");
    if (cwm->get_log_stream().empty())
        throw exception("cloudwatch_writer_factory: The writer's log stream is not set");
    std::size_t batch_sz = cwm->get_batch_size() ? *cwm->get_batch_size() : cloudwatch_writer::DEFAULT_BATCH_SIZE;
    std::unique_ptr<cloudwatch_writer> cw;
    if (cwm->get_region().empty())
    {
        cw = std::make_unique<cloudwatch_writer>(cwm->get_name(),
                                                 std::move(fmt),
                                                 cwm->get_log_group(),
                                                 cwm->get_log_stream(),
                                                 batch_sz);
    }
    else
    {
        cw = std::make_unique<cloudwatch_writer>(cwm->get_name(),
                                                 std::move(fmt),
                                                 cwm->get_log_group(),
                                                 cwm->get_log_stream(),
                                                 cwm->get_region(),
                                                 batch_sz);
    }
    set_filters(*cw, *cwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cw)));
    return std::move(cw);
}

std::unique_ptr<memento> cloudwatch_writer_factory::create_memento(configurator& cfg)
{
    std::unique_ptr<memento> mnto = std::make_unique<cloudwatch_writer_memento>(cfg);
    return std::move(mnto);
}

}
