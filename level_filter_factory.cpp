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

#include <chucho/level_filter_factory.hpp>
#include <chucho/level_filter_memento.hpp>
#include <chucho/level_filter.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

level_filter_factory::level_filter_factory()
{
    set_status_origin("level_filter_factory");
}

std::unique_ptr<configurable> level_filter_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto lfm = dynamic_cast<level_filter_memento*>(mnto.get());
    assert(lfm != nullptr);
    if (lfm->get_name().empty())
        throw exception("level_filter_factory: The name must be set");
    if (!lfm->get_level())
        throw exception("level_filter_factory: The level must be set");
    if (!lfm->get_on_match())
        throw exception("level_filter_factory: The on_match key must be set");
    if (!lfm->get_on_mismatch())
        throw exception("level_filter_factory: The on_mismatch key must be set");
    auto cnf = std::make_unique<level_filter>(lfm->get_name(),
                                              lfm->get_level(),
                                              *lfm->get_on_match(),
                                              *lfm->get_on_mismatch());
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return std::move(cnf);
}

std::unique_ptr<memento> level_filter_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<level_filter_memento>(cfg, get_memento_key_set(cfg));
    return std::move(mnto);
}

}
