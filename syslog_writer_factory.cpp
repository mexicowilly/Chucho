/*
 * Copyright 2013-2016 Will Mason
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

std::shared_ptr<configurable> syslog_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    std::shared_ptr<configurable> cnf;
    auto swm = std::dynamic_pointer_cast<syslog_writer_memento>(mnto);
    assert(swm);
    if (!swm->get_formatter())
        throw exception("syslog_writer_factory: The writer's formatter is not set");
    if (!swm->get_facility())
        throw exception("syslog_writer_factory: The writer's facility is not set");
    if (swm->get_host_name().empty())
    {
        cnf.reset(new syslog_writer(swm->get_formatter(),
                                    *swm->get_facility()));
    }
    else
    {
        if (swm->get_port())
        {
            cnf.reset(new syslog_writer(swm->get_formatter(),
                                        *swm->get_facility(),
                                        swm->get_host_name(),
                                        *swm->get_port()));
        }
        else
        {
            cnf.reset(new syslog_writer(swm->get_formatter(),
                                        *swm->get_facility(),
                                        swm->get_host_name()));
        }
    }
    set_filters(cnf, swm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> syslog_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto(new syslog_writer_memento(cfg));
    return mnto;
}

}
