/*
 * Copyright 2013 Will Mason
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

#include <chucho/numbered_file_roller_factory.hpp>
#include <chucho/numbered_file_roller_memento.hpp>
#include <chucho/numbered_file_roller.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

numbered_file_roller_factory::numbered_file_roller_factory()
{
    set_status_origin("numbered_file_roller_factory");
}

std::shared_ptr<configurable> numbered_file_roller_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto nfrm = std::dynamic_pointer_cast<numbered_file_roller_memento>(mnto);
    assert(nfrm);
    if (!nfrm->get_max_index())
        throw exception("numbered_file_roller_factory: The max_index field must be set");
    std::shared_ptr<numbered_file_roller> nfr;
    if (nfrm->get_min_index())
        nfr.reset(new numbered_file_roller(*nfrm->get_min_index(), *nfrm->get_max_index()));
    else
        nfr.reset(new numbered_file_roller(*nfrm->get_max_index()));
    report_info("Created a " + demangle::get_demangled_name(typeid(*nfr)));
    return std::static_pointer_cast<configurable>(nfr);
}

std::shared_ptr<memento> numbered_file_roller_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new numbered_file_roller_memento(cfg));
    return mnto;
}

}
