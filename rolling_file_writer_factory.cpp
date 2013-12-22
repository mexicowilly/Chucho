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

std::shared_ptr<configurable> rolling_file_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto rfwm = std::dynamic_pointer_cast<rolling_file_writer_memento>(mnto);
    assert(rfwm);
    if (!rfwm->get_file_roller())
        throw exception("rolling_file_writer_factory: A file_roller is required when creating a rolling_file");
    if (!rfwm->get_formatter())
        throw exception("rolling_file_writer_factory: The writer's formatter is not set");
    std::shared_ptr<configurable> cnf;
    if (rfwm->get_file_name().empty())
    {
        if (rfwm->get_on_start() && rfwm->get_flush())
        {
            cnf.reset(new rolling_file_writer(rfwm->get_formatter(),
                                              *rfwm->get_on_start(),
                                              *rfwm->get_flush(),
                                              rfwm->get_file_roller(),
                                              rfwm->get_file_roll_trigger()));
        }
        else if (rfwm->get_flush())
        {
            cnf.reset(new rolling_file_writer(rfwm->get_formatter(),
                                              file_writer::on_start::APPEND,
                                              *rfwm->get_flush(),
                                              rfwm->get_file_roller(),
                                              rfwm->get_file_roll_trigger()));
        }
        else if (rfwm->get_on_start())
        {
            cnf.reset(new rolling_file_writer(rfwm->get_formatter(),
                                              *rfwm->get_on_start(),
                                              true,
                                              rfwm->get_file_roller(),
                                              rfwm->get_file_roll_trigger()));
        }
        else
        {
            cnf.reset(new rolling_file_writer(rfwm->get_formatter(),
                                              rfwm->get_file_roller(),
                                              rfwm->get_file_roll_trigger()));
        }
    }
    else
    {
        if (rfwm->get_on_start() && rfwm->get_flush())
        {
            cnf.reset(new rolling_file_writer(rfwm->get_formatter(),
                                              rfwm->get_file_name(),
                                              *rfwm->get_on_start(),
                                              *rfwm->get_flush(),
                                              rfwm->get_file_roller(),
                                              rfwm->get_file_roll_trigger()));
        }
        else if (rfwm->get_flush())
        {
            cnf.reset(new rolling_file_writer(rfwm->get_formatter(),
                                              rfwm->get_file_name(),
                                              file_writer::on_start::APPEND,
                                              *rfwm->get_flush(),
                                              rfwm->get_file_roller(),
                                              rfwm->get_file_roll_trigger()));
        }
        else if (rfwm->get_on_start())
        {
            cnf.reset(new rolling_file_writer(rfwm->get_formatter(),
                                              rfwm->get_file_name(),
                                              *rfwm->get_on_start(),
                                              true,
                                              rfwm->get_file_roller(),
                                              rfwm->get_file_roll_trigger()));
        }
        else
        {
            cnf.reset(new rolling_file_writer(rfwm->get_formatter(),
                                              rfwm->get_file_name(),
                                              rfwm->get_file_roller(),
                                              rfwm->get_file_roll_trigger()));
        }
    }
    set_filters(cnf, rfwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> rolling_file_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new rolling_file_writer_memento(cfg));
    return mnto;
}

}
