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

#include <chucho/sliding_numbered_file_roller_factory.hpp>
#include <chucho/sliding_numbered_file_roller_memento.hpp>
#include <chucho/sliding_numbered_file_roller.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

sliding_numbered_file_roller_factory::sliding_numbered_file_roller_factory()
{
    set_status_origin("sliding_numbered_file_roller_factory");
}

std::shared_ptr<configurable> sliding_numbered_file_roller_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto snfrm = std::dynamic_pointer_cast<sliding_numbered_file_roller_memento>(mnto);
    assert(snfrm);
    if (!snfrm->get_max_count())
        throw exception("sliding_numbered_file_roller_factory: The max_count field must be set");
    std::shared_ptr<sliding_numbered_file_roller> snfr;
    if (snfrm->get_min_index())
    {
        snfr.reset(new sliding_numbered_file_roller(*snfrm->get_min_index(),
                                                    *snfrm->get_max_count(),
                                                    snfrm->get_file_compressor()));
    }
    else
    {
        snfr.reset(new sliding_numbered_file_roller(*snfrm->get_max_count(),
                                                    snfrm->get_file_compressor()));
    }
    report_info("Created a " + demangle::get_demangled_name(typeid(*snfr)));
    return snfr;
}

std::shared_ptr<memento> sliding_numbered_file_roller_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto(new sliding_numbered_file_roller_memento(cfg));
    return mnto;
}

}
