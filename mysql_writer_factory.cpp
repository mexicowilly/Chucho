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

#include <chucho/mysql_writer_factory.hpp>
#include <chucho/mysql_writer_memento.hpp>
#include <chucho/mysql_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

mysql_writer_factory::mysql_writer_factory()
{
    set_status_origin("mysql_writer_factory");
}

std::shared_ptr<configurable> mysql_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto mwm = std::dynamic_pointer_cast<mysql_writer_memento>(mnto);
    assert(mwm);
    if (mwm->get_name().empty())
        throw exception("mysql_writer_factory: The name is not set");
    if (!mwm->get_formatter())
        throw exception("mysql_writer_factory: The writer's formatter is not set");
    if (mwm->get_host().empty()) 
        throw exception("mysql_writer_factory: The MySQL host name must be set");
    if (mwm->get_user().empty()) 
        throw exception("mysql_writer_factory: The MySQL user name must be set");
    if (mwm->get_password().empty()) 
        throw exception("mysql_writer_factory: The MySQL user password must be set");
    if (mwm->get_database().empty()) 
        throw exception("mysql_writer_factory: The MySQL database name must be set");
    std::uint16_t port = mwm->get_port() ?
        *mwm->get_port() : mysql_writer::DEFAULT_PORT;
    std::size_t queue_cap = mwm->get_queue_capacity() ?
        *mwm->get_queue_capacity() : async_writer::DEFAULT_QUEUE_CAPACITY;
    std::shared_ptr<level> dis = mwm->get_discard_threshold() ?
        mwm->get_discard_threshold() : level::INFO_();
    bool flsh = mwm->get_flush_on_destruct() ?
        *mwm->get_flush_on_destruct() : true;
    auto mw = std::make_shared<mysql_writer>(mwm->get_name(),
                                             mwm->get_formatter(),
                                             mwm->get_host(),
                                             mwm->get_user(),
                                             mwm->get_password(),
                                             mwm->get_database(),
                                             port,
                                             queue_cap,
                                             dis,
                                             flsh);
    set_filters(mw, mwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*mw)));
    return mw;
}

std::shared_ptr<memento> mysql_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto = std::make_shared<mysql_writer_memento>(cfg);
    return mnto;
}

}
