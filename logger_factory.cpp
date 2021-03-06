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

#include <chucho/logger_factory.hpp>
#include <chucho/logger_memento.hpp>
#include <chucho/logger.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <algorithm>
#include <assert.h>

namespace chucho
{

logger_factory::logger_factory()
{
    set_status_origin("logger_factory");
}

std::unique_ptr<configurable> logger_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto lm = dynamic_cast<logger_memento*>(mnto.get());
    assert(lm != nullptr);
    if (!lm->get_name())
        throw exception("logger_memento: The logger's name must be set");
    auto nm = *lm->get_name();
    if (nm == "<root>")
        nm.clear();
    auto lgr = logger::get_impl(nm);
    if (lm->get_level())
        lgr->set_level(lm->get_level());
    std::for_each(lm->get_writers().begin(),
                  lm->get_writers().end(),
                  [&] (std::unique_ptr<writer>& w) { lgr->add_writer(std::move(w)); });
    if (lm->get_writes_to_ancestors())
        lgr->set_writes_to_ancestors(*lm->get_writes_to_ancestors());
    std::shared_ptr<configurable> cnf = std::static_pointer_cast<configurable>(lgr);
    if (!nm.empty())
        report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)) + " named " + nm);
    return std::move(std::unique_ptr<configurable>());
}

std::unique_ptr<memento> logger_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<logger_memento>(cfg);
    return std::move(mnto);
}

}
