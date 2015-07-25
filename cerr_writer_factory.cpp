/*
 * Copyright 2013-2015 Will Mason
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

#include <chucho/cerr_writer_factory.hpp>
#include <chucho/cerr_writer_memento.hpp>
#include <chucho/cerr_writer.hpp>
#include <chucho/demangle.hpp>
#include <chucho/exception.hpp>
#include <assert.h>

namespace chucho
{

cerr_writer_factory::cerr_writer_factory()
{
    set_status_origin("cerr_writer_factory");
}

std::shared_ptr<configurable> cerr_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto cwm = std::dynamic_pointer_cast<cerr_writer_memento>(mnto);
    assert(cwm);
    if (!cwm->get_formatter())
        throw exception("cerr_writer_factory: The writer's formatter is not set");
    std::shared_ptr<configurable> cnf(new cerr_writer(cwm->get_formatter()));
    set_filters(cnf, cwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> cerr_writer_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto(new cerr_writer_memento(cfg));
    return mnto;
}

}
