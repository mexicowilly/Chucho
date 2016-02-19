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

#include <chucho/zlib_compressor_factory.hpp>
#include <chucho/zlib_compressor_memento.hpp>
#include <chucho/zlib_compressor.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

zlib_compressor_factory::zlib_compressor_factory()
{
    set_status_origin("zlib_compressor_factory");
}

std::shared_ptr<configurable> zlib_compressor_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto zcm = std::dynamic_pointer_cast<zlib_compressor_memento>(mnto);
    assert(zcm);
    optional<int> lvl = zcm->get_compression_level();
    std::shared_ptr<configurable> cnf;
    if (lvl)
        cnf = std::make_shared<zlib_compressor>(*lvl);
    else
        cnf = std::make_shared<zlib_compressor>();
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> zlib_compressor_factory::create_memento(configurator& cfg)
{
    std::shared_ptr<memento> mnto(new zlib_compressor_memento(cfg));
    return mnto;
}

}