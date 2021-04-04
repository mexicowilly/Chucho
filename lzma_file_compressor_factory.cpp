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

#include <chucho/lzma_file_compressor_factory.hpp>
#include <chucho/file_compressor_memento.hpp>
#include <chucho/lzma_file_compressor.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

lzma_file_compressor_factory::lzma_file_compressor_factory()
{
    set_status_origin("lzma_file_compressor_factory");
}

std::unique_ptr<configurable> lzma_file_compressor_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
    auto fcm = dynamic_cast<file_compressor_memento*>(mnto.get());
    assert(fcm != nullptr);
    if (!fcm->get_min_index())
        throw exception("lzma_file_compressor_factory: The min_index field must be set");
    unsigned mi = *fcm->get_min_index();
    if (mi == 0)
    {
        report_warning("A min_index of 0 was given, which will be reset to the minimum value of 1");
        mi = 1;
    }
    auto gfc = std::make_unique<lzma_file_compressor>(mi);
    report_info("Created a " + demangle::get_demangled_name(typeid(*gfc)));
    return std::move(gfc);
}

}
