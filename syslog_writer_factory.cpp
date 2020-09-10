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

#include <chucho/syslog_writer_factory.hpp>
#include <chucho/syslog_writer_memento.hpp>
#include <chucho/syslog_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

syslog_writer_factory::syslog_writer_factory()
{
    set_status_origin("syslog_writer_factory");
}

std::unique_ptr<configurable> syslog_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    std::unique_ptr<configurable> cnf;
    auto swm = dynamic_cast<syslog_writer_memento*>(mnto.get());
    assert(swm != nullptr);
    if (swm->get_name().empty())
        throw exception("syslog_writer_factory: The name is not set");
    auto fmt = std::move(swm->get_formatter());
    if (!fmt)
        throw exception("syslog_writer_factory: The writer's formatter is not set");
    if (!swm->get_facility())
        throw exception("syslog_writer_factory: The writer's facility is not set");
    if (swm->get_host_name().empty())
    {
        cnf = std::make_unique<syslog_writer>(swm->get_name(),
                                              std::move(fmt),
                                              *swm->get_facility());
    }
    else
    {
        if (swm->get_port())
        {
            cnf = std::make_unique<syslog_writer>(swm->get_name(),
                                                  std::move(fmt),
                                                  *swm->get_facility(),
                                                  swm->get_host_name(),
                                                  *swm->get_port());
        }
        else
        {
            cnf = std::make_unique<syslog_writer>(swm->get_name(),
                                                  std::move(fmt),
                                                  *swm->get_facility(),
                                                  swm->get_host_name());
        }
    }
    set_filters(*cnf, *swm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> syslog_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<syslog_writer_memento>(cfg);
    return std::move(mnto);
}

}
