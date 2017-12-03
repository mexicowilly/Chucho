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

#include <chucho/sliding_numbered_file_roller_factory.hpp>
#include <chucho/sliding_numbered_file_roller_memento.hpp>
#include <chucho/sliding_numbered_file_roller.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

sliding_numbered_file_roller_factory::sliding_numbered_file_roller_factory()
{
    set_status_origin("sliding_numbered_file_roller_factory");
}

std::unique_ptr<configurable> sliding_numbered_file_roller_factory::create_configurable(const memento& mnto)
{
    auto snfrm = dynamic_cast<const sliding_numbered_file_roller_memento&>(mnto);
    if (!snfrm.get_max_count())
        throw exception("sliding_numbered_file_roller_factory: The max_count field must be set");
    std::unique_ptr<configurable> snfr;
    if (snfrm.get_min_index())
    {
        snfr = std::make_unique<sliding_numbered_file_roller>(*snfrm.get_min_index(),
                                                              *snfrm.get_max_count(),
                                                              snfrm.get_file_compressor()));
    }
    else
    {
        snfr = std::make_unique<sliding_numbered_file_roller>(*snfrm.get_max_count(),
                                                              snfrm.get_file_compressor()));
    }
    report_info("Created a " + demangle::get_demangled_name(typeid(*snfr)));
    return std::move(snfr);
}

std::unique_ptr<memento> sliding_numbered_file_roller_factory::create_memento(configurator& cfg)
{
    auto mnto = std::make_unique<sliding_numbered_file_roller_memento>(cfg);
    return std::move(mnto);
}

}
