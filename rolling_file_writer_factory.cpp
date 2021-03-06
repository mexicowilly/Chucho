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

#include <chucho/rolling_file_writer_factory.hpp>
#include <chucho/rolling_file_writer_memento.hpp>
#include <chucho/rolling_file_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

rolling_file_writer_factory::rolling_file_writer_factory()
{
    set_status_origin("rolling_file_writer_factory");
}

std::unique_ptr<configurable> rolling_file_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto rfwm = dynamic_cast<rolling_file_writer_memento*>(mnto.get());
    assert(rfwm != nullptr);
    if (rfwm->get_name().empty())
        throw exception("rolling_file_writer_factory: The name is not set");
    auto rlr = std::move(rfwm->get_file_roller());
    if (!rlr)
        throw exception("rolling_file_writer_factory: A file_roller is required when creating a rolling_file");
    auto fmt = std::move(rfwm->get_formatter());
    if (!fmt)
        throw exception("rolling_file_writer_factory: The writer's formatter is not set");
    std::unique_ptr<configurable> cnf;
    if (rfwm->get_file_name().empty())
    {
        if (rfwm->get_on_start() && rfwm->get_flush())
        {
            cnf = std::make_unique<rolling_file_writer>(rfwm->get_name(),
                                                        std::move(fmt),
                                                        *rfwm->get_on_start(),
                                                        *rfwm->get_flush(),
                                                        std::move(rlr),
                                                        std::move(rfwm->get_file_roll_trigger()));
        }
        else if (rfwm->get_flush())
        {
            cnf = std::make_unique<rolling_file_writer>(rfwm->get_name(),
                                                        std::move(fmt),
                                                        file_writer::on_start::APPEND,
                                                        *rfwm->get_flush(),
                                                        std::move(rlr),
                                                        std::move(rfwm->get_file_roll_trigger()));
        }
        else if (rfwm->get_on_start())
        {
            cnf = std::make_unique<rolling_file_writer>(rfwm->get_name(),
                                                        std::move(fmt),
                                                        *rfwm->get_on_start(),
                                                        true,
                                                        std::move(rlr),
                                                        std::move(rfwm->get_file_roll_trigger()));
        }
        else
        {
            cnf = std::make_unique<rolling_file_writer>(rfwm->get_name(),
                                                        std::move(fmt),
                                                        std::move(rlr),
                                                        std::move(rfwm->get_file_roll_trigger()));
        }
    }
    else
    {
        if (rfwm->get_on_start() && rfwm->get_flush())
        {
            cnf = std::make_unique<rolling_file_writer>(rfwm->get_name(),
                                                        std::move(fmt),
                                                        rfwm->get_file_name(),
                                                        *rfwm->get_on_start(),
                                                        *rfwm->get_flush(),
                                                        std::move(rlr),
                                                        std::move(rfwm->get_file_roll_trigger()));
        }
        else if (rfwm->get_flush())
        {
            cnf = std::make_unique<rolling_file_writer>(rfwm->get_name(),
                                                        std::move(fmt),
                                                        rfwm->get_file_name(),
                                                        file_writer::on_start::APPEND,
                                                        *rfwm->get_flush(),
                                                        std::move(rlr),
                                                        std::move(rfwm->get_file_roll_trigger()));
        }
        else if (rfwm->get_on_start())
        {
            cnf = std::make_unique<rolling_file_writer>(rfwm->get_name(),
                                                        std::move(fmt),
                                                        rfwm->get_file_name(),
                                                        *rfwm->get_on_start(),
                                                        true,
                                                        std::move(rlr),
                                                        std::move(rfwm->get_file_roll_trigger()));
        }
        else
        {
            cnf = std::make_unique<rolling_file_writer>(rfwm->get_name(),
                                                        std::move(fmt),
                                                        rfwm->get_file_name(),
                                                        std::move(rlr),
                                                        std::move(rfwm->get_file_roll_trigger()));
        }
    }
    set_filters(*cnf, *rfwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> rolling_file_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<rolling_file_writer_memento>(cfg);
    return std::move(mnto);
}

}
