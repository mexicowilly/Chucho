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

#include <chucho/zip_file_compressor_factory.hpp>
#include <chucho/file_compressor_memento.hpp>
#if defined(CHUCHO_HAVE_LIBARCHIVE)
#include <chucho/zip_file_compressor.hpp>
#endif
#include <chucho/noop_file_compressor.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

zip_file_compressor_factory::zip_file_compressor_factory()
{
    set_status_origin("zip_file_compressor_factory");
}

std::unique_ptr<configurable> zip_file_compressor_factory::create_configurable(std::unique_ptr<memento>& mnto)
{
#if defined(CHUCHO_HAVE_LIBARCHIVE)
    auto fcm = dynamic_cast<const file_compressor_memento&>(mnto);
    if (!fcm.get_min_index())
        throw exception("zip_file_compressor_factory: The min_index field must be set");
    unsigned mi = *fcm.get_min_index();
    if (mi == 0)
    {
        report_warning("A min_index of 0 was given, which will be reset to the minimum value of 1");
        mi = 1;
    }
    auto zfc = std::make_unique<zip_file_compressor>(mi);
#else
    report_warning("A zip_file_compressor was requested in the configuration, but this Chucho library was built without zip archive support.");
    auto zfc = std::make_unique<noop_file_compressor>();
#endif
    report_info("Created a " + demangle::get_demangled_name(typeid(*zfc)));
    return std::move(zfc);
}

}


