/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/email_writer_factory.hpp>
#include <chucho/email_writer_memento.hpp>
#include <chucho/email_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

email_writer_factory::email_writer_factory()
{
    set_status_origin("email_writer_factory");
}

std::unique_ptr<configurable> email_writer_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto ewm = dynamic_cast<email_writer_memento*>(mnto.get());
    assert(ewm != nullptr);
    if (ewm->get_name().empty())
        throw exception("email_writer_factory: The name is not set");
    auto fmt = std::move(ewm->get_formatter());
    if (!fmt)
        throw exception("email_writer_factory: The writer's formatter is not set");
    if (!ewm->get_connection_type()) 
        throw exception("email_writer_factory: The connection type must be set");
    auto trg = std::move(ewm->get_email_trigger());
    if (!trg)
        throw exception("email_writer_factory: The email trigger must be set");
    if (ewm->get_from().empty()) 
        throw exception("email_writer_factory: The from field must be set");
    if (ewm->get_host().empty()) 
        throw exception("email_writer_factory: The host must be set");
    if (ewm->get_subject().empty()) 
        throw exception("email_writer_factory: The email subject must be set");
    if (ewm->get_to().empty()) 
        throw exception("email_writer_factory: At least one recipient in the to field must be set");
    std::uint16_t port = ewm->get_port() ? *ewm->get_port() : email_writer::DEFAULT_PORT;
    std::size_t buf_size = ewm->get_buffer_size() ? *ewm->get_buffer_size() : email_writer::DEFAULT_BUFFER_CAPACITY;
    std::unique_ptr<email_writer> wrt;
    if (ewm->get_user().empty() && ewm->get_password().empty())
    {
        wrt = std::make_unique<email_writer>(ewm->get_name(),
                                             std::move(fmt),
                                             ewm->get_host(),
                                             *ewm->get_connection_type(),
                                             ewm->get_to(),
                                             ewm->get_from(),
                                             ewm->get_subject(),
                                             std::move(trg),
                                             port,
                                             buf_size);
    }
    else
    {
        wrt = std::make_unique<email_writer>(ewm->get_name(),
                                             std::move(fmt),
                                             ewm->get_host(),
                                             *ewm->get_connection_type(),
                                             ewm->get_to(),
                                             ewm->get_from(),
                                             ewm->get_subject(),
                                             std::move(trg),
                                             ewm->get_user(),
                                             ewm->get_password(),
                                             port,
                                             buf_size);

    }
    set_filters(*wrt, *ewm);
    if (ewm->get_verbose())
        wrt->set_verbose(*ewm->get_verbose());
    report_info("Created a " + demangle::get_demangled_name(typeid(*wrt)));
    return std::move(wrt);
}

std::unique_ptr<memento> email_writer_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<email_writer_memento>(cfg);
    return std::move(mnto);
}

}
